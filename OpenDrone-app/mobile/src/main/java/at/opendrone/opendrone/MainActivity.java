/*
 * Last modified: 30.09.18 15:07
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

package at.opendrone.opendrone;

import android.content.Context;
import android.content.SharedPreferences;
import android.support.annotation.NonNull;
import android.support.design.widget.NavigationView;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.MenuItem;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener {

    private DrawerLayout drawerLayout;

    public static FragmentManager fm;

    private boolean isOpened = false;

    private void initFragments() {
        /*Do this when changing Fragment:

        HomeFragment defFragment = new HomeFragment();

        FragmentTransaction ft  =fm.beginTransaction();
        ft.replace(R.id.frameLayout_FragmentContainer, defFragment);
        ft.commit();*/
    }


    private void initToolbar() {
        Toolbar toolbar = findViewById(R.id.toolbar_Main);
        setSupportActionBar(toolbar);

        ActionBar actionbar = getSupportActionBar();
        actionbar.setDisplayHomeAsUpEnabled(true);
        actionbar.setHomeAsUpIndicator(R.drawable.ic_menu);
    }

    private void initNavView() {
        drawerLayout = findViewById(R.id.drawerLayout);
        NavigationView navView = findViewById(R.id.navView);

        navView.setNavigationItemSelectedListener(this);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initToolbar();
        initNavView();
        fm = getSupportFragmentManager();
        initHomeFragment();
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                if (isOpened) {
                    closeDrawer();
                } else {
                    openDrawer();
                }
                return true;
            default:
                Log.i("MainActivity", "OnOptionItemSelected default case");
                return super.onOptionsItemSelected(item);
        }
    }

    public void clearContainer() {
        for (Fragment fragment : fm.getFragments()) {
            if (fragment != null) {
                getSupportFragmentManager().beginTransaction().remove(fragment).commit();
            }
        }
    }

    public void initHomeFragment() {
        HomeFragment hf = new HomeFragment();
        FragmentTransaction ft = fm.beginTransaction();
        ft.add(R.id.frameLayout_FragmentContainer, hf);
        ft.commit();
    }

    @Override
    public boolean onNavigationItemSelected(@NonNull MenuItem item) {
        switch (item.getItemId()) {
            case R.id.navItem_Home:
                clearContainer();
                initHomeFragment();
                closeDrawer();
                return true;
            case R.id.navItem_Drones:
                Toast.makeText(getApplicationContext(), "Pressed Drones", Toast.LENGTH_SHORT).show();
                //initDroneSettingsFragment();
                clearContainer();
                initDroneRecyclerFragment();
                closeDrawer();
                return true;
            case R.id.navItem_FlightPlanner:
                clearContainer();
                Toast.makeText(getApplicationContext(), "Pressed FlightPlanner", Toast.LENGTH_SHORT).show();
                initFlightplaner();
                closeDrawer();
                return true;
            case R.id.navItem_Fly:
                clearContainer();
                Toast.makeText(getApplicationContext(), "Pressed Fly", Toast.LENGTH_SHORT).show();
                initFlyStartFragment();
                closeDrawer();
                return true;
            default:
                Log.i("MainActivity", "OnNavigationItem Default case");
                return false;
        }
    }

    private void closeDrawer() {
        drawerLayout.closeDrawer(GravityCompat.START);
        isOpened = false;
    }

    private void openDrawer() {
        drawerLayout.openDrawer(GravityCompat.START);
        isOpened = true;
    }

    @Override
    public void onBackPressed() {
        clearContainer();
        initHomeFragment();
    }

    private void initDronesFragment() {
        DroneCardList defFragment = new DroneCardList();

        FragmentTransaction ft = fm.beginTransaction();
        ft.replace(R.id.frameLayout_FragmentContainer, defFragment.createFragment());
        ft.commit();
    }

    private void initDroneRecyclerFragment() {
        DroneCardListRecyclerFragment defFragment = new DroneCardListRecyclerFragment();

        FragmentTransaction ft = fm.beginTransaction();
        ft.add(R.id.frameLayout_FragmentContainer, defFragment);
        ft.commit();
    }

    private void initFlyStartFragment() {
        FlyStart defFragment = new FlyStart();

        FragmentTransaction ft = fm.beginTransaction();
        ft.replace(R.id.frameLayout_FragmentContainer, defFragment);
        ft.commit();
    }

    private void initFlightplaner() {

        SharedPreferences sp = getApplication().getSharedPreferences("at.opendrone.opendrone", Context.MODE_PRIVATE);
        sp.edit().remove(OpenDroneUtils.SP_FLIGHTPLAN_HOLDER).apply();

        FlightPlanListFragment defFragment = new FlightPlanListFragment();

        FragmentTransaction ft = fm.beginTransaction();
        ft.replace(R.id.frameLayout_FragmentContainer, defFragment);
        ft.commit();
    }


}
