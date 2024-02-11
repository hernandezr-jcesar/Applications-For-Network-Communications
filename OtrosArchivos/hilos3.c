#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct datos{
	int entero;
	char cadena[20];
}dato;

int* manejoarchivos(void* param){
	dato ejem = *(dato*) param;
	printf("Yo soy el hilo: %d\n",ejem.entero);
	
	FILE *archivo = NULL;
	archivo = fopen("/home/cesar/Documentos/6.Semestre/Redes2/archivo", "w");
	if(archivo == NULL){
		printf("No fue posible abrir el archivo\n");
		return -1;
	}

	fputs("HOLA DESDE EL ARCHIVO, ATT.HILO HIJO\n", archivo);
	fputs("HOla desde el segundo renglon\n", archivo);


	fclose(archivo);

	pthread_exit(NULL);

}
void* holamundo(void* param){
	dato ejem = *(dato*) param;
	printf("Yo soy el hilo: %d\n",ejem.entero);
	printf("%s", ejem.cadena);
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	if(argc != 2){
		printf("Hace falta indicar el numero de hilos a crear.");
		exit(-1);
	}
	//paso 1: Creacion de hilos
	int cuenta = atoi(argv[1]);
	pthread_t id[cuenta];
	dato datos[cuenta];

	for(int i=0; i<cuenta; i++){		
		datos[i].entero = i;		
		pthread_create(&id[i], NULL, manejoarchivos,&datos[i]);
	}


	//paso 2: espera de los hilos hijos
	for(int j=0; j<cuenta; j++){
		pthread_join(id[j], NULL);
	}

	//Lectura del archivo por el proceso padre
	FILE *entrada;
	entrada = fopen("/home/cesar/Documentos/6.Semestre/Redes2/archivo", "r");

	char *contents = NULL;
	size_t len = 0;
	while(getline(&contents, &len, entrada) != -1){
		printf("%s",contents);
	}
	fclose(entrada);
	free(contents);


	return 0;
}
//El padre manda una estructura al hijo, y este los imprime.
//retornar parametros del hilo hacia el padre
//estudiar todo lo de los archivos de texto