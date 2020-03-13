#include "array.h"
#include <stdio.h>

int main () {
    
    int i = 5;
    char cadena [7] = "patata3";
    int n = 1000;
        
    printf("Crear vector \n");
    int error = init(cadena, n);
    printf("%d \n", error);
    if (error == 0){
        printf("Éxito al crear el vector\n");
    } else {
        printf("Vector ya creado\n");
    }
    
    error = set(cadena, i, 3);
    printf("%d \n", error);
    if (error == 0){
        printf("Éxito al añadir un elemento al vector\n");
    } else {
        printf("Error al añadir un elemento al vector\n");
    }
    
    error = set(cadena, i+1, 4);
    printf("%d \n", error);
    if (error == 0){
        printf("Éxito al añadir un elemento al vector\n");
    } else {
        printf("Error al añadir un elemento al vector\n");
    }
        
    int patata = 0;
    for(int j = 0; j <= 30000; j++){
        error = get(cadena, i, &patata);
        printf("%d, %d \n", error, patata);
        if (error == 0){
            printf("Éxito al retornar un elemento del vector\n");
        } else {
            printf("Error al retornar un elemento del vector\n");
        }
        
        error = get(cadena, i+1, &patata);
        printf("%d, %d \n", error, patata);
        if (error == 0){
            printf("Éxito al retornar un elemento del vector\n");
        } else {
            printf("Error al retornar un elemento del vector\n");
        }
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
