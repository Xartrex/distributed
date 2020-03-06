#include "mensaje.h"
#include "array.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

int get (char *clave, int i, int *valor) {
    struct peticion p;
    struct respuesta r;
    char nombrecola[128];
    sprintf(nombrecola,"/micola-%d", getpid());
    
    struct mq_attr q_attr;
    
    q_attr.mq_maxmsg = 10;
    q_attr.mq_msgsize = sizeof(r);
    
    printf("GET: Inicializar colas\n");
    
    mqd_t cc = mq_open(nombrecola, O_CREAT|O_RDONLY, 0777, &q_attr);  //Se crea la cola del cliente
    if(cc == -1) {
        perror("GET: Ha ocurrido un error al crear la cola del cliente \n");
    }
    
    mqd_t cs = mq_open("/SERVIDOR", O_WRONLY);    //Se crea la cola del servidor
    if(cs == -1) {
        perror("GET: Ha ocurrido un error al crear la cola del servidor \n");
    }
    
    p.operacion = 2; //Definido en servidor.c
    strcpy(p.clave, clave);
    strcpy(p.cola_cliente, nombrecola);

    p.n = i;
    
    int checkSend = mq_send(cs, (const char*) &p, sizeof(p), 0);    //Se envía la cola del servidor
    if(checkSend == -1) {
        perror("GET: Ha ocurrido un error al enviar a la cola del servidor \n");
        if (mq_close(cc) == -1) {
            perror("GET: Error al cerrar la cola del cliente");
        }
        if (mq_close(cs) == -1) {
            perror("GET: Error al cerrar la cola del servidor");
        }
        if (mq_unlink(nombrecola) == -1) {
            perror("GET: Error al desvincular la cola");
        }
        return(-1);
    }
    
    int e = mq_receive(cc, (char*) &r, sizeof(r), 0);   //Se recibe la cola del cliente
    if (e == -1) {
        perror("GET: Ha ocurrido un error al recibir de la cola del servidor \n");
        if (mq_close(cc) == -1) {
            perror("GET: Error al cerrar la cola del cliente");
        }
        if (mq_close(cs) == -1) {
            perror("GET: Error al cerrar la cola del servidor");
        }
        if (mq_unlink(nombrecola) == -1) {
            perror("GET: Error al desvincular la cola");
        }
        return(-1);
    }
    
    *valor = r.valor;
    if (mq_close(cc) == -1) {
        perror("GET: Error al cerrar la cola del cliente");
    }
    if (mq_close(cs) == -1) {
        perror("GET: Error al cerrar la cola del servidor");
    }
    if (mq_unlink(nombrecola) == -1) {
        perror("GET: Error al desvincular la cola");
    }
    
    return r.cod_error;
    
}

int set (char *clave, int i, int valor) {
    struct peticion p;
    struct respuesta r;
    
    char nombrecola[128];
    sprintf(nombrecola,"/micola-%d", getpid());
    
    struct mq_attr q_attr;
    
    q_attr.mq_maxmsg = 10;
    q_attr.mq_msgsize = sizeof(r);
    
    printf("SET: Inicializar colas\n");
    
    mqd_t cc = mq_open(nombrecola, O_CREAT|O_RDONLY, 0777, &q_attr);  //Se crea la cola del cliente
    if(cc == -1) {
        perror("SET: Ha ocurrido un error al crear la cola del cliente \n");
    }
    
    mqd_t cs = mq_open("/SERVIDOR", O_WRONLY);    //Se crea la cola del servidor
    if(cs == -1) {
        perror("SET: Ha ocurrido un error al crear la cola del servidor \n");
    }
    
    p.operacion = 1; //Definido en servidor.c
    strcpy(p.clave, clave);
    strcpy(p.cola_cliente, nombrecola);
    p.n = i;
    p.valor = valor;
    
    int checkSend = mq_send(cs, (const char*) &p, sizeof(p), 0);    //Se envía la cola del servidor
    if(checkSend == -1){
        perror("SET: Ha ocurrido un error al enviar a la cola del servidor \n");
        if (mq_close(cc) == -1) {
            perror("SET: Error al cerrar la cola del cliente");
        }
        if (mq_close(cs) == -1) {
            perror("SET: Error al cerrar la cola del servidor");
        }
        if (mq_unlink(nombrecola) == -1) {
            perror("SET: Error al desvincular la cola");
        }
        return(-1);
    }
    
    int e = mq_receive(cc, (char*) &r, sizeof(r), 0);    //Se recibe la cola del cliente
    if (e == -1) {
        perror("SET: Ha ocurrido un error al recibir de la cola del servidor \n");
        if (mq_close(cc) == -1) {
            perror("SET: Error al cerrar la cola del cliente");
        }
        if (mq_close(cs) == -1) {
            perror("SET: Error al cerrar la cola del servidor");
        }
        if (mq_unlink(nombrecola) == -1) {
            perror("SET: Error al desvincular la cola");
        }
        return(-1);
    }
    
    if (mq_close(cc) == -1) {
        perror("SET: Error al cerrar la cola del cliente");
    }
    if (mq_close(cs) == -1) {
        perror("SET: Error al cerrar la cola del servidor");
    }
    if (mq_unlink(nombrecola) == -1) {
        perror("SET: Error al desvincular la cola");
    }
    
    return r.cod_error;
    
}

