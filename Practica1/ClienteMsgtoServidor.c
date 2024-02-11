#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

#define MAXIMA_LONGITUD_CADENA 100
#define MAXSIZE 1024

int main(int argc, char const *argv[])
{
	if (argc =! 3)
	{
		printf("Uso: cliente [DIRECCION IP] [PUERTO]\n");		
		exit(-1);
	}	
	
	//Primer paso, definir variables							
	char buffer[MAXIMA_LONGITUD_CADENA];
	char msgtoserver[MAXIMA_LONGITUD_CADENA];
	char msgfromserver[MAXIMA_LONGITUD_CADENA];	


	//Estructura que recibira informacion sobre el nodo remoto
	struct sockaddr_in servidor;

	//Paso2. Definicion de socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd<0){
		printf("Socket erroneo\n");
		exit(-1);
	}
	//Para reusar el puerto
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1},sizeof(int))<0){
		perror("setsockopt(SO_REUSADDR failed");
	}

	

	//Datos del servidor
	servidor.sin_family = AF_INET;
	servidor.sin_port = atoi(argv[2]);
	servidor.sin_addr.s_addr = inet_addr(argv[1]);
	
	//Paso 3.Conectarnos al servidor
	int codigo_connect = connect(sockfd, (struct sockaddr *)&servidor, sizeof(struct sockaddr));
	if(codigo_connect < 0){
		printf("ERROR: connect no exitoso\n");
		exit(-1);
	}else{
		printf("Conexion establecida correctamente...\n");
	}	
	//!strstr(msgtoserver,"exit") && !strstr(msgtoserver,"exit")
	while(1){
		bzero(msgtoserver,MAXIMA_LONGITUD_CADENA);
		bzero(msgfromserver,MAXIMA_LONGITUD_CADENA);
		printf("Mensaje para servidor: \n");
		fgets(msgtoserver, MAXIMA_LONGITUD_CADENA, stdin);
		//strtok(msgtoserver, "\n");				
		int num_bytes = send(sockfd, msgtoserver, strlen(msgtoserver),0);
		if(num_bytes < 0){
				printf("ERROR: send no exitoso\n");
				exit(-1);				
		}
		//else{
			//printf("Mensaje enviado\n");
		//}

		if(!strstr(msgtoserver,"exit")){
			int num_bytes_r = recv(sockfd, msgfromserver, sizeof(msgfromserver),0);				
			if(num_bytes_r < 0){
				printf("ERROR: recv no exitoso\n");
				exit(-1);				
			}				
			if(strstr(msgfromserver,"exit")){
				printf("exit por servidor\n");
				close(codigo_connect);	
				return 0;
			}

			msgfromserver[num_bytes_r='\0'];
			printf("Mensaje del servidor: %s\n", msgfromserver);		
		}else{
			printf("exit por Cliente\n");
			close(codigo_connect);	
			return 0;
		}	
	}			
	close(codigo_connect);
}