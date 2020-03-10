#include "array.h"
#include <stdio.h>

int main () {
//     int v;
//     init (  );
//     set ("cl1", &);
//     get ("cl2", &r);
    
    int i = 5;
    char cadena [7] = "patata3";
    int n = 1000;
        
    printf("Crear vector \n");
    int error = init(cadena, n);
    printf("%d \n", error);
    
    error = set(cadena, i, 3);
    printf("%d \n", error);
        
    int patata = 0;
    for(;;){
        error = get(cadena, i, &patata);
        printf("%d, %d \n", error, patata);
    }
    
    
    
//     detalles de uso
    return 0;
}
