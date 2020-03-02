#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "lines.h"



int main(int argc, char *argv[])
{
    int sd, newsd;
	int val;
	int err;
    socklen_t size;
    struct sockaddr_in server_addr, client_addr;

    sd =  socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd < 0) {
        perror("Error en socket");
        exit(1);
    }

    val = 1;
    err = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &val,
        sizeof(int));
    if (err < 0) {
        perror("Error en opction");
        exit(1);
    }


    // completar .....
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2800);
    server_addr.sin_addr.s_addr= INADDR_ANY;

    /* bind */
    if(bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Erroren el bind\n");
        return(-1);
    }
    
    listen(sd, SOMAXCONN);
    for (;;) {
        newsd = accept(sd, (struct sockaddr*) &client_addr, &size);
        if (newsd < 0) {
            printf("Error en el accept");
            return(-1);
        }
        /* transferir datos sobre newsd*/
        
        
        for(;;) {
            char inputBuff[256];
            int lline = readLine(newsd, inputBuff, 256);
            if(lline == -1){
                printf("error2");
                return -1;
            }
            printf("%s\n", inputBuff);
            if(strcmp(inputBuff, "EXIT") == 0) {
                break;
            }
         
            int mesg = enviar(newsd, inputBuff, strlen(inputBuff)+1);
            if(mesg == -1){
                printf("error enviar");
                return -1;
            }
            
        }
        /* procesarla peticiónutilizando newsd*/
        close(newsd);
    }
    close(sd);
    return(0);
}

