#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/stat.h>
#define MAX 100
#define PORT 8080
#define SA struct sockaddr


int main(int argc, char ** argv)
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
   
    if (argc !=5){
        printf("Introducir [Direccion IP] [PUERTO] [DIRECCION CLIENTE] [PUERTO CLIENTE]\n");
        exit(-1);
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("Error en creación de socket\n");
        exit(0);
    }
    bzero(&servaddr, sizeof(servaddr));
   
   //especificar hacia qué dirección y hacie qué puerto

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]); //INADDR_ANY puede dar problemas
    servaddr.sin_port = htons(atoi(argv[2]));

    cli.sin_family = AF_INET;
    cli.sin_addr.s_addr = inet_addr(argv[3]);
    cli.sin_port = htons(atoi(argv[4]));

    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr))) < 0) {
        printf("Error en bind\n");
        exit(-1);
    }
    

    while(1){
   
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, MAX);
   

        //read(connfd, buff, sizeof(buff));
        int varSizeof = sizeof(cli);
        recvfrom(sockfd,buff,sizeof(buff),0,(SA*)&cli,&varSizeof);

        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Saliendo del cliente\n");
            //return;
        }
        else{
            printf("Cliente: %s\t Servidor : ", buff);
            bzero(buff, MAX);
            n = 0;
            while ((buff[n++] = getchar()) != '\n');
    
            //write(connfd, buff, sizeof(buff));
            sendto(sockfd,buff,sizeof(buff),0,(SA*)&cli,varSizeof);
        }
        
        /*
        if (strncmp("exit", buff, 4) == 0) {
            printf("Saliendo del servidor\n");
            break;
        }
        */
        
    }

    }
    
   
    close(sockfd);
}