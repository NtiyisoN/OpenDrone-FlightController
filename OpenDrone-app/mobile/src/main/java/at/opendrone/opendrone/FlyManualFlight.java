package at.opendrone.opendrone;


import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.pm.ActivityInfo;
import android.graphics.Path;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.app.Fragment;
import android.text.format.Formatter;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.TextView;

import org.w3c.dom.Text;

import java.io.IOException;
import java.util.Stack;

import at.opendrone.opendrone.network.TCPClient;
import io.github.controlwear.virtual.joystick.android.JoystickView;

import static android.content.Context.WIFI_SERVICE;
import static java.util.Objects.requireNonNull;


/**
 * A simple {@link Fragment} subclass.
 */
public class FlyManualFlight extends Fragment {

    private View view;
    private JoystickView throttle;
    private JoystickView direction;

    private TextView position;
    private TextView height;
    private TextView temp;
    private TextView status;
    private EditText ip;
    private FloatingActionButton homeFAB;

    public static String TARGET = "192.168.1.254";

    private TCPClient client;

    public FlyManualFlight() {
        // Required empty public constructor
    }

    @Override
    public void onResume() {
        getActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        ((MainActivity)getActivity()).closeDrawer();

        if(client == null){
            client = new TCPClient(TARGET);
            client.execute();
        }
        super.onResume();
    }

    @Override
    public void onPause() {
        getActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);

        /*if(client != null){
            while(client.isAlive()){
                client.interrupt();
            }
        }*/
        super.onPause();
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view =  inflater.inflate(R.layout.fragment_fly_manual_flight, container, false);
        setRetainInstance(true);
        findViews();
        setValues();
        initJoysticks();

        return view;
    }

    private void displayHomeConfirmationDialog(){
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        builder.setMessage("Do you rly want go home fam?")
                .setPositiveButton("Hell yeah", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        // FIRE ZE MISSILES!
                    }
                })
                .setNegativeButton("What am i doing here?!", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        // User cancelled the dialog
                    }
                });
        builder.create().show();
        // Create the AlertDialog object and return it
    }

    private void printAR(int[][] ar){
        for(int i = 0; i<ar.length; i++){
            for(int j = 0; j<ar.length; j++){
                Log.i("Testy", "Index: "+i+"/"+j+": "+ar[i][j]);
            }
        }
    }

    private void initJoysticks() {
        throttle = (JoystickView) view.findViewById(R.id.throttleStick);
        throttle.setOnMoveListener(new JoystickView.OnMoveListener() {
            @Override
            public void onMove(int angle, int strength) {
                int[][] cmd = interpretThrottleStick(throttle, angle, strength);
                requireNonNull(client);
                Log.i("manualFlightys", cmd[0][0]+"");


                client = new TCPClient(TARGET);
                client.updateValues(cmd[0][1], (byte)cmd[0][0], cmd[1][1], (byte)cmd[1][0]);
                client.execute();
                //client.execute();
                //client.setValue(cmd);
            }
        });

        direction = (JoystickView) view.findViewById(R.id.directionStick);
        direction.setOnMoveListener(new JoystickView.OnMoveListener() {
            @Override
            public void onMove(int angle, int strength) {
                int[][] cmd = interpretDirectionStick(direction, angle,strength);
                printAR(cmd);
                requireNonNull(client);
                Log.i("manualFlighty", "direction");
                //Log.i("manualFlighty", cmd);
                client = new TCPClient(TARGET);
                client.updateValues(cmd[0][1], (byte)cmd[0][0], cmd[1][1], (byte)cmd[1][0]);
                client.execute();
                //client.setValue(cmd);
            }
        });
    }

    private void setValues() {
        position.setText(position.getText() + "\nLat: 40°5324324234\nLong: 34°5243542352354");
        height.setText("Height: 120m");
        temp.setText("Temp: 14,8°C");
        status.setText(status.getText() + " OK");
    }

    private void findViews() {
        position = (TextView) view.findViewById(R.id.txt_MF_Position);
        height = (TextView) view.findViewById(R.id.txt_MF_Height);
        temp = (TextView) view.findViewById(R.id.txt_MF_Temp);
        status = (TextView) view.findViewById(R.id.txt_MF_Connection);
        ip = view.findViewById(R.id.editTxt_IP);
        homeFAB = view.findViewById(R.id.homeFab);

        homeFAB.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                displayHomeConfirmationDialog();
            }
        });
    }

    private void addListenerToEditText(){
        ip.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
                if(!hasFocus){
                    //client.changeIP(ip.getText().toString());
                }
            }
        });
    }

    private int[][] interpretThrottleStick(JoystickView stick, int angle, int strength){
        int[][] values = new int[2][2];
        String returnValue = "";

        double rad = angle*Math.PI/180;

        //Calculation for the x-axis
        double hypothenusis  = strength;
        int adjacentX = (int) (Math.cos(rad)*hypothenusis);
        if(adjacentX < 0){
            //returnValue += OpenDroneUtils.CODE_YAW_LEFT + "," + (adjacentX*(-1)) + ";";
            values[0][0] =  OpenDroneUtils.CODE_YAW_LEFT;
            values[0][1] = (adjacentX*(-1));
        }else{
            //returnValue += OpenDroneUtils.CODE_YAW_RIGHT + "," + adjacentX + ";";
            values[0][0] =  OpenDroneUtils.CODE_YAW_RIGHT;
            values[0][1] = adjacentX;
        }

        //Calculation for the y-axis
        int opposite = (int) (Math.sin(rad)*hypothenusis);
        if(opposite < 0){
            //returnValue += OpenDroneUtils.CODE_THROTTLE_DOWN + "," + (opposite*(-1)) + ";";
            values[1][0] =  OpenDroneUtils.CODE_THROTTLE_DOWN;
            values[1][1] =  (opposite*(-1));
        }else{
            //returnValue += OpenDroneUtils.CODE_THROTTLE_UP + "," + opposite + ";";
            values[1][0] =  OpenDroneUtils.CODE_THROTTLE_UP;
            values[1][1] =  opposite;
        }

        Log.i("manualFlightys", values[1][1]+"");

        return values;
    }

    private int[][] interpretDirectionStick(JoystickView stick, int angle, int strength){
        int[][] values = new int[2][2];

        String returnValue = "";

        double rad = angle*Math.PI/180;

        //Calculation for the x-axis
        double hypothenusis  = strength;
        int adjacentX = (int) (Math.cos(rad)*hypothenusis);
        if(adjacentX < 0){
            values[0][0] = OpenDroneUtils.CODE_ROLL_LEFT;
            values[0][1] = (adjacentX*(-1));
            //returnValue += OpenDroneUtils.CODE_ROLL_LEFT + "," + (adjacentX*(-1)) + ";";
        }else{
            values[0][0] = OpenDroneUtils.CODE_ROLL_RIGHT;
            values[0][1] = adjacentX;
            //returnValue += OpenDroneUtils.CODE_ROLL_RIGHT + "," + adjacentX + ";";
        }

        //Calculation for the y-axis
        int opposite = (int) (Math.sin(rad)*hypothenusis);
        if(opposite < 0){
            values[1][0] = OpenDroneUtils.CODE_PITCH_BACKWARD;
            values[1][1] = (opposite*(-1));
            //returnValue += OpenDroneUtils.CODE_PITCH_BACKWARD + "," + (opposite*(-1)) + ";";
        }else{
            values[1][0] = OpenDroneUtils.CODE_PITCH_FORWARD;
            values[1][1] = opposite;
            //returnValue += OpenDroneUtils.CODE_PITCH_FORWARD + "," + opposite + ";";
        }
        return values;
    }

}
