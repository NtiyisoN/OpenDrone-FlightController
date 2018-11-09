/*
 * Last modified: 09.09.18 17:21
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

package at.opendrone.opendrone;


import android.os.Bundle;
import android.os.Environment;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.util.Xml;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import org.xmlpull.v1.XmlSerializer;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOError;
import java.io.IOException;
import java.io.Serializable;


/**
 * A simple {@link Fragment} subclass.
 */
public class DroneCardList extends DroneCardListSingleFragment {


    public DroneCardList() {
        // Required empty public constructor
    }

    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_drone__card_list, container, false);
    }

    protected Fragment createFragment() {
        return DroneCardListRecyclerFragment.newInstance();
    }
}

class Drone implements Serializable {

    String name;
    String description;
    String type;

    }
        this.name = name;
        this.description = desc;
        this.type = type;
    }
}