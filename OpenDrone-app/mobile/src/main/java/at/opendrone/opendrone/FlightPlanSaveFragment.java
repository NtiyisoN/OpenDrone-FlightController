/*
 * Last modified: 30.10.18 13:47
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

package at.opendrone.opendrone;


import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.gson.Gson;

import org.osmdroid.util.GeoPoint;

import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;


/**
 * A simple {@link Fragment} subclass.
 */
public class FlightPlanSaveFragment extends Fragment {
    private static final String TAG = "FlightPlanSavy";

    private RecyclerView flightPlanContainer;
    private EditText nameTxt;
    private EditText descTxt;
    private FloatingActionButton add;
    private FloatingActionButton save;
    private FloatingActionButton returnToMap;

    private String name = "";
    private String desc = "";
    private int position = -1;
    private LinkedHashMap<Double, GeoPoint> points;
    private List<Flightplan> flightplans = new LinkedList<Flightplan>();
    private FlightPlaner planer;

    private View view;

    private SharedPreferences sp;
    private Gson gson = new Gson();

    public FlightPlanSaveFragment() {

    }

    public FlightPlanSaveFragment(LinkedHashMap<Double, GeoPoint> points) {
        this.points = points;
    }

    public FlightPlanSaveFragment(String name, String desc, LinkedHashMap<Double, GeoPoint> points, int position) {
        this.name = name;
        this.desc = desc;
        this.points = points;
        this.position = position;
    }

    private void findViews() {
        nameTxt = view.findViewById(R.id.flightPlan_Name);
        descTxt = view.findViewById(R.id.flightPlan_Description);
        save = view.findViewById(R.id.btn_SaveFlightPlan);
        flightPlanContainer = view.findViewById(R.id.flightPlan_Coordinates);
        returnToMap = view.findViewById(R.id.btn_returnToMap);
        add = view.findViewById(R.id.fpAFAB);

        nameTxt.setText(name);
        descTxt.setText(desc);

        flightPlanContainer.setHasFixedSize(true);
        flightPlanContainer.setLayoutManager(new LinearLayoutManager(getActivity()));
    }

    private void setAdapter() {
        GeoPointRecyclerViewAdapter adapter = new GeoPointRecyclerViewAdapter(new LinkedList<GeoPoint>(points.values()), getActivity(), this);
        flightPlanContainer.setAdapter(adapter);
    }

