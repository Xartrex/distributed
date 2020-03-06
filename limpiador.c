#include "claves.h"
#include <stdio.h>

int main () {
//     int v;
//     init (  );
//     set ("cl1", &);
//     get ("cl2", &r);
    
    int i = 5;
    char cadena1 [7] = "patata1";
    char cadena2 [7] = "patata2";
        
    int patata = 0;
    int error = get(cadena1, i, &patata);
    printf("%d, %d \n", error, patata);
    
    error = get(cadena2, i, &patata);
    printf("%d, %d \n", error, patata);
    
    error = destroy(cadena1);
    printf("%d \n", error);
    error = destroy(cadena2);
    printf("%d \n", error);

    
    printf("%d \n", error);


    
//     detalles de uso
    return 0;
}
  
