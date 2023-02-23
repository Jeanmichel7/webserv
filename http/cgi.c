#include <stdio.h>
#include <unistd.h>

int main() {
    char buffer[1024];
    char *path = NULL;
    path = getcwd(buffer, 1024);
    printf("Emplacement courant : %s\n", path);
    return 0;
}
