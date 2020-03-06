// init() //no necesita argumentos
// set//devuelve codigo de error, argumentos:clave y valor
// get//devuelve codigo de error y valor, argumentos:clave
// free//devuelve codigo de error, argumentos: clave


#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include "mensaje.h"
#include "implementacion.h"

pthread_mutex_t mutex_mensaje;
int mensaje_no_copiado = 0;
pthread_cond_t cond_mensaje;
pthread_attr_t t_attr;
pthread_t thid;

void tratar_peticion(struct peticion *a){
    pthread_mutex_lock(&mutex_mensaje);
    struct peticion mess = *a;
    mensaje_no_copiado = 1;
    pthread_cond_signal(&cond_mensaje);
    pthread_mutex_unlock(&mutex_mensaje);

    //Dependiendo del código de operación se realiza una función u otra
    if(mess.operacion == 0){
        pthread_create(&thid, &t_attr, inita, &mess);
    }
    if(mess.operacion == 1){
        pthread_create(&thid, &t_attr, seta, &mess);
    }
    if(mess.operacion == 2){
        pthread_create(&thid, &t_attr, geta, &mess);
    }
    if(mess.operacion == 3){
        pthread_create(&thid, &t_attr, freea, &mess);
    }
}


int main(){

    mqd_t q_servidor;
    struct peticion mess;
    struct mq_attr q_attr;
    
    q_attr.mq_maxmsg = 10;
    q_attr.mq_msgsize = sizeof(struct peticion);
 
    
    q_servidor = mq_open ("/SERVIDOR", O_CREAT|O_RDONLY, 0777, &q_attr);
    if(q_servidor == -1){
        perror("No se puede crear la cola de servidor");
        return 1;
    }
    
    pthread_mutex_init(&mutex_mensaje, NULL);
    pthread_cond_init(&cond_mensaje, NULL);
    pthread_attr_init(&t_attr);
    
    pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);
    
    while(true){
        int e = mq_receive(q_servidor, (char *) &mess, sizeof(struct peticion),0);
        if (e == -1) {
            perror("Ha ocurrido un error al recibir de la cola del cliente \n");
            if (mq_close(q_servidor) == -1) {
                perror("Ha ocurrido un error al cerrar la cola del servidor");
            }
            if (mq_unlink("/SERVIDOR") == -1) {
                perror("Ha ocurrido un error al desvincular la cola del servidor");
            }
        }
        
        tratar_peticion(&mess);
        
        while (mensaje_no_copiado = 0){
            pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
        }
        mensaje_no_copiado = 0;
    }

    if (mq_close(q_servidor) == -1) {
        perror("Ha ocurrido un error al cerrar la cola del servidor");
        return (-1);
    }
    if (mq_unlink("/SERVIDOR") == -1) {
        perror("Ha ocurrido un error al desvincular la cola del servidor");
        return (-1);
    }

}