int destroy(char* clave) {
    struct peticion p;
    struct respuesta r;
    char nombrecola[128];
    //char nombrerespuesta[128];
    sprintf(nombrecola,"/micola-%d", getpid());
    //sprintf(nombrerespuesta,"/mi cola2- %d", getpid());
    
    struct mq_attr q_attr;
    
    q_attr.mq_maxmsg = 10;
    q_attr.mq_msgsize = sizeof(r);
    
    printf("DESTROY: Inicializar colas\n");
    
    mqd_t cc = mq_open(nombrecola, O_CREAT|O_RDONLY, 0777, &q_attr);  //Se crea la cola del cliente
    if(cc == -1) {
        perror("DESTROY: Ha ocurrido un error al crear la cola del cliente \n");
    }
    
    mqd_t cs = mq_open("/SERVIDOR", O_WRONLY);    //Se crea la cola del servidor
    if(cs == -1) {
        perror("DESTROY: Ha ocurrido un error al crear la cola del servidor \n");
    }
    
    p.operacion = 3; //Definido en servidor.c
    strcpy(p.clave, clave);
    strcpy(p.cola_cliente, nombrecola);
    //strcpy(p.cola_respuesta, nombrerespuesta);
    
    int checkSend = mq_send(cs, (const char*) &p, sizeof(p), 0);    //Se envía la cola del servidor
    if(checkSend == -1){
        perror("DESTROY: Ha ocurrido un error al enviar a la cola del servidor \n");
        if (mq_close(cc) == -1) {
            perror("DESTROY: Error al cerrar la cola del cliente");
        }
        if (mq_close(cs) == -1) {
            perror("DESTROY: Error al cerrar la cola del servidor");
        }
        if (mq_unlink(nombrecola) == -1) {
            perror("DESTROY: Error al desvincular la cola");
        }
        return(-1);
    }
    
    int e = mq_receive(cc, (char*) &r, sizeof(r), 0);    //Se recibe la cola del cliente
    if (e == -1) {
        perror("DESTROY: Ha ocurrido un error al recibir de la cola del servidor \n");
        if (mq_close(cc) == -1) {
            perror("DESTROY: Error al cerrar la cola del cliente");
        }
        if (mq_close(cs) == -1) {
            perror("DESTROY: Error al cerrar la cola del servidor");
        }
        if (mq_unlink(nombrecola) == -1) {
            perror("DESTROY: Error al desvincular la cola");
        }
        return(-1);
    }

    if (mq_close(cc) == -1) {
        perror("DESTROY: Error al cerrar la cola del cliente");
    }
    if (mq_close(cs) == -1) {
        perror("DESTROY: Error al cerrar la cola del servidor");
    }
    if (mq_unlink(nombrecola) == -1) {
        perror("DESTROY: Error al desvincular la cola");
    }
    
    return r.cod_error;
    
}

int init (char *clave, int valor) {
    struct peticion p;
    struct respuesta r;
    char nombrecola[128];
    sprintf(nombrecola,"/micola-%d", getpid());
    
    struct mq_attr q_attr;
    
    q_attr.mq_maxmsg = 10;
    q_attr.mq_msgsize = sizeof(r);
        
    printf("INIT: Inicializar colas \n");
    
    mqd_t cc = mq_open(nombrecola, O_CREAT|O_RDONLY, 0777, &q_attr);  //Se crea la cola del cliente
    if(cc == -1) {
        perror("INIT: Ha ocurrido un error al crear la cola del cliente \n");
    }
    
    mqd_t cs = mq_open("/SERVIDOR", O_WRONLY);    //Se crea la cola del servidor
    if(cs == -1) {
        perror("INIT: Ha ocurrido un error al crear la cola del servidor \n");
    }
    
    p.operacion = 0; //Definido en servidor.c
    strcpy(p.clave, clave);
    strcpy(p.cola_cliente, nombrecola);
    printf("%s \n", p.cola_cliente);
    p.n = valor;
    
    int checkSend = mq_send(cs, (const char*) &p, sizeof(p), 0);    //Se envía la cola del servidor
    if(checkSend == -1){
        perror("INIT: Ha ocurrido un error al enviar a la cola del servidor \n");
        if (mq_close(cc) == -1) {
            perror("INIT: Error al cerrar la cola del cliente");
        }
        if (mq_close(cs) == -1) {
            perror("INIT: Error al cerrar la cola del servidor");
        }
        if (mq_unlink(nombrecola) == -1) {
            perror("INIT: Error al desvincular la cola");
        }
        return(-1);
    }
    
    int e = mq_receive(cc, (char*) &r, sizeof(r), 0);    //Se recibe la cola del cliente
    if (e == -1) {
        perror("INIT: Ha ocurrido un error al recibir de la cola del servidor \n");
        if (mq_close(cc) == -1) {
            perror("INIT: Error al cerrar la cola del cliente");
        }
        if (mq_close(cs) == -1) {
            perror("INIT: Error al cerrar la cola del servidor");
        }
        if (mq_unlink(nombrecola) == -1) {
            perror("INIT: Error al desvincular la cola");
        }
        return(-1);
    }
    
    if (mq_close(cc) == -1) {
        perror("INIT: Error al cerrar la cola del cliente");
    }
    if (mq_close(cs) == -1) {
        perror("INIT: Error al cerrar la cola del servidor");
    }
    if (mq_unlink(nombrecola) == -1) {
        perror("INIT: Error al desvincular la cola");
    }
    
    return r.cod_error;
    
}
