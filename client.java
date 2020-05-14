import java.io.*;
import gnu.getopt.Getopt;

import java.lang.* ;
import java.io.* ;
import java.net.* ;
import java.util.* ;

class TratarPeticion extends Thread{
	private ServerSocket serverSock;
	private	Socket sc = null;
	private	int num[];  //peticion
	private	int res;

	TratarPeticion(ServerSocket a){
		serverSock = a;
	}


	public void run(){
		try{
			while(true){
                
// 				sc = serverSock.accept(); //esperando conexion
// 
// 				InputStream istream = sc.getInputStream(); //recibimos info del server
// 				ObjectInput in = new ObjectInputStream(istream); //lo transformamos a algo legible para java
// 
// 				num = (int[]) in.readObject();
// 				res = num[0] + num[1];
// 
// 				DataOutputStream ostream = new DataOutputStream(sc.getOutputStream());
// 
// 				ostream.writeInt(res);
// 				ostream.flush();
// 				sc.close();
// 				
				
                Socket s_local = serverSock.accept();
                DataOutputStream out = new DataOutputStream(s_local.getOutputStream());
                DataInputStream in = new DataInputStream(s_local.getInputStream());
                
                //Lee del socket
                // mensaje = leer(in);
                System.out.println("Leemos el GET_FILE");
                byte[] ch = new byte[1];
                String mensajeR = new String();
                do{
                    ch[0] = in.readByte();
                    if (ch[0] != '\0'){
                        String d = new String(ch);
                        mensajeR = mensajeR + d;
                    }
                } while(ch[0] != '\0');

                //si la cadena recibida no es GET_FILE error
                if(!mensajeR.equals("GET_FILE")){
                    System.out.println("Error");
                    sc.close();
                    s_local.close();
                    return;
                }
                //recibe del socket el fichero
                //String rmt_file = leer(in);
                System.out.println("Leemos el archivo");
                String fichero = new String();
                do{
                    ch[0] = in.readByte();
                    if (ch[0] != '\0'){
                        String d = new String(ch);
                        fichero = fichero + d;
                    }
                } while(ch[0] != '\0');
                System.out.println("Nombre recibido: " + fichero);
                
                File fd = new File(fichero);

                //Si existe el fichero lo copia al socket y envia codigo 0
                if(fd.exists() && fd.isFile()){
                    //escribir(os, "0");
                    System.out.println("Archivo detectado, se procede a enviar");
                    out.writeBytes("0");
                    out.write('\0'); // inserta el código ASCII 0 al final
                    
                    InputStream inp = new FileInputStream(fichero);
                    DataInputStream inputfile = new DataInputStream(inp);
                    copy.copyFile(inputfile, out);
                    //cierre del fichero
                    inputfile.close();
                }
                //Si no existe envia codigo 1
                else{
                    //escribir(os, "1");
                    out.writeBytes("0");
                    out.write('\0'); // inserta el código ASCII 0 al final
                }
                s_local.close();
            }
        }catch(Exception e){
            System.err.println("exception " + e.toString());
            e.printStackTrace();
        }
	}
/*
    public void run(){
        while(true){
            try{
                sc = serverSock.accept(); //esperando conexion

                InputStream istream = sc.getInputStream(); //recibimos info del server
                ObjectInput in = new ObjectInputStream(istream); //lo transformamos a algo legible para java

                num = (int[]) in.readObject();
                res = num[0] + num[1];

                DataOutputStream ostream = new DataOutputStream(sc.getOutputStream());

                ostream.writeInt(res);
                ostream.flush();
                sc.close();
            }catch(Exception e){
                System.err.println("exception " + e.toString());
                e.printStackTrace();
            }
        }
    }
    */
}

//Clase usuario para almacenar en un futuro el listado de usuarios conectados
class DUsuario{
    String name;
    String ip;
    int port;

    public DUsuario (String name, String ip, int port){
        this.name = name;
        this.ip = ip;
        this.port = port;
    }
}

class client {

	
	/******************* ATTRIBUTES *******************/
	
	private static String _server   = null;
	private static int _port = -1;
	private static String usuario;
	
