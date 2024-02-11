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


void write_file(int sockfd,char *filename);
  char flag[RCVBUFFER];
  int tamRecv;
  
int main (int argc, char **argv)
{
  int sock;
  struct sockaddr_in broadcastAddr;
  unsigned short puertoBroadcast;


  //Paso 0: verificaci{on de parametros de entrada
  if(argc < 2){
    printf("Faltan parametros. Uso: 1.- Puerto");
    exit(1);
  }

  puertoBroadcast = atoi(argv[1]);

  //Paso 1: Crea el socket Datagrama/UDP
  if((sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0){
    perror("Se genero el siguiente error en SOCKET ");
    printf("\nValor de errno = %d",errno);
    exit(1);
  }

  //Paso 2: LLenado de la estructura
  memset(&broadcastAddr,0,sizeof(broadcastAddr));
  broadcastAddr.sin_family = AF_INET;
  broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  broadcastAddr.sin_port = htons(puertoBroadcast);


//Establece la reutilizacion de la direccion para multiples clientes

 int opt_en=1;       
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,&opt_en, 
          sizeof(opt_en)) < 0)
        {
        printf("setsockopt() failed");   
	exit(0);

}


  //Paso 3: Publicacion del puerto broadcast
  if(bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0){
    perror("Se genero el siguiente error en BIND ");
    printf("\nValor de errno = %d",errno);
    exit(1);
  }

  //recibe 5 archivos y muere
  for(int i=0; i<5; i++){
  
 
 
  if((tamRecv = recvfrom(sock,flag,RCVBUFFER, 0, NULL,0)) < 0){
      perror("Se genero el siguiente error en RECvFROM ");
      printf("\nValor de errno = %d",errno);
      exit(1);
    }
  
    flag[tamRecv] = '\0';
   // printf("\nMensaje = %s\n",flag);
    
    if(strcmp(flag,"1")==0)
  
  {
  	
 	 char *filename;
	 char name[100]="rec";
	 char n=i+'0';
	 name[2]=n;
	 	
	 filename=name;
	 				
	 write_file(sock,filename);
		
	 printf("Archivo recibido\n");  
  	fflush(stdout);
  
  }
  else{
  
   printf("Transmision de archivo esta en curso\n");  
   i=i-1;
  }
  
  
  
  }
  printf("5 archivos recibidos");
  fflush(stdout);
  close(sock);
  exit(0);

}





void write_file(int sockfd,char *filename){
	
 	ssize_t n;
   	FILE *fp = fopen(filename, "wb");
	char buff[SIZE]={0};
	 
	
	 
	 while(1)
	 {
	 
	 
	   if((tamRecv = recvfrom(sockfd,flag,RCVBUFFER, 0, NULL,0)) < 0){
      perror("Se genero el siguiente error en RECvFROM ");
      printf("\nValor de errno = %d",errno);
      exit(1);
    }
  
    flag[tamRecv] = '\0';
  
    if(strcmp(flag,"3")==0)
    {
    printf("\nMensaje = %s\n",flag);
    break;
    }
    
    
       if((n = recvfrom(sockfd,buff,SIZE, 0, NULL,0)) < 0){
      perror("Se genero el siguiente error en RECvFROM ");
      printf("\nValor de errno = %d",errno);
      exit(1);
    }
	 
	 	if(n == -1)
	 	{
	 		printf("error");
	 		exit(1);	
	 	
	 	}
	 	
	 	if(fwrite(buff,sizeof(char), n, fp) !=n)
	 	{
	 	
	 		printf("error");
	 		exit(1);
	 	}
	 	fflush(stdout);
	 	fflush(fp);
	 	memset(buff,0,SIZE);
	 	
	 	
	 }

 fclose(fp);
}
 

