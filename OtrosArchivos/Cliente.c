#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

#define MAXIMA_LONGITUD_CADENA 100

int main(int argc, char const *argv[])
{
	if (argc =! 3)
	{
		printf("Uso: cliente [DIRECCION IP] [PUERTO]\n");		
		exit(-1);
	}	
	while(1){
		char buffer[MAXIMA_LONGITUD_CADENA];
		char msgtoserver[MAXIMA_LONGITUD_CADENA];
		char msgfromserver[MAXIMA_LONGITUD_CADENA];	
		
		printf("Mensaje para Servidor:\n");
		fgets(msgtoserver, MAXIMA_LONGITUD_CADENA, stdin);
		strtok(msgtoserver, "\n");



		int sockfd = socket(AF_INET, SOCK_STREAM, 0);

		if(sockfd<0){
		printf("No se pudo mandar al socket\n");
		exit(-1);
		}
		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1},sizeof(int))<0){
		perror("setsockopt(SO_REUSADDR failed");
		}

		struct sockaddr_in servidor;
		servidor.sin_addr.s_addr = inet_addr(argv[1]);
		servidor.sin_port = atoi(argv[2]);
		servidor.sin_family = AF_INET;

		int codigo_connect = connect(sockfd, (struct sockaddr *)&servidor, sizeof(servidor));

		if(codigo_connect < 0){
		printf("ERROR: connect no exitoso\n");
		exit(-1);
		}		
		if(strcmp(msgtoserver, "exit")==0){
			//printf("Mensaje de exit\n");	
			//Comienza fase de comunicacion
			int num_bytes = send(sockfd, msgtoserver, strlen(msgtoserver),0);
			if(num_bytes<0){
				printf("ERROR: send no exitoso\n");
				exit(-1);
			}

			return 0;
		}else{	
			int num_bytes = send(sockfd, msgtoserver, strlen(msgtoserver),0);
			if(num_bytes<0){
				printf("ERROR: send no exitoso\n");
				exit(-1);
			}
			int num_bytes_r = recv(sockfd, msgfromserver, sizeof(msgfromserver),0);

			if(num_bytes_r < 0){
				printf("ERROR: recv no exitoso\n");
				exit(-1);
			}
			msgfromserver[num_bytes_r] =  '\0';
			printf("Mensaje del servidor: %s\n", msgfromserver);			
		}
		
		
		
		
	}
	

	return 0;
}