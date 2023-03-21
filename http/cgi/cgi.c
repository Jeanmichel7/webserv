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
    write(2, " ", strlen("NIQUE TA "));
    write(1, "Content-Type: text/html\r\n\r\n", strlen("Content-Type: text/html\r\n\r\n"));
    int fd = open("http/cgi/cgi.html", O_RDONLY);
    if (fd == -1)
    {
        printf("Erreur: impossible d'ouvrir le fichier.\n");
        exit(EXIT_FAILURE);
    }

    int bytes_read = 0; 
    while ((bytes_read = recv(fd, buffer, BUFFER_SIZE, 0)) > 0)
    {
        write(STDOUT_FILENO, buffer, bytes_read);
    }
    close(fd);
    write(1, "<h1 class = \"cgi\" >CGI STDIN </h1>\n", strlen("<h1 class = \"cgi\" >CGI STDIN </h1>\n"));
    write(1, "<p class = \"cgi\" >\n", strlen("<p class = \"cgi\" >\n"));
    int c;
    unsigned int i = 0;
    while (1) {
        c = fgetc(stdin);
        if (c == EOF)
            break;
        //  write(2, &c, 1);        
        if (c == '\n')
        {
            write(1, "<br>\n", strlen("<br>\n")); // utilise la balise HTML pour les nouvelles lignes
        }
        else if (c == '<')
        {
            write(1, "&lt;", strlen("&lt;")); // utilise la référence HTML pour le caractère '<'
        }
        else if (c == '>')
        {
            write(1, "&gt;", strlen("&gt;")); // utilise la référence HTML pour le caractère '>'
        }
        else if (c == '&')
        {
            write(1, "&amp;", strlen("&amp;")); // utilise la référence HTML pour le caractère '&'
        }
        else if (c == '\0')
        {
            write(1 ,"&#0;", strlen("&#0;"));
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
