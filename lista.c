#include <stdio.h>
#include <pthread.h>
#include <mqueue.h>
#include "mensaje.h"
#include "implementacion.h"

TipoLista lista = NULL;

pthread_mutex_t mutex_lista;

void inicializarpatata(){
        pthread_mutex_init(&mutex_lista, NULL);
}

void *inita(void *pet){
    
    struct peticion* a = (struct peticion*) pet;
    struct respuesta ress;
    int cod_error = 0;
    
    mqd_t cc = mq_open(a->cola_cliente, O_WRONLY);    //Se crea la cola del cliente
    if(cc == -1) {
        perror("INITA: Ha ocurrido un error al crear la cola del cliente \n");
    }
    pthread_mutex_lock(&mutex_lista);
    cod_error = insertar(&lista, a->clave, a->n);
    pthread_mutex_unlock(&mutex_lista);
    ress.cod_error = cod_error;
    
    printf("%s\n", a->cola_cliente);
    
    int checkSend = mq_send(cc, (const char*) &ress, sizeof(ress), 0);  //Se envía la cola
    if(checkSend == -1){
        perror("INITA: Ha ocurrido un error al enviar a la cola del cliente \n");
        if (mq_close(cc) == -1) {
            perror("INITA: Error al cerrar la cola del cliente");
        }
        if (mq_unlink(a->cola_respuesta) == -1) {
            perror("INITA: Error al desvincular la cola");
        }
    }
    if (mq_close(cc) == -1) {
        perror("INITA: Error al cerrar la cola del cliente");
    }
}

void *seta(void *pet){
    struct peticion* a = (struct peticion*) pet;
    struct respuesta ress;
    int cod_error = 0;
    printf("REPATATA");
    mqd_t cc = mq_open(a->cola_cliente, O_WRONLY); //Se crea la cola del cliente
    if(cc == -1) {
        perror("SETA: Ha ocurrido un error al crear la cola del cliente \n");
    }
        pthread_mutex_lock(&mutex_lista);

    cod_error = meter(&lista, a->clave, a->n, a->valor);
     pthread_mutex_unlock(&mutex_lista);
    ress.cod_error = cod_error;
    int checkSend = mq_send(cc, (const char*) &ress, sizeof(ress), 0); //Se envía la cola
    if(checkSend == -1){
        perror("SETA: Ha ocurrido un error al enviar a la cola del cliente \n");
        if (mq_close(cc) == -1) {
            perror("SETA: Error al cerrar la cola del cliente");
        }
        if (mq_unlink(a->cola_respuesta) == -1) {
            perror("SETA: Error al desvincular la cola");
        }
    }
    if (mq_close(cc) == -1) {
        perror("SETA: Error al cerrar la cola del cliente");
    }
}

void *geta(void *pet){
    struct peticion* a = (struct peticion*) pet;
    struct respuesta ress;
    int cod_error = 0;
    int valor = 0;
    
    mqd_t cc = mq_open(a->cola_cliente, O_WRONLY);    //Se crea la cola del cliente
    if(cc == -1) {
        printf("GETA %s: Ha ocurrido un error al crear la cola del cliente \n",a->cola_cliente);
    }
    
    pthread_mutex_lock(&mutex_lista);
    valor = recuperar(&lista, a->clave, a->n);
    pthread_mutex_unlock(&mutex_lista);

    if(valor == -1){
        cod_error = -1;
    }
    ress.cod_error = cod_error;
    ress.valor = valor;
    
    int checkSend = mq_send(cc, (const char*) &ress, sizeof(ress), 0);  //Se envía la cola
    if(checkSend == -1){
        perror("GETA: Ha ocurrido un error al enviar a la cola del cliente \n");
        if (mq_close(cc) == -1) {
            perror("GETA: Error al cerrar la cola del cliente");
        }
        if (mq_unlink(a->cola_respuesta) == -1) {
            perror("GETA: Error al desvincular la cola");
        }
    }
    if (mq_close(cc) == -1) {
        perror("GETA: Error al cerrar la cola del cliente");
    }
}

void *freea(void *pet){
    struct peticion* a = (struct peticion*) pet;
    struct respuesta ress;
    int cod_error = 0;
    
    mqd_t cc = mq_open(a->cola_cliente, O_WRONLY);    //Se crea la cola del cliente
    if(cc == -1) {
        perror("FREEA: Ha ocurrido un error al crear la cola del cliente \n");
    }
    pthread_mutex_lock(&mutex_lista);
    cod_error = liberar(&lista, a->clave);
    pthread_mutex_unlock(&mutex_lista);
    ress.cod_error = cod_error;
    
    int checkSend = mq_send(cc, (const char*) &ress, sizeof(ress), 0);  //Se envía la cola
    if(checkSend == -1){
        perror("FREEA: Ha ocurrido un error al enviar a la cola del cliente \n");
        if (mq_close(cc) == -1) {
            perror("FREEA: Error al cerrar la cola del cliente");
        }
        if (mq_unlink(a->cola_respuesta) == -1) {
            perror("FREEA: Error al desvincular la cola");
        }
    }
    if (mq_close(cc) == -1) {
        perror("FREEA: Error al cerrar la cola del cliente");
    }
}

