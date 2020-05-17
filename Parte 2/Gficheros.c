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
#include <fcntl.h>
#include "Gficheros.h"



int registro(char *patata) {
    struct stat st = {0};
    //se usa la llamada stat para comprobar si existe la carpeta en la que se localizaran los ficheros del server
    if (stat("./ficheros", &st) == -1) {
        //En caso de que no exista la crea
        if (mkdir("./ficheros", 0777) != 0) {
            return 2;
        }
    }
    //Comprobar si existe la carpeta en la que se almacenaran los usuarios registadros, sino existe la crea
    if (stat("./ficheros/usuarios", &st) == -1) {
        if (mkdir("./ficheros/usuarios", 0777) != 0) {
            return 2;
        }
    }

    //metemos en un string la futura ruta de la carpeta del usuario a registrar
    char usuario[256];
    sprintf(usuario,"./ficheros/usuarios/%s", patata);

    //Comprueba si existe la carpeta, si lo esta es que ya esta registrado
    if (stat(usuario, &st) == 0) { 
        return 1;
    }
    //Sino existe crea la carpera, indicando que se ha registrado
    if (stat(usuario, &st) == -1) { 
        if (mkdir(usuario, 0777) != 0) {
            return 2;
        }
    }
    return 0;
        
}

int baja(char *patata) {
    struct stat st = {0};
    //se usa la llamada stat para comprobar si existe la carpeta en la que se localizaran los ficheros del server
    if (stat("./ficheros", &st) == -1) {
        mkdir("./ficheros", 0777);
        return -1;
    }
    //metemos en un string la ruta de la carpeta del usuario a darse de baja
    char usuario[256];
    sprintf(usuario,"./ficheros/usuarios/%s", patata);
    //si existe la carpeta la borra indicando que ya no esta registrado
    if (stat(usuario, &st) == 0) { 
        remove(usuario);
        return 0;
    }
    //Si no existe se retorna error de que no existia un usuario con el nombre a dar de baja
    if (stat(usuario, &st) == -1) { 
        return 1;
    }
    
    return 0;
}

int conectar(char *usuario, int s_local, char *puerto) {
    struct stat st = {0};
    //crear directorio raiz si no esta creado
    if (stat("./ficheros", &st) == -1) {
        mkdir("./ficheros", 0777);
        return 1; //el usuario no existe
    }
    //crear directorio de usuarios conectados si no esta creado
    if (stat("./ficheros/usuarios conectados", &st) == -1) {
        mkdir("./ficheros/usuarios conectados", 0777);
    }
    //metemos en un string la ruta de la carpeta del usuario que quiere conectarse
    char patata[256];
    sprintf(patata,"./ficheros/usuarios/%s", usuario);

    //si no existe es que no esta registrado
    if (stat(patata, &st) == -1) { 
        return 1; //el usuario no existe
    }
    //metemos en un string la ruta del fichero del usuario que quiere conectarse que indica que esta conectado
    char conexion[256];
    sprintf(conexion,"./ficheros/usuarios conectados/%s", usuario);

    //Comprobar si el usuario ya está conectado desde otra terminal
    int ff = open(conexion, O_RDONLY);
    if( ff > 0){
	    return 2;
        close(ff);
    }
    //Si el usuario esta registrado procede a crear el fichero que indica que esta conectado
    if (stat(patata, &st) == 0) { 
        FILE *fd;
        //cogemos del socket la informaci'on del puerto y de la ip
        struct sockaddr_in addr;
        socklen_t addr_size = sizeof(struct sockaddr_in);
        int res = getpeername(s_local, (struct sockaddr *)&addr, &addr_size);
        if(res == -1){
            return 2;
        }
        char clientip[20];
        strcpy(clientip, inet_ntoa(addr.sin_addr));

        //crea el fichero 
        fd = fopen(conexion, "w+");
        if(fd == NULL){
            return 3; //no se pudo poner como conectado/fallo al crear el archivo
        }

        //escribe el puerto y la ip en el fichero
        fprintf(fd, "%s,%s", puerto, clientip);

        fclose(fd);
    }
    
    return 0;
}

int publicar(char *descripcion, char *nombre, char *usuario){

    //Si recibe un asterisco es que el cliente no se ha conectado previamente
    if(0 == strcmp(usuario, "#")){
	    return 2;
    }

    //crea la ruta del fichero a crear
    char fichero[256];
    sprintf(fichero,"./ficheros/usuarios/%s/%s", usuario, nombre);

    //Comprueba si el fichero ya está publicado
    struct stat st = {0};
    if (stat(fichero, &st) == 0) {
        return 3;
    }
    //printf("Print del Gficheros: %s\n", usuario);
    //Crea el fichero
    FILE *fd;
    fd = fopen(fichero, "w+");
    //Mete en el fichero la descripcion
    fprintf(fd, "%s", descripcion);
    fclose(fd);
    return 0;
}

