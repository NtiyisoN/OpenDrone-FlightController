package at.opendrone.opendrone.network;

import android.app.Activity;
import android.os.NetworkOnMainThreadException;
import android.util.Log;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.util.Stack;
import java.util.Timer;
import java.util.TimerTask;
import java.util.logging.Logger;

import at.opendrone.opendrone.R;

import static java.util.Objects.requireNonNull;

public class TCPClient extends Thread{
    private Socket server;
    private PrintWriter serverWriter;
    private BufferedReader serverReader;
    private String target;

    private Stack<String> value = new Stack<String>();
    public String rec;
    private int state = 0;

    private static final int PORT = 2018;
    private static final String TAG = "udpy";

    public TCPClient(String target){
        this.target = target;
    }

    public void setValue(String msg){
        this.value.push(msg);
    }


    @Override
    public void run() {
            initSocket();
            while(!this.isInterrupted()){
                if(this.state > 0){
                    // Please insert Error-Handling here
                    Log.println(100,TCPClient.TAG,"No ACK");
                    sendMessage("No ACK recieved, shutting down connection...");
                    try {
                        this.server.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    this.interrupt();
                }
                if(this.value.size() > 0){
                    sendMessage(this.value.pop());
                    TimerTask ft = new TimerTask(){
                        public void run(){
                            receiveMessage();
                            if (TCPClient.this.rec == null || server.isClosed()){
                                TCPClient.this.state = 1;
                            }
                        }
                    };

                    (new Timer()).schedule(ft, 100);
                }

            }
    }

    private void sendValue(String message){
        sendMessage(message);
    }

    public String getMessage(){
        return this.rec;
    }

    /*sends message to server*/
    public void sendMessage(String message) {
        try{
            requireNonNull(serverWriter);
            serverWriter.println(message);
            serverWriter.flush();
        }catch(Exception ex){
            initSocket();
        }
    }

    public void receiveMessage(){
        String line = null;

        try {
            try {
                if (serverReader.ready()) {
                    while ((line = serverReader.readLine()) != null) {
                        this.rec = line;
                        break;
                    }
                }
            }catch(SocketTimeoutException ex){
                this.value.push("ERROR STATE");
            }

        }catch(Exception e){
            Log.i("INFORMATIONY","Errorrly "+e.getLocalizedMessage());
        }

    }

    /*Creates the necessary streams*/
    private void createStreams() {
        requireNonNull(server);
        try {
            serverReader = new BufferedReader(new
                    InputStreamReader(server.getInputStream()));
            serverWriter = new PrintWriter(server.getOutputStream());
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    private void initSocket(){
        if(server == null){
            try {
                server = new Socket(target, PORT);
                createStreams();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
