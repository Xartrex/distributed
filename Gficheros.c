#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 256

int registro(char *patata) {
    
	struct stat st = {0};

	if (stat("./ficheros", &st) == -1) {
		mkdir("./ficheros", 0777);
	}
	char usuario[256];
	sprintf(usuario,"./ficheros/%s", patata);
	// ./ficheros/Usuaro1
	if (stat(usuario, &st) == 0) { //poner comprobacion de si ya esta registrado con == 0
		return -1;
	}
	if (stat(usuario, &st) == -1) { //poner comprobacion de si ya esta registrado con == 0
		mkdir(usuario, 0777);
	}
	return 0;
	
}

