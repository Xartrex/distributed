#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include "lines.h"

#define MAX_LINE 	256

int main(int argc, char *argv[])
{
    int sd;
    struct sockaddr_in server_addr;
    struct hostent *hp;

    if (argc != 3) {
            printf("Uso: client <direccion_servidor> <puerto_servidor>\n");
            exit(0);
    }

    // se crea el socket
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd < 0){
            perror("Error en socket");
            exit(1);
    }

    // se prepara la dirección del servidor
    bzero((char *)&server_addr, sizeof(server_addr));
    hp = gethostbyname (argv[1]);
    if (hp == NULL) {
            perror("Error en gethostbyname");
            exit(1);
    }

    memcpy (&(server_addr.sin_addr), hp->h_addr, hp->h_length);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));

    // Completar.....
    // en primer lugar establecer la conexión
    if(connect(sd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        printf("Error en la conexión\n");
        return(0);
    }
    
    char buffer[MAX_LINE];
    int err = 0;
    int n;
    
    while (err != -1) {
        n = readLine(0, buffer, MAX_LINE);
        if (n!=-1){
            int fd = writeLine(sd, buffer, n);
            if(fd == -1){
                printf("error writeline");
                return -1;
            }
        }
        if(strcmp(buffer, "EXIT")==0){
            break;
        }
        enviar(1, buffer, strlen(buffer)+1);
        int recibo = recibir(sd, buffer, n);
        if(recibo == -1){
            printf("error recibir");
            return -1;
        }
    }
    
    close(sd);
    return (sd);
    return(0);
}

