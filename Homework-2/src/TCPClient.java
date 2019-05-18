import java.net.*;
import java.io.*;
// CISC 611-90-O-2019/Late Spring - Network Operating Systems Homework-2
// Youwei Lu
public class TCPClient {
	public static void main (String args[]) {
		// args give message contents and server hostname
		if (args.length < 2) {
			System.out.println(
					"Incorrect inputs. Quit."
			);
			System.out.println("Usage:java UDPClient <hostname> <server port>");
			System.out.println("While sending message, input 'quit' to quit.");
			return;
		}
		Socket s = null;
		try{
			int serverPort = Integer.parseInt(args[1]); // For example, 7896
			String aHost = args[0]; // For example, localhost
			s = new Socket(aHost, serverPort);
			DataInputStream in = new DataInputStream( s.getInputStream());
			DataOutputStream out =new DataOutputStream( s.getOutputStream());
			BufferedReader br =new BufferedReader(new InputStreamReader(System.in));
			String data = "";
			while (!data.equals("quit")) {
				data = br.readLine();
				out.writeUTF(data);
			}
			out.flush();
			out.close();
			s.close();
		}catch (UnknownHostException e){System.out.println("Socket:"+e.getMessage());
		}catch (EOFException e){System.out.println("EOF:"+e.getMessage());
		}catch (IOException e){System.out.println("readline:"+e.getMessage());
		}finally {if(s!=null) try {s.close();}catch (IOException e){System.out.println("close:"+e.getMessage());}}
     }
}