	//Arraylist para guardar los datos del LIST_USERS
    private static ArrayList<DUsuario> usuariosConectados;
	
	/********************* METHODS ********************/
	
	/**
	 * @param user - User name to register in the system
	 * 
	 * @return ERROR CODE
	 */
	static int register(String user) 
	{

		int res = 2;

		try
		{
			//Se crea el socket del cliente
			Socket sc = new Socket(_server, _port);
			String mensaje = "REGISTER";
			DataOutputStream out = new DataOutputStream(sc.getOutputStream());
			DataInputStream in = new DataInputStream(sc.getInputStream());
		
            //Se envía la cadena REGISTER con el nombre de usuario
			out.writeBytes(mensaje);
			out.write('\0'); // inserta el código ASCII 0 al final
            if(mensaje.equals("REGISTER")==true){
                out.writeBytes(user);
                out.write('\0'); // inserta el código ASCII 0 al final
            }

            //Leemos la respuesta del servidor
			byte[] ch = new byte[1];
			String mensajeR = new String();
			do{
				ch[0] = in.readByte();
				if (ch[0] != '\0'){
					String d = new String(ch);
					mensajeR = mensajeR + d;
				}
			} while(ch[0] != '\0');

			//Se pasa a int
            res = Integer.parseInt(mensajeR);
            
            //Se cierra la conexión
            //sc.close();
			
		}//fin del try

		catch (Exception e)
		{
			 System.err.println("excepcion " + e.toString() );
			 e.printStackTrace();
			 res = 2;
		}
		
        if(res == 0){
			System.out.println("c> REGISTER OK");
		} else if(res == 1){
			System.out.println("c> USERNAME IN USE");
		} else {
			System.out.println("c> REGISTER FAIL");
		}
		
		return res;
		
	}
	
	/**
	 * @param user - User name to unregister from the system
	 * 
	 * @return ERROR CODE
	 */
	static int unregister(String user) 
	{

        int res = 2;

        try
        {
            // se crea el socket del cliente
            Socket sc = new Socket(_server, _port);
            String mensaje = "UNREGISTER";
            DataOutputStream out = new DataOutputStream(sc.getOutputStream());
            DataInputStream in = new DataInputStream(sc.getInputStream());
        
            out.writeBytes(mensaje);
            out.write('\0'); // inserta el código ASCII 0 al final
            
            if(mensaje.equals("UNREGISTER")==true){
                out.writeBytes(user);
                out.write('\0'); // inserta el código ASCII 0 al final
            }

            byte[] ch = new byte[1];
            String mensajeR = new String();
            do{
                ch[0] = in.readByte();
                if (ch[0] != '\0'){
                    String d = new String(ch);
                    mensajeR = mensajeR + d;
                }
            } while(ch[0] != '\0');
            
            //Se pasa a int
            res = Integer.parseInt(mensajeR);
            
            //Se cierra la conexión
            //sc.close();

        }//fin del try

        catch (Exception e)
        {
            System.err.println("excepcion " + e.toString() );
            e.printStackTrace();
        }
        
        if(res == 0){
            System.out.println("c> UNREGISTER " + user);
        } else if(res == 1){
            System.out.println("c> USER DOES NOT EXIST");
        } else {
            System.out.println("c> UNREGISTER FAIL");
        }
        return res;
	}
	
