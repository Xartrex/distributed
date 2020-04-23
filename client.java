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
}

class client {

	
	/******************* ATTRIBUTES *******************/
	
	private static String _server   = null;
	private static int _port = -1;
		
	
	/********************* METHODS ********************/
	
	/**
	 * @param user - User name to register in the system
	 * 
	 * @return ERROR CODE
	 */
	static int register(String user) 
	{

		InetAddress server_addr = null;
		int res;
		int num[] = new int[2];


		try
		{
			// Crear las conexiones

			// se crea el socket del cliente
			Socket sc = new Socket(_server, _port);
			//int dir = Integer.parseInt(_port);
			
			// direción del servidor
			server_addr = InetAddress.getByName(_server);

			InputStreamReader is = new InputStreamReader(System.in);
			BufferedReader br = new BufferedReader(is);

			String mensaje = "REGISTER";
			boolean hecho = false;

			
			DataOutputStream out = new DataOutputStream(sc.getOutputStream());
		
			out.writeBytes(mensaje);
			out.write('\0'); // inserta el código ASCII 0 al final
			
				if(mensaje.equals("REGISTER")==true){
					out.writeBytes(user);
					out.write('\0'); // inserta el código ASCII 0 al final
				}
				//if(mensaje.equals("terminar")==true){
			//		break;
			//	}
			DataInputStream in = new DataInputStream(sc.getInputStream());
			byte[] ch = new byte[1];
			String mensajeR = new String();
			do{
				ch[0] = in.readByte();
				if (ch[0] != '\0'){
					String d = new String(ch);
					mensajeR = mensajeR + d;
				}
			} while(ch[0] != '\0');

			//mensajeR = br.readLine();
			System.out.println(mensajeR);

		}//fin del try

		catch (Exception e)
		{
			 System.err.println("excepcion " + e.toString() );
			 e.printStackTrace();
		}
		return 0;
	}
	
	/**
	 * @param user - User name to unregister from the system
	 * 
	 * @return ERROR CODE
	 */
	static int unregister(String user) 
	{

		InetAddress server_addr = null;
		int res;
		int num[] = new int[2];


		try
		{
			// Crear las conexiones
			// incluir el código aqui

			// se crea el socket del cliente
		
			//int dir = Integer.parseInt(_port);
			Socket sc = new Socket(_server, _port);
			// direción del servidor
			server_addr = InetAddress.getByName(_server);

			InputStreamReader is = new InputStreamReader(System.in);
			BufferedReader br = new BufferedReader(is);

			String mensaje = "UNREGISTER";
			boolean hecho = false;

			
			DataOutputStream out = new DataOutputStream(sc.getOutputStream());
		
			out.writeBytes(mensaje);
			out.write('\0'); // inserta el código ASCII 0 al final
			
				if(mensaje.equals("UNREGISTER")==true){
					out.writeBytes(user);
					out.write('\0'); // inserta el código ASCII 0 al final
				}
				//if(mensaje.equals("terminar")==true){
			//		break;
			//	}
			DataInputStream in = new DataInputStream(sc.getInputStream());
			byte[] ch = new byte[1];
			String mensajeR = new String();
			do{
				ch[0] = in.readByte();
				if (ch[0] != '\0'){
					String d = new String(ch);
					mensajeR = mensajeR + d;
				}
			} while(ch[0] != '\0');

			//mensajeR = br.readLine();
			//System.out.println(mensajeR);

		}//fin del try

		catch (Exception e)
		{
			 System.err.println("excepcion " + e.toString() );
			 e.printStackTrace();
		}
		System.out.println("UNREGISTER " + user);
		return 0;
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
		
		//empieza lo tipico de lo que hace el cliente
		InetAddress server_addr = null;
		int res;
		int num[] = new int[2];


		try
		{
			// Crear las conexiones
			// incluir el código aqui

			// se crea el socket del cliente
		
			//int dir = Integer.parseInt(_port);
			Socket sc = new Socket(_server, _port);
			// direción del servidor
			server_addr = InetAddress.getByName(_server);

			InputStreamReader is = new InputStreamReader(System.in);
			BufferedReader br = new BufferedReader(is);

			String mensaje = "CONNECT";
			boolean hecho = false;

			
			DataOutputStream out = new DataOutputStream(sc.getOutputStream());
		
			out.writeBytes(mensaje);
			out.write('\0'); // inserta el código ASCII 0 al final
			
				if(mensaje.equals("CONNECT")==true){
					out.writeBytes(user);
					out.write('\0'); // inserta el código ASCII 0 al final
					out.writeBytes(port);
					out.write('\0');
				}
				//if(mensaje.equals("terminar")==true){
			//		break;
			//	}
			DataInputStream in = new DataInputStream(sc.getInputStream());
			byte[] ch = new byte[1];
			String mensajeR = new String();
			do{
				ch[0] = in.readByte();
				if (ch[0] != '\0'){
					String d = new String(ch);
					mensajeR = mensajeR + d;
				}
			} while(ch[0] != '\0');

			//mensajeR = br.readLine();
			//System.out.println(mensajeR);

		}//fin del try

		catch (Exception e)
		{
			 System.err.println("excepcion " + e.toString() );
			 e.printStackTrace();
		}

		return 0;
	}
	
