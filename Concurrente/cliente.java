import java.lang.* ;
import java.io.* ;
import java.net.* ;
import java.util.* ;


public class cliente
{
	public static void main ( String [] args)
	{
		byte bsend[] = new byte[100];
		byte brecv[] = new byte[100];

		InetAddress server_addr = null;
		//DatagramSocket s = null;
		//DatagramPacket out = null;
		int res;
		int num[] = new int[2];

		if (args.length != 2)
		{
			System.out.println("Uso: cliente <host> <dest_port>");
			System.exit(0);
		}

		try
		{
			// Crear las conexiones
			// incluir el código aqui

			// se crea el socket del cliente
			for(int i=0; i<args.length;i++){
				System.out.println("Arg "+ i+ ": " +args[i]);
			}
			int dir = Integer.parseInt(args[1]);
			Socket sc = new Socket(args[0], dir);
			// direción del servidor
			server_addr = InetAddress.getByName(args[1]);

			InputStreamReader is = new InputStreamReader(System.in);
			BufferedReader br = new BufferedReader(is);

			String mensaje;
			boolean hecho = false;

			while (!hecho) {
				// completar el código para comunicar con el servidor
				Scanner scan = new Scanner(System.in);
			
				// entrada de una cadena
			        mensaje = scan.nextLine();
				
				DataOutputStream out = new DataOutputStream(sc.getOutputStream());
		
				out.writeBytes(mensaje);
				out.write('\0'); // inserta el código ASCII 0 al final
				if(mensaje.equals("EXIT")==true)
					break;
/*
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
*/
			}

		}
		catch (Exception e)
		{
			 System.err.println("excepcion " + e.toString() );
			 e.printStackTrace();
		}
	}
}

