#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<errno.h>
#include <sys/types.h>
#include <sys/stat.h>

extern int errno; 
#define RCVBUFFER 100

#define SIZE 2000

void send_file(FILE *fp, int sockfd);
struct sockaddr_in broadcastAddr;
  char flag[RCVBUFFER];
  char status1[6]="1";
  char status2[6]="2";
  char status3[6]="3";
  int tamMensaje;

int main (int argc, char **argv)
{
  int sock;
  
  unsigned short puertoBroadcast;
  char *IPBroadcast;
  int permisoBroadcast;

  //Paso 0: verificaci{on de parametros de entrada
  if(argc < 3){
    printf("Faltan parametros. Uso: 1.- IP 2.- Puerto");
    exit(1);
  }

  IPBroadcast = argv[1];
  puertoBroadcast = atoi(argv[2]);
  

  //Paso 1: Crea el socket Datagrama/UDP
  if((sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0){
    perror("Se genero el siguiente error en SOCKET ");
    printf("\nValor de errno = %d",errno);
    exit(1);
  }

  //paso 2: Permitirle al socket realizar broadcast
  permisoBroadcast = 1;
  if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *)&permisoBroadcast, sizeof(permisoBroadcast)) < 0){
    perror("Se genero un error en Setsockopt");
    printf("\nValor de errno = %d",errno);
    exit(1);
  }

  //Paso 3: LLenar la estructura de la direcci{on
  memset(&broadcastAddr,0,sizeof(broadcastAddr));
  broadcastAddr.sin_family = AF_INET;
  broadcastAddr.sin_addr.s_addr = inet_addr(IPBroadcast);
  broadcastAddr.sin_port = htons(puertoBroadcast);
  
  //Paso 4: hacer que no muera el servidor
  while(1){
  
  for(int i=0; i<5; i++)//manda ciclos de 5 archivos
  {
    

  tamMensaje = strlen(flag);
  strcpy(flag,status1);
  
    if(sendto(sock, flag,tamMensaje,0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) != tamMensaje){
      perror("Se genero un error en Sendto");
      printf("\nValor de errno = %d",errno);
      exit(1);
    }
    
  strcpy(flag,status1);


FILE *fp;
char *filename;
		
		
		filename = "foto.jpg";
    	  	fp = fopen(filename, "rb");
	  	if (fp == NULL) {
	    	   perror("[-]Error in reading file.");
	    	   exit(1);
	  		}
	  	send_file(fp, sock);
	  	fclose(fp);
	  	printf("envio exitoso");

	    
    strcpy(flag,status3);
  
    if(sendto(sock, flag,tamMensaje,0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) != tamMensaje){
      perror("Se genero un error en Sendto");
      printf("\nValor de errno = %d",errno);
      exit(1);
    }
       
    
        printf("archivo %d",i+1);
        fflush(stdout);
   sleep(2);
    }
    
    
  }


}











//////Funcion enviar archivo/////////////

void send_file(FILE *fp, int sockfd){
  int n;
  char buff[SIZE] = {0};
 
 
 while((n=fread(buff,sizeof(char),SIZE,fp))>0)
 {
 
 
     strcpy(flag,status2);
  
    if(sendto(sockfd, flag,tamMensaje,0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) != tamMensaje){
      perror("Se genero un error en Sendto");
      printf("\nValor de errno = %d",errno);
      exit(1);
    }
 
 	
 	if(n!=SIZE && ferror(fp))
 	{
 	printf("read file error");
 	exit(1);
 	
 	}
 
 
 
  if(sendto(sockfd, buff,n,0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) != n){
      perror("Se genero un error en Sendto");
      printf("\nValor de errno = %d",errno);
      exit(1);
    }

 	memset(buff,0,SIZE);
 
 
 }

}