	 /**
	 * @param user - User name to disconnect from the system
	 * 
	 * @return ERROR CODE
	 */
	static int disconnect(String user) 
	{
		// Write your code here
		System.out.println("DISCONNECT " + user);
		return 0;
	}

	 /**
	 * @param file_name    - file name
	 * @param description - descrition
	 * 
	 * @return ERROR CODE
	 */
	static int publish(String file_name, String description) 
	{
		byte bsend[] = new byte[100];
		byte brecv[] = new byte[100];

		InetAddress server_addr = null;
		//DatagramSocket s = null;
		//DatagramPacket out = null;
		int res;
		int num[] = new int[2];


		try
		{
			// Crear las conexiones
			// incluir el código aqui

			// se crea el socket del cliente
		
			//int dir = Integer.parseInt(_port);
			Socket sc = new Socket(_server, _port);
			// direción del servidor
			server_addr = InetAddress.getByName(_server);

			InputStreamReader is = new InputStreamReader(System.in);
			BufferedReader br = new BufferedReader(is);

			String mensaje = "PUBLISH";
			boolean hecho = false;

			
			DataOutputStream out = new DataOutputStream(sc.getOutputStream());
		
			out.writeBytes(mensaje);
			out.write('\0'); // inserta el código ASCII 0 al final
			
				if(mensaje.equals("PUBLISH")==true){
					out.writeBytes(file_name);
					out.write('\0'); // inserta el código ASCII 0 al final
					out.writeBytes(description);
					out.write('\0');
				}
				
			DataInputStream in = new DataInputStream(sc.getInputStream());
			byte[] ch = new byte[1];
			String mensajeR = new String();
			do{
				ch[0] = in.readByte();
				if (ch[0] != '\0'){
					String d = new String(ch);
					mensajeR = mensajeR + d;
				}
			} while(ch[0] != '\0');

			//mensajeR = br.readLine();
			//System.out.println(mensajeR);

		}//fin del try

		catch (Exception e)
		{
			 System.err.println("excepcion " + e.toString() );
			 e.printStackTrace();
		}
		System.out.println("PUBLISH " + file_name + " " + description);
		return 0;
	}

	 /**
	 * @param file_name    - file name
	 * 
	 * @return ERROR CODE
	 */
	static int delete(String file_name)
	{
		// Write your code here
		System.out.println("DELETE " + file_name);
		return 0;
	}

	 /**
	 * @return ERROR CODE
	 */
	static int list_users()
	{
		// Write your code here
		System.out.println("LIST_USERS " );
		return 0;
	}


	 /**
	 * @param user_name    - user name
	 * 
	 * @return ERROR CODE
	 */
	static int list_content(String user_name)
	{
		// Write your code here
		System.out.println("LIST_CONTENT " + user_name);
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
		// Write your code here
		System.out.println("GET_FILE " + user_name + " "  + remote_file_name + " " + local_file_name);
		return 0;
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
		
		// Write code here
		
		shell();
		System.out.println("+++ FINISHED +++");
	}
}
