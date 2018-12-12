/*
 * Last modified: 30.10.18 13:47
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

package at.opendrone.opendrone;


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

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view = inflater.inflate(R.layout.fragment_flight_plan_save, container, false);
        findViews();
        setAdapter();
        setListeners();
        setAttributes();

        if (points.size() <= 0) {
            showAddFAB();
        } else {
            showFAB();
        }

        return view;
    }

    private void showFAB() {
        save.setVisibility(View.VISIBLE);
        add.setVisibility(View.INVISIBLE);
    }

    private void showAddFAB() {
        save.setVisibility(View.INVISIBLE);
        add.setVisibility(View.VISIBLE);
    }

    private void setListeners() {
        save.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SharedPreferences sp = getActivity().getSharedPreferences("at.opendrone.opendrone", Context.MODE_PRIVATE);
                Gson gson = new Gson();
                Flightplan flightplan = null;
                try {
                    flightplans = new LinkedList<>(Arrays.asList(gson.fromJson(sp.getString(OpenDroneUtils.SP_FLIGHTPLANS, ""), Flightplan[].class)));
                    String object = sp.getString(OpenDroneUtils.SP_FLIGHTPLAN_HOLDER, "");
                    flightplan = gson.fromJson(object, Flightplan.class);
                } catch (Exception e) {
                    Toast.makeText(getContext(), "Could not read flightplans", Toast.LENGTH_LONG).show();
                    flightplans = new LinkedList<Flightplan>();
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
        });

        add.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                displayAddDialog();
            }
        });

        save.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View view) {
                displayAddDialog();
                return true;
            }
        });

        returnToMap.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                SharedPreferences sp = getActivity().getSharedPreferences("at.opendrone.opendrone", Context.MODE_PRIVATE);
                sp.edit().remove(OpenDroneUtils.SP_FLIGHTPLAN_HOLDER).apply();
                Gson gson = new Gson();
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
                    //planer.setPoints(points);
                    //planer.drawPoints();
                } else {
                    FlightPlaner fp = new FlightPlaner();
                    FragmentTransaction ft = getActivity().getSupportFragmentManager().beginTransaction();
                    ft.replace(R.id.frameLayout_FragmentContainer, fp);
                    ft.commit();
                    fp.setPoints(points);
                }
            }
        });
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
            //TODO: Error handling
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

        final EditText txt_latitude = (EditText) promptView.findViewById(R.id.txt_lat);
        final EditText txt_longitude = (EditText) promptView.findViewById(R.id.txt_long);
        TextView view = (TextView) promptView.findViewById(R.id.txt_HeaderEdit);
        view.setText("Add Coordinate");
        // setup a dialog window

        alertDialogBuilder.setPositiveButton("Save", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                String latitude = txt_latitude.getText().toString();
                String longitude = txt_longitude.getText().toString();
                try {
                    double lat = Double.parseDouble(latitude);
                    double lon = Double.parseDouble(longitude);
                    GeoPoint p = new GeoPoint(lat, lon);
                    points.put(points.size() + 0d, p);
                    showFAB();
                } catch (NumberFormatException e) {
                    Toast.makeText(getActivity(), "Please enter a correct format in the fields!", Toast.LENGTH_LONG).show();
                }

            }
        });
        alertDialogBuilder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.cancel();
            }
        });

        alertDialogBuilder.show();
    }

}
