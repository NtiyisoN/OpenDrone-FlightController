/*
 * Last modified: 14.10.18 22:47
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

package at.opendrone.opendrone;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.media.Image;
import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.constraint.ConstraintLayout;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.ActivityOptionsCompat;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.CardView;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.google.gson.Gson;
import com.squareup.picasso.Picasso;

import org.w3c.dom.Text;

import java.io.File;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;


    private List<Flightplan> flightplans;
    private AppCompatActivity activity;
    private FlightPlanListFragment fragment;
    private SharedPreferences sp;

        this.flightplans = flightplans;
        this.activity = activity;
        this.fragment = fragment;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View v = LayoutInflater.from(parent.getContext()).inflate(R.layout.fragment_single_flight_plan, parent, false);

        return new ViewHolder(v);
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, final int position) {
        Flightplan fp = flightplans.get(position);
        holder.flightplan_name.setText(fp.getName());
        holder.flightplan_description.setText(fp.getDescription());
        holder.flightplan = fp;
        holder.flightplan.setId(position);
        holder.position = position;

    @Override
    public int getItemCount() {
        return this.flightplans.size();
    }


        private CardView mCardView;
        private TextView flightplan_name;
        private TextView flightplan_description;
        private ImageView btn_deleteFlightplan;
        private Button btn_editFlightplan;
        private Flightplan flightplan;
        private int position = -1;

        public ViewHolder(View itemView) {
            super(itemView);
            mCardView = itemView.findViewById(R.id.single_flightplan_card);
            flightplan_name = itemView.findViewById(R.id.text_FlightPlanName);
            flightplan_description = itemView.findViewById(R.id.textView_FlightPlan);
            btn_deleteFlightplan = itemView.findViewById(R.id.imgBtn_Delete_Flightplan);
            btn_editFlightplan = itemView.findViewById(R.id.btn_Settings_flightplan);
            btn_editFlightplan.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    SharedPreferences sp = activity.getSharedPreferences("at.opendrone.opendrone", Context.MODE_PRIVATE);
                    Gson gson = new Gson();

                    sp.edit().remove(OpenDroneUtils.SP_FLIGHTPLAN_HOLDER).apply();

                    String object = gson.toJson(flightplan);

                    FlightPlanSaveFragment fp = new FlightPlanSaveFragment(flightplan.getName(), flightplan.getDescription(), flightplan.getCoordinates(), position);
                    ft.replace(R.id.frameLayout_FragmentContainer, fp);
                    ft.commit();
                }
            });
            btn_deleteFlightplan.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    fragment.deletePosition(position);
                    FlightPlanListFragment fp = new FlightPlanListFragment();
                    ft.replace(R.id.frameLayout_FragmentContainer, fp);
                    ft.commit();
                }
            });
        }
    }
}
