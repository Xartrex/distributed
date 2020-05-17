import java.io.*;
import gnu.getopt.Getopt;

import java.lang.* ;
import java.io.* ;
import java.net.* ;
import java.util.* ;
import Mayus.MayusServiceService;
import Mayus.MayusService;
import java.util.List;

//Clase copia, ejemplo de aula global modificado para ser implantado en la práctica.
class copy {
    static void copyFile(DataInputStream inputStream, DataOutputStream outputStream) throws IOException {
        try {
            byte[] buffer = new byte[1024];
            int length = 0;
            while ((length = inputStream.read(buffer)) > 0) {
                outputStream.write(buffer, 0, length);
            }
        }
        catch(Exception e){
            System.err.println("Copy file exception: " + e.toString());
            throw e;
        }
    }
}

//Clase para la parte servidor del cliente.
class TratarPeticion extends Thread{
	private ServerSocket sc;

	public TratarPeticion(ServerSocket a){
		sc = a;
	}

    public void run(){
        try{
            while(true){
                //Se incializa el socket
                Socket s_local = sc.accept();
                DataOutputStream out = new DataOutputStream(s_local.getOutputStream());
                DataInputStream in = new DataInputStream(s_local.getInputStream());
                
                //Lee del socket
                byte[] ch = new byte[1];
                String mensajeR = new String();
                do{
                    ch[0] = in.readByte();
                    if (ch[0] != '\0'){
                        String d = new String(ch);
                        mensajeR = mensajeR + d;
                    }
                } while(ch[0] != '\0');
                //Comprobamos la cadena recibida (GET_FILE)
                if(!mensajeR.equals("GET_FILE")){
                    sc.close();
                    s_local.close();
                    return;
                }
                //Recibe el nombre del fichero
                String fichero = new String();
                do{
                    ch[0] = in.readByte();
                    if (ch[0] != '\0'){
                        String d = new String(ch);
                        fichero = fichero + d;
                    }
                } while(ch[0] != '\0');
                File fd = new File(fichero);

                //Si existe el fichero lo copia al socket y envia codigo 0
                if(fd.exists() && fd.isFile()){
                    out.writeBytes("0");
                    out.write('\0'); // inserta el código ASCII 0 al final
                    
                    InputStream inp = new FileInputStream(fichero);
                    DataInputStream inputfile = new DataInputStream(inp);
                    copy.copyFile(inputfile, out);
                    //Cerramos el fichero
                    inputfile.close();
                } else{
                    out.writeBytes("1");
                    out.write('\0'); // inserta el código ASCII 0 al final
                }
                
                s_local.close();
            }
        //Capturamos las excepciones pertinenetes
        }catch(SocketException e) {}
        catch(Exception e){
            System.err.println("exception " + e.toString());
            e.printStackTrace();
        }
    }
    
    //Función para cerrar el ServerSocket
    public void PararSSC(){
        try{
            sc.close();
        }
        catch(SocketException e) {}
        catch(Exception e){
            System.err.println("excepcion "+ e.toString());
            e.printStackTrace();
        }
    }
}

