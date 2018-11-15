/*
 * Last modified: 09.09.18 17:21
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

package at.opendrone.opendrone;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.constraint.ConstraintLayout;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.ActivityOptionsCompat;
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

import com.squareup.picasso.Picasso;

import java.io.File;
import java.util.List;

public class DroneRecyclerViewAdapter extends RecyclerView.Adapter<DroneRecyclerViewAdapter.ViewHolder> {

    private List<Drone> drones;
    private Activity activity;
    private DroneCardListRecyclerFragment fragment;
    private SharedPreferences sp;

    public DroneRecyclerViewAdapter(List<Drone> drones, Activity activity, DroneCardListRecyclerFragment fragment, SharedPreferences sp) {
        this.drones = drones;
        this.activity = activity;
        this.fragment = fragment;
        this.sp = sp;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View v = LayoutInflater.from(parent.getContext()).inflate(R.layout.fragment_drone_card_list_dronecard, parent, false);

        return new ViewHolder(v);
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, final int position) {
        final Drone drone = drones.get(position);
        holder.text_droneName.setText(drone.name);
        holder.text_droneDescription.setText(drone.description);
        final ConstraintLayout sourceView = holder.itemView.findViewById(R.id.constraintLayout_Item);

        final String imgString = sp.getString("DroneImg" + position, "");
        if (!imgString.equals("")) {
            Uri imgUri = Uri.fromFile(new File(imgString));
            Log.i("Picky", "asdf: " + imgString);
            Picasso.get().load(imgUri).into(holder.imageView);
        }

        holder.btn_Settings.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(activity, DroneSettingsActivity.class);
                intent.putExtra("Mode", "edit");
                intent.putExtra("Drone", drone);
                intent.putExtra("Position", position);
                intent.putExtra("Img", imgString);

                Bundle options = ActivityOptionsCompat.makeScaleUpAnimation(
                        sourceView, 0, 0, sourceView.getWidth(), sourceView.getHeight()).toBundle();

                ActivityCompat.startActivity(activity, intent, options);
            }
        });

        holder.btn_Delete.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //Toast.makeText(activity.getApplicationContext(), "Pressed Delete", Toast.LENGTH_LONG).show();
                AlertDialog.Builder builder = new AlertDialog.Builder(activity);
                // Add the buttons
                builder.setMessage(R.string.alertDialog_Delete);

                builder.setPositiveButton(R.string.alertDialog_Accept, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        fragment.remove(position);

                        sp.edit().putString("DroneImg" + position, "").apply();

                        /*SharedPreferences sp = activity.getSharedPreferences("at.opendrone.opendrone", MODE_PRIVATE);
                        String droneJSON = sp.getString("DroneList","");

                        Gson gson = new Gson();
                        String serialized = gson.toJson(DroneCardListRecyclerFragment.drones.toArray());

                        sp.edit().putString("DroneList", serialized).apply();*/
                    }
                });

                builder.setNegativeButton(R.string.alertDialog_Deny, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {

                    }
                });

                // Create the AlertDialog
                AlertDialog dialog = builder.create();
                dialog.show();
            }
        });
        holder.btn_Settings.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View view) {
                fragment.remove(position);
                fragment.updateAdapter();
                return true;
            }
        });
    }

    @Override
    public int getItemCount() {
        return drones.size();
    }

    public class ViewHolder extends RecyclerView.ViewHolder {
        private CardView mCardView;
        private TextView text_droneName;
        private TextView text_droneDescription;
        private Button btn_Settings;
        private ImageButton btn_Delete;
        private ImageView imageView;

        public ViewHolder(View itemView) {
            super(itemView);
            mCardView = itemView.findViewById(R.id.card_container);
            text_droneName = itemView.findViewById(R.id.text_DroneName);
            text_droneDescription = itemView.findViewById(R.id.textView_FlightPlan);
            btn_Settings = itemView.findViewById(R.id.btn_Settings);
            btn_Delete = itemView.findViewById(R.id.imgBtn_Delete);
            imageView = itemView.findViewById(R.id.imageView);
        }
    }
}
