#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main() {
    char buffer[BUFFER_SIZE];
    int fd = open("http/cgi/cgi.html", O_RDONLY);
    if (fd == -1) {
        printf("Erreur: impossible d'ouvrir le fichier.\n");
        exit(EXIT_FAILURE);
    }

    int bytes_read = 0;
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }
    /*char *path = NULL;
    path = getcwd(buffer, 1024);
    printf("Emplacement courant : %s\n", path);*/
    close(fd);
    return 0;
}
