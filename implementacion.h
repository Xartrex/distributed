#ifndef IMPLEMENTACION_H_INCLUDED
#define IMPLEMENTACION_H_INCLUDED


typedef struct elementoLista *TipoLista;

int insertar(TipoLista *l, char *name, int elementos);
int recuperar(TipoLista *l, char *name, int n);
int meter(TipoLista *l, char *name, int n, int valor);
int liberar(TipoLista *l, char *name);



#endif // IMPLEMENTACION_H_INCLUDED
