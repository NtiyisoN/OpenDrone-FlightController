/*
 * Last modified: 30.09.18 15:07
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

package at.opendrone.opendrone;

import android.content.Context;
import android.content.Intent;
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
import android.view.View;
import android.widget.FrameLayout;

import com.google.android.gms.oss.licenses.OssLicensesMenuActivity;

public class MainActivity extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener {

    //public static TCPSend client;
    public static FragmentManager fm;

    private DrawerLayout drawerLayout;
    private boolean isOpened = false;
    private FrameLayout fragmentContainer;
    private int lastFragment;

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

        //client = new TCPSend(TARGET);
        //client.start();

        findViews();

        initToolbar();
        initNavView();
        fm = getSupportFragmentManager();
        initHomeFragment();
        //initHomeFragment();
    }

    private void findViews() {
        fragmentContainer = findViewById(R.id.frameLayout_FragmentContainer);
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
        lastFragment = OpenDroneUtils.LF_HOME;
        HomeFragment hf = new HomeFragment();
        FragmentTransaction fragmentTransaction = fm.beginTransaction();
        fragmentTransaction.replace(fragmentContainer.getId(), hf);
        fragmentTransaction.setTransition(FragmentTransaction.TRANSIT_FRAGMENT_FADE);
        fragmentTransaction.commit();
        //updateFragment(hf);
    }

    @Override
    public void onRestoreInstanceState(Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);
        // Restore UI state from the savedInstanceState.
        // This bundle has also been passed to onCreate.
        lastFragment = savedInstanceState.getInt("LastFragment");
        switch (lastFragment) {
            case OpenDroneUtils.LF_HOME:
                initHomeFragment();
                break;
            case OpenDroneUtils.LF_DRONE:
                initDronesFragment();
                break;
            case OpenDroneUtils.LF_FP:
                initFlightplaner();
                break;
            case OpenDroneUtils.LF_FLY:
                initFlyStartFragment();
                break;
            default:
                Log.i("Mainy", "Default onRestore");
                break;
        }
    }

    @Override
    public void onSaveInstanceState(Bundle savedInstanceState) {
        super.onSaveInstanceState(savedInstanceState);
        // Save UI state changes to the savedInstanceState.
        // This bundle will be passed to onCreate if the process is
        // killed and restarted.
        savedInstanceState.putInt("LastFragment", lastFragment);
        // etc.
    }

    @Override
    public boolean onNavigationItemSelected(@NonNull MenuItem item) {
        switch (item.getItemId()) {
            case R.id.navItem_Home:
                //clearContainer();
                initHomeFragment();
                closeDrawer();
                return true;
            case R.id.navItem_Drones:
                //Toast.makeText(getApplicationContext(), "Pressed Drones", Toast.LENGTH_SHORT).show();
                //initDroneSettingsFragment();
                //clearContainer();
                initDronesFragment();
                closeDrawer();
                return true;
            case R.id.navItem_FlightPlanner:
                //clearContainer();
                //Toast.makeText(getApplicationContext(), "Pressed FlightPlanner", Toast.LENGTH_SHORT).show();
                initFlightplaner();
                closeDrawer();
                return true;
            case R.id.navItem_Fly:
                //clearContainer();
                //Toast.makeText(getApplicationContext(), "Pressed Fly", Toast.LENGTH_SHORT).show();
                initFlyStartFragment();
                closeDrawer();
                return true;
            case R.id.navItem_Libs:
                displayLibraries();
            default:
                Log.i("MainActivity", "OnNavigationItem Default case");
                return false;
        }
    }

    public void closeDrawer() {
        drawerLayout.closeDrawer(GravityCompat.START);
        isOpened = false;
    }

    private void openDrawer() {
        drawerLayout.addDrawerListener(new DrawerLayout.DrawerListener() {
            @Override
            public void onDrawerSlide(@NonNull View view, float v) {

            }

            @Override
            public void onDrawerOpened(@NonNull View view) {
                isOpened = true;
            }

            @Override
            public void onDrawerClosed(@NonNull View view) {
                isOpened = false;
            }

            @Override
            public void onDrawerStateChanged(int i) {

            }
        });
        drawerLayout.openDrawer(GravityCompat.START);
        isOpened = true;
    }

    @Override
    public void onBackPressed() {
        clearContainer();
        initHomeFragment();
    }

    private void initDronesFragment() {
        lastFragment = OpenDroneUtils.LF_DRONE;
        Fragment defFragment = new DroneCardListRecyclerFragment();
        updateFragment(defFragment);
    }

    private void updateFragment(Fragment fragment) {
        FragmentTransaction fragmentTransaction = fm.beginTransaction();
        fragmentTransaction.replace(fragmentContainer.getId(), fragment);
        fragmentTransaction.setTransition(FragmentTransaction.TRANSIT_FRAGMENT_FADE);
        fragmentTransaction.commit();
    }


    private void initFlyStartFragment() {
        //FlyStart defFragment = new FlyStart();
        lastFragment = OpenDroneUtils.LF_FLY;
        FlyManualFlight defFragment = new FlyManualFlight();
        updateFragment(defFragment);
    }

    private void initFlightplaner() {
        lastFragment = OpenDroneUtils.LF_FP;
        SharedPreferences sp = getApplication().getSharedPreferences("at.opendrone.opendrone", Context.MODE_PRIVATE);
        sp.edit().remove(OpenDroneUtils.SP_FLIGHTPLAN_HOLDER).apply();
        FlightPlanListFragment defFragment = new FlightPlanListFragment();
        updateFragment(defFragment);
    }

    private void displayLibraries() {
// When the user selects an option to see the licenses:
        startActivity(new Intent(this, OssLicensesMenuActivity.class));
    }


}
