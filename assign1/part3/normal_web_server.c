#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
int create_socket;
// **************** URL DECODER ********************
void urldecode2(char *dst, char *src)
{
   char a, b;
   while (*src) {
      if ((*src == '%') && ((a = src[1]) && (b = src[2])) && (isxdigit(a) && isxdigit(b))) {
         
         if (a >= 'a')
               a -= 'a'-'A';
         if (a >= 'A')
               a -= ('A' - 10);
         else
               a -= '0';
         if (b >= 'a')
               b -= 'a'-'A';
         if (b >= 'A')
               b -= ('A' - 10);
         else
               b -= '0';
         *dst++ = 16*a+b;
         src+=3;
      } else if (*src == '+') {
         *dst++ = ' ';
         src++;
      } else {
         *dst++ = *src++;
      }
   }
   *dst++ = '\0';
}

//*************RETURNS THE URL **************
char* get_command(char *body){
   int length = strchr(body,'\n')-body;
   char *url_encoded_command = (char *)malloc(length);
   char *command = (char *)malloc(length-19);
   strncpy(url_encoded_command, body+10, length-19);
   url_encoded_command[length]='\0';
   urldecode2(command, url_encoded_command);
   length = length-19-strlen(url_encoded_command)+strlen(command);
   command[length]=' ';
   command[length + 1]='2';
   command[length + 2]='>';
   command[length + 3]='&';
   command[length + 4]='1';
   command[length + 5]='\0';
   free(url_encoded_command);
   return command;
}

void handle_good_request(int *socket, char *request_body){
   FILE *fp;
   char path[1035];
   char *entries[1000];
   char *size_string = (char *) malloc(10);
   int count=0, i=0, tot_size=0;
   char *command=get_command(request_body);
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
   sprintf(size_string, "%d", tot_size);
   write(*socket, "HTTP/1.1 200 OK\n", 16);
   write(*socket, "Content-length: ", 16);
   write(*socket, size_string, strlen(size_string));
   write(*socket, "\n", 1);
   write(*socket, "Content-Type: text/html\n\n", 25);
   for (i=0;i<count;i++){
      write(*socket, entries[i], strlen(entries[i]));
   }
   free(size_string);
   free(command);
   for (i=0;i<count;i++){
      free(entries[i]);
   }
}

// ************* HANDLE REQUEST IN A SEPERATE THREAD ***************
void* execute_thread(void* thread_param){
   int bufsize = 1024;
   char *request_body = (char *) malloc(bufsize);
   char *expected_request = "GET /exec/";
   int *socket = (int *) thread_param;
   recv(*socket, request_body, bufsize, 0);
   if (strncmp(request_body, expected_request, 10)!=0) {
      write(*socket, "HTTP/1.1 404 Not Found\n", 23);
      write(*socket, "Content-length: 46\n", 19);
      write(*socket, "Content-Type: text/html\n\n", 25);
      write(*socket, "<html><body><H1>404 responc</H1></body></html>\n",47);
   } else {
      handle_good_request(socket, request_body);
   }
   free(request_body);
   close(*socket);
}

// ************* GRACEFULLY EXIT PROGRAM ***************
void gracefully_exit(char *message){
   close(create_socket);
   exit(0);
}

// ************* SIGNAL HANDLER ***************
void sig_handler(int sig_no){
   close(create_socket);
   exit(0);
}


int main(int argc, char *argv[]) {
   pthread_t *thread;
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
   // *************** CATCH ALL SIGNALS ***************
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
         // printf("The Client is connected...\n");
      }
      thread = (pthread_t*)malloc(sizeof(pthread_t));
      pthread_create(thread, NULL, execute_thread, (void *)&new_socket);
      usleep(10);
      pthread_join(*thread, NULL);
      // execute_thread(&new_socket);
   }
   close(create_socket);
   return 0;
}