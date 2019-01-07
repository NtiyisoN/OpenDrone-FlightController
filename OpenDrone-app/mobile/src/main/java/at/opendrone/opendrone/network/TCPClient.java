package at.opendrone.opendrone.network;

import android.os.AsyncTask;
import android.util.Log;


import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

import at.opendrone.opendrone.OpenDroneUtils;

public class TCPClient extends AsyncTask<Void, Void, String> {
    private static final int PORT = 2018;
    private static final String TAG = "modbusy";
    private static final int UNIT_ID = 1;
    private static String target;

    private int data1 = 0;
    private byte code1 = OpenDroneUtils.CODE_THROTTLE_UP;
    private int data2 = 0;
    private byte code2 = OpenDroneUtils.CODE_YAW_LEFT;


    private Socket clientSocket;

    public TCPClient(String target) {
        this.target = target;
    }

    public void updateValues(int data1, byte code1, int data2, byte code2){
        this.data1 = data1;
        Log.i("Testy23", data1+"");
        this.code1 = code1;
        this.code2 = code2;
        this.data2 = data2;
    }

    private void send(){

        if(clientSocket == null){
            Log.i(TAG, "Socket is null!");
            return;
        }

        Log.i("Testy2", data1+"");
        OpenDroneFrame f = new OpenDroneFrame((byte)1, code1, data1, code2, data2);
        Log.i("Testy2", data1+"");

        //Log.i("tcpy")

       /* try {
            boolean before = master.readCoils(UNIT_ID, 1, 1).getBit(0);
            SerialParameters params = new SerialParameters();
            params.setBaudRate(9600);
            params.setParity(0);
            params.setStopbits(1);
            params.setDatabits(8);

            master = new ModbusSerialMaster(params);
            master.writeCoil(1,1, before);
        } catch (ModbusException e) {
            e.printStackTrace();
        }*/



        try {
            BufferedOutputStream dos = new BufferedOutputStream(new DataOutputStream(clientSocket.getOutputStream()));
            BufferedReader clientIn = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            dos.write(f.toString().getBytes());
            dos.flush();
            Log.i("tcpy", "sent: "+ f.toString());
            clientSocket.close();
        } catch (IOException ex) {
            System.err.println(ex.getMessage());
        }
    }

    private void disconnect() {
        if(clientSocket != null && clientSocket.isConnected()){
            try {
                clientSocket.close();
            } catch (IOException e) {
                Log.e("tcpy", "ERROR", e);
            }
        }
    }

    private void connect() {
        try {
            if(clientSocket != null && clientSocket.isConnected()){
                return;
            }
            clientSocket = new Socket(target, 2018);
            send();
        } catch (IOException e) {
            Log.i("tcpy", "connect");
            Log.e("tcpy", "ERROR", e);
        }
    }

    @Override
    protected String doInBackground(Void... voids) {
        connect();
        send();
        return "success";
    }

    @Override
    protected void onPostExecute(String s) {
        super.onPostExecute(s);
        disconnect();
    }
}
