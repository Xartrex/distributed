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
#include <dirent.h>

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
		return 1;
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
		return 1;
	}
	return 0;
	
}

int conectar(char *usuario, int s_local, char *puerto) {
    
	struct stat st = {0};
	//crear directorio raiz
	if (stat("./ficheros", &st) == -1) {
		mkdir("./ficheros", 0777);
		return 1; //el usuario no existe
	}
	//crear directorio de usuarios conectados
	if (stat("./ficheros/usuarios conectados", &st) == -1) {
		mkdir("./ficheros/usuarios conectados", 0777);
	}

	char patata[256];
	sprintf(patata,"./ficheros/usuarios/%s", usuario);


	// ./ficheros/Usuaro1 si no existe es que no esta registrado
	if (stat(patata, &st) == -1) { //poner comprobacion de si ya esta registrado con == 0
		return 1; //el usuario no existe
	}

	char conexion[256];
	sprintf(conexion,"./ficheros/usuarios conectados/%s", usuario);

	if (stat(patata, &st) == 0) { //poner comprobacion de si ya esta registrado con == 0
		FILE *fd;
		struct sockaddr_in addr;
    	socklen_t addr_size = sizeof(struct sockaddr_in);
    	int res = getpeername(s_local, (struct sockaddr *)&addr, &addr_size);
		if(res == -1){
			return 2;
		}
    	char clientip[20];
    	strcpy(clientip, inet_ntoa(addr.sin_addr));
		//unsigned short puerto = addr.sin_port;

		//crea el fichero 
		fd = fopen(conexion, "w+");
		if(fd == NULL){
			return 3; //no se pudo poner como conectado/fallo al crear el archivo
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
    printf(fichero);

	remove(fichero);

	return 0;
}

int desconectar(char *usuario){
	//coger ruta del fichero
	char fichero[256];
	sprintf(fichero,"./ficheros/usuarios conectados/%s", usuario);

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
        return 2;
    }

	int contador = 0;
	while((direntp = readdir(dirp)) != NULL){
		if(direntp->d_type == DT_REG){
			contador++;
		}
	}
	//enviar contador
	char contadore[256];
	sprintf(contadore,"%d", contador*3); //se envian tres parametros por cliente
	int mesg2 = enviar(s_local, contadore, strlen(contadore)+1);
	if(mesg2 == -1){
		printf("error enviar2\n");
		return 2;
	}

    while((direntp = readdir(dirp)) != NULL){
        //char nombre[256];
        FILE *fd;
        char rutaFichero[512];
        sprintf(rutaFichero, "./ficheros/usuarios conectados/%s", direntp->d_name);
        fd = fopen(rutaFichero, "r");
        
        char buffer[256];
		char puerto[6];
		char ip[20];
        fread(buffer, 256, 1, fd);
		int precoma = 0;
		int j = 0;
		//parser para leer el fichero y extraer el puerto y la ip
        for(int i = 0; i < 256; i++){
            if(precoma == 0 && buffer[i]!= ','){
				puerto[j]=buffer[i];
				j++;
			}
			if(buffer[i]== ','){
				precoma=1;
				j=0;
			}
			if(precoma==1 && buffer[i+1]!= ' '){
				ip[j] = buffer[i+1];
			}
        }
		fclose(fd);
		//se envia el nombre
        char envio1[256];
        sprintf(envio1, "%s", direntp->d_name); 
		mesg2 = enviar(s_local, envio1, strlen(envio1)+1);
		if(mesg2 == -1){
			printf("error enviar2\n");
			return 2;
		}
		//se envia la ip
		char envio2[256];
        sprintf(envio2, "%s", ip); 
		mesg2 = enviar(s_local, envio2, strlen(envio2)+1);
		if(mesg2 == -1){
			printf("error enviar2\n");
			return 2;
		}
		//Se envia el puerto
		char envio3[256];
        sprintf(envio3, "%s", puerto); 
		mesg2 = enviar(s_local, envio3, strlen(envio3)+1);
		if(mesg2 == -1){
			printf("error enviar2\n");
			return 2;
		}


    }
    
    

	return 0;
}

int list_contenido(char *usuario, int s_local){
	struct stat st = {0};
    char ruta[256];
    sprintf(ruta, "./ficheros/usuarios/%s", usuario);
	if (stat(ruta, &st) == -1) {
		return 1;// usuario no registrado
	}

	char conectado[256];
	sprintf(conectado, "./ficheros/usuarios conectados/%s", usuario);
	FILE *ff;
	ff = fopen(conectado, "r");
	if(ff == NULL){
		return 3; //el usuario no esta conectado
	}
	fclose(ff);

    DIR *dirp;
    struct dirent *direntp;
    dirp = opendir(ruta);
    if(dirp == NULL){
        return 2;
    }
	int contador = 0;
	while((direntp = readdir(dirp)) != NULL){
		if(direntp->d_type == DT_REG){
			contador++;
		}
	}
	//enviar contador
	char contadore[256];
	sprintf(contadore,"%d", contador*2);// se multipiplica por 2 ya que se envian 2 parametros por fichero, para el bucle de java
	int mesg2 = enviar(s_local, contadore, strlen(contadore)+1);
	if(mesg2 == -1){
		printf("error enviar2\n");
		return 2;
	}

    while((direntp = readdir(dirp)) != NULL){
		FILE *fd;
		char rutaFichero2[512];
        sprintf(rutaFichero2, "./ficheros/usuarios/%s/%s", usuario, direntp->d_name);
		fd = fopen(rutaFichero2, "r");
        
        char buffer[256];
		//char descripcion[256];
        fread(buffer, 256, 1, fd);
		//int j = 0;
		//parser para leer el fichero y extraer el puerto y la ip
        /*for(int i = 0; i < 256; i++){
            if(precoma == 0 && buffer[i]!= ' '){
				descripcion[j]=buffer[i];
				j++;
			}*/
		fclose(fd);

		//envia el nombre primero
        char envio1[256];
        sprintf(envio1, "%s", direntp->d_name); 
		mesg2 = enviar(s_local, envio1, strlen(envio1)+1);
		if(mesg2 == -1){
			printf("error enviar2\n");
			return 2;
		}
		//envia la descripcion
		char envio2[256];
        sprintf(envio2, "%s", buffer); 
		mesg2 = enviar(s_local, envio2, strlen(envio2)+1);
		if(mesg2 == -1){
			printf("error enviar2\n");
			return 2;
		}


    }
    
    

	return 0;
}
