#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <stdbool.h>
#include "Gficheros.c"
#include "lines.h"

#define MAX_THREADS 8
pthread_attr_t attr;
pthread_t thid[MAX_THREADS];
pthread_mutex_t m;
pthread_cond_t c;
int condicio = 0;
int contadorT = 0;

void print_usage() {
	    printf("Usage: server -p puerto \n");
}


void tratar_peticion (int *s) {
    int s_local;
    
    pthread_mutex_lock(&m);
    contadorT++;
    s_local = *s;
    condicio = 1;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
	char user[256];
    /* tratar la petición utilizando el descriptor s_local */
    
    //variable para enviar mensaje de feedback a cliente
    char oka[256];
    sprintf(oka, "nada");

    /* transferir datos sobre newsd*/
    for(;;) {
        char inputBuff[256];
        int lline = readLine(s_local, inputBuff, 256);
        if(lline == -1){
            printf("error2\n");
	    break;
            
        }
	//de la forma planteada, los errores deben mostrarlos 
	//las funciones llamadas dentro de los IFs
	printf("%s\n", inputBuff);

	if(0 == strcmp(inputBuff,"REGISTER")){
		//si el comando es register, cogemos el parámetro
        	lline = readLine(s_local, inputBuff, 256);
	        if(lline == -1){
        	    printf("error2\n");
		    break;
		}
		//pequeño printf de control		
		printf("Registro de usuario solicitado");
		int conclusion;
		conclusion = registro(inputBuff);
		//ponemos en el char de enviar, registrado, para feedback de cliente
		if(conclusion == 0){sprintf(oka, "registrado");}
    	}
	else if(0 == strcmp(inputBuff,"CONNECT")){
	
        lline = readLine(s_local, inputBuff, 256);
	    if(lline == -1){
        	    printf("error2\n");
		    break;
		}
		//pequeño printf de control		
		printf("conexion de usuario solicitado");

		//copiar el nombre del usuario al hacer connect para tenerlo a la hora de publicar y borrar contenidos
		strcpy(user, inputBuff);
        printf("%s", user);
		
		//aqui pilla el puerto
		lline = readLine(s_local, inputBuff, 256);
	    if(lline == -1){
        	    printf("error2\n");
		    break;
		}
		
		int conclusion;
		conclusion = conectar(user, s_local, inputBuff);
		//ponemos en el char de enviar, registrado, para feedback de cliente
		if(conclusion == 0){sprintf(oka, "conectado");}
    	}
	else if(0 == strcmp(inputBuff, "UNREGISTER")){

        //si el comando es unregister, cogemos el parámetro
        	lline = readLine(s_local, inputBuff, 256);
	        if(lline == -1){
        	    printf("error2\n");
		    	break;
		}
		//pequeño printf de control		
		printf("darse de baja de usuario solicitado");

		int conclusion;
		conclusion = baja(inputBuff);
		//ponemos en el char de enviar, registrado, para feedback de cliente
		if(conclusion == 0){sprintf(oka, "registro eliminado");}
    	
	}
	else if(0 == strcmp(inputBuff, "DISCONNECT")){
		sprintf(oka, "desconectado");
	}
	else if(0 == strcmp(inputBuff, "PUBLISH")){
	    //si el comando es unregister, cogemos el primer parámetro, que es el nombre del fichero
        lline = readLine(s_local, inputBuff, 256);
	    if(lline == -1){
            printf("error2\n");
			break;
		}

		//coger nombre del archivo
		char name[256];
		strcpy(name, inputBuff);
        
		//coger la descripcion
		lline = readLine(s_local, inputBuff, 256);
	    if(lline == -1){
            printf("error2\n");
			break;
		}

		//pequeño printf de control		
		printf("publicar contenido del usuario\n");
        printf("%s\n", user);

		int conclusion;
		conclusion = publicar(inputBuff, name, user);
		//ponemos en el char de enviar, registrado, para feedback de cliente
		if(conclusion == 0){sprintf(oka, "publicado");}
    	
	}
	else if(0 == strcmp(inputBuff, "DELETE")){
	 //si el comando es unregister, cogemos el primer parámetro, que es el nombre del fichero
        lline = readLine(s_local, inputBuff, 256);
	    if(lline == -1){
            printf("error2\n");
			break;
		}

		//pequeño printf de control		
		printf("eliminar contenido del usuario");

		int conclusion;
		conclusion = borrar(inputBuff, user);
		//ponemos en el char de enviar, registrado, para feedback de cliente
		if(conclusion == 0){sprintf(oka, "eliminado");}
    	
	}
	else if (0 == strcmp(inputBuff, "LIST_USERS")){
		sprintf(oka, "lista de usuarios");
	}
	else if(0 == strcmp(inputBuff, "LIST_CONNECT")){
		sprintf(oka, "lista de usuarios conectados");
	}
	else if(0 == strcmp(inputBuff, "GET_FILE")){
		sprintf(oka, "archivo obtenido");
	}

        int mesg2 = enviar(s_local, oka, strlen(oka)+1);
        if(mesg2 == -1){
            printf("error enviar2\n");
	    	break;
           
        }        
    }//fin del for infinito, fin de conexion

    printf("Cerrando la conexion\n");
    close(s_local);
    pthread_exit(NULL);

}

int main(int argc, char *argv[]) {
	int  option = 0;
	char port[256]= "";
	

	while ((option = getopt(argc, argv,"p:")) != -1) {
		switch (option) {
		    	case 'p' : 
				strcpy(port, optarg);
		    		break;
		    	default: 
				print_usage(); 
		    		exit(-1);
		    }
	}
	if (strcmp(port,"")==0){
		print_usage(); 
		exit(-1);
	}


	printf("Port: %s\n", port);
	int puerto = atoi(port);

	//  Empieza lo bueno de prueba
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

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(puerto);
    server_addr.sin_addr.s_addr= INADDR_ANY;

    /* bind */
    if(bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Erroren el bind\n");
        return(-1);
    }
    
    listen(sd, SOMAXCONN);
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    for (;;) {
	printf("Esperando conexión\n");

        newsd = accept(sd, (struct sockaddr*) &client_addr, &size);
        if (newsd < 0) {
            printf("Error en el accept");
            return(-1);
        }
        pthread_create(&thid[contadorT], &attr, (void*)tratar_peticion, &newsd);
        
        /* esperar a que el hijo copie el descriptor */
        
        pthread_mutex_lock(&m);
        
        while(condicio == 0) {
            pthread_cond_wait(&c, &m);
        }
        condicio = 0;
        pthread_mutex_unlock(&m);
        
        /* procesarla peticiónutilizando newsd*/
        
    }
    close(sd);
	//termina la prueba de registrar
	
	return 0;
}
	
