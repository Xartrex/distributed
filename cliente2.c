#include "array.h"
#include <stdio.h>

int main () {
    
    int i = 5;
    char cadena [7] = "patata2";
    int n = 6;
        
    printf("Crear vector \n");
    int error = init(cadena, n);
    printf("%d \n", error);
    if (error == 0){
        printf("Éxito al crear el vector\n");
    } else {
        printf("Vector ya creado\n");
    }
    
    error = set(cadena, i, 5);
    printf("%d \n", error);
    if (error == 0){
        printf("Éxito al añadir un elemento al vector\n");
    } else {
        printf("Error al añadir un elemento al vector\n");
    }
    
    int patata = 0;
    //error = get(cadena, i, &patata);
    //printf("%d, %d \n", error, patata);
    
    /*for(;;){
        error = get(cadena, i, &patata);
        printf("%d, %d \n", error, patata);
        printf("A");
        printf("B");
    }*/
    error = get(cadena, i, &patata);
    printf("%d \n", error);
    if (error == 0){
        printf("Éxito al retornar un elemento del vector\n");
    } else {
        printf("Error al retornar un elemento del vector\n");
    }
    
    error = destroy(cadena);
    printf("%d \n", error);
    if (error == 0){
        printf("Éxito al destruir el vector\n");
    } else {
        printf("Error al destruir el vector\n");
    }
    
    return 0;
}
 
