/*
 * Last modified: 09.09.18 17:21
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

package at.opendrone.opendrone;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Environment;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.constraint.ConstraintLayout;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.ActivityOptionsCompat;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.widget.CardView;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Xml;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.google.gson.Gson;

import org.w3c.dom.Text;
import org.xmlpull.v1.XmlSerializer;

import java.io.Console;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.Serializable;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

import static android.content.Context.MODE_PRIVATE;

public class DroneCardListRecyclerFragment extends Fragment {

    public static List<Drone> drones = new LinkedList<>();
    private RecyclerView recyclerView;
    private SharedPreferences sp;

    public DroneCardListRecyclerFragment() {

    }

    public static Fragment newInstance() {
        return new DroneCardListRecyclerFragment();
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_drone_card_list_recycler, container, false);

        recyclerView = view.findViewById(R.id.recycler_view);
        recyclerView.setLayoutManager(new LinearLayoutManager(getActivity()));
        recyclerView.setAdapter(new DroneRecyclerViewAdapter(drones, getActivity(), DroneCardListRecyclerFragment.this, sp));

        sp = getActivity().getSharedPreferences("at.opendrone.opendrone", MODE_PRIVATE);
        String droneJSON = sp.getString("DroneList", "");

        if (!droneJSON.equals("")) {
            Gson gson = new Gson();
            Drone[] droneAr = gson.fromJson(droneJSON, Drone[].class);

            drones = new LinkedList<>(Arrays.asList(droneAr));
        }


        FloatingActionButton startConfig = view.findViewById(R.id.btn_StartConfiguration);
        final ConstraintLayout sourceView = container.findViewById(R.id.constraintLayout_Item);
        startConfig.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Activity activity = getActivity();
                Intent intent = new Intent(activity, DroneSettingsActivity.class);
                intent.putExtra("Mode", "new");
                /*Bundle options = ActivityOptionsCompat.makeScaleUpAnimation(
                        sourceView, 0, 0, sourceView.getWidth(), sourceView.getHeight()).toBundle();*/

                //ActivityCompat.startActivity(activity, intent, options);
                startActivity(intent);
            }
        });

        return view;
    }

    public void remove(int i) {
        drones.remove(i);
        updateAdapter();
    }

    public void updateAdapter() {
        DroneRecyclerViewAdapter adapter = new DroneRecyclerViewAdapter(drones, this.getActivity(), DroneCardListRecyclerFragment.this, sp);
        recyclerView.setAdapter(adapter);
        //atm json, later Java DOM XML
        Gson gson = new Gson();
        String serialized = gson.toJson(drones.toArray());

        sp.edit().putString("DroneList", serialized).apply();
    }

    @Override
    public void onResume() {
        updateAdapter();
        super.onResume();
    }

}
