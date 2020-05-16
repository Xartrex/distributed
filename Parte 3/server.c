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
#include <signal.h>

#define MAX_THREADS 8
pthread_attr_t attr;
pthread_t thid[MAX_THREADS];
pthread_mutex_t m;
pthread_cond_t c;
int condicio = 0;
int contadorT = 0;

int sd;

void print_usage() {
	    printf("Usage: server -p puerto \n");
}

//Manejador de señal para CTRL+C
void sigintHandler(int sig_num) 
{ 
    printf("s> SIG HANDLER, CLOSING SERVER");
    signal(SIGINT, sigintHandler); 
    close(sd);
    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&c);
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
            //printf("Error readLine tratar petición\n");
	    break;
            
        }
	//de la forma planteada, los errores deben mostrarlos 
	//las funciones llamadas dentro de los IFs
	////printf("%s\n", inputBuff);

	if(0 == strcmp(inputBuff,"REGISTER")){
		//Si el comando es register, cogemos el parámetro
        lline = readLine(s_local, inputBuff, 256);
        if(lline <= 0){
            //printf("Error en readLine\n");
            break;
		}
		
		//pequeño printf de control		
		//printf("Registro de usuario solicitado");
        strcpy(user, inputBuff);
        printf("s> OPERATION FROM %s\n", user);

        //Se ejecuta la funcón correspondiente
		int conclusion = registro(inputBuff);
		//ponemos en el char de enviar, registrado, para feedback de cliente
        
        char resultado[2];
        sprintf(resultado, "%d", conclusion);
        if(enviar(s_local, resultado, strlen(resultado)+1) < 0){
            //printf("Error al enviar\n");
            exit(0);
        }
    }
	else if(0 == strcmp(inputBuff,"CONNECT")){
	
        lline = readLine(s_local, inputBuff, 256);
	    if(lline == -1){
            //printf("Error en el readLine\n");
		    break;
		}
		//pequeño printf de control		
		//printf("conexion de usuario solicitado");

		//copiar el nombre del usuario al hacer connect para tenerlo a la hora de publicar y borrar contenidos
		 strcpy(user, inputBuff);
        printf("s> OPERATION FROM %s\n", user);
		
		//aqui recibe el puerto
		lline = readLine(s_local, inputBuff, 256);
	    if(lline == -1){
            //printf("Error en el readLine\n");
		    break;
		}
		
		//Se ejecuta la función correspondiente y se envía el coódigo de error
        int conclusion = conectar(user, s_local, inputBuff);
        //printf("res = %d\n",conclusion);
        char resultado[2];
        sprintf(resultado, "%d", conclusion);
        if(enviar(s_local, resultado, strlen(resultado)+1) < 0){
            //printf("Error al enviar\n");
            exit(0);
        }
    }
	else if(0 == strcmp(inputBuff, "UNREGISTER")){

        //si el comando es unregister, cogemos el parámetro
        lline = readLine(s_local, inputBuff, 256);
        if(lline == -1){
            //printf("Error en el readLine\n");
            break;
		}
		//pequeño printf de control		
		strcpy(user, inputBuff);
        printf("s> OPERATION FROM %s\n", user);

        //Se ejecuta la función correspondiente y se envía el coódigo de error
		int conclusion = baja(inputBuff);
        char resultado[2];
        sprintf(resultado, "%d", conclusion);
        if(enviar(s_local, resultado, strlen(resultado)+1) < 0){
            //printf("Error al enviar\n");
            exit(0);
        }
	}
	else if(0 == strcmp(inputBuff, "DISCONNECT")){
        
        lline = readLine(s_local, inputBuff, 256);
        if(lline == -1){
            //printf("Error en el readLine\n");
            break;
        }
        strcpy(user, inputBuff);
        printf("s> OPERATION FROM %s\n", user);
        
        //Se ejecuta la función correspondiente y se envía el coódigo de error
        int conclusion = desconectar(inputBuff);
        char resultado[2];
        sprintf(resultado, "%d", conclusion);
        if(enviar(s_local, resultado, strlen(resultado)+1) < 0){
            //printf("Error al enviar\n");
            exit(0);
        }
	}
    else if(0 == strcmp(inputBuff, "PUBLISH")){

        lline = readLine(s_local, inputBuff, 256);
        if(lline == -1){
            //printf("Error en el readLine\n");
            break;
        }
        //coger el nombre de usuario
        char usery[256];
        strcpy(usery, inputBuff);
        printf("s> OPERATION FROM %s\n", user);
        
        lline = readLine(s_local, inputBuff, 256);
        if(lline == -1){
            //printf("Error en el readLine\n");
            break;
        }
        //coger nombre del archivo
        char name[256];
        strcpy(name, inputBuff);
        
        //coger la descripcion
        lline = readLine(s_local, inputBuff, 256);
        if(lline == -1){
            //printf("Error en el readLine\n");
            break;
        }

        //pequeño printf de control
        // printf("publicar contenido del usuario\n");
        //printf("%s\n", user);

        //Se ejecuta la función correspondiente y se envía el coódigo de error
        int conclusion = publicar(inputBuff, name, usery);
        char resultado[2];
        sprintf(resultado, "%d", conclusion);
        if(enviar(s_local, resultado, strlen(resultado)+1) < 0){
            //printf("Error al enviar\n");
            exit(0);
        }            
	}
	else if(0 == strcmp(inputBuff, "DELETE")){

        lline = readLine(s_local, inputBuff, 256);
        if(lline == -1){
            //printf("Error en el readLine\n");
            break;
        }
        //coger el nombre de usuario
        char usery[256];
        strcpy(usery, inputBuff);
        printf("s> OPERATION FROM %s\n", user);
        
        lline = readLine(s_local, inputBuff, 256);
        if(lline == -1){
            //printf("Error en el readLine\n");
            break;
        }
        //coger nombre del archivo
        char name[256];
        strcpy(name, inputBuff);

        //Se ejecuta la función correspondiente y se envía el coódigo de error
        int conclusion = borrar(name, usery);
        char resultado[2];
        sprintf(resultado, "%d", conclusion);
        if(enviar(s_local, resultado, strlen(resultado)+1) < 0){
            //printf("Error al enviar\n");
            exit(0);
        }
	}
	else if (0 == strcmp(inputBuff, "LIST_USERS")){

        lline = readLine(s_local, inputBuff, 256);
        if(lline == -1){
            //printf("Error en el readLine\n");
            break;
        }
        //coger el nombre de usuario
        char usery[256];
        strcpy(usery, inputBuff);
        printf("s> OPERATION FROM %s\n", user);
        
        //Se ejecuta la función correspondiente para obtener si el usuario está conectado
        int conectados = list_users_connected(usery);
        char resultado[2];
        sprintf(resultado, "%d", conectados);
        if(enviar(s_local, resultado, strlen(resultado)+1) < 0){
            //printf("Error al enviar\n");
            exit(0);
        }
        
        //Primer control de errores
        if(conectados == 0) {
            //Se enumeran los usuarios conectados
            conectados = list_users_contador();
            //printf("Usuarios conectados : %d\n", conectados);
            sprintf(resultado, "%d", conectados);
            if(enviar(s_local, resultado, strlen(resultado)+1) < 0){
                //printf("Error al enviar\n");
                exit(0);
            }
            
            if (conectados > 0) {
                //Si hay usuarios conectados se envían sus datos con la función correspondiente
                list_users(s_local);
            }
        }
    }
	else if(0 == strcmp(inputBuff, "LIST_CONTENT")){
        lline = readLine(s_local, inputBuff, 256);
        if(lline == -1){
            //printf("Error2\n");
            break;
        }
        char usery[256];
        strcpy(usery, inputBuff);
        printf("s> OPERATION FROM %s\n", user);

        lline = readLine(s_local, inputBuff, 256);
        if(lline == -1){
            //printf("Error en el readLine\n");
            break;
        }
        
        //Se comprueba si el usuario está conectado
        int conclusion = list_contenido_connected(usery,inputBuff);

        char resultado[2];
        sprintf(resultado, "%d", conclusion);
        if(enviar(s_local, resultado, strlen(resultado)+1) < 0){
            //printf("Error al enviar\n");
            exit(0);
        }

        //Primer control de errores
        if(conclusion==0){
            //Mismo proceso que para list_users
            conclusion = list_contenido_contador(inputBuff);
            sprintf(resultado, "%d", conclusion);
            if(enviar(s_local, resultado, strlen(resultado)+1) < 0){
                //printf("Error al enviar\n");
                exit(0);
            }

            if (conclusion > 0) {
                list_contenido(inputBuff,s_local);
            }
        }
	}
    int mesg2 = enviar(s_local, oka, strlen(oka)+1);
        if(mesg2 == -1){
            //printf("Error enviar2\n");
            break;
        }    
    printf("s>\n");
    }//fin del for infinito, fin de conexion
    
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



	int puerto = atoi(port);

	int newsd;
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
        //printf("Erroren el bind\n");
        return(-1);
    }
    char clientip[20];
    strcpy(clientip, inet_ntoa(server_addr.sin_addr));

    printf("Init server %s:%d\n", clientip, puerto);
    listen(sd, SOMAXCONN);
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    signal(SIGINT, sigintHandler);
    for (;;) {
	//printf("Esperando conexión\n");
   

        newsd = accept(sd, (struct sockaddr*) &client_addr, &size);
        if (newsd < 0) {
            //printf("Error en el accept");
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
	
