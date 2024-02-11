#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>

#define MAXIMA_LONGITUD_CADENA 100

int main(int argc, char const *argv[])
{
	if (argc =! 3)
	{
		printf("Uso: cliente [DIRECCION IP] [PUERTO]\n");		
		exit(-1);
	}

		//Paso1. 
		//Se necesitan dos estructuras del tipo sockaddr
		//La primera guarda info del server
		//La segunda del cliente
		struct sockaddr_in servidor, cliente;

		//Configuracion del servidor
		servidor.sin_family = AF_INET;	//Familia TCP/IP	
		servidor.sin_port = atoi(argv[2]);	//Puerto
		servidor.sin_addr.s_addr = inet_addr(argv[1]); //Cliente a conectar
		

		//Paso 2.Definicion del socket
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd<0){
			printf("Error de apertura de socket\n");
			exit(-1);
		}
		//Para reusar un puerto
		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1},sizeof(int))<0){
			perror("setsockopt(SO_REUSADDR failed");
		}


		//Paso3.Avisar al sistema que se creo un socket
		int codigo_bind = bind(sockfd, (struct sockaddr *)&servidor, sizeof(struct sockaddr));
		if(codigo_bind < 0){
			printf("ERROR: bind no exitoso\n");
			exit(-1);
		}
	
		//Paso 4. Establecer el socket en modo escucha
		if(listen(sockfd,4)<0){
			printf("Error: listen no exitoso\n");
			exit(-1);
		}
	while(1){
		char msgtoclient[MAXIMA_LONGITUD_CADENA];
		char msgfromclient[MAXIMA_LONGITUD_CADENA];	
		printf("Esperando cliente...\n");
		//Paso 5.Aceptar conexiones

		int longitud_cliente = sizeof(struct sockaddr_in);		
		//lo que devuelve accept es el identificador del canal del comunicacion credo con el cliente		
		//La llamada a accept()
		int c_accept = accept(sockfd, (struct sockaddr *)&cliente,&longitud_cliente);
		if(c_accept < 0){
			printf("ERROR: accept no exitoso\n");
			exit(-1);
		}else{
			printf("Cliente conectado\n");		
		}
		//!strstr(msgtoclient, "exit") && !strstr(msgtoclient,"exit")
		while(1){
			bzero(msgtoclient,MAXIMA_LONGITUD_CADENA);
			bzero(msgfromclient,MAXIMA_LONGITUD_CADENA);			
			int num_bytes_r = recv(c_accept, msgfromclient, sizeof(msgfromclient),0);			
			if(!strstr(msgfromclient,"exit")){
				printf("Mensaje del Cliente: %s",msgfromclient);
				printf("Mensaje para Cliente:\n");	
				fgets(msgtoclient, MAXIMA_LONGITUD_CADENA, stdin);
				send(c_accept, msgtoclient, strlen(msgtoclient),0);	
				if(strstr(msgtoclient,"exit")){
					printf("exit por el servidor\n");
					close(c_accept);
					break;	
				}
							
			}else{
				printf("exit por el cliente\n");
				close(c_accept);
				break;

			}								
		}
		close(c_accept);	
	}

	return 0;
}