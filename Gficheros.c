#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "lines.h"
#include "dirent.h"

#define MAX 256

int registro(char *patata) {
    
	struct stat st = {0};

	if (stat("./ficheros", &st) == -1) {
		mkdir("./ficheros", 0777);
	}

	mkdir("./ficheros/usuarios", 0777);

	char usuario[256];
	sprintf(usuario,"./ficheros/usuarios/%s", patata);
	// ./ficheros/Usuaro1
	if (stat(usuario, &st) == 0) { //poner comprobacion de si ya esta registrado con == 0
		return -1;
	}
	if (stat(usuario, &st) == -1) { //poner comprobacion de si ya esta registrado con == 0
		mkdir(usuario, 0777);
	}
	return 0;
	
}

int baja(char *patata) {
    
	struct stat st = {0};

	
	if (stat("./ficheros", &st) == -1) {
		mkdir("./ficheros", 0777);
		return -1;
	}

	char usuario[256];
	sprintf(usuario,"./ficheros/usuarios/%s", patata);
	// ./ficheros/Usuaro1
	if (stat(usuario, &st) == 0) { //poner comprobacion de si ya esta registrado con == 0
		remove(usuario);
		return 0;
	}
	if (stat(usuario, &st) == -1) { //poner comprobacion de si ya esta registrado con == 0
		return -1;
	}
	return 0;
	
}

int conectar(char *usuario, int s_local, char *puerto) {
    
	struct stat st = {0};
	//crear directorio raiz
	if (stat("./ficheros", &st) == -1) {
		mkdir("./ficheros", 0777);
		return -1; //el usuario no existe
	}
	//crear directorio de usuarios conectados
	if (stat("./ficheros/usuarios conectados", &st) == -1) {
		mkdir("./ficheros/usuarios conectados", 0777);
	}

	char usuario[256];
	sprintf(usuario,"./ficheros/usuarios/%s", usuario);


	// ./ficheros/Usuaro1 si no existe es que no esta registrado
	if (stat(usuario, &st) == -1) { //poner comprobacion de si ya esta registrado con == 0
		return -1; //el usuario no existe
	}

	char conexion[256];
	sprintf(conexion,"./ficheros/usuarios conectados/%s", usuario);

	if (stat(usuario, &st) == 0) { //poner comprobacion de si ya esta registrado con == 0
		FILE *fd;
		struct sockaddr_in addr;
    	socklen_t addr_size = sizeof(struct sockaddr_in);
    	int res = getpeername(s_local, (struct sockaddr *)&addr, &addr_size);
		if(res == -1){
			return -2;
		}
    	char clientip[20];
    	strcpy(clientip, inet_ntoa(addr.sin_addr));
		//unsigned short puerto = addr.sin_port;

		//crea el fichero 
		fd = fopen(conexion, "w+");
		if(fd == NULL){
			return -2; //no se pudo poner como conectado/fallo al crear el archivo
		}

		//escribe el puerto en el fichero
		fprintf(fd, "%s,%s", puerto,clientip);

		fclose(fd);
	}
	return 0;
	
}

int publicar(char *descripcion, char *nombre, char *usuario){
	//coger ruta del fichero
	char fichero[256];
	sprintf(fichero,"./ficheros/usuarios/%s/%s", usuario, nombre);
    printf("Print del Gficheros: %s\n", usuario);
	FILE *fd;
	fd = fopen(fichero, "w+");

	fprintf(fd, "%s", descripcion);
	fclose(fd);
	return 0;
}

int borrar(char *nombre, char *usuario){
	//coger ruta del fichero
	char fichero[256];
	sprintf(fichero,"./ficheros/usuarios/%s/%s", usuario, nombre);

	remove(fichero);

	return 0;
}

int desconectar(char *usuario){
	//coger ruta del fichero
	char fichero[256];
	sprintf(fichero,"./ficheros/usuarios concetados/%s", usuario);

	remove(fichero);

	return 0;
}

int list_users(int s_local){
    char ruta[256];
    sprintf(ruta, "./ficheros/usuarios conectados");
    DIR *dirp;
    struct dirent *direntp;
    dirp = opendir(ruta);
    if(dirp == NULL){
        return -2;
    }
    while((direntp = readdir(dirp)) != NULL){
        char nombre[256];
        FILE *fd;
        char rutaFichero[256];
        sprintf(rutaFichero, "./ficheros/usuarios conectados/%s", direntp->d_name);
        fd = fopen(rutaFichero, "r");
        
        uint16_t puerto;
        char buffer[256];
        fread(buffer, 256, fd);
        for(int i = 0; i < 256; i++){
            
        }
        
        sprintf("%s", direntp->d_name ); 
    }
    
    int mesg2 = enviar(s_local, oka, strlen(oka)+1);
    if(mesg2 == -1){
        printf("error enviar2\n");
        break;
    }

	return 0;
}
