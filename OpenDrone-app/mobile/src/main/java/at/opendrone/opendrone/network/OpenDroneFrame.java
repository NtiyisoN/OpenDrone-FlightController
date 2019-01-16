package at.opendrone.opendrone.network;

import android.util.Log;

import java.nio.file.attribute.FileAttribute;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

public class OpenDroneFrame {
    private byte slaveID;
    private byte functionCode1;
    private int data1;
    private int parity1;
    private byte functionCode2;
    private int data2;
    private int parity2;

    public OpenDroneFrame(byte slaveID, byte functionCode1, int data1, byte functionCode2, int data2) {
        this.slaveID = slaveID;
        this.functionCode1 = functionCode1;
        this.functionCode2 = functionCode2;

        if (data1 % 2 == 0) {
            parity1 = 0;
        } else {
            parity1 = 1;
        }

        if (data2 % 2 == 0) {
            parity2 = 0;
        } else {
            parity2 = 1;
        }

        this.data1 = data1;
        this.data2 = data2;
    }

    private List<Byte> generateStart() {
        List<Byte> bytes = new LinkedList<>();
        for (int i = 0; i < 16; i++) {
            if (i >= 16 - 1 - 3) {
                bytes.add((byte) 0);
                continue;
            }
            if (i % 2 == 0) {
                bytes.add((byte) 1);
            } else {
                bytes.add((byte) 0);
            }
        }
        return bytes;
    }

    private List<Byte> generateEnd() {
        List<Byte> bytes = generateStart();
        Collections.reverse(bytes);
        return bytes;
    }

    public byte[] convert(Byte[] toConvert) {
        byte[] bytes = new byte[toConvert.length];
        int j = 0;
        // Unboxing Byte values. (Byte[] to byte[])
        for (Byte b : toConvert)
            bytes[j++] = b.byteValue();
        return bytes;
    }

    @Override
    public String toString() {
        String str = "";
        str += "1010101010101000;";
        str += (int) slaveID + ";";
        str += "2;";
        str += (int) functionCode1 + ";";
        str += data1 + ";";
        str += parity1 + ";";
        str += (int) functionCode2 + ";";
        str += data2 + ";";
        str += parity2 + ";";
        str += "0001010101010101;";

        Log.i("tcpy", data2+"");
        return str;
    }
}
