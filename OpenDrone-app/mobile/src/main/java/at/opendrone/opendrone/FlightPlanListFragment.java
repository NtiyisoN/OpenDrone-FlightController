/*
 * Last modified: 30.10.18 12:13
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

package at.opendrone.opendrone;


import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import com.google.gson.Gson;

import java.io.Serializable;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;


/**
 * A simple {@link Fragment} subclass.
 */
public class FlightPlanListFragment extends Fragment {

    private View view;
    private RecyclerView recyclerView;
    private FloatingActionButton btn_AddFP;
    private List<Flightplan> plans;

    public FlightPlanListFragment() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        SharedPreferences sp = getActivity().getSharedPreferences("at.opendrone.opendrone", Context.MODE_PRIVATE);

        // Inflate the layout for this fragment
        view = inflater.inflate(R.layout.fragment_flight_plan_list, container, false);
        recyclerView = view.findViewById(R.id.flightplans);

        btn_AddFP = view.findViewById(R.id.btn_AddFlightPlan);

        try {
            Gson gson = new Gson();
            String flightplanJSON = sp.getString(OpenDroneUtils.SP_FLIGHTPLANS, "");
                Flightplan[] flightPlanAr = gson.fromJson(flightplanJSON, Flightplan[].class);
                plans = new LinkedList<>(Arrays.asList(flightPlanAr));
                setAdapter();
            }
        }
        btn_AddFP.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SharedPreferences sp = getActivity().getSharedPreferences("at.opendrone.opendrone", Context.MODE_PRIVATE);
                sp.edit().remove(OpenDroneUtils.SP_FLIGHTPLAN_HOLDER).apply();
                FlightPlaner fp = new FlightPlaner();
                ft.replace(R.id.frameLayout_FragmentContainer, fp);
                ft.commit();
            }
        });


        return view;
    }

        plans.remove(position);
            SharedPreferences sp = getActivity().getSharedPreferences("at.opendrone.opendrone", Context.MODE_PRIVATE);
            Gson gson = new Gson();
            String serialized = gson.toJson(plans.toArray());
            sp.edit().putString(OpenDroneUtils.SP_FLIGHTPLANS, serialized).apply();

        }

    }

        FlightPlanRecyclerViewAdapter adapter = new FlightPlanRecyclerViewAdapter(plans, (AppCompatActivity) getActivity(), this);
        recyclerView.setLayoutManager(new LinearLayoutManager(getActivity()));
        recyclerView.setAdapter(adapter);
    }

}
