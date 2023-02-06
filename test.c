#include <stdio.h>

int main(int argc, char **argv, char **env)
{
		int i;
				printf("argc : %d\n",argc);
		for (i = 0; i < argc; i++) {
				printf("arguments : %d%s\n",i, argv[i]); }
		for (i = 0; env[i] != NULL; i++) {
				printf("env : %d %s\n",i, env[i]); }
		printf("bonjour, j'en ai marre de ce cours\n");
}