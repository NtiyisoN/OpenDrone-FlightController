package at.opendrone.opendrone.network;

import android.util.Log;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

public class UDPClient extends Thread{
    private DatagramSocket clientSocket =null;
    private String target;

    private int value;
    private int lastValue;

    private static final int PORT = 2018;
    private static final String TAG = "updy";

    public UDPClient(String target){
        this.target = target;
    }

    public void setValue(int value){
        this.value = value;
    }

    @Override
    public void run() {
        try{
            initSocket();
            while(!this.isInterrupted()){
                if(lastValue != value){
                    sendValue();
                }
                lastValue = value;
            }
        }catch(SocketException ex){
            Log.e(TAG, "SocketException",ex);

        }
    }

    private void sendValue(){
        Log.i(TAG, "Sending value "+value);
        try {
            byte[] buf = new byte [256];
            String sendText = ""+value;
            buf = sendText.getBytes();
            DatagramPacket packet = new DatagramPacket(buf, buf.length);
            InetAddress to = InetAddress.getByName(target);
            packet.setPort(PORT);
            packet.setAddress(to);
            clientSocket.send(packet);

            Log.i(TAG, "sent value "+sendText);

        } catch (IOException ex) {
            Log.e(TAG, "IOException",ex);
        }
    }

    private void initSocket() throws SocketException {
        if(clientSocket == null){
            clientSocket = new DatagramSocket();
        }
    }
}
