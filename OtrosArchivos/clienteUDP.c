#include <unistd.h> 
#include <arpa/inet.h> 
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> 
#include <sys/socket.h>
#include <pthread.h>

#define MAX 100
#define PORT 8080
#define SA struct sockaddr



 
int main(int argc, char ** argv)
{

    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    char * nombreCliente;
 
    // Creacion del socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Error en creación de socket\n");
        exit(-1);
    }
    bzero(&servaddr, sizeof(servaddr));
 
    printf("Conectado al servidor\n");
 
    cli.sin_family = AF_INET;
    cli.sin_addr.s_addr = inet_addr(argv[1]);
    cli.sin_port = htons(atoi(argv[2]));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[3]); //INADDR_ANY puede dar problemas
    servaddr.sin_port = htons(atoi(argv[4]));
 
    if (bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) < 0) {
        printf("Error en bind\n");
        exit(-1);
    }

    char buff[MAX];
    //int sockfd, connfd;
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Introduce una cadena : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n');

        int varSizeof = sizeof(servaddr);

        //write(sockfd, buff, sizeof(buff));
        sendto(sockfd,buff,sizeof(buff),0,(SA*)&servaddr,varSizeof);

        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Saliendo del cliente\n");
            exit(0);
        }
        bzero(buff, sizeof(buff));
        //read(sockfd, buff, sizeof(buff));
        
        recvfrom(sockfd,buff,sizeof(buff),0,(SA*)&servaddr,&varSizeof);

        printf("Servidor: %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Saliendo del cliente\n");
            exit(0);
        }
    }
 
    close(sockfd);
}