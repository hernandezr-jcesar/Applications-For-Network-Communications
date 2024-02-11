#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define MAX_LARGO_PALABRA 50

char *palabras[MAX_LARGO_PALABRA];

int main(int argc, char const *argv[])
{
	
	char *temp;
	temp =malloc(sizeof(MAX_LARGO_PALABRA));
	for (int i = 0; i < 3; ++i)
	{		
		printf("Introduce la palabra #%d: \n", i+1);		
		palabras[i] = malloc(sizeof(MAX_LARGO_PALABRA));
		fgets(palabras[i], MAX_LARGO_PALABRA, stdin);
	}
	for (int i = 0; i < 3; ++i)
	{
		printf("%s\n", palabras[i]);	
	}
	
	return 0;
}