    	/**
	 * @param user - User name to connect to the system
	 * 
	 * @return ERROR CODE
	 */
	static int connect(String user) 
	{
        // Write your code here
        System.out.println("CONNECT " + user);

        ServerSocket serverSock = null;
        try{
            serverSock = new ServerSocket(0); //nos devuelve el socket que quiera
            new TratarPeticion(serverSock).start();
        }catch(Exception e){
            System.err.println("Error cerrandi socket");
        }
        int puerto = serverSock.getLocalPort();
        String port = Integer.toString(puerto);
        
        int res = 3;

        try
        {
            // se crea el socket del cliente
            Socket sc = new Socket(_server, _port);
            String mensaje = "CONNECT";
            DataOutputStream out = new DataOutputStream(sc.getOutputStream());
            DataInputStream in = new DataInputStream(sc.getInputStream());
        
            out.writeBytes(mensaje);
            out.write('\0'); // inserta el código ASCII 0 al final
            
            if(mensaje.equals("CONNECT")==true){
                out.writeBytes(user);
                out.write('\0'); // inserta el código ASCII 0 al final
                out.writeBytes(port);
                out.write('\0');
            }

            //mete en la variable global el usuario conectado, uno por terminal a la vez
            usuario = user;

            byte[] ch = new byte[1];
            String mensajeR = new String();
            do{
                ch[0] = in.readByte();
                if (ch[0] != '\0'){
                    String d = new String(ch);
                    mensajeR = mensajeR + d;
                }
            } while(ch[0] != '\0');

            //Se pasa a int
            res = Integer.parseInt(mensajeR);
            
            //Se cierra la conexión
            //sc.close();

        }//fin del try

        catch (Exception e)
        {
            System.err.println("excepcion " + e.toString() );
            e.printStackTrace();
        }

        if(res == 0){
            System.out.println("c> CONNECT OK");
        } else if(res == 1){
            System.out.println("c> CONNECT FAIL, USER DOES NOT EXIST");
        } else if(res == 2){
            System.out.println("c> USER ALREADY CONNECTED");
        } else {
            System.out.println("c> CONNECT FAIL");
        }
        return res;
    }
	
	 /**
	 * @param user - User name to disconnect from the system
	 * 
	 * @return ERROR CODE
	 */
	static int disconnect(String user) 
	{
        int res = 3;

        try
        {
            //Se crea el socket del cliente
            Socket sc = new Socket(_server, _port);
            String mensaje = "DISCONNECT";
            DataOutputStream out = new DataOutputStream(sc.getOutputStream());
            DataInputStream in = new DataInputStream(sc.getInputStream());
        
            out.writeBytes(mensaje);
            out.write('\0'); // inserta el código ASCII 0 al final
            
            if(mensaje.equals("DISCONNECT")==true){
                out.writeBytes(user);
                out.write('\0'); // inserta el código ASCII 0 al final
            }

            byte[] ch = new byte[1];
            String mensajeR = new String();
            do{
                ch[0] = in.readByte();
                if (ch[0] != '\0'){
                    String d = new String(ch);
                    mensajeR = mensajeR + d;
                }
            } while(ch[0] != '\0');
            
            //Se pasa a int
            res = Integer.parseInt(mensajeR);
            
            //Se cierra la conexión
            //sc.close();

        }//fin del try

        catch (Exception e)
        {
            System.err.println("excepcion " + e.toString() );
            e.printStackTrace();
        }
        
        if(res == 0){
            System.out.println("c> DISCONNECT OK");
        } else if(res == 1){
            System.out.println("c> DISCONNECT FAIL / USER DOES NOT EXIST");
        } else if(res == 2){
            System.out.println("c> DISCONNECT FAIL / USER NOT CONNECTED");
        } else {
            System.out.println("c> DISCONNECT FAIL");
        }
        return res;
	}

	 /**
	 * @param file_name    - file name
	 * @param description - descrition
	 * 
	 * @return ERROR CODE
	 */
	static int publish(String file_name, String description) 
	{
        
        int res = 4;

        try
        {
            //Se crea el socket del cliente
            Socket sc = new Socket(_server, _port);
            String mensaje = "PUBLISH";
            DataOutputStream out = new DataOutputStream(sc.getOutputStream());
            DataInputStream in = new DataInputStream(sc.getInputStream());
        
            out.writeBytes(mensaje);
            out.write('\0'); // inserta el código ASCII 0 al final
            
            if(mensaje.equals("PUBLISH")==true){
                out.writeBytes(usuario);
                out.write('\0');
                out.writeBytes(file_name);
                out.write('\0'); // inserta el código ASCII 0 al final
                out.writeBytes(description);
                out.write('\0');
            }
            
            byte[] ch = new byte[1];
            String mensajeR = new String();
            do{
                ch[0] = in.readByte();
                if (ch[0] != '\0'){
                    String d = new String(ch);
                    mensajeR = mensajeR + d;
                }
            } while(ch[0] != '\0');

            //Se pasa a int
            res = Integer.parseInt(mensajeR);
            
            //Se cierra la conexión
            //sc.close();

        }//fin del try

        catch (Exception e)
        {
            System.err.println("excepcion " + e.toString() );
            e.printStackTrace();
        }
        System.out.println("PUBLISH " + file_name + " " + description);
        
        if(res == 0){
            System.out.println("c> PUBLISH OK");
        } else if(res == 1){
            System.out.println("c> PUBLISH FAIL, USER DOES NOT EXIST");
        } else if(res == 2){
            System.out.println("c> PUBLISH FAIL, USER NOT CONNECTED");
        } else if(res == 3){
            System.out.println("c> PUBLISH FAIL, CONTENT ALREDAY PUBLISHED");
        } else {
            System.out.println("c> PUBLISH FAIL");
        }
        return res;
    }

	 /**
	 * @param file_name    - file name
	 * 
	 * @return ERROR CODE
	 */
	static int delete(String file_name)
	{
        int res = 4;

        try
        {
            //Se crea el socket del cliente
            Socket sc = new Socket(_server, _port);
            String mensaje = "DELETE";
            DataOutputStream out = new DataOutputStream(sc.getOutputStream());
            DataInputStream in = new DataInputStream(sc.getInputStream());
        
            out.writeBytes(mensaje);
            out.write('\0'); // inserta el código ASCII 0 al final
            
            if(mensaje.equals("DELETE")==true){
                out.writeBytes(usuario);
                out.write('\0');
                out.writeBytes(file_name);
                out.write('\0'); // inserta el código ASCII 0 al final
            }
            
            byte[] ch = new byte[1];
            String mensajeR = new String();
            do{
                ch[0] = in.readByte();
                if (ch[0] != '\0'){
                    String d = new String(ch);
                    mensajeR = mensajeR + d;
                }
            } while(ch[0] != '\0');

            //Se pasa a int
            res = Integer.parseInt(mensajeR);
            
            //Se cierra la conexión
            //sc.close();

        }//fin del try

        catch (Exception e)
        {
            System.err.println("excepcion " + e.toString() );
            e.printStackTrace();
        }
        System.out.println("DELETE " + file_name);
        
        if(res == 0){
            System.out.println("c> DELETE OK");
        } else if(res == 1){
            System.out.println("c> DELETE FAIL, USER DOES NOT EXIST");
        } else if(res == 2){
            System.out.println("c> DELETE FAIL, USER NOT CONNECTED");
        } else if(res == 3){
            System.out.println("c> DELETE FAIL, CONTENT NOT PUBLISHED");
        } else {
            System.out.println("c> PUBLISH FAIL");
        }
        return res;
	}

	 /**
	 * @return ERROR CODE
	 */
	static int list_users()
	{
        int res = 0;

        try
        {

            // se crea el socket del cliente
            Socket sc = new Socket(_server, _port);
            String mensaje = "LIST_USERS";
            DataOutputStream out = new DataOutputStream(sc.getOutputStream());
            DataInputStream in = new DataInputStream(sc.getInputStream());
        
            out.writeBytes(mensaje);
            out.write('\0'); // inserta el código ASCII 0 al final
            
            if(mensaje.equals("LIST_USERS")==true){
                out.writeBytes(usuario);
                out.write('\0'); // inserta el código ASCII 0 al final
            }

            byte[] ch = new byte[1];
            String mensajeR = new String();
            do{
                ch[0] = in.readByte();
                if (ch[0] != '\0'){
                    String d = new String(ch);
                    mensajeR = mensajeR + d;
                }
            } while(ch[0] != '\0');

            //Se pasa a int
            int conectados = Integer.parseInt(mensajeR);
            
            if(conectados == 0) {
                mensajeR = new String();
                do{
                    ch[0] = in.readByte();
                    if (ch[0] != '\0'){
                        String d = new String(ch);
                        mensajeR = mensajeR + d;
                    }
                } while(ch[0] != '\0');
                conectados = Integer.parseInt(mensajeR);
                //System.out.println(conectados);
                if (conectados == -1){
                    res = 3;
                } else  {
                    usuariosConectados = new ArrayList<DUsuario>();
                    //int contador = conectados *3;
                    while(conectados > 0){
                        mensajeR = new String();
                        do{
                            ch[0] = in.readByte();
                            if (ch[0] != '\0'){
                                String d = new String(ch);
                                mensajeR = mensajeR + d;
                                
                            }
                        } while(ch[0] != '\0');
                        String user = mensajeR;
                        
                        mensajeR = new String();
                        do{
                            ch[0] = in.readByte();
                            if (ch[0] != '\0'){
                                String d = new String(ch);
                                mensajeR = mensajeR + d;
                                
                            }
                        } while(ch[0] != '\0');
                        String ip = mensajeR;
                        
                        mensajeR = new String();
                        do{
                            ch[0] = in.readByte();
                            if (ch[0] != '\0'){
                                String d = new String(ch);
                                mensajeR = mensajeR + d;
                                
                            }
                        } while(ch[0] != '\0');
                        String puerto = mensajeR;
                        
                        System.out.printf("%10s%10s%10s\n", user, ip, puerto);
                        usuariosConectados.add(new DUsuario(user, ip, Integer.parseInt(puerto)));
                        conectados--;
                    }
                }
            }

        }//fin del try

        catch (Exception e)
        {
            System.err.println("excepcion " + e.toString() );
            e.printStackTrace();
        }
        
        if(res == 0){
            System.out.println("c> LIST_USERS OK");
        } else if(res == 1){
            System.out.println("c> LIST_USERS FAIL, USER DOES NOT EXIST");
        } else if(res == 2){
            System.out.println("c> LIST_USERS FAIL, USER NOT CONNECTED");
        } else {
            System.out.println("c> LIST_USERS FAIL");
        }
        return res;
        
    }


	 /**
	 * @param user_name    - user name
	 * 
	 * @return ERROR CODE
	 */
	static int list_content(String user_name)
	{
		int res = 0;

        try
        {

            // se crea el socket del cliente
            Socket sc = new Socket(_server, _port);
            String mensaje = "LIST_CONTENT";
            DataOutputStream out = new DataOutputStream(sc.getOutputStream());
            DataInputStream in = new DataInputStream(sc.getInputStream());
        
            out.writeBytes(mensaje);
            out.write('\0'); // inserta el código ASCII 0 al final
            
            if(mensaje.equals("LIST_CONTENT")==true){
                out.writeBytes(usuario);
                out.write('\0'); // inserta el código ASCII 0 al final
            }

            out.writeBytes(user_name);
            out.write('\0'); // 

            byte[] ch = new byte[1];
            String mensajeR = new String();
            do{
                ch[0] = in.readByte();
                if (ch[0] != '\0'){
                    String d = new String(ch);
                    mensajeR = mensajeR + d;
                }
            } while(ch[0] != '\0');

            //Se pasa a int el byte de respuesta
            int conectados = Integer.parseInt(mensajeR);
            
            if(conectados == 0) {
                mensajeR = new String();
                do{
                    ch[0] = in.readByte();
                    if (ch[0] != '\0'){
                        String d = new String(ch);
                        mensajeR = mensajeR + d;
                    }
                } while(ch[0] != '\0');
                conectados = Integer.parseInt(mensajeR);
                //System.out.println(conectados);
                if (conectados == -1){
                    res = 3;
                } else  {
                    int contador= conectados;
                    while(contador>0){
                    
                     mensajeR = new String();
                    do{
                        ch[0] = in.readByte();
                        if (ch[0] != '\0'){
                            String d = new String(ch);
                            mensajeR = mensajeR + d;
                            
                        }
                    } while(ch[0] != '\0');
                    contador--;
                    System.out.println(mensajeR);
                    }
                }
            }

        }//fin del try

        catch (Exception e)
        {
            System.err.println("excepcion " + e.toString() );
            e.printStackTrace();
        }
		return 0;
	}

