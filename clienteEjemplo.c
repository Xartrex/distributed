#include "array.h"
#include <stdio.h>
#include <string.h>

int main () {
    init("vector1", 100);
    init("vector2", 200);
    set("vector1", 0, 40);
    set("vector1", 120, 30);
    init("vector1", 200);
    destroy("vector1");
    destroy("vector2");
    
    return 0;
}
