/*
 * Last modified: 05.09.18 19:29
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

package at.opendrone.opendrone;


import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.Toast;


/**
 * A simple {@link Fragment} subclass.
 */
public class FlyAutoFly extends Fragment {

    private View view;
    private Button btn_Land;
    private Button btn_GoHome;
    private Button btn_Control;


    public FlyAutoFly() {
        // Required empty public constructor
    }

    private void initButtons() {
        btn_Land = view.findViewById(R.id.btn_Land);
        btn_GoHome = view.findViewById(R.id.btn_GoHome);
        btn_Control = view.findViewById(R.id.btn_Control);

        btn_Land.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //TODO: Send the Land-command to the drone
            }
        });
        btn_GoHome.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //TODO: Send the GoHome-command to the drone
            }
        });
        btn_Control.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //TODO: Open Control-Fragment
            }
        });
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        // Inflate the layout for this fragment
        view = inflater.inflate(R.layout.fragment_fly_auto_fly, container, false);
        initButtons();

        return view;
    }

}
