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
		DatagramSocket s = null;
		//DatagramPacket out = null;
		int res;
		int num[] = new int[2];
		System.out.println("PATATA");

		//if (args.length != 2)
		//{
			//System.out.println("Uso: cliente <host> <puerto>");
			//System.exit(0);
		//}

		try
		{
			// Crear las conexiones
			// incluir el código aqui

			// se crea el socket del cliente
			Socket sc = new Socket("localhost", 2800);
			// direción del servidor
			server_addr = InetAddress.getByName("localhost");

			InputStreamReader is = new InputStreamReader(System.in);
			BufferedReader br = new BufferedReader(is);

			String mensaje;
			boolean hecho = false;

			while (!hecho) {
				// completar el código para comunicar con el servidor
				System.out.println("Escriba algo");
				mensaje = new String("Hola");

				DataOutputStream out = new DataOutputStream(sc.getOutputStream());

				out.writeBytes(mensaje);
				out.write('\0'); // inserta el código ASCII 0 al final

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

			}

		}
		catch (Exception e)
		{
			 System.err.println("excepcion " + e.toString() );
			 e.printStackTrace();
		}
	}
}

