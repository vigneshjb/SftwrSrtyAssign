#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <curl/curl.h>
int create_socket;

//*************RETURNS THE URL **************
char* get_command(char *url_decoded_body){
   int length = strchr(url_decoded_body,'\n')-url_decoded_body;
   char *command = (char *)malloc(length);
   command[0]='\0';
   strncpy(command, url_decoded_body+10, length-19);
   command[length-20]=' ';
   command[length-19]='2';
   command[length-18]='>';
   command[length-17]='&';
   command[length-16]='1';
   command[length-15]='\0';
   return command;
}

void handle_good_request(int *socket, char *request_body){
   FILE *fp;
   char path[1035];
   char *entries[1000];
   char *size_string = (char *) malloc(10);
   int count=0, i=0, tot_size=0;
   char *command=get_command(request_body);
   printf("Cmd :%s sz :%lu \n", command, strlen(command));
   fp = popen(command, "r");
   if (fp == NULL) {
      printf("Failed to run command\n" );
   } else {
      while (fgets(path, sizeof(path)-1, fp) != NULL ) {
         entries[count] = (char*)malloc(strlen(path));
         strcpy(entries[count], path);
         count++;
      }
   }
   pclose(fp);
   for (i=0;i<count;i++)
      tot_size+=strlen(entries[i]);
   tot_size++;
   sprintf(size_string, "%d", tot_size);
   write(*socket, "HTTP/1.1 200 OK\r\n", 17);
   write(*socket, "Content-length: ", 16);
   write(*socket, size_string, strlen(size_string));
   write(*socket, "\r\n", 2);
   write(*socket, "Connection: close\r\n",19);
   write(*socket, "Content-Type: text/plain\r\n\r\n", 28);
   for (i=0;i<count;i++){
      write(*socket, entries[i], strlen(entries[i]));
   }
   write(*socket, "\n", 1);
}

// ************* HANDLE REQUEST IN A SEPERATE THREAD ***************
void execute_thread(int *socket){
   int bufsize = 1024;
   CURL *curl;
   curl = curl_easy_init();
   char *request_body = (char *) malloc(bufsize);
   char *expected_request = "GET /exec/";
   recv(*socket, request_body, bufsize, 0);
   char *decoded_request_body = curl_easy_unescape(curl, request_body, strlen(request_body), NULL);
   if (strncmp(decoded_request_body, expected_request, 10)!=0) {
      write(*socket, "HTTP/1.1 404 Not Found\r\n", 24);
      write(*socket, "Content-length: 46\r\n", 20);
      write(*socket, "Content-Type: text/html\r\n\r\n", 27);
      write(*socket, "<html><body><H1>404 responc</H1></body></html>\n",47);
   } else {
      handle_good_request(socket, decoded_request_body);
   }
   close(*socket);
   printf("Closed connected Client\n");
}

// ************* GRACEFULLY EXIT PROGRAM ***************
void gracefully_exit(char *message){
   printf("%s\n", message);
   close(create_socket);
   exit(0);
}

// ************* SIGNAL HANDLER ***************
void sig_handler(int sig_no){
   printf("SIGINT \n");
   close(create_socket);
   exit(0);
}


int main(int argc, char *argv[]) {
   int port_number;
   int new_socket;
   socklen_t addrlen;
   struct sockaddr_in address;
   // *********** READING PORT NUMBER **************
   if (argc!=2 || strlen(argv[1])<=0) {
      gracefully_exit("Incorrect program execution");
   } else {
      port_number = atoi(argv[1]);
      if (port_number<=0) {
         gracefully_exit("Port is not in the right format");
      }
   }
   // *************** CATCH SIGINT SIGNALS ***************
   signal(SIGINT, sig_handler);
   // *************** CREATING A SOCKET ***************
   if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
      gracefully_exit("Socket creation failed");
   }
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port = htons(port_number);
   if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) != 0) {
       gracefully_exit("Binding Socket Failed :(");
   }
   while (1) {
      if (listen(create_socket, 10) < 0) {
         perror("server: listen");
         exit(1);
      }
      if ((new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen)) < 0) {
         perror("server: accept");
         exit(1);
      }
      if (new_socket > 0) {
         printf("The Client is connected...\n");
      }
      execute_thread(&new_socket);
   }
   close(create_socket);
   return 0;
}