//Clase usuario y constructor para almacenar los usuarios conectados (Comando LIST_USERS)
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
	private static String usuario; //Variable global para almacenar el uusuario actualmente conectado
	private static boolean registrado;
	private static TratarPeticion sthread = null;
	
	//Arraylist para guardar los datos del LIST_USERS
    private static ArrayList<DUsuario> usuariosConectados;
	
	/********************* METHODS ********************/
	
	/**
	 * @param user - User name to register in the system
	 * 
	 * @return ERROR CODE
	 */


    //Función read, integra el proceso de lectura desde readByte() para refactorizar el código
    static String read(DataInputStream in){
        try{
            byte[] ch = new byte[1];
            String mensajeR = new String();
            do{
                ch[0] = in.readByte();
                if (ch[0] != '\0'){
                    String d = new String(ch);
                    mensajeR = mensajeR + d;
                }
            } while(ch[0] != '\0');
            return mensajeR;
        }catch(Exception e){
            System.err.println("excepcion " + e.toString() );
            e.printStackTrace();
            return "\0";
        }
	}

	static int register(String user) 
	{

        //Variable para el control de errores
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
            String mensajeR = read(in);

            //Se pasa a int
            res = Integer.parseInt(mensajeR);
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
        
            //Se envía la cadena UNREGISTER
            out.writeBytes(mensaje);
            out.write('\0'); // inserta el código ASCII 0 al final
            
            if(mensaje.equals("UNREGISTER")==true){
                out.writeBytes(user);
                out.write('\0'); // inserta el código ASCII 0 al final
            }

            //Leemos la respuesta del servidor
            String mensajeR = read(in);

            //Se pasa a int
            res = Integer.parseInt(mensajeR);
        }//fin del try

        catch (Exception e)
        {
            System.err.println("excepcion " + e.toString() );
            e.printStackTrace();
        }
        
        if(res == 0){
            System.out.println("c> UNREGISTER OK");
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
        //Se crea el ServerSocket para la futura transferencia de archivos
        ServerSocket ssc = null;
        try{
            ssc = new ServerSocket(0); //nos devuelve el socket que quiera
            new TratarPeticion(ssc).start();
        }catch(Exception e){
            System.err.println("Error cerrando socket");
        }
        int puerto = ssc.getLocalPort();
        String port = Integer.toString(puerto);
        
        int res = 3;
	if(usuario == null || usuario == "#"){
		try
		{
		    // se crea el socket del cliente
		    Socket sc = new Socket(_server, _port);
		    String mensaje = "CONNECT";
		    DataOutputStream out = new DataOutputStream(sc.getOutputStream());
		    DataInputStream in = new DataInputStream(sc.getInputStream());
		
            //Se envía la cadena CONNECT
		    out.writeBytes(mensaje);
		    out.write('\0'); // inserta el código ASCII 0 al final
		    
		    if(mensaje.equals("CONNECT")==true){

			out.writeBytes(user);
			out.write('\0'); // inserta el código ASCII 0 al final
			out.writeBytes(port);
			out.write('\0');
		    }

		    //Leemos la respuesta del servidor
		    String mensajeR = read(in);

		    //Se pasa a int
		    res = Integer.parseInt(mensajeR);
		    
		    if (res == 0){
		    //mete en la variable global el usuario conectado, uno por terminal a la vez
		    usuario = user;

		    sthread = new TratarPeticion(ssc);
		    sthread.start();
		    }
		
		}//fin del try

		catch (Exception e)
		{
		    System.err.println("excepcion " + e.toString() );
		    e.printStackTrace();
		}
	}else{ res = 2;}
	
        if(res == 0){
            System.out.println("c> CONNECT OK");
        } else if(res == 1){
            System.out.println("c> CONNECT FAIL, USER DOES NOT EXIST");
        } else if(res == 2){
            System.out.println("c> CONNECT FAIL, USER ALREADY CONNECTED");
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
            
            //Se envía la cadena DISCONNECT
            out.writeBytes(mensaje);
            out.write('\0'); // inserta el código ASCII 0 al final
            
            if(mensaje.equals("DISCONNECT")==true){
                out.writeBytes(user);
                out.write('\0'); // inserta el código ASCII 0 al final
            }
           /* 
            if (false == user.equals(usuario)){
                res = 3;
            }*/
                String mensajeR = read(in);

                //Se pasa a int
                res = Integer.parseInt(mensajeR);
            
                usuariosConectados = new ArrayList<DUsuario>(); //Esta lista ya no nos sirve pues está desactualizada
            
        }//fin del try

        catch (Exception e)
        {
            //Ha fallado la desconexión, debemos resetear la lista de usuarios, el usuario conectado y el thread para GET_FILE
            System.err.println("excepcion " + e.toString() );
            e.printStackTrace();
            usuariosConectados = new ArrayList<DUsuario>();
            res = 3;
            //Debemos parar el server socket ante un error desconocido por precaución
            if(sthread != null){
                sthread.PararSSC();
                sthread = null;
                usuario = null;
            }
        }
        //Si nos desconectamos correctamente debemos parar el server socket
        if(res == 0){
            sthread.PararSSC();
            sthread = null;
            usuario = null;
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
        
    	    if(usuario == null){
	    	usuario = "#";
    	    }	
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

                    //Se llama al servicio web para pasar la cadena descripción a mayúsculas
                    URL url = new URL("http://localhost:8888/Mayus");
                    MayusServiceService service = new MayusServiceService(url);
                    MayusService port = service.getMayusServicePort();

                    String descripcionMAYUS = port.toUpper(description);
                    
                    System.out.println(description);
                    System.out.println(descripcionMAYUS);
                    
                    out.writeBytes(descripcionMAYUS);
                    out.write('\0');
                }
                
                //Leemos la respuesta del servidor
                String mensajeR = read(in);

                //Se pasa a int
                res = Integer.parseInt(mensajeR);
                
            }//fin del try

            catch (Exception e)
            {
                System.err.println("excepcion " + e.toString() );
                e.printStackTrace();
            }
        
                
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

    	    if(usuario == null){
	    	usuario = "#";
    	    }	
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
                
                //Leemos la respuesta del servidor
                String mensajeR = read(in);

                //Se pasa a int
                res = Integer.parseInt(mensajeR);
                
            }//fin del try

            catch (Exception e)
            {
                System.err.println("excepcion " + e.toString() );
                e.printStackTrace();
            }
        
        
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
    	    if(usuario == null){
	    	usuario = "#";
    	    }	
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

                //Leemos la respuesta del servidor
                String mensajeR = read(in);

                //Se pasa a int
                int conectados = Integer.parseInt(mensajeR);
                
                //La variable conectados sirve para el primer control de errores
                if(conectados == 0) {
                    //Leemos la respuesta del servidor
                    mensajeR = read(in);

                    //Despues, esta variable se emplea para almacenar el número de usuarios conectados que el servidor manda
                    conectados = Integer.parseInt(mensajeR);
                    if (conectados == -1){
                        res = 3;
                    } else  {
                        System.out.println("c> LIST_USERS OK");
                        usuariosConectados = new ArrayList<DUsuario>();
                        //Por cada usuario conectado deben imprimirse sus datos
                        while(conectados > 0){
                            mensajeR = new String();

                            //Leemos usuario del server
                            mensajeR = read(in);
                            String user = mensajeR;
                            
                            mensajeR = new String();
                            
                            //Leemos ip 
                            mensajeR = read(in);
                            String ip = mensajeR;
                            
                            mensajeR = new String();
                            
                            //Leemos puerto 
                            mensajeR = read(in);
                            String puerto = mensajeR;
                            
                            System.out.printf("%10s%10s%10s\n", user, ip, puerto);
                            usuariosConectados.add(new DUsuario(user, ip, Integer.parseInt(puerto)));
                            conectados--;
                        }
                    }
                }

	    else{ res = conectados;}
            }//fin del try

            catch (Exception e)
            {
                System.err.println("excepcion " + e.toString() );
                e.printStackTrace();
            }
        
        if(res == 0){

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
    	    if(usuario == null){
	    	usuario = "#";
    	    }	
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

                //Leemos la respuesta del servidor
                String mensajeR = read(in);

                //Se pasa a int el byte de respuesta
                int conectados = Integer.parseInt(mensajeR);
                
                //Se sigue el mismo procedimiento que con list_users
                if(conectados == 0) {
                    mensajeR = new String();
                    //Leemos la respuesta del servidor
                    mensajeR = read(in);

                    conectados = Integer.parseInt(mensajeR);
                    if (conectados == -1){
                        res = 3;
                    } else  {
                        int contador= conectados;
                        while(contador>0){
                        
                            mensajeR = new String();
                            //Leemos la respuesta del servidor
                            mensajeR = read(in);

                            System.out.println(mensajeR);
                            contador--;
                        }
                    }
                }

	    else{ res = conectados;}
            }//fin del try

            catch (Exception e)
            {
                System.err.println("excepcion " + e.toString() );
                e.printStackTrace();
            }
        
        if(res == 0){
            System.out.println("c> LIST_CONTENT OK");
        } else if(res == 1){
            System.out.println("c> LIST_CONTENT FAIL, USER DOES NOT EXIST");
        } else if(res == 2){
            System.out.println("c> LIST_CONTENT FAIL, USER NOT CONNECTED");
        } else if(res == 3){
            System.out.println("c> LIST_CONTENT FAIL, REMOTE USER DOES NOT EXIST");
        }else{
            System.out.println("c> LIST_USERS FAIL");
        }
        return res;
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
		
	    if(usuario == null){
		    usuario = "#";
	    }
            try{
                //Debemos buscar al usuario en nuestra lista
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
                
                String mensaje = "GET_FILE";
                out.writeBytes(mensaje);
                out.write('\0'); // inserta el código ASCII 0 al final
                
                if(mensaje.equals("GET_FILE")==true){
                    out.writeBytes(remote_file_name);
                    out.write('\0'); // inserta el código ASCII 0 al final
                }
                
                //Leemos la respuesta del servidor
                String mensajeR = read(in);

                //Se pasa a int
                res = Integer.parseInt(mensajeR);
                //si el resultado es 0, copiamos el fichero remoto del socket al fichero local
                if(res == 0){
                    // abrimos un file para escribir
                    OutputStream outFile = new FileOutputStream(local_file_name);
                    DataOutputStream outputfile = new DataOutputStream(outFile);
                    // leemos del socket y escribimos en el file
                    copy.copyFile(in, outputfile);
                    //cerramos el fichero 
                    outputfile.close();
                }                
            }
                
            catch(Exception e){
                System.err.println("excepcion " + e.toString());
                res = 2;
            }

        if(res == 0){
            System.out.println("c> GET_FILE OK");
        } else if(res == 1){
            System.out.println("c> GET_FILE FAIL / FILE DOES NOT EXIST");
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

    static public void attachCTRLC(){
        Runtime.getRuntime().addShutdownHook(new Thread() {
            @Override
            public void run() {
                if(sthread != null){
                    System.out.println("\n+++ DISCONNECTING +++");
                    disconnect(usuario);
                }
            }
        });
    }
	
	/********************* MAIN **********************/
	
	public static void main(String[] argv) 
	{
        attachCTRLC();
        
        if(!parseArguments(argv)) {
            usage();
            return;
        }
        System.out.println("Host: "+_server+" Port: "+ _port);
        System.out.println("To quit type: 'QUIT' or press Ctrl+C\n");
        
        try{
            //iniciamos la shell
            shell();
            //si el server esta activo lo desactiva
            if(sthread != null){
                System.out.println("\n+++ DISCONNECTING +++");
                disconnect(usuario);
            }
            System.out.println("+++ FINISHED +++");
        }
        catch(Exception e){
            System.out.println("+++ FINISHED +++");
        }
    }
}
