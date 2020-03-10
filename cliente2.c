#include "array.h"
#include <stdio.h>

int main () {
//     int v;
//     init (  );
//     set ("cl1", &);
//     get ("cl2", &r);
    
    int i = 5;
    char cadena [7] = "patata2";
    int n = 6;
        
    printf("Crear vector \n");
    int error = init(cadena, n);
    printf("%d \n", error);
    
    error = set(cadena, i, 5);
    printf("%d \n", error);
    
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
        printf("%d, %d \n", error, patata);
    printf("%d \n", error);


    
//     detalles de uso
    return 0;
}
 
