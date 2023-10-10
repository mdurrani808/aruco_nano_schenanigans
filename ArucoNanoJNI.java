package org.photonvision.vision.aruco.nano;


public class ArucoNanoJNI {
    static {
        System.loadLibrary("aruco_nano");
    }

    public static class Marker {
        public int id = -1;
        public float[][] points = new float[4][2];
    }

    public native Marker[] detect(long matAddr, int maxAttemptsPerCandidate, int dict);
}
