#ifndef _LIBRARY_CLAVES_H
#define _LIBRARY_CLAVES_H

int init();
int set(char *clave, int  valor); //clave es un string asociada a un int
int get(char *clave, int *valor);
int free(char *clave);

#endif

