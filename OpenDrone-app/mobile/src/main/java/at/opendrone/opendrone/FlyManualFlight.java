package at.opendrone.opendrone;


import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.SeekBar;
import android.widget.TextView;

import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;


/**
 * A simple {@link Fragment} subclass.
 */
public class FlyManualFlight extends Fragment {

    private TextView percentageTxt;
    private SeekBar percentageSeekBar;

    private FirebaseDatabase db;
    private DatabaseReference valueRef;

    private int lastValue;

    private static final String FB_SPEED_QUEUE = "speedQueue";

    private View view;

    public FlyManualFlight() {
        // Required empty public constructor
    }

    private void findViews(){
        percentageSeekBar = view.findViewById(R.id.sb_ChangeSpeed);
        percentageTxt = view.findViewById(R.id.txtView_Percentage);

        addListenerToSeekbar();
    }

    private void initFB(){
        db = FirebaseDatabase.getInstance();
        valueRef = db.getReference(FB_SPEED_QUEUE);
    }

    private void uploadValue(int value){
        valueRef.child(System.currentTimeMillis()+"").setValue(value);
    }

    private void setPercentage(int value){
        percentageTxt.setText(Math.round(value/(double)percentageSeekBar.getMax()*100)+"%");
    }

    private void addListenerToSeekbar(){
        percentageSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                uploadValue(i);
                setPercentage(i);
                lastValue = i;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                valueRef.setValue(null);
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                Log.i("seeky", "end");
            }
        });
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view =  inflater.inflate(R.layout.fragment_fly_manual_flight, container, false);

        initFB();
        findViews();

        return view;
    }

}
