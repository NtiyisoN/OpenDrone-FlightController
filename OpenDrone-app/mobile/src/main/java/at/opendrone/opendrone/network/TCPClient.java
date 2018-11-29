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
import java.util.Stack;

import at.opendrone.opendrone.R;

import static java.util.Objects.requireNonNull;

public class TCPClient extends Thread{
    private Socket server;
    private PrintWriter serverWriter;
    private BufferedReader serverReader;
    private String target;

    private Activity activity;

    private Stack<String> value = new Stack<String>();
    private String rec = "";

    private static final int PORT = 2018;
    private static final String TAG = "udpy";

    public TCPClient(String target, Activity activity){
        this.target = target;
        this.activity = activity;
    }

    public void setValue(String msg){
        this.value.push(msg);
    }

    @Override
    public void run() {
            initSocket();
            while(!this.isInterrupted()){
                if(this.value.size() > 0){
                    sendMessage(this.value.pop());
                    receiveMessage();
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
        // *** My question is about the next line **/
        try {
            while ((line = serverReader.readLine()) != null) {
                this.rec = line;
                break;
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
