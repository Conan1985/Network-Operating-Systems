import java.net.*;
import java.io.*;
// CISC 611-90-O-2019/Late Spring - Network Operating Systems Homework-2
// Youwei Lu
public class TCPServer{
	public static void main(String args[]) {
		if (args.length < 1) {
			System.out.println(
					"Incorrect inputs. Quit."
			);
			System.out.println("Usage:java UDPClient <server port>");
			System.out.println("While reading message, use enter key to read the next message  ");
			return;
		}
		Socket clientSocket = null;
		try {
			int serverPort = Integer.parseInt(args[0]); // For example, 7896
			ServerSocket listenSocket = new ServerSocket(serverPort);
			clientSocket = listenSocket.accept();
			DataInputStream in = new DataInputStream(clientSocket.getInputStream());
			DataOutputStream out = new DataOutputStream(clientSocket.getOutputStream());
			BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
			String messageIn = "";
			while (!messageIn.equals("quit")) {
				messageIn = in.readUTF();
				System.out.println("Receive messge: " + messageIn);
				String messageOut = br.readLine();
				out.writeUTF(messageOut);
				out.flush();
			}
			in.close();
			clientSocket.close();
			listenSocket.close();
		} catch(IOException e) {System.out.println("Listen socket:"+e.getMessage());
		} finally{ try {clientSocket.close();}catch (IOException e){/*close failed*/}}
	}
}