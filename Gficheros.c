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

int conectar(char *patata, int s_local, char *puerto) {
    
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
	sprintf(usuario,"./ficheros/usuarios/%s", patata);


	// ./ficheros/Usuaro1 si no existe es que no esta registrado
	if (stat(usuario, &st) == -1) { //poner comprobacion de si ya esta registrado con == 0
		return -1; //el usuario no existe
	}

	char conexion[256];
	sprintf(conexion,"./ficheros/usuarios conectados/%s", patata);

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
	sprintf(fichero,"./ficheros/usuarios/%s/%s", usuario,nombre);
	FILE *fd;
	fd = fopen(fichero, "w+");

	fprintf(fd, "%s", descripcion);
	fclose(fd);
	return 0;
}

int borrar(char *nombre, char *usuario){
	//coger ruta del fichero
	char fichero[256];
	sprintf(fichero,"./ficheros/usuarios/%s/%s", usuario,nombre);

	remove(fichero);

	return 0;
}