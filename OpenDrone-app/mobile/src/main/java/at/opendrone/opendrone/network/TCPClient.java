package at.opendrone.opendrone.network;

import android.app.Activity;
import android.util.Log;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

import at.opendrone.opendrone.R;

import static java.util.Objects.requireNonNull;

public class TCPClient extends Thread{
    private Socket server;
    private PrintWriter serverWriter;
    private BufferedReader serverReader;
    private String target;

    private Activity activity;

    private int value;
    private int lastValue;

    private static final int PORT = 2018;
    private static final String TAG = "udpy";

    public TCPClient(String target, Activity activity){
        this.target = target;
        this.activity = activity;
    }

    public void setValue(int value){
        this.value = value;
    }

    @Override
    public void run() {
            initSocket();
            while(!this.isInterrupted()){
                if(lastValue != value){
                    sendValue();
                }
                lastValue = value;
            }
    }

    private void sendValue(){
        Log.i(TAG, "Sending value "+value);
        String sendText = ""+value;
        sendMessage(sendText);
        Log.i(TAG, "sent value "+sendText);
    }

    /*sends message to server*/
    private void sendMessage(String message) {
        try{
            requireNonNull(serverWriter);
            serverWriter.print(message);
            serverWriter.flush();
        }catch(Exception ex){
            //Toast.makeText(activity, activity.getResources().getString(R.string.exception_sorry), Toast.LENGTH_SHORT).show();
            initSocket();
            Log.e(TAG, "Error", ex);
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
