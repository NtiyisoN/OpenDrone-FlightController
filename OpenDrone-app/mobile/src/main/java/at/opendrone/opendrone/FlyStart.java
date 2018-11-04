/*
 * Last modified: 06.09.18 15:58
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

package at.opendrone.opendrone;


import android.os.Bundle;
import android.support.constraint.ConstraintLayout;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;


/**
 * A simple {@link Fragment} subclass.
 */
public class FlyStart extends Fragment {

    private View view;
    private FragmentManager fm;

    private Button btn_FlyAuto;
    private Button btn_TestConnection;
    private Spinner spinner_FlightMode;
    private Spinner spinner_Mission;
    private TextView txtView_FlightMode;
    private TextView txtView_Mission;
    private ConstraintLayout cl_FlightPlan;

    public FlyStart() {
        // Required empty public constructor
    }

    private void initElements(){
        btn_FlyAuto = view.findViewById(R.id.btn_FlyAuto);
        btn_TestConnection = view.findViewById(R.id.btn_FlyTestConnection);
        spinner_FlightMode = view.findViewById(R.id.spinner_FlightMode);
        spinner_Mission = view.findViewById(R.id.spinner_Mission);
        txtView_FlightMode = view.findViewById(R.id.txtView_FlightMode);
        txtView_Mission = view.findViewById(R.id.txtView_Mission);
        cl_FlightPlan = view.findViewById(R.id.cl_FlightMode);

        btn_FlyAuto.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String flightMode = (String) spinner_FlightMode.getSelectedItem();
                String mission;

                switch (flightMode){
                    case "Manual":
                        //TODO: Open the manual Fragment
                        break;
                    case "Automatic":
                        mission = (String) spinner_Mission.getSelectedItem();
                        initFlyAutoFlyFragment();
                        break;
                }
            }
        });
        btn_TestConnection.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                //TODO: implement TestConnection
                Toast.makeText(getActivity(), "Connection established", Toast.LENGTH_SHORT).show(); //We always have a connection!
            }
        });
        spinner_FlightMode.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                if (position == 0){
                    cl_FlightPlan.setVisibility(View.GONE);
                }else if (position == 1){
                    cl_FlightPlan.setVisibility(View.VISIBLE);
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
    }

    private void initFlyAutoFlyFragment() {
        FlyAutoFly defFragment = new FlyAutoFly();

        fm = getActivity().getSupportFragmentManager();
        FragmentTransaction ft = fm.beginTransaction();
        ft.add(R.id.frameLayout_FragmentContainer, defFragment);
        ft.remove(this);
        ft.commit();
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view = inflater.inflate(R.layout.fragment_fly_start, container, false);
        initElements();

        return view;
    }

}
