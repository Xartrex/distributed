#ifndef _LIBRARY_MENSAJE_H
#define _LIBRARY_MENSAJE_H

#define MAXSIZE 128

struct peticion{
    int operacion;
    char clave[MAXSIZE];
    int valor;
    int n;
    char cola_cliente[MAXSIZE];
    char cola_respuesta[MAXSIZE];

};

struct respuesta{
    int cod_error;
    int valor;
};

void *inita(void *pet);
void *seta(void *pet); //clave es un string asociada a un int
void *geta(void *pet);
void *freea(void *pet);
void inicializar();

#endif