int borrar(char *nombre, char *usuario){
    struct stat st = {0};
    //Si recibe un asterisco es que el cliente no se ha conectado previamente
    if(0 == strcmp(usuario, "#")){
	    return 2;
    }

    //Si no existe el directorio raiz no se puede borrar nada
    if (stat("./ficheros", &st) == -1) {
        return 4; 
    }

    //Mete en un string la ruta de la carpeta que asegura que el usuario esta registrado
    char patata[256];
    sprintf(patata,"./ficheros/usuarios/%s", usuario);

    //Si no existe es que no esta registrado
    if (stat(patata, &st) == -1) { 
        return 1; //el usuario no existe
    }
    //coger ruta del fichero
    char fichero[256];
    sprintf(fichero,"./ficheros/usuarios/%s/%s", usuario, nombre);

    //Comprobar si el fichero existe
    if (stat(fichero, &st) == -1) {
        return 3;
    }
    //printf(fichero);

    remove(fichero);

    return 0;
}

int desconectar(char *usuario){
    //Si no existe el directorio raiz nadie se desconecta, no hay nada
    struct stat st = {0};
    if (stat("./ficheros", &st) == -1) {
        return 3;
    }
    //Mete en un string la ruta de la carpeta que asegura que el usuario esta registrado
    char patata[256];
    sprintf(patata,"./ficheros/usuarios/%s", usuario);
    //Si no existe es que no esta registrado
    if (stat(patata, &st) == -1) { 
        return 1;
    }

    //coger ruta del fichero que indica que esta conectado
    char fichero[256];
    sprintf(fichero,"./ficheros/usuarios conectados/%s", usuario);

    //comprueba que existe el fichero, sino es que no estaba conectado
    if (stat(fichero, &st) == -1) {
        return 2;
    }

    remove(fichero);
    
    return 0;
}

int list_users_connected(char *user){
    //Si recibe un asterisco es que el cliente no se ha conectado previamente
    if(0 == strcmp(user, "#")){
	    return 2;
    }

    struct stat st = {0};
    //Comprueba que el usuario esta registrado
    char directorio[256];
    sprintf(directorio, "./ficheros/usuarios/%s", user);
    if (stat(directorio, &st) == -1) {
        return 1;
    }
    //Si la carpeta que contiene los usuarios conectados no existe es que no hay nadie conectado
    if (stat("./ficheros/usuarios conectados", &st) == -1) {
        return 3;
    }
    //Comprueba que el que realiza la operacion esta conectado, a diferencia de antes, en caso de que el cliente tenga que esta conectado como el usuario, pero el servidor puede que no tenga constancia de eso
    char fichero[256];
    sprintf(fichero,"./ficheros/usuarios conectados/%s", user);
    int fd = open(fichero, O_RDONLY);
    if (fd == -1) {
        return 2;
    }
    close(fd);
    //Si esta conectado procede a la siguiente fase que es contar cuantos usuarios conectados hay
    return 0;
}

int list_users_contador() {
    //Abre el directorio de usuarios conectados para contar cuantos hay y enviar la informacion al cliente
    char ruta[256];
    sprintf(ruta, "./ficheros/usuarios conectados");
    DIR *dirp;
    struct dirent *direntp;
    dirp = opendir(ruta);
    if(dirp == NULL){
        return -1;
    }

    int contador = 0;
    while((direntp = readdir(dirp)) != NULL){
        if(direntp->d_type == DT_REG){
            contador++;
        }
    }

    return contador;
}

