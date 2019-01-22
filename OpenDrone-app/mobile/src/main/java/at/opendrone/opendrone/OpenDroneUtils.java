/*
 * Last modified: 23.10.18 08:23
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

package at.opendrone.opendrone;

public class OpenDroneUtils {
    public static String SP_POINTS = "sp_points";
    public static final int RQ_GPS = 7;
    public static String SP_FLIGHTPLANS = "sp_flightplans";
    public static String SP_FLIGHTPLAN_HOLDER = "sp_flightplan";
    public static final double DEFAULT_LAT = 48.2468036;
    public static final double DEFAULT_LNG = 14.6199875;
    public static final String THROTTLE_UP = "THRT_UP";
    public static final String THROTTLE_DOWN = "THRT_DOWN";
    public static final String YAW_RIGHT = "YAW_R";
    public static final String YAW_LEFT = "YAW_L";
    public static final String PITCH_FORWARDS = "PTCH_F";
    public static final String PITCH_BACKWARDS = "PTCH_B";
    public static final String ROLL_LEFT = "ROLL_L";
    public static final String ROLL_RIGHT = "ROLL_R";

    public static final int LF_HOME = 0;
    public static final int LF_DRONE = 1;
    public static final int LF_FP = 2;
    public static final int LF_FLY = 3;

    public static final byte CODE_THROTTLE_UP= (byte) 1;
    public static final byte CODE_THROTTLE_DOWN= (byte) 2;
    public static final byte CODE_YAW_RIGHT= (byte) 3;
    public static final byte CODE_YAW_LEFT= (byte) 4;
    public static final byte CODE_PITCH_FORWARD= (byte) 5;
    public static final byte CODE_PITCH_BACKWARD= (byte) 6;
    public static final byte CODE_ROLL_LEFT= (byte) 7;
    public static final byte CODE_ROLL_RIGHT= (byte) 8;
    public static final byte CODE_GO_HOME= (byte) 9;
    public static final byte CODE_ABORT= (byte) 10;

    public static final int CODE_CONTROLLER_TEMP = 1;
    public static final int CODE_AIR_TEMP = 2;
    public static final int CODE_POSITTION = 3;
    public static final int CODE_PRESSURE = 4;
    public static final int CODE_HEIGHT = 5;
    public static final int CODE_STATUS = 6;
    public static final int CODE_VELOCITY = 7;
}
