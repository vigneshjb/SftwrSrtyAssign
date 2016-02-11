#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

void urldecode2(char *dst, char *src){
	FILE *fp;
   	char path[1035];
   	char *entries[1000];
   	int count =0, tot_size =0;

   	char *command = (char *) malloc(sizeof(char)*1200);

   	sprintf(command, "python -c \"import urllib, sys; print urllib.unquote(\\\"%s\\\")\" ", src);

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

	for(int i=0;i<count;i++){
		strcat(dst, entries[i]);
	}
	dst[strlen(dst)-1]='\0';

}


int main() {
	char *encoded = "!%40%23%24%25%5E%26*()%20%20%20_%20%2B%20%3D%20abcde\nfghijklmnopqrstuvwxyz%201234567890%20~%60%27%22%3B%3A%2C%3C.%3E%2F%3F%5C%7C%5D%5B%7D%7B";
	char *expected = "!@#$%^&*()   _ + = abcde\nfghijklmnopqrstuvwxyz 1234567890 ~`'\";:,<.>/?\\|][}{";
	char *decoded = (char *) malloc(strlen(encoded));
	urldecode2(decoded, encoded);
	
	printf("%s<\n%s<\n",expected, decoded );
	printf("comapre = %d\n", strcmp(decoded,expected));
	printf("%lu\n%lu", strlen(expected), strlen(decoded) );

	// python -c "import urllib, sys; print urllib.unquote(sys.argv[1])"
}



_________________________________________________________________

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

int main(void)
{
CURL *curl;
curl = curl_easy_init();
//char *src="awk%20-F%20%27%5C%5Ct%27%20%27%7B%20if%20(%242)%20print%20%242%7D%27%20%2Fetc%2Fhosts%20%3E%3E%20file.txt";
//    char *expected = "awk -F '\\\t' '{ if ($2) print $2}' /etc/hosts >> file.txt";
//char *dest = malloc(100);
char *encoded = "!%40%23%24%25%5E%26*()%20%20%20_%20%2B%20%3D%20abcde\nfghijklmnopqrstuvwxyz%201234567890%20~%60%27%22%3B%3A%2C%3C.%3E%2F%3F%5C%7C%5D%5B%7D%7B";
char *expected = "!@#$%^&*()   _ + = abcde\nfghijklmnopqrstuvwxyz 1234567890 ~`'\";:,<.>/?\\|][}{";
char *decoded= curl_easy_unescape(curl, encoded,strlen(encoded) , NULL);

printf("%s<\n%s<\n",expected, decoded );
printf("comapre = %d\n", strcmp(decoded,expected));
printf("%lu\n%lu", strlen(expected), strlen(decoded) );

return 0;
}













