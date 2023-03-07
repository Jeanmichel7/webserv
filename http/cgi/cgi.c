#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 16384

int main(int argc, char *argv[], char *envp[])
{
    char buffer[BUFFER_SIZE];
    write(1, "Content-Type: text/html\r\n\r\n", strlen("Content-Type: text/html\r\n\r\n"));
    int fd = open("http/cgi/cgi.html", O_RDONLY);
    if (fd == -1)
    {
        printf("Erreur: impossible d'ouvrir le fichier.\n");
        exit(EXIT_FAILURE);
    }

    int bytes_read = 0; 
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        write(STDOUT_FILENO, buffer, bytes_read);
    }
    /*char *path = NULL;
    path = getcwd(buffer, 1024);
    printf("Emplacement courant : %s\n", path);*/
    close(fd);
    printf("<h1 class = \"cgi\" >CGI STDIN </h1>\n");
    printf("<p class = \"cgi\" >\n");
    int c;
    unsigned int i = 0;
    while (1) {
        c = fgetc(stdin);
        if (c == EOF)
            break;
        //  write(2, &c, 1);        
        if (c == '\n')
        {
            printf("<br>\n"); // utilise la balise HTML pour les nouvelles lignes
        }
        else if (c == '<')
        {
            printf("&lt;"); // utilise la référence HTML pour le caractère '<'
        }
        else if (c == '>')
        {
            printf("&gt;"); // utilise la référence HTML pour le caractère '>'
        }
        else if (c == '&')
        {
            printf("&amp;"); // utilise la référence HTML pour le caractère '&'
        }
        else if (c == '\0')
        {
            printf("&#0;");
        }
        else
        {
            putchar(c);
        }
        if (i % 100000 == 0)
            fprintf(stderr, "%d\n", i);
        i++;
    }
    write(2, "Nombre de caractères lus sur la STDIN : FIIIIIIIIIN \n", strlen("Nombre de caractères lus sur la STDIN : FIIIIIIIIIN \n"));
    printf("\nCARACTERE LU SUR LA STDIN : %d\n", i);
    printf("\n");
    printf("</p>\n");
    printf("<h1 class = \"cgi\" >CGI ENV </h1>\n");
    printf("<ul class = \"cgi\">\n");
    for (int i = 0; envp[i] != NULL; i++)
    {
        printf("<li>");
        printf("%s/n", envp[i]);
        printf("</li>");
    }
    printf("</ul>\n");
    printf("</body>\n");
    printf("</html>\n");
    printf("\r\n");
    return 0;
}
