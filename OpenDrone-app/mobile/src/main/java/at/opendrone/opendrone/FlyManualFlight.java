package at.opendrone.opendrone;


import android.annotation.TargetApi;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.RequiresApi;
import android.support.v4.app.Fragment;
import android.text.format.Formatter;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.SeekBar;
import android.widget.TextView;

import at.opendrone.opendrone.network.UDPClient;

import static android.content.Context.WIFI_SERVICE;


/**
 * A simple {@link Fragment} subclass.
 */
public class FlyManualFlight extends Fragment {

    private TextView percentageTxt;
    private SeekBar percentageSeekBar;

    private UDPClient client;

    private int lastValue;

    private static final String FB_SPEED_QUEUE = "speedQueue";
    private static final String TARGET_IP="172.16.54.9";

    private static final int OFFSET = 200;

    private View view;

    public FlyManualFlight() {
        // Required empty public constructor
    }

    @Override
    public void onResume() {
        startClient();
        super.onResume();
    }

    @Override
    public void onPause() {
        stopClient();
        super.onPause();
    }

    private void findViews(){
        percentageSeekBar = view.findViewById(R.id.sb_ChangeSpeed);
        percentageTxt = view.findViewById(R.id.txtView_Percentage);
        addListenerToSeekbar();
    }

    private void setPercentage(int value){
        percentageTxt.setText(Math.round((value/(double)percentageSeekBar.getMax())*100)+"%");
        //percentageTxt.setText(value+"%");
    }

    private void sendValue(final int value){
        startClient();
        client.setValue(value+OFFSET);

    }

    private void getIp(){
        WifiManager wm = (WifiManager) getActivity().getApplicationContext().getSystemService(WIFI_SERVICE);
        String ip = Formatter.formatIpAddress(wm.getConnectionInfo().getIpAddress());
        Log.i("udpy", "IP: "+ip);
    }

    private void addListenerToSeekbar(){
        percentageSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                sendValue(i==0?-20:i);
                setPercentage(i);
                lastValue = i;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                Log.i("seeky", "end");
            }
        });
    }

    private void initClient(){
        client = new UDPClient(TARGET_IP);
    }

    private void startClient(){
        if(client != null && !client.isAlive()){
            client.start();
        }
    }

    private void stopClient(){
        if (client.isAlive()){
            client.interrupt();
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view =  inflater.inflate(R.layout.fragment_fly_manual_flight, container, false);

        initClient();
        findViews();
        getIp();

        return view;
    }

}
