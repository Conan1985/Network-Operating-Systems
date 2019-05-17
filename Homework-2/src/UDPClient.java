import java.net.*;
import java.io.*;
public class UDPClient{
    public static void main(String args[]){
    	// args give message contents and destination hostname
		if (args.length < 4) {
			System.out.println(
					"Incorrect inputs. Quit."
			);
			System.out.println(
					"Usage:java UDPClient <hostname> <server port> <number of message to be sent> <length of messages>");
			return;
		}
		DatagramSocket aSocket = null;
		try {
			aSocket = new DatagramSocket();
			InetAddress aHost = InetAddress.getByName(args[0]); // For example: localhost
			int serverPort = Integer.parseInt(args[1]); // For example: 6789
			int number = Integer.parseInt(args[2]);
			int length = Integer.parseInt(args[3]);
			String text = "";
			for (int i = 0; i < length; i++) {
				text += '*';
			}
			for (int i = 1; i <= number; i++) {
				String message = "Message number " + i + text;
				byte [] m = message.getBytes();
				DatagramPacket request =
						new DatagramPacket(m,  message.length(), aHost, serverPort);
				aSocket.send(request);
				byte[] buffer = new byte[1000];
				DatagramPacket reply = new DatagramPacket(buffer, buffer.length);
				aSocket.receive(reply);
				System.out.println("Reply: " + new String(reply.getData()));
			}
		}catch (SocketException e){System.out.println("Socket: " + e.getMessage());
		}catch (IOException e){System.out.println("IO: " + e.getMessage());
		}finally {if(aSocket != null) aSocket.close();}
	}		      	
}
