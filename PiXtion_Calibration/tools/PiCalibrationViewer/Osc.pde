import oscP5.*;
import netP5.*;

PVector dot1 = new PVector(0,0);
PVector dot2 = new PVector(0,0);
ArrayList<String> hostList;
int numHosts = 2;

byte[] rgbVideoBytes, depthVideoBytes;

String ipNumber = "127.0.0.1";
int sendPort = 9998;
int receivePort = 7110;
int datagramSize = 1000000;
OscP5 oscP5;
NetAddress myRemoteLocation;

void oscSetup() {
  OscProperties op = new OscProperties();
  op.setListeningPort(receivePort);
  op.setDatagramSize(datagramSize);
  oscP5 = new OscP5(this, op);
  myRemoteLocation = new NetAddress(ipNumber, sendPort);
  hostList = new ArrayList<String>();
}

// Receive message example
void oscEvent(OscMessage msg) {
  println(msg);
  if (msg.checkAddrPattern("/points") && msg.checkTypetag("sbb")) {    
    String hostname = msg.get(0).stringValue();
    
    byte[] rgbVideoBytes = msg.get(1).blobValue();
    PImage rgb = fromJpeg(rgbVideoBytes);
    
    /*
    byte[] readPointsBytes = msg.get(2).blobValue();
   
    ArrayList<PVector> points = new ArrayList<PVector>();
    for (int i = 0; i < readPointsBytes.length; i += 12) { //+=16) { 
      byte[] bytesX = { readPointsBytes[i], readPointsBytes[i+1], readPointsBytes[i+2], readPointsBytes[i+3] };
      byte[] bytesY = { readPointsBytes[i+4], readPointsBytes[i+5], readPointsBytes[i+6], readPointsBytes[i+7] };
      byte[] bytesZ = { readPointsBytes[i+8], readPointsBytes[i+9], readPointsBytes[i+10], readPointsBytes[i+11] };
      //byte[] bytesW = { readPointsBytes[i+12], readPointsBytes[i+13], readPointsBytes[i+14], readPointsBytes[i+15] };

      float x = asFloat(bytesX);
      float y = asFloat(bytesY);
      float z = asFloat(bytesZ);
      //float w = asFloat(bytesW);
      if (!Float.isNaN(x) && !Float.isNaN(y)) { // && !Float.isNaN(z)) {
        PVector p = new PVector(-x, -y, -z);
        points.add(p);
        //println(p.x + ", " + p.z + ", " + p.y);
      }
    }
    
    if (frame.firstRun) frame.init(rgb, points);
    */
    image(rgb, 0, 0);
  }
}
