/*
 * Last modified: 09.09.18 18:56
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

package at.opendrone.opendrone;


import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.wang.avi.AVLoadingIndicatorView;


/**
 * A simple {@link Fragment} subclass.
 */
public class DroneCalibrationActivity extends AppCompatActivity {

    private Button btn_calibration;
    private TextView txtView_calibration;
    private AVLoadingIndicatorView avi;

    private Drone drone;
    private String mode;
    private int position;


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                onBackPressed();
                return false;
            default:
                Log.i("DroneCalibration", "Default case");
                return super.onOptionsItemSelected(item);
        }

        //return super.onOptionsItemSelected(item);
    }

    private void initActionbar() {
        Toolbar toolbar = findViewById(R.id.toolbar_Calibrate);
        setSupportActionBar(toolbar);
        ActionBar ab = getSupportActionBar();
        ab.setDisplayHomeAsUpEnabled(true);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_drone_calibration);

        Intent i = getIntent();
        if (i != null) {
            mode = i.getStringExtra("Mode");
            drone = (Drone) i.getSerializableExtra("Drone");
            position = i.getIntExtra("Position", -1);
        }

        initActionbar();

        initButtons();
        avi.hide();
    }

    private void initButtons() {

        btn_calibration = findViewById(R.id.btn_calibration);
        txtView_calibration = findViewById(R.id.txtView_calibration);
        avi = findViewById(R.id.avi);

        btn_calibration.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startCalibration();
            }
        });


    }

    public void startCalibration() {
        avi.show();
        txtView_calibration.setText(R.string.txt_calibration_inprogress);

        //TODO: Calibration

        txtView_calibration.setText(R.string.txt_calibration_end);
        avi.hide();
        btn_calibration.setEnabled(false);
    }

}
