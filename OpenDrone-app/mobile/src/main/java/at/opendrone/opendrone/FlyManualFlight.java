package at.opendrone.opendrone;


import android.content.pm.ActivityInfo;
import android.graphics.Path;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.text.format.Formatter;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.SeekBar;
import android.widget.TextView;

import org.w3c.dom.Text;

import java.util.Stack;

import at.opendrone.opendrone.network.TCPClient;
import io.github.controlwear.virtual.joystick.android.JoystickView;

import static android.content.Context.WIFI_SERVICE;


/**
 * A simple {@link Fragment} subclass.
 */
public class FlyManualFlight extends Fragment {

    private View view;
    private JoystickView throttle;
    private JoystickView direction;

    public FlyManualFlight() {
        // Required empty public constructor
    }

    @Override
    public void onResume() {
        getActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        super.onResume();
    }

    @Override
    public void onPause() {
        getActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
        super.onPause();
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view =  inflater.inflate(R.layout.fragment_fly_manual_flight, container, false);

        final TextView throttleStick = (TextView) view.findViewById(R.id.txtThrottle);
        final TextView directionStick = (TextView) view.findViewById(R.id.txtDirection);

        throttle = (JoystickView) view.findViewById(R.id.throttleStick);
        throttle.setOnMoveListener(new JoystickView.OnMoveListener() {
            @Override
            public void onMove(int angle, int strength) {
                throttleStick.setText("Angle: "+angle+" / Strength: "+strength);
                String cmd = interpretThrottleStick(throttle, angle, strength);
            }
        });

        direction = (JoystickView) view.findViewById(R.id.directionStick);
        direction.setOnMoveListener(new JoystickView.OnMoveListener() {
            @Override
            public void onMove(int angle, int strength) {
                directionStick.setText("Angle: "+angle+" / Strength: "+strength);
                String cmd = interpretDirectionStick(direction, angle,strength);
            }
        });

        return view;
    }

    private String interpretThrottleStick(JoystickView stick, int angle, int strength){
        String returnValue = "";

        double rad = angle*Math.PI/180;

        //Calculation for the x-axis
        double hypothenusis  = strength;
        int adjacentX = (int) (Math.cos(rad)*hypothenusis);
        if(adjacentX < 0){
            returnValue += OpenDroneUtils.YAW_LEFT + "," + (adjacentX*(-1)) + ";";
        }else{
            returnValue += OpenDroneUtils.YAW_RIGHT + "," + adjacentX + ";";
        }

        //Calculation for the y-axis
        int opposite = (int) (Math.sin(rad)*hypothenusis);
        if(opposite < 0){
            returnValue += OpenDroneUtils.THROTTLE_DOWN + "," + (opposite*(-1)) + ";";
        }else{
            returnValue += OpenDroneUtils.THROTTLE_UP + "," + opposite + ";";
        }
        return returnValue;
    }

    private String interpretDirectionStick(JoystickView stick, int angle, int strength){
        String returnValue = "";

        double rad = angle*Math.PI/180;

        //Calculation for the x-axis
        double hypothenusis  = strength;
        int adjacentX = (int) (Math.cos(rad)*hypothenusis);
        if(adjacentX < 0){
            returnValue += OpenDroneUtils.ROLL_LEFT + "," + (adjacentX*(-1)) + ";";
        }else{
            returnValue += OpenDroneUtils.ROLL_RIGHT + "," + adjacentX + ";";
        }

        //Calculation for the y-axis
        int opposite = (int) (Math.sin(rad)*hypothenusis);
        if(opposite < 0){
            returnValue += OpenDroneUtils.PITCH_BACKWARDS + "," + (opposite*(-1)) + ";";
        }else{
            returnValue += OpenDroneUtils.PITCH_FORWARDS + "," + opposite + ";";
        }
        return returnValue;
    }

}
