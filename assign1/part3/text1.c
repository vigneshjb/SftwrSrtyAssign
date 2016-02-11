#include <stdio.h>
#include <string.h>
int main()
{
	char *one = "GET /34234234 HTTP/1.1 \nHost: localhost:8080 \nConnection: keep-alive \nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8 \nUpgrade-Insecure-Requests: 1 \nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/48.0.2564.103 Safari/537.36 \nAccept-Encoding: gzip, deflate, sdch \nAccept-Language: en-US,en;q=0.8 \nCookie: author=AnonymousCoward";
	char *two = "GET /exec/";
	printf("%d\n", strncmp(one, two, 10));
}