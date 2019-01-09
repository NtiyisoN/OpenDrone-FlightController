package at.opendrone.opendrone;


import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.pm.ActivityInfo;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.TextView;

import at.opendrone.opendrone.network.OpenDroneFrame;
import at.opendrone.opendrone.network.TCPHandler;
import io.github.controlwear.virtual.joystick.android.JoystickView;

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
    private ImageButton homeBtn;

    public static final String TARGET = "192.168.1.254";
    public static final int PORT = 2018;

    private static final String TAG = "manualFlighty";
    private static final String TAG_ERROR = "errortcpreceive";

    private TCPHandler mTCPHandler;

    public FlyManualFlight() {
        // Required empty public constructor
    }

    @Override
    public void onResume() {
        getActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        ((MainActivity) getActivity()).closeDrawer();

        super.onResume();
    }

    @Override
    public void onPause() {
        super.onPause();
        getActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
        if (mTCPHandler != null) {
            // disconnect
            new DisconnectTask().execute();
        }
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view = inflater.inflate(R.layout.fragment_fly_manual_flight, container, false);
        setRetainInstance(true);
        findViews();
        setValues();
        initJoysticks();


        return view;
    }

    private void displayHomeConfirmationDialog() {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        builder.setMessage("Do you rly want go home fam?")
                .setPositiveButton("Hell yeah", (dialog, id) -> new ConnectTask().execute(""))
                .setNegativeButton("What am i doing here?!", (dialog, id) -> {
                    if (mTCPHandler == null) {
                        return;
                    }

                    new DisconnectTask().execute();
                });
        builder.create().show();
        // Create the AlertDialog object and return it
    }

    private void printAR(int[][] ar) {
        for (int i = 0; i < ar.length; i++) {
            for (int j = 0; j < ar.length; j++) {
                Log.i("Testy", "Index: " + i + "/" + j + ": " + ar[i][j]);
            }
        }
    }

    private void initJoysticks() {
        throttle = view.findViewById(R.id.throttleStick);
        throttle.setOnMoveListener((angle, strength) -> {
            int[][] cmd = interpretThrottleStick(throttle, angle, strength);
            sendData(cmd[0][1], (byte)cmd[0][0], cmd[1][1], (byte)cmd[1][0]);
        });

        direction =view.findViewById(R.id.directionStick);
        direction.setOnMoveListener((angle, strength) -> {
            int[][] cmd = interpretDirectionStick(direction, angle, strength);
            printAR(cmd);
            sendData(cmd[0][1], (byte)cmd[0][0], cmd[1][1], (byte)cmd[1][0]);
        });
    }

    private void sendData(int data1, byte code1, int data2, byte code2){
        if (mTCPHandler != null) {
            OpenDroneFrame f = new OpenDroneFrame((byte)1, code1, data1, code2, data2);
            mTCPHandler.sendMessage(f.toString());
        }else{
            Log.i(TAG, "NULL");
        }
    }

    public void updateTextViews(int data, int type) {

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
        homeBtn = view.findViewById(R.id.homeFab);

        homeBtn.setOnClickListener(v -> displayHomeConfirmationDialog());
    }

    private int[][] interpretThrottleStick(JoystickView stick, int angle, int strength) {
        int[][] values = new int[2][2];
        double rad = angle * Math.PI / 180;

        //Calculation for the x-axis
        double hypothenusis = strength;
        int adjacentX = (int) (Math.cos(rad) * hypothenusis);
        if (adjacentX < 0) {
            //returnValue += OpenDroneUtils.CODE_YAW_LEFT + "," + (adjacentX*(-1)) + ";";
            values[0][0] = OpenDroneUtils.CODE_YAW_LEFT;
            values[0][1] = (adjacentX * (-1));
        } else {
            //returnValue += OpenDroneUtils.CODE_YAW_RIGHT + "," + adjacentX + ";";
            values[0][0] = OpenDroneUtils.CODE_YAW_RIGHT;
            values[0][1] = adjacentX;
        }

        //Calculation for the y-axis
        int opposite = (int) (Math.sin(rad) * hypothenusis);
        if (opposite < 0) {
            //returnValue += OpenDroneUtils.CODE_THROTTLE_DOWN + "," + (opposite*(-1)) + ";";
            values[1][0] = OpenDroneUtils.CODE_THROTTLE_DOWN;
            values[1][1] = (opposite * (-1));
        } else {
            //returnValue += OpenDroneUtils.CODE_THROTTLE_UP + "," + opposite + ";";
            values[1][0] = OpenDroneUtils.CODE_THROTTLE_UP;
            values[1][1] = opposite;
        }

        Log.i("manualFlightys", values[1][1] + "");

        return values;
    }

    private int[][] interpretDirectionStick(JoystickView stick, int angle, int strength) {
        int[][] values = new int[2][2];
        double rad = angle * Math.PI / 180;

        //Calculation for the x-axis
        double hypothenusis = strength;
        int adjacentX = (int) (Math.cos(rad) * hypothenusis);
        if (adjacentX < 0) {
            values[0][0] = OpenDroneUtils.CODE_ROLL_LEFT;
            values[0][1] = (adjacentX * (-1));
            //returnValue += OpenDroneUtils.CODE_ROLL_LEFT + "," + (adjacentX*(-1)) + ";";
        } else {
            values[0][0] = OpenDroneUtils.CODE_ROLL_RIGHT;
            values[0][1] = adjacentX;
            //returnValue += OpenDroneUtils.CODE_ROLL_RIGHT + "," + adjacentX + ";";
        }

        //Calculation for the y-axis
        int opposite = (int) (Math.sin(rad) * hypothenusis);
        if (opposite < 0) {
            values[1][0] = OpenDroneUtils.CODE_PITCH_BACKWARD;
            values[1][1] = (opposite * (-1));
            //returnValue += OpenDroneUtils.CODE_PITCH_BACKWARD + "," + (opposite*(-1)) + ";";
        } else {
            values[1][0] = OpenDroneUtils.CODE_PITCH_FORWARD;
            values[1][1] = opposite;
            //returnValue += OpenDroneUtils.CODE_PITCH_FORWARD + "," + opposite + ";";
        }
        return values;
    }

    /**
     * Disconnects using a background task to avoid doing long/network operations on the UI thread
     */
    @SuppressLint("StaticFieldLeak")
    public class DisconnectTask extends AsyncTask<Void, Void, Void> {

        @Override
        protected Void doInBackground(Void... voids) {

            // disconnect
            mTCPHandler.stopClient();
            mTCPHandler = null;

            return null;
        }

        @Override
        protected void onPostExecute(Void nothing) {
            super.onPostExecute(nothing);
        }
    }

    @SuppressLint("StaticFieldLeak")
    public class ConnectTask extends AsyncTask<String, String, TCPHandler> {

        @Override
        protected TCPHandler doInBackground(String... message) {

            //we create a TCPClient object and
            mTCPHandler = new TCPHandler(TARGET, PORT, new TCPHandler.OnMessageReceived() {
                @Override
                //here the messageReceived method is implemented
                public void messageReceived(String message) {
                    //this method calls the onProgressUpdate
                    publishProgress(message);
                }
            });
            mTCPHandler.run();

            return null;
        }

        @Override
        protected void onProgressUpdate(String... values) {
            super.onProgressUpdate(values);
            Log.i(TAG, "RECEIVE: "+values[0]);
        }
    }

}
