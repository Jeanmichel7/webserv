#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_SIZE 16384


int main(int argc, char *argv[], char *envp[])
{
    char buffer[BUFFER_SIZE];
    write(1, "Content-Type: image/jpeg\r\n\r\n", strlen("Content-Type: image/jpeg\r\n\r\n"));
		int i = 0;
		char c = 0;
    while (1) {
        c = fgetc(stdin);
        putchar(c);
        i++;
    }
}