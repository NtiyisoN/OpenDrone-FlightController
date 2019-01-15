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
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.TextView;

import org.osmdroid.tileprovider.tilesource.TileSourceFactory;
import org.osmdroid.views.MapView;
import org.osmdroid.views.overlay.Polyline;

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

    private TextView positionTxtView;
    private TextView heightTxtView;
    private TextView airTempTxtView;
    private TextView controllerTempTxtView;
    private TextView statusTxtView;
    private TextView velocityTxtView;
    private ImageButton homeBtn;
    private ImageButton stopRotorBtn;
    private ImageButton changeViewBtn;
    private MapView mapView;
    private FrameLayout cameraView;

    private boolean mapViewShown = false;

    public static final String TARGET = "192.168.1.254";
    public static final int PORT = 2018;

    private static final String TAG = "manualFlighty";
    private static final String TAG_ERROR = "errortcpreceive";

    private String positionTxt = "";
    private String heightTxt = "";
    private String airTempTxt = "";
    private String controllerTempTxt = "";
    private String statusTxt = "";
    private String velocityTxt = "";

    private TCPHandler mTCPHandler;

    public FlyManualFlight() {
        // Required empty public constructor
    }

    @Override
    public void onResume() {
        getActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        ((MainActivity) getActivity()).closeDrawer();
        super.onResume();

        if(mapView != null){
            mapView.onResume();
        }

    }

    @Override
    public void onPause() {
        super.onPause();
        getActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
        if (mTCPHandler != null) {
            // disconnect
            new DisconnectTask().execute();
        }

        if(mapView != null) {
            mapView.onPause();
        }
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view = inflater.inflate(R.layout.fragment_fly_manual_flight, container, false);
        initStrings();
        setRetainInstance(true);
        findViews();
        setValues();
        initJoysticks();

        return view;
    }

    private void initStrings(){
        positionTxt = getString(R.string.manual_flight_TxtView_Position);
        heightTxt = getString(R.string.manual_flight_TxtView_Height);
        airTempTxt = getString(R.string.manual_flight_TxtView_AirTemp);
        controllerTempTxt = getString(R.string.manual_flight_TxtView_ControllerTemp);
        statusTxt = getString(R.string.manual_flight_TxtView_Status);
        velocityTxt = getString(R.string.manual_flight_TxtView_Velocity);
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

    public void extractData(String raw){
        String[] dataAr = raw.split(";");
        String val = dataAr[0];
        int code = 0;

        try{
            code = Integer.parseInt(dataAr[0]);
        }catch (Exception ex){
            Log.e(TAG_ERROR, "ERROR", ex);
            return;
        }

        TextView txtView = null;
        String format = "";
        switch(code){
            case OpenDroneUtils.CODE_CONTROLLER_TEMP:
                txtView = controllerTempTxtView;
                format = controllerTempTxt;
                break;
            case OpenDroneUtils.CODE_AIR_TEMP:
                txtView = airTempTxtView;
                format = airTempTxt;
                break;
            case OpenDroneUtils.CODE_POSITTION:
                txtView = positionTxtView;
                format = positionTxt;
                break;
            case OpenDroneUtils.CODE_HEIGHT:
                txtView = heightTxtView;
                format = heightTxt;
                break;
            case OpenDroneUtils.CODE_STATUS:
                txtView = statusTxtView;
                format = statusTxt;
                break;
            case OpenDroneUtils.CODE_VELOCITY:
                txtView = velocityTxtView;
                format = velocityTxt;
                break;
            default:
                return;
        }

        updateTextViews(txtView, format, val);
    }

    public void updateTextViews(TextView txtView, String format, String value) {
        txtView.setText(String.format(format, value));
    }

    private void setValues() {
        positionTxtView.setText(String.format(positionTxt, "40°5324324234", "34°5243542352354"));
        //positionTxtView.setText(positionTxtView.getText() + "\nLat: 40°5324324234\nLong: 34°5243542352354");
        heightTxtView.setText(String.format(heightTxt, "120"));
        airTempTxtView.setText(String.format(airTempTxt, "14.8"));
        controllerTempTxtView.setText(String.format(controllerTempTxt, "120"));
        statusTxtView.setText(String.format(statusTxt, "OK"));
        velocityTxtView.setText(String.format(velocityTxt, "302"));
    }

    private void findViews() {
        positionTxtView = view.findViewById(R.id.txt_MF_Position);
        heightTxtView = view.findViewById(R.id.txt_MF_Height);
        airTempTxtView = view.findViewById(R.id.txt_MF_AirTemp);
        controllerTempTxtView = view.findViewById(R.id.txt_MF_ControllerTemp);
        statusTxtView = view.findViewById(R.id.txt_MF_Connection);
        velocityTxtView = view.findViewById(R.id.txt_MF_Velocity);
        homeBtn = view.findViewById(R.id.homeBtn);
        changeViewBtn = view.findViewById(R.id.changeViewBtn);
        stopRotorBtn = view.findViewById(R.id.stopRotorBtn);
        mapView = view.findViewById(R.id.mapView);
        cameraView = view.findViewById(R.id.cameraView);


        initMapView();
        homeBtn.setOnClickListener(v -> displayHomeConfirmationDialog());
        stopRotorBtn.setOnClickListener(v -> displayStopRotorDialog());
        changeViewBtn.setOnClickListener(v-> changeView());
    }

    private void initMapView(){
        mapView.setTileSource(TileSourceFactory.MAPNIK);

        mapView.setBuiltInZoomControls(false);
        mapView.setMultiTouchControls(true);
    }

    private void changeView(){
        if(mapViewShown){
            hideMapView();
        }else{
            showMapView();
        }
    }

    private void setImageBtnImage(ImageButton btn, int drawableID){
        btn.setImageDrawable(getResources().getDrawable(drawableID));
    }

    private void showMapView(){
        mapView.setVisibility(View.VISIBLE);
        setImageBtnImage(changeViewBtn, R.drawable.ic_view_live_feed);
        mapViewShown = true;
    }

    private void hideMapView(){
        mapView.setVisibility(View.GONE);
        setImageBtnImage(changeViewBtn, R.drawable.ic_view_map);
        mapViewShown = false;
    }

    private void displayStopRotorDialog(){
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        builder.setMessage(getString(R.string.stop_rotor_alarm_message))
                .setPositiveButton(getString(R.string.stop_rotor_alarm_pos), (dialog, id) -> new ConnectTask().execute(""))
                .setNegativeButton(getString(R.string.stop_rotor_alarm_neg), (dialog, id) -> {
                    if (mTCPHandler == null) {
                        return;
                    }

                    new DisconnectTask().execute();
                });
        builder.create().show();
    }

    private void displayHomeConfirmationDialog() {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        builder.setMessage(getString(R.string.go_home_alarm_message))
                .setPositiveButton(getString(R.string.go_home_alarm_pos), (dialog, id) -> new ConnectTask().execute(""))
                .setNegativeButton(getString(R.string.go_home_alarm_neg), (dialog, id) -> {
                    if (mTCPHandler == null) {
                        return;
                    }

                    new DisconnectTask().execute();
                });
        builder.create().show();
        // Create the AlertDialog object and return it
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
            extractData(values[0]);
        }
    }

}
