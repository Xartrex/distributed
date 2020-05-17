#ifndef _GFICHEROS_H
#define _GFICHEROS_H

int registro(char *patata);
int baja(char *patata);
int conectar(char *usuario, int s_local, char *puerto);
int publicar(char *descripcion, char *nombre, char *usuario);
int borrar(char *nombre, char *usuario);
int desconectar(char *usuario);
int list_users_connected(char *user);
int list_users_contador();
int list_users(int s_local);
int list_contenido_connected(char *user, char *destino);
int list_contenido_contador(char *usuario);
int list_contenido(char *usuario, int s_local);

#endif