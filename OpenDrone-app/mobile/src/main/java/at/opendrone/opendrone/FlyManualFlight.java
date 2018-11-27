package at.opendrone.opendrone;


import android.content.pm.ActivityInfo;
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

import at.opendrone.opendrone.network.TCPClient;
import io.github.controlwear.virtual.joystick.android.JoystickView;

import static android.content.Context.WIFI_SERVICE;


/**
 * A simple {@link Fragment} subclass.
 */
public class FlyManualFlight extends Fragment {

    private View view;

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


        final JoystickView throttle = (JoystickView) view.findViewById(R.id.throttleStick);
        throttle.setOnMoveListener(new JoystickView.OnMoveListener() {
            @Override
            public void onMove(int angle, int strength) {
                throttleStick.setText("Angle: "+angle+" / Strength: "+strength);
            }
        });

        JoystickView direction = (JoystickView) view.findViewById(R.id.directionStick);
        direction.setOnMoveListener(new JoystickView.OnMoveListener() {
            @Override
            public void onMove(int angle, int strength) {
                directionStick.setText("Angle: "+angle+" / Strength: "+strength);
            }
        });

        return view;
    }

}
