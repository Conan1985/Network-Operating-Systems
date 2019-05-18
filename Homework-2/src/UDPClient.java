import java.net.*;
import java.io.*;
// CISC 611-90-O-2019/Late Spring - Network Operating Systems Homework-2
// Youwei Lu
public class UDPClient{
	public static void main(String args[]){
		int timeoutLimit = 1000;
		DatagramSocket aSocket = null;
	// args give message contents and server hostname
		if (args.length < 2) {
			System.out.println(
					"Incorrect inputs. Quit."
			);
			System.out.println("Usage:java UDPClient <hostname> <server port>");
			System.out.println("While sending message, input 'quit' to quit.");
			return;
		}
		if (args.length == 2) {
			testByUserInput(timeoutLimit, args, aSocket); // Excercise 4.4
		}
		if (args.length == 3) {
			System.out.println(
					"Incorrect inputs. Quit."
			);
			System.out.println(
					"Usage:java UDPClient <hostname> <server port> <number of message to be sent> <length of messages>"
			);
			return;
		}
		if (args.length == 4) {
			testByAutoGeneration(timeoutLimit, args, aSocket); // Excercise 4.3
		}
	}

	private static void testByUserInput(int timeoutLimit, String args[], DatagramSocket aSocket) {
		try {
			aSocket = new DatagramSocket();
			InetAddress aHost = InetAddress.getByName(args[0]); // For example: localhost
			int serverPort = Integer.parseInt(args[1]); // For example: 6789
			BufferedReader inFromUser = new BufferedReader(new InputStreamReader(System.in));
			String message;
			byte [] m;
			DatagramPacket request;
			message = inFromUser.readLine();
			if(message.length() > 0){
				m = message.getBytes();
				request = new DatagramPacket(m, m.length, aHost, serverPort);
				aSocket.send(request);
				aSocket.setSoTimeout(timeoutLimit);
			}
			while(aSocket.getSoTimeout() > 0){
				// recieve data until timeout
				try {
					byte[] buffer = new byte[1000];
					DatagramPacket reply = new DatagramPacket(buffer, buffer.length);
					aSocket.receive(reply);
					System.out.println("Reply: " + new String(reply.getData()));
					if(aSocket.getSoTimeout() > 0){
						message = inFromUser.readLine();
						if(message.equals("quit")){
							System.out.println("Socket Closed !!! ");
							aSocket.close();
						} if(message.length() > 0){
							m = message.getBytes();
							request = new DatagramPacket(m, m.length, aHost, serverPort);
							aSocket.send(request);
							aSocket.setSoTimeout(timeoutLimit);
						}
					}
				}
				catch (SocketTimeoutException e) {
					// timeout exception.
					System.out.println("Timeout reached!!! " + e);
					aSocket.close();
				}
			}

		} catch (SocketException e){System.out.println("Socket: " + e.getMessage());
		} catch (IOException e){System.out.println("IO: " + e.getMessage());
		} finally { if(aSocket != null) aSocket.close();}
	}

	private static void testByAutoGeneration(int timeoutLimit, String args[], DatagramSocket aSocket) {
		try {
			aSocket = new DatagramSocket();
			InetAddress aHost = InetAddress.getByName(args[0]); // For example: localhost
			int serverPort = Integer.parseInt(args[1]); // For example: 6789
			int number = Integer.parseInt(args[2]);
			int length = Integer.parseInt(args[3]);
			String message;
			byte [] m;
			DatagramPacket request;
			message = "*";
			if(message.length() > 0){
				m = message.getBytes();
				request = new DatagramPacket(m, m.length, aHost, serverPort);
				aSocket.send(request);
				aSocket.setSoTimeout(timeoutLimit);
			}
			int count = 1;
			while(aSocket.getSoTimeout() > 0 && count <= number + 1){
				// recieve data until timeout
				try {
					byte[] buffer = new byte[1000];
					DatagramPacket reply = new DatagramPacket(buffer, buffer.length);
					aSocket.receive(reply);
					System.out.println("Reply: " + new String(reply.getData()));
					if(aSocket.getSoTimeout() > 0){
						message = "";
						for (int i = 0; i < length; i++) {
							message += "*";
						}
						message = "Message number " + count + message;
						if(message.length() > 0){
							m = message.getBytes();
							request = new DatagramPacket(m, m.length, aHost, serverPort);
							aSocket.send(request);
							aSocket.setSoTimeout(timeoutLimit);
						}
					}
					count ++;
				}
				catch (SocketTimeoutException e) {
					// timeout exception.
					System.out.println("Timeout reached!!! " + e + " at " + count + " message");
					aSocket.close();
				}
			}
		} catch (SocketException e){System.out.println("Socket: " + e.getMessage());
		} catch (IOException e){System.out.println("IO: " + e.getMessage());
		} finally { if(aSocket != null) aSocket.close();}
	}
}