	 /**
	 * @param user_name    - user name
	 * @param remote_file_name    - remote file name
	 * @param local_file_name  - local file name
	 * 
	 * @return ERROR CODE
	 */
	static int get_file(String user_name, String remote_file_name, String local_file_name)
	{
        int res = 3;
        try{
            //Comprobamos si hay usuarios conectados
            if(usuariosConectados.isEmpty()) {
                System.out.println("c> GET_FILE FAIL");
                return 2;
            }
            else{
                int contador = 0;
                while(contador < usuariosConectados.size()){
                    if(usuariosConectados.get(contador).name.equals(user_name)) {
                        break;
                    } else {
                        contador++;
                    }
                }
                //Si no encuentra al usuario devuelve 2 e imprime el siguiente error
                if(contador == usuariosConectados.size()) {
                    System.out.println("c> GET_FILE FAIL");
                    return 2;
                }

                //almacenamos la ip y el puerto del usuario encontrado
                String ip = usuariosConectados.get(contador).ip;
                int port = usuariosConectados.get(contador).port;
                
                //creacion del socket
                Socket sc = new Socket(ip, port);
                
                
                DataOutputStream out = new DataOutputStream(sc.getOutputStream());
                DataInputStream in = new DataInputStream(sc.getInputStream());
                
                System.out.println("Enviamos el GET_FILE");
                String mensaje = "GET_FILE";
                out.writeBytes(mensaje);
                out.write('\0'); // inserta el código ASCII 0 al final
            
                System.out.println("Enviamos el nombre del fichero remoto");
                if(mensaje.equals("GET_FILE")==true){
                    out.writeBytes(remote_file_name);
                    out.write('\0'); // inserta el código ASCII 0 al final
                }
                System.out.println(remote_file_name);

                //introducimos un sleep para darle tiempo al servidor local a enviar la respuesta antes de leerla
                //Thread.sleep(5000);
                
                System.out.println("Recibimos la respuesta");
                byte[] ch = new byte[1];
                String mensajeR = new String();
                do{
                    ch[0] = in.readByte();
                    if (ch[0] != '\0'){
                        String d = new String(ch);
                        mensajeR = mensajeR + d;
                    }
                } while(ch[0] != '\0');

                //Se pasa a int
                res = Integer.parseInt(mensajeR);
                
                //si el resultado es 0, copiamos el fichero remoto del socket al fichero local
                if(res == 0){
                    //introducimos un sleep para dar tiempo al servidor de que comience a enviar el fichero
                    //Thread.sleep(500);
                    // abrimos un file para escribir
                    OutputStream outFile = new FileOutputStream(local_file_name);
                    DataOutputStream outputfile = new DataOutputStream(outFile);
                    // leemos del socket y escribimos en el file
                    copy.copyFile(in, outputfile);
                    //cerramos el fichero 
                    outputfile.close();
                }
                
                /*
                OutputStream out = new OutputStream(sc.getOutputStream());
                ObjectOutput s = new ObjectoutputStream(out);
                InputStream in = new InputStream(sc.getInputStream());
                
                System.out.println("Enviamos el GET_FILE");
                String mensaje = "GET_FILE";
                s.writeObject(mensaje);
                */
                
            }
            
        }
        catch(Exception e){
            System.err.println("excepcion " + e.toString());
            res = 2;
        }
        if(res == 0){
            System.out.println("c> GET_FILE OK");
        } else if(res == 1){
            System.out.println("c> GET_FILE FAIL, FILE DOES NOT EXIST");
        } else {
            System.out.println("c> GET_FILE FAIL");
        }
        
        return res;
	}

	
	/**
	 * @brief Command interpreter for the client. It calls the protocol functions.
	 */
	static void shell() 
	{
		boolean exit = false;
		String input;
		String [] line;
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

		while (!exit) {
			try {
				System.out.print("c> ");
				input = in.readLine();
				line = input.split("\\s");

				if (line.length > 0) {
					/*********** REGISTER *************/
					if (line[0].equals("REGISTER")) {
						if  (line.length == 2) {
							register(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: REGISTER <userName>");
						}
					} 
					
					/********** UNREGISTER ************/
					else if (line[0].equals("UNREGISTER")) {
						if  (line.length == 2) {
							unregister(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: UNREGISTER <userName>");
						}
                    			} 
                    			
                    			/************ CONNECT *************/
                    			else if (line[0].equals("CONNECT")) {
						if  (line.length == 2) {
							connect(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: CONNECT <userName>");
                    				}
                    			} 
                    
                    			/********** DISCONNECT ************/
                    			else if (line[0].equals("DISCONNECT")) {
						if  (line.length == 2) {
							disconnect(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: DISCONNECT <userName>");
                    				}
                    			} 
                    
                    			/************** PUBLISH **************/
                    			else if (line[0].equals("PUBLISH")) {
						if  (line.length >= 3) {
							// Remove first two words
							//String description = input.substring(input.indexOf(' ')+1).substring(input.indexOf(' ')+1);
							String description = input.substring(input.indexOf(' ')+1);
							description = description.substring(description.indexOf(' ')+1);
							publish(line[1], description); // file_name = line[1]
						} else {
							System.out.println("Syntax error. Usage: PUBLISH <file_name> <description>");
                    				}
                    			} 

                    			/************ DELETE *************/
                    			else if (line[0].equals("DELETE")) {
						if  (line.length == 2) {
							delete(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: DELETE <file name>");
                    				}
                    			} 
                    
                    			/************** LIST_USERS **************/
                    			else if (line[0].equals("LIST_USERS")) {
						if  (line.length == 1) {
							// Remove first two words
							list_users(); 
						} else {
							System.out.println("Syntax error. Usage: LIST_USERS ");
                    				}
                    			} 
                    
                    			/************ LIST_CONTENT *************/
                    			else if (line[0].equals("LIST_CONTENT")) {
						if  (line.length == 2) {
							list_content(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: LIST_CONTENT <user name>");
                    				}
                    			} 
                    
                    			/************** GET_FILE **************/
                    			else if (line[0].equals("GET_FILE")) {
						if  (line.length == 4) {
							get_file(line[1], line[2], line[3]); 
						} else {
							System.out.println("Syntax error. Usage: GET_FILE <user> <remote_file_name> <local_file_name>");
                    				}
                    			} 

                    
                    			/************** QUIT **************/
                    			else if (line[0].equals("QUIT")){
						if (line.length == 1) {
							exit = true;
						} else {
							System.out.println("Syntax error. Use: QUIT");
						}
					} 
					
					/************* UNKNOWN ************/
					else {						
						System.out.println("Error: command '" + line[0] + "' not valid.");
					}
				}				
			} catch (java.io.IOException e) {
				System.out.println("Exception: " + e);
				e.printStackTrace();
			}
		}
	}
	
	/**
	 * @brief Prints program usage
	 */
	static void usage() 
	{
		System.out.println("Usage: java -cp . client -s <server> -p <port>");
	}
	
	/**
	 * @brief Parses program execution arguments 
	 */ 
	static boolean parseArguments(String [] argv) 
	{
		Getopt g = new Getopt("client", argv, "ds:p:");

		int c;
		String arg;

		while ((c = g.getopt()) != -1) {
			switch(c) {
				//case 'd':
				//	_debug = true;
				//	break;
				case 's':
					_server = g.getOptarg();
					break;
				case 'p':
					arg = g.getOptarg();
					_port = Integer.parseInt(arg);
					break;
				case '?':
					System.out.print("getopt() returned " + c + "\n");
					break; // getopt() already printed an error
				default:
					System.out.print("getopt() returned " + c + "\n");
			}
		}
		
		if (_server == null)
			return false;
		
		if ((_port < 1024) || (_port > 65535)) {
			System.out.println("Error: Port must be in the range 1024 <= port <= 65535");
			return false;
		}

		return true;
	}

	
	/********************* MAIN **********************/
	
	public static void main(String[] argv) 
	{
        if(!parseArguments(argv)) {
            usage();
            return;
        }
        System.out.println("Host: "+_server+" Port: "+ _port);
        System.out.println("To quit type: 'QUIT' or press Ctrl+C\n");
        
        try{
            //iniciamos la shell
            shell();
            System.out.println("+++ FINISHED +++");
        }
        catch(Exception e){
            System.out.println("+++ FINISHED +++");
        }
    }
}
