
program fileserverRPC{
    version fileServer{
        int registerUser(string usuario) = 1;
        int unregisterUser(string usuario) = 2;
        int connectUser(string usuario, int socket, string port) = 3;
        int disconnectUser(string usuario) = 4;
        int publishFile(string descripcion, string nombre, string usuario) = 5;
        int deleteFile(string nombre, string usuario) = 6;
    }=1;
}=99;