    private void checkShowAddFAB(){
        if (points.size() <= 0) {
            showAddFAB();
        } else {
            showFAB();
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view = inflater.inflate(R.layout.fragment_flight_plan_save, container, false);

        sp = getActivity().getSharedPreferences("at.opendrone.opendrone", Context.MODE_PRIVATE);

        findViews();
        setAdapter();
        setListeners();
        setAttributes();
        checkShowAddFAB();
        return view;
    }

    @SuppressLint("RestrictedApi")
    private void showFAB() {
        save.setVisibility(View.VISIBLE);
        add.setVisibility(View.INVISIBLE);
    }

    @SuppressLint("RestrictedApi")
    private void showAddFAB() {
        save.setVisibility(View.INVISIBLE);
        add.setVisibility(View.VISIBLE);
    }

    private void setListeners() {
        save.setOnClickListener(v -> saveFlightPlan());
        add.setOnClickListener(v -> displayAddDialog());
        save.setOnLongClickListener(view -> {
            displayAddDialog();
            return true;
        });
        returnToMap.setOnClickListener(v -> returnToMap());
    }

    private void returnToMap() {
        sp.edit().remove(OpenDroneUtils.SP_FLIGHTPLAN_HOLDER).apply();
        Flightplan flight = new Flightplan();
        flight.setName(nameTxt.getText().toString());
        flight.setDescription(descTxt.getText().toString());
        flight.setCoordinates(points);
        flight.setId(position);
        String object = gson.toJson(flight);
        sp.edit().putString(OpenDroneUtils.SP_FLIGHTPLAN_HOLDER, object).apply();

        if (planer != null) {
            FragmentTransaction ft = getActivity().getSupportFragmentManager().beginTransaction();
            ft.replace(R.id.frameLayout_FragmentContainer, planer);
            ft.commit();
        } else {
            FlightPlaner fp = new FlightPlaner();
            FragmentTransaction ft = getActivity().getSupportFragmentManager().beginTransaction();
            ft.replace(R.id.frameLayout_FragmentContainer, fp);
            ft.commit();
            fp.setPoints(points);
        }
    }

    private void saveFlightPlan() {
        Log.i(TAG, nameTxt.getText().toString());
        if(nameTxt.getText().toString().equals("")){
            nameTxt.setError(getString(R.string.flight_plan_save_error_name_empty));
            return;
        }

        Flightplan flightplan = null;
        try {
            flightplans = new LinkedList<>(Arrays.asList(gson.fromJson(sp.getString(OpenDroneUtils.SP_FLIGHTPLANS, ""), Flightplan[].class)));
            String object = sp.getString(OpenDroneUtils.SP_FLIGHTPLAN_HOLDER, "");
            flightplan = gson.fromJson(object, Flightplan.class);
        } catch (Exception e) {
            Toast.makeText(getContext(), getString(R.string.flight_plan_save_error_read_fp), Toast.LENGTH_LONG).show();
            flightplans = new LinkedList<>();
        }
        Flightplan fp = new Flightplan();
        fp.setName(nameTxt.getText().toString());
        fp.setDescription(descTxt.getText().toString());
        fp.setCoordinates(points);

        if (flightplan != null) {
            position = flightplan.getId();
        }

        if (position == -1) {
            flightplans.add(fp);
        } else {
            flightplans.set(position, fp);
        }


        String serialized = gson.toJson(flightplans.toArray());
        sp.edit().putString(OpenDroneUtils.SP_FLIGHTPLANS, serialized).apply();

        FlightPlanListFragment fpsf = new FlightPlanListFragment();
        FragmentTransaction ft = getActivity().getSupportFragmentManager().beginTransaction();
        ft.replace(R.id.frameLayout_FragmentContainer, fpsf);
        ft.commit();
    }

    public void setAttributes() {
        SharedPreferences sp = getActivity().getSharedPreferences("at.opendrone.opendrone", Context.MODE_PRIVATE);
        Gson gson = new Gson();

        Flightplan fp = null;
        try {
            if (sp.contains(OpenDroneUtils.SP_FLIGHTPLAN_HOLDER)) {
                String object = sp.getString(OpenDroneUtils.SP_FLIGHTPLAN_HOLDER, "");
                fp = gson.fromJson(object, Flightplan.class);
            }
        } catch (Exception e) {
            //Go back
            getActivity().onBackPressed();
            Toast.makeText(getContext(), "Could not read flightplan", Toast.LENGTH_LONG).show();
        }

        if (fp != null) {
            this.nameTxt.setText(fp.getName());
            this.descTxt.setText(fp.getDescription());
        }
    }

    public void setFlightPlaner(FlightPlaner fp) {
        this.planer = fp;
    }

    private void displayAddDialog() {
        LayoutInflater layoutInflater = LayoutInflater.from(getActivity());
        View promptView = layoutInflater.inflate(R.layout.fragment_edit_geopoint, null);
        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(getActivity());
        alertDialogBuilder.setView(promptView);
        TextView view = promptView.findViewById(R.id.txt_HeaderEdit);
        view.setText(getString(R.string.flight_plan_save_add_coord));
        // setup a dialog window
        alertDialogBuilder.setPositiveButton(getString(R.string.flight_plan_save_add_coord_pos), (dialog, which) -> {
            EditText txt_latitude = promptView.findViewById(R.id.txt_lat);
            EditText txt_longitude = promptView.findViewById(R.id.txt_long);
            String latitude = txt_latitude.getText().toString();
            String longitude = txt_longitude.getText().toString();
            try {
                double lat = Double.parseDouble(latitude);
                double lon = Double.parseDouble(longitude);
                GeoPoint p = new GeoPoint(lat, lon);
                points.put(points.size() + 0d, p);
                showFAB();
            } catch (NumberFormatException e) {
                Toast.makeText(getActivity(), getString(R.string.flight_plan_save_invalid_coord), Toast.LENGTH_LONG).show();
            }

        });
        alertDialogBuilder.setNegativeButton(getString(R.string.flight_plan_save_add_coord_neg), (dialog, which) -> dialog.cancel());
        alertDialogBuilder.show();
    }

}
