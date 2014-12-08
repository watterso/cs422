import java.io.DataInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class fsclient{
	public static void main(String[] args) throws NumberFormatException, UnknownHostException{
		if(args.length < 4){
			System.out.println(args.length);
			for(int i=0; i<args.length; i++){
				System.out.print(args[i]+"|");
			}
			System.out.println("Syntax Error");
			System.out.println("usage:\n\tfsclient ip-address port pathname filename");
		}
		else{
			Socket le_socket;
			try {
				le_socket = new Socket(args[0], Integer.parseInt(args[1]));
				PrintWriter oot = new PrintWriter(le_socket.getOutputStream());
				oot.write(args[2]);
				oot.flush();
				DataInputStream een = new DataInputStream(le_socket.getInputStream());
				byte[] init_line = new byte[15];
				een.read(init_line);
				String init = "";
				for(byte b : init_line){
					init+=(char)b;
				}
				if(!"INVALID REQUEST".equals(init)){
					FileOutputStream out_file = new FileOutputStream(args[3]);
					out_file.write(init_line);
					int c = een.read(init_line);
					while(c!=-1){
						out_file.write(init_line, 0, c);
						c = een.read(init_line);
					}
					out_file.close();
					oot.close();
					een.close();
					le_socket.close();
				}else{
					System.out.println("ERROR: FILE NOT FOUND");
				}

			} catch (IOException e) {
				System.exit(-1);
				e.printStackTrace();
			}


		}
	}
}

