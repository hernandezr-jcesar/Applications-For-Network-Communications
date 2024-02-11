#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXIMA_LONGITUD_CADENA 100

int main(int argc, char const *argv[])
{
	if (argc =! 3)
	{
		printf("Uso: cliente [DIRECCION IP] [PUERTO]\n");		
		exit(-1);
	}
	char msgtoclient[MAXIMA_LONGITUD_CADENA];
	char msgfromclient[MAXIMA_LONGITUD_CADENA];	

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd<0){
		printf("Error opening socket\n");
		exit(-1);
	}
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1},sizeof(int))<0){
		perror("setsockopt(SO_REUSADDR failed");
	}
	
	struct sockaddr_in servidor, cliente;
	servidor.sin_addr.s_addr = inet_addr(argv[1]);
	servidor.sin_port = atoi(argv[2]);
	servidor.sin_family = AF_INET;	
	int codigo_bind = bind(sockfd, (struct sockaddr *)&servidor, sizeof(servidor));

	if(codigo_bind < 0){
		printf("ERROR: bind no exitoso\n");
		exit(-1);
	}

	if(listen(sockfd,4)<0){
		printf("Error: listen no exitoso\n");
		exit(-1);
	}
	while(1){
		printf("Mensaje para Cliente:\n");		

		//lo que devuelve accept es el identificador del canal del comunicacion credo con el cliente		
		int size = sizeof(cliente);
		int c_accept = accept(sockfd, (struct sockaddr *)&cliente,&size);
		if(c_accept < 0){
			printf("ERROR: accept no exitoso\n");
			exit(-1);
		}
		
		while(1){
			int num_bytes_r = recv(c_accept, msgfromclient, sizeof(msgfromclient),0);

			if(num_bytes_r <0){
				printf("ERROR: recv no exitoso\n");
				exit(-1);
			}
			if(strcmp(msgfromclient, "exit")==0){											
				close(c_accept);	
			}		
			fgets(msgtoclient, MAXIMA_LONGITUD_CADENA, stdin);
			strtok(msgtoclient, "\n");

			if(strcmp(msgfromclient, "exit")==0){								
				//Comienza fase de comunicacion
				int num_bytes_r = recv(c_accept, msgfromclient, sizeof(msgfromclient),0);

				if(num_bytes_r <0){
					printf("ERROR: recv no exitoso\n");
					exit(-1);
				}	
				printf("Ejecucion de Cliente finalizada. Esperando otro cliente\n");
				close(c_accept);	

			}else{
				int num_bytes = send(c_accept, msgtoclient, strlen(msgtoclient),0);
				if(num_bytes<0){
					printf("ERROR: send no exitoso\n");
					exit(-1);
				}	

				//Comienza fase de comunicacion
				int num_bytes_r = recv(c_accept, msgfromclient, sizeof(msgfromclient),0);

				if(num_bytes_r <0){
					printf("ERROR: recv no exitoso\n");
					exit(-1);
				}	
				msgfromclient[num_bytes_r] =  '\0';
				printf("Mensaje del cliente: %s\n", msgfromclient);						
			}
	}						
				
		close(c_accept);		
	}
	
	return 0;
}