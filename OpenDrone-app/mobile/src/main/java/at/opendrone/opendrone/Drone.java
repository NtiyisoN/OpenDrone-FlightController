package at.opendrone.opendrone;

import java.io.Serializable;

class Drone implements Serializable {

    String name;
    String description;
    String type;

    public Drone() {
    }

    public Drone(String name, String desc, String type) {
        this.name = name;
        this.description = desc;
        this.type = type;
    }
}