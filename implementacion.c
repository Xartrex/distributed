#include <stdio.h>
#include "implementacion.h"
#include <stdlib.h>
#include <string.h>

struct elementoLista{
    char nombre[1024]; //Modificar para añadir el puntero con la direccion del array
    int *direccion;
    int elementos;
    struct elementoLista *enlace;
};

int insertar(TipoLista *l, char *nombre, int elementos){
    TipoLista p1, p2, lista;
    
    p1 = *l;
    lista = *l;
    
     while (lista != NULL) {
         if(strcmp(lista->nombre, nombre) == 0){
             return (-1);
         }else{
             lista=lista->enlace;
         }
     }
    
    
    if (p1 == NULL){
        p1 = (struct elementoLista *)malloc(sizeof(struct elementoLista));
        strncpy(p1->nombre, nombre, 1024);
        p1->elementos = elementos;
        p1->direccion=(int *) malloc(elementos*sizeof(int));
        p1->enlace = NULL;
        *l = p1;
    } else {
        while (p1->enlace != NULL) {
            p1 = p1->enlace;
        }
        p2 = (struct elementoLista *)malloc(sizeof(struct elementoLista));
        strncpy(p2->nombre, nombre, 1024);
        p2->elementos = elementos;
        p2->enlace = NULL;
        p2->direccion = (int *) malloc(elementos*sizeof(int));
        p1->enlace = p2;
    }
    return 0;
}


int recuperar(TipoLista *l, char *name, int n){
    TipoLista lista;
        
    lista = *l;
    while (lista != NULL) {
        if(strcmp(lista->nombre, name) == 0){
            if(n < lista->elementos && (n) >= 0){
                 int valor;
                 valor = lista->direccion[n];
                 return valor;
                 break;
            } else {
                perror("Error al recuperar, fuera de rango");
                //return -1;
            }
        } else {
            lista = lista->enlace;
        }
    }
    perror("Error al recuperar");
    //return -1;
}

int meter(TipoLista *l, char *name, int n, int valor){
    TipoLista lista;
    lista = *l;
    while (lista != NULL) {
        if(strcmp(lista->nombre, name) == 0){
            if(n < lista->elementos && (n) >= 0){
                 lista->direccion[n] = valor;
                 return 0;
                 break;
            } else {
                perror("Error al meter, fuera de rango");
                return -1;
            }
        } else {
            lista = lista->enlace;
        }
        
    }
    return -1;
}

int liberar(TipoLista *l, char *name){
    TipoLista lista, aux = NULL;
    lista = *l;

     while (lista != NULL) {
        if(strcmp(lista->nombre, name) == 0){
            if(aux == NULL){
                aux = lista->enlace;
                free(lista->direccion);
                free(lista);
                *l = aux;
                return 0;
            }
            aux->enlace = lista->enlace;
            free(lista->direccion);
            free(lista);
            lista = aux;
            return 0;
            break;
        }
        aux = lista;
        lista = lista->enlace;
    }
    return -1;
}
