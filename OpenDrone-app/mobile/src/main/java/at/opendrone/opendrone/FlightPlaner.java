/*
 * Last modified: 14.10.18 22:52
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

package at.opendrone.opendrone;


import android.Manifest;
import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.graphics.Rect;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.annotation.NonNull;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.location.FusedLocationProviderClient;
import com.google.android.gms.location.LocationCallback;
import com.google.android.gms.location.LocationRequest;
import com.google.android.gms.location.LocationResult;
import com.google.android.gms.location.LocationServices;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.gson.Gson;

import org.osmdroid.api.IMapController;
import org.osmdroid.config.Configuration;
import org.osmdroid.events.MapEventsReceiver;
import org.osmdroid.tileprovider.tilesource.TileSourceFactory;
import org.osmdroid.util.GeoPoint;
import org.osmdroid.views.MapView;
import org.osmdroid.views.overlay.MapEventsOverlay;
import org.osmdroid.views.overlay.Marker;
import org.osmdroid.views.overlay.Polyline;
import org.w3c.dom.Text;

import java.util.LinkedList;
import java.util.List;

import static android.content.Context.LOCATION_SERVICE;

public class FlightPlaner extends Fragment {

    private MapView mMapView;

    private List<GeoPoint> points = new LinkedList<>();

    private SharedPreferences sp;
    private FloatingActionButton saveFAB;
    private FloatingActionButton removeFAB;
    private boolean canAddMarker = true;
    private Location location;

    private boolean mRequestingLocationUpdates = false;
    private boolean hasAlreadyCentered = false;

    private FusedLocationProviderClient mFusedLocationClient;
    private LocationCallback mLocationCallback;
    private LocationRequest mLocationRequest;

    private Polyline line = new Polyline();

    private Rect outRect = new Rect();
    private int[] locationAr = new int[2];

    private MotionEvent lastEvent;

    private List<Marker> markers = new LinkedList<>();

    private int draggedPosition = -1;

    public FlightPlaner() {
        // Required empty public constructor
        points = new LinkedList<>();
    }

    public void onResume() {
        super.onResume();
        //this will refresh the osmdroid configuration on resuming.
        //if you make changes to the configuration, use
        //SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
        //Configuration.getInstance().load(this, PreferenceManager.getDefaultSharedPreferences(this));

        mMapView.onResume(); //needed for compass, my location overlays, v6.0.0 and up

        if (mRequestingLocationUpdates) {
            startLocationUpdates();
        }
    }

    public void onPause() {
        super.onPause();
        //this will refresh the osmdroid configuration on resuming.
        //if you make changes to the configuration, use
        //SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
        //Configuration.getInstance().save(this, prefs);
        mMapView.onPause();  //needed for compass, my location overlays, v6.0.0 and up
        stopLocationUpdates();
    }

    private void stopLocationUpdates() {
        try {
            mFusedLocationClient.removeLocationUpdates(mLocationCallback);
        } catch (Exception ex) {
            Log.e("flightplany", "error", ex);
        }
    }

    private boolean checkGPSPermission() {
        return ContextCompat.checkSelfPermission(getActivity(), Manifest.permission.ACCESS_FINE_LOCATION)
                == PackageManager.PERMISSION_GRANTED || ContextCompat.checkSelfPermission(getActivity(), Manifest.permission.ACCESS_COARSE_LOCATION)
                == PackageManager.PERMISSION_GRANTED;
    }

    protected void createLocationRequest() {
        mLocationRequest = new LocationRequest();
        mLocationRequest.setInterval(10000);
        mLocationRequest.setFastestInterval(5000);
        mLocationRequest.setPriority(LocationRequest.PRIORITY_HIGH_ACCURACY);
    }


    /**
     * Only call this method when you have GPS permissions
     * starts the location updates with a callback and a locationrequest
     */
    @SuppressLint("MissingPermission")
    private void startLocationUpdates() {
        mFusedLocationClient.requestLocationUpdates(mLocationRequest,
                mLocationCallback,
                null /* Looper */);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
        switch (requestCode) {
            case OpenDroneUtils.RQ_GPS: {
                if (grantResults.length > 1
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED || grantResults[1] == PackageManager.PERMISSION_GRANTED) {
                    initWithCurrentPosition();
                } else {
                    setCenter(OpenDroneUtils.DEFAULT_LAT, OpenDroneUtils.DEFAULT_LNG);
                }
                return;
            }
            default: {
                Toast.makeText(getContext(), "Didn't grant all Permissions", Toast.LENGTH_LONG).show();
            }
        }
    }

    private void requestPermissionAndSetLocation() {
        if (!checkGPSPermission()) {
            setCenter(OpenDroneUtils.DEFAULT_LAT, OpenDroneUtils.DEFAULT_LNG);
            requestPermissions(new String[]{Manifest.permission.ACCESS_FINE_LOCATION, Manifest.permission.ACCESS_COARSE_LOCATION},
                    OpenDroneUtils.RQ_GPS);
        } else {
            initWithCurrentPosition();
        }
    }


    private void initWithCurrentPosition() {
        initLocationManager();
        createLocationRequest();
        initLocationCallback();
        mRequestingLocationUpdates = true;
        setLastKnownLocation();

    }

    /**
     * Only call this method when you have GPS permissions
     * Sets the last known position.
     */
    @SuppressLint("MissingPermission")
    private void setLastKnownLocation() {
        mFusedLocationClient.getLastLocation()
                .addOnSuccessListener(getActivity(), new OnSuccessListener<Location>() {
                    @Override
                    public void onSuccess(Location location) {
                        if (location != null) {
                            Log.i("flightplany", location.getLatitude() + "/ " + location.getLongitude());
                            setCoordinates(location);
                            hasAlreadyCentered = true;
                        } else {
                            setCenter(OpenDroneUtils.DEFAULT_LAT, OpenDroneUtils.DEFAULT_LNG);
                            Toast.makeText(getActivity(), getResources().getString(R.string.exception_no_location), Toast.LENGTH_SHORT).show();
                        }
                    }
                }).addOnFailureListener(getActivity(), new OnFailureListener() {
            @Override
            public void onFailure(@NonNull Exception e) {
                Toast.makeText(getActivity(), getResources().getString(R.string.exception_sorry), Toast.LENGTH_SHORT).show();
            }
        });
    }

    private void initMapView() {
        Log.i("flightplany", "test");
        mMapView.setTileSource(TileSourceFactory.MAPNIK);
        line = new Polyline(mMapView);

        mMapView.setBuiltInZoomControls(false);
        mMapView.setMultiTouchControls(true);

        //Manually entered a point -> jump to that location
        if (points.size() > 0) {
            GeoPoint p = points.get(0);
            setCenter(p.getLatitude(), p.getLongitude());
        } else {
            requestPermissionAndSetLocation();
        }
        //mLocationManager.getLastKnownLocation().getLongitude();

        addEventListener();
    }

    private void setCenter(double lat, double lng) {
        Log.i("flightplany", lat + "/ " + lng);
        IMapController mapController = mMapView.getController();
        mapController.setZoom(17);
        GeoPoint startPoint = new GeoPoint(lat, lng);
        mapController.setCenter(startPoint);
    }

    public void setCoordinates(Location l) {
        this.location = l;
        setCenter(location.getLatitude(), location.getLongitude());
    }

    private void showFAB() {
        saveFAB.setVisibility(View.VISIBLE);
    }

    private void addMarker(GeoPoint p) {
        Marker startMarker = new Marker(mMapView);
        startMarker.setPosition(p);
        startMarker.setAnchor(Marker.ANCHOR_CENTER, Marker.ANCHOR_BOTTOM);
        startMarker.setEnableTextLabelsWhenNoImage(true);

        addListenersToMarker(startMarker);


        mMapView.getOverlays().add(startMarker);
        markers.add(startMarker);

        addToLine(startMarker.getPosition());
        showFAB();
    }

    private void drawLine() {
        line.setPoints(points);
        mMapView.getOverlayManager().remove(line);
        mMapView.getOverlayManager().add(line);
        mMapView.invalidate();
    }

    private void addToLine(GeoPoint p) {
        points.add(p);
        drawLine();
    }

    private void addEventListener() {
        MapEventsReceiver mReceive = new MapEventsReceiver() {
            @Override
            public boolean singleTapConfirmedHelper(GeoPoint p) {
                if (canAddMarker) {
                    addMarker(p);
                }

                return false;
            }

            @Override
            public boolean longPressHelper(GeoPoint p) {
                return false;
            }
        };


        MapEventsOverlay OverlayEvents = new MapEventsOverlay(getContext(), mReceive);
        mMapView.getOverlays().add(OverlayEvents);
    }

    private void initLocationManager() {
        mFusedLocationClient = LocationServices.getFusedLocationProviderClient(getActivity());
    }

    private void initLocationCallback() {
        mLocationCallback = new LocationCallback() {
            @Override
            public void onLocationResult(LocationResult locationResult) {
                if (locationResult == null) {
                    return;
                }
                Location bestLocation = null;
                for (Location location : locationResult.getLocations()) {
                    if (bestLocation == null || location.getAccuracy() < bestLocation.getAccuracy()) {
                        bestLocation = location;
                    }
                }
                if (!hasAlreadyCentered) {
                    initWithCurrentPosition();
                }

            }
        };
    }

    private void loadConfig() {
        Context ctx = getActivity().getApplicationContext();
        Configuration.getInstance().load(ctx, PreferenceManager.getDefaultSharedPreferences(ctx));
    }

    private void addListeners() {
        saveFAB.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                savePointsAndAddInfo();
            }
        });

    }

    private void savePointsAndAddInfo() {
        FlightPlanSaveFragment fp = new FlightPlanSaveFragment(points);
        fp.setFlightPlaner(this);
        FragmentTransaction ft = getActivity().getSupportFragmentManager().beginTransaction();
        ft.replace(R.id.frameLayout_FragmentContainer, fp);
        ft.commit();
    }

    private void initSP() {
        sp = getActivity().getSharedPreferences("at.opendrone.opendrone", Context.MODE_PRIVATE);
    }

    private void findViews(View view) {
        saveFAB = view.findViewById(R.id.fpFAB);
        removeFAB = view.findViewById(R.id.deleteFab);
        mMapView = view.findViewById(R.id.map);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        loadConfig();
        View view = inflater.inflate(R.layout.fragment_flightplaner, container, false);
        findViews(view);
        initSP();
        addListeners();
        initMapView();
        drawPoints();
        return view;
    }

    public void setPoints(List<GeoPoint> points) {
        this.points = points;
    }

    private void addListenersToMarker(Marker marker) {
        Log.i("flightplany", "setMarker");
        marker.setDraggable(true);
        marker.setOnMarkerDragListener(new Marker.OnMarkerDragListener() {
            @Override
            public void onMarkerDrag(Marker marker) {
                if ((!canAddMarker && draggedPosition == points.size() - 1) || (!canAddMarker && draggedPosition == 0)) {
                    points.set(0, marker.getPosition());
                    points.set(points.size() - 1, marker.getPosition());
                } else {
                    points.set(draggedPosition, marker.getPosition());
                }
                drawLine();
            }

            @Override
            public void onMarkerDragEnd(Marker marker) {
                if (lastEvent != null && isViewInBounds(removeFAB, (int) lastEvent.getX(), (int) lastEvent.getY() + (int) (marker.getIcon().getIntrinsicHeight() * 1.5))) {
                    removeMarker(marker);
                } else {
                    if ((!canAddMarker && draggedPosition == points.size() - 1) || (!canAddMarker && draggedPosition == 0)) {
                        points.set(0, marker.getPosition());
                        points.set(points.size() - 1, marker.getPosition());
                    } else {
                        points.set(draggedPosition, marker.getPosition());
                    }
                }
                removeFAB.setVisibility(View.GONE);
                saveFAB.setVisibility(View.VISIBLE);
                drawLine();
            }

            @Override
            public void onMarkerDragStart(Marker marker) {
                //poiMarkers.invalidate();
                //mapView.invalidate();
                Log.i("flightplany", "dragstart");
                draggedPosition = points.lastIndexOf(marker.getPosition());
                removeFAB.setVisibility(View.VISIBLE);
                saveFAB.setVisibility(View.GONE);

            }
        });
        marker.setOnMarkerClickListener(new Marker.OnMarkerClickListener() {
            @Override
            public boolean onMarkerClick(Marker marker, MapView mapView) {
                //savePointsAndAddInfo();
                addToLine(marker.getPosition());
                markers.add(marker);
                canAddMarker = false;
                return true;
            }
        });

        mMapView.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                lastEvent = motionEvent;
                return false;
            }
        });
    }

    private void removeMarker(Marker marker) {
        Toast.makeText(getContext(), "DELETE", Toast.LENGTH_SHORT).show();
        if ((draggedPosition == 0 || draggedPosition == points.size() - 1) && !canAddMarker) {
            int lastPosition = points.size() - 1;
            deleteFromLists(lastPosition);
            deleteFromLists(0);
        } else {
            deleteFromLists(draggedPosition);
            //points.remove(draggedPosition);
            //mMapView.getOverlayManager().remove(marker);
        }

        canAddMarker = true;

        drawLine();
    }

    private void deleteFromLists(int position) {
        Marker tmp = markers.get(position);
        markers.remove(position);
        mMapView.getOverlayManager().remove(tmp);
        points.remove(position);
        mMapView.invalidate();
    }

    private boolean isViewInBounds(View view, int x, int y) {
        view.getDrawingRect(outRect);
        view.getLocationOnScreen(locationAr);
        outRect.offset(locationAr[0], locationAr[1]);
        Log.i("flightplany", outRect.left + " / " + outRect.bottom + " / " + outRect.top + " / " + outRect.right);
        return outRect.contains(x, y);
    }

    public void drawPoints() {
        for (int i = 0; i < points.size(); i++) {
            Marker startMarker = new Marker(mMapView);
            startMarker.setPosition(points.get(i));
            startMarker.setAnchor(Marker.ANCHOR_CENTER, Marker.ANCHOR_BOTTOM);
            addListenersToMarker(startMarker);
            mMapView.getOverlays().add(startMarker);
        }
        drawLine();
        showFAB();
    }
}
