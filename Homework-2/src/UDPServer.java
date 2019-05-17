import java.net.*;
import java.io.*;
public class UDPServer{
    public static void main(String args[]){
		if (args.length < 1) {
			System.out.println(
					"Incorrect inputs. Quit."
			);
			System.out.println(
					"Usage:java UDPServer <server port>");
			return;
		}
    	DatagramSocket aSocket = null;
		try{
			aSocket = new DatagramSocket(Integer.parseInt(args[0])); // For example: 6789
					// create socket at agreed port
			byte[] buffer = new byte[1000];
 			while(true){
 				DatagramPacket request = new DatagramPacket(buffer, buffer.length);
  				aSocket.receive(request);     
    			DatagramPacket reply = new DatagramPacket(request.getData(), request.getLength(), 
    				request.getAddress(), request.getPort());
    			aSocket.send(reply);
    		}
		}catch (SocketException e){System.out.println("Socket: " + e.getMessage());
		}catch (IOException e) {System.out.println("IO: " + e.getMessage());
		}finally {if(aSocket != null) aSocket.close();}
    }
}