int list_users(int s_local){
    //Si ha llegado hasta aqui es que esta listo para enviar la informacion de los usuarios conectados al cliente
    char ruta[256];
    sprintf(ruta, "./ficheros/usuarios conectados");
    //Se abre el directorio
    DIR *dirp;
    struct dirent *direntp;
    dirp = opendir(ruta);
    if(dirp == NULL){
        return -1;
    }
    
    int mesg2=0;
    //Despues de enviar el número de usuarios 
    while((direntp = readdir(dirp)) != NULL){
        //Nos recorremos todos los archivos y se van abriendo los ficheros
        FILE *fd;
        char rutaFichero[512];
        sprintf(rutaFichero, "./ficheros/usuarios conectados/%s", direntp->d_name);
        fd = fopen(rutaFichero, "r");
        //printf("%s\n", rutaFichero);

        //Nos saltamos el directorio actual y la referencia al padre
        if ( !strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, "..") ){
            continue;
        }

        char buffer[256];
        char puerto[6];
        char ip[20];
        //Se almacenan los contenidos del fichero que indica que el usuario esta conectado
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
                j++;
                //printf("%s", ip);
            }
        }
        fclose(fd);
        //se envia el nombre
        char envio1[256];
        sprintf(envio1, "%s", direntp->d_name); 
        printf("%s\n", envio1);
        mesg2 = enviar(s_local, envio1, strlen(envio1)+1);
        if(mesg2 == -1){
            printf("error enviar2\n");
            return 2;
        }
        //se envia la ip
        char envio2[256];
        sprintf(envio2, "%s", ip); 
        printf("%s\n", envio2);
        mesg2 = enviar(s_local, envio2, strlen(envio2)+1);
        if(mesg2 == -1){
            printf("error enviar2\n");
            return 2;
        }
        //Se envia el puerto
        char envio3[256];
        sprintf(envio3, "%s", puerto);
        printf("%s\n", envio3); 
        mesg2 = enviar(s_local, envio3, strlen(envio3)+1);
        if(mesg2 == -1){
            printf("error enviar2\n");
            return 2;
        }
    }
    return 0;
}

int list_contenido_connected(char *user, char *destino){


    struct stat st = {0};
    //Si recibe un asterisco es que el cliente no se ha conectado previamente
    if(0 == strcmp(user, "#")){
	    return 2;
    }

    char directorio[256];
    //comprobamos que el usuario que realiza la operacion existe
    sprintf(directorio, "./ficheros/usuarios/%s", user);
    if (stat(directorio, &st) == -1) {
        return 1;
    }
    
    char fichero[256];
    //Comprueba que el que realiza la operacion esta conectado, a diferencia de antes, en caso de que el cliente tenga que esta conectado como el usuario, pero el servidor puede que no tenga constancia de eso
    sprintf(fichero,"./ficheros/usuarios conectados/%s", user);
    int fd = open(fichero, O_RDONLY);
    if (fd == -1) {
        return 2;
    }
    close(fd);

    char directorio2[256];
    //comprobamos que el usuario cuyo contenido se quiere conocer existe
    sprintf(directorio2, "./ficheros/usuarios/%s", destino);
    if (stat(directorio2, &st) == -1) {
	    printf("User no existe\n");
        return 3;
    }

    //error en otro caso
    if (stat("./ficheros/usuarios conectados", &st) == -1) {
        return 4;
    }
    //Si todo es correcto procedera a contar cuantos ficheros tiene publicados
    return 0;
}


int list_contenido_contador(char *usuario){
    //Abre el directorio del usuario para contar cuantos ficheros tiene hay y enviar la informacion al cliente
    char ruta[256];
    sprintf(ruta, "./ficheros/usuarios/%s", usuario);

    DIR *dirp;
    struct dirent *direntp;
    dirp = opendir(ruta);
    if(dirp == NULL){
        return 1;
    }
    int contador = 0;
    while((direntp = readdir(dirp)) != NULL){
        if(direntp->d_type == DT_REG){
            contador++;
        }
    }
    //Si tiene ficheros publicados, podra enviar la informacion de estos
    return contador;
}


int list_contenido(char *usuario, int s_local){
    //Si recibe un asterisco es que el cliente no se ha conectado previamente
    if(0 == strcmp(usuario, "#")){
	    return 2;
    }
    //Abre el directorio del usuario
    char ruta[256];
    sprintf(ruta, "./ficheros/usuarios/%s", usuario);
    DIR *dirp;
    struct dirent *direntp;
    dirp = opendir(ruta);
    if(dirp == NULL){
        return 1;
    }
    
    int mesg2=0;
    //procede a leer abrir los ficheros
    while((direntp = readdir(dirp)) != NULL){
        FILE *fd;
        char rutaFichero2[512];
        sprintf(rutaFichero2, "./ficheros/usuarios/%s/%s", usuario, direntp->d_name);
        fd = fopen(rutaFichero2, "r");
        //Nos saltamos el directorio actual y la referencia al padre
        if ( !strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, "..") ){
            continue;
        }
        //lee su contenido
        char buffer[256];
        fread(buffer, 256, 1, fd);
        fclose(fd);

        //envia el nombre 
        char envio1[256];
        sprintf(envio1, "%s", direntp->d_name); 
        mesg2 = enviar(s_local, envio1, strlen(envio1)+1);
        if(mesg2 == -1){
            printf("error enviar2\n");
            return 2;
        }
    }
    
    return 0;
}
