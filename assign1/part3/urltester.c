#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void urldecode2(char *dst, char *src)
{
    char a, b;
    while (*src) {
        if ((*src == '%') &&
            ((a = src[1]) && (b = src[2])) &&
            (isxdigit(a) && isxdigit(b))) {
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

int main(){
    char *src="awk%20-F%20%27%5C%5Ct%27%20%27%7B%20if%20(%242)%20print%20%242%7D%27%20%2Fetc%2Fhosts%20%3E%3E%20file.txt";
    char *expected = "awk -F '\\\t' '{ if ($2) print $2}' /etc/hosts >> file.txt";
    char *dest = malloc(100);
    // *(expected+57) = '\0';
    urldecode2(dest, src);
    printf("%s\n", dest);
    printf("%s\n", expected);

}