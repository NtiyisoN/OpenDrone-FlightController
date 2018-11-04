/*
 * Last modified: 09.09.18 18:56
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

package at.opendrone.opendrone;


import android.app.Activity;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Spinner;
import android.widget.EditText;


/**
 * A simple {@link Fragment} subclass.
 */
public class DroneSettings extends Fragment {

    private TextView txtView_Calibration;
    private TextView txtView_PinConfiguration;
    private EditText txtView_Drone_Description;
    private EditText txt_DroneName;
    private Spinner spinner_DroneTyp;
    private View view;

    private Drone selectedDrone;
    private String mode;

    public DroneSettings() {

    }

    public void setDrone(Drone drone) {
        this.selectedDrone = drone;
    }

    public void setMode(String mode) {
        this.mode = mode;
    }

    public void setValuesForDrone(Drone drone) {
        txtView_Drone_Description.setText(drone.description);
        txt_DroneName.setText(drone.name);

        String[] values = getResources().getStringArray(R.array.array_DroneTyp);

        for (int i = 0; i < values.length; i++) {
            if (values[i].equals(drone.type)) {
                spinner_DroneTyp.setSelection(i);
                break;
            }
        }


    }

    public void intiButtons() {
        txtView_Calibration = view.findViewById(R.id.txtView_Calibration);
        txtView_PinConfiguration = view.findViewById(R.id.txtView_PinConfiguration);
        txt_DroneName = view.findViewById(R.id.txt_DroneName);
        spinner_DroneTyp = view.findViewById(R.id.spinner_DroneTyp);
        txtView_Drone_Description = view.findViewById(R.id.txt_Drone_description);
        txtView_Calibration.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                initCalibration();
            }
        });

        txtView_PinConfiguration.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //TODO: PinConfiguration
            }
        });

        if (!(view instanceof EditText)) {
            view.setOnTouchListener(new View.OnTouchListener() {
                public boolean onTouch(View v, MotionEvent event) {
                    hideSoftKeyboard();
                    return false;
                }
            });
        }

        switch (this.mode) {
            case ("edit"): {
                Button btn_SaveSettings = view.findViewById(R.id.btn_saveChanges);
                btn_SaveSettings.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        String droneName = String.valueOf(txt_DroneName.getText());
                        String droneDescription = String.valueOf(txtView_Drone_Description.getText());
                        String droneType = String.valueOf(spinner_DroneTyp.getSelectedItem().toString());

                        Drone drone = new Drone(droneName, droneDescription, droneType);

                        for (int i = 0; i < DroneCardListRecyclerFragment.drones.size(); i++) {
                            if ((DroneCardListRecyclerFragment.drones.get(i).name == selectedDrone.name) && (DroneCardListRecyclerFragment.drones.get(i).description == selectedDrone.description)) {
                                DroneCardListRecyclerFragment.drones.set(i, drone);
                            }
                        }

                        DroneCardList defFragment = new DroneCardList();
                        FragmentManager fm = getFragmentManager();
                        FragmentTransaction ft = fm.beginTransaction();
                        ft.replace(R.id.frameLayout_FragmentContainer, defFragment.createFragment());
                        ft.commit();
                    }
                });
                break;
            }
            case ("new"): {
                Button btn_SaveSettings = view.findViewById(R.id.btn_saveChanges);
                btn_SaveSettings.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        String droneName = String.valueOf(txt_DroneName.getText());
                        String droneDescription = String.valueOf(txtView_Drone_Description.getText());
                        String droneType = String.valueOf(spinner_DroneTyp.getSelectedItem().toString());

                        Drone drone = new Drone(droneName, droneDescription, droneType);
                        DroneCardListRecyclerFragment.drones.add(drone);

                        DroneCardList defFragment = new DroneCardList();
                        FragmentManager fm = getFragmentManager();
                        FragmentTransaction ft = fm.beginTransaction();
                        ft.replace(R.id.frameLayout_FragmentContainer, defFragment.createFragment());
                        ft.commit();
                    }
                });
                break;
            }
        }

    }

    public void initCalibration() {
        DroneCalibrationActivity defFragment = new DroneCalibrationActivity();

        //FragmentTransaction ft  = MainActivity.fm.beginTransaction();
        //ft.replace(R.id.frameLayout_FragmentContainer, defFragment);
        //ft.add(R.id.frameLayout_FragmentContainer, defFragment);
        //ft.commit();
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {

        view = inflater.inflate(R.layout.fragment_drone_settings, container, false);
        intiButtons();

        if (selectedDrone != null) {
            setValuesForDrone(this.selectedDrone);
        }

        return view;
    }

    private void hideSoftKeyboard() {
        InputMethodManager inputMethodManager = (InputMethodManager) getActivity().getSystemService(Activity.INPUT_METHOD_SERVICE);
        inputMethodManager.hideSoftInputFromWindow(getActivity().getCurrentFocus().getWindowToken(), 0);
    }

}
