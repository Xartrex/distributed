#ifndef _LIBRARY_ARRAY_H
#define _LIBRARY_ARRAY_H

int init(char *clave, int valor);
int set(char *clave, int i, int valor); //clave es un string asociada a un int
int get(char *clave, int i, int *valor);
int destroy(char *clave); 


#endif
