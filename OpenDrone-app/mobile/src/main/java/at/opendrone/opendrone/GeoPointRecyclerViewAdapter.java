/*
 * Last modified: 30.10.18 12:49
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

package at.opendrone.opendrone;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.support.annotation.NonNull;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import org.osmdroid.util.GeoPoint;

import java.util.LinkedList;
import java.util.List;

public class GeoPointRecyclerViewAdapter extends RecyclerView.Adapter<GeoPointRecyclerViewAdapter.ViewHolder>{

    private List<GeoPoint> points = new LinkedList<>();
    private Activity activity;
    private FlightPlanSaveFragment fragment;
    private SharedPreferences sp;

    public GeoPointRecyclerViewAdapter(List<GeoPoint> points, Activity activity, FlightPlanSaveFragment fragment){
        this.points = points;
        this.activity = activity;
        this.fragment = fragment;
        this.sp = sp;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View v = LayoutInflater.from(parent.getContext()).inflate(R.layout.fragment_geopoint, parent, false);

        return new ViewHolder(v);
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, final int position) {
            GeoPoint p = points.get(position);
            holder.coordsTxt.setText(p.getLatitude()+" / "+p.getLongitude());
            holder.coords = "" + p.getLatitude()+" / "+p.getLongitude();
            holder.position = position;
    }

    @Override
    public int getItemCount() {
        return this.points.size();
    }

    public class ViewHolder extends RecyclerView.ViewHolder{

        private String coords;
        private TextView coordsTxt;
        private ImageView btn_EditCoords;
        private ImageView btn_DeleteCoords;
        private int position;

        public ViewHolder(View itemView) {
            super(itemView);

            coordsTxt = itemView.findViewById(R.id.txt_Coords);
            btn_DeleteCoords = itemView.findViewById((R.id.btn_DeleteGeoPoint));
            btn_EditCoords = itemView.findViewById(R.id.btn_EditGeoPoint);

            addListeners();
        }

        private void addListeners(){
            //final
            btn_EditCoords.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    String[] coords = coordsTxt.getText().toString().split("/");
                    displayAddDialog(coords[0].trim(), coords[1].trim());
                }
            });

            btn_DeleteCoords.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    points.remove(position);
                    RecyclerView flightPlanContainer = activity.findViewById(R.id.flightPlan_Coordinates);
                    flightPlanContainer.setHasFixedSize(true);
                    flightPlanContainer.setLayoutManager(new LinearLayoutManager(activity));

                    GeoPointRecyclerViewAdapter adapter = new GeoPointRecyclerViewAdapter(points,activity,null);
                    flightPlanContainer.setAdapter(adapter);
                }
            });
        }

        private void displayAddDialog(String latitude, String longitude){
            LayoutInflater layoutInflater = LayoutInflater.from(activity);
            View promptView = layoutInflater.inflate(R.layout.fragment_edit_geopoint, null);
            AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(activity);
            alertDialogBuilder.setView(promptView);

            final EditText txt_latitude = (EditText) promptView.findViewById(R.id.txt_lat);
            final EditText txt_longitude = (EditText) promptView.findViewById(R.id.txt_long);
            // setup a dialog window

            txt_latitude.setText(latitude);
            txt_longitude.setText(longitude);

            alertDialogBuilder.setPositiveButton("Save", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    String latitude = txt_latitude.getText().toString();
                    String longitude = txt_longitude.getText().toString();
                    GeoPoint p = new GeoPoint(Double.parseDouble(latitude), Double.parseDouble(longitude));
                    points.set(position, p);
                    coordsTxt.setText(p.getLatitude()+" / "+p.getLongitude());
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
}
