import oscP5.*;
import netP5.*;

PVector dot1 = new PVector(0,0);
PVector dot2 = new PVector(0,0);
ArrayList<String> hostList;
int numHosts = 2;

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
  if (msg.checkAddrPattern("/contour") && msg.checkTypetag("sibb")) {    
    String hostname = msg.get(0).stringValue();
    int index = msg.get(1).intValue();
    byte[] readColorBytes = msg.get(2).blobValue();
    byte[] readPointsBytes = msg.get(3).blobValue();
   
    byte[] bytesR = { readColorBytes[0], readColorBytes[1], readColorBytes[2], readColorBytes[3] };
    byte[] bytesG = { readColorBytes[4], readColorBytes[5], readColorBytes[6], readColorBytes[7] };
    byte[] bytesB = { readColorBytes[8], readColorBytes[9], readColorBytes[10], readColorBytes[11] };

    float r = asFloat(bytesR);
    float g = asFloat(bytesG);
    float b = asFloat(bytesB);
    color c = color(255);
    if (!Float.isNaN(r) && !Float.isNaN(g) && !Float.isNaN(b)) {
      c = color(r,g,b);
    }
 
    ArrayList<PVector> points = new ArrayList<PVector>();
    for (int i = 0; i < readPointsBytes.length; i += 8) { //+=16) { 
      byte[] bytesX = { readPointsBytes[i], readPointsBytes[i+1], readPointsBytes[i+2], readPointsBytes[i+3] };
      byte[] bytesY = { readPointsBytes[i+4], readPointsBytes[i+5], readPointsBytes[i+6], readPointsBytes[i+7] };
      //byte[] bytesZ = { readPointsBytes[i+8], readPointsBytes[i+9], readPointsBytes[i+10], readPointsBytes[i+11] };
      //byte[] bytesW = { readPointsBytes[i+12], readPointsBytes[i+13], readPointsBytes[i+14], readPointsBytes[i+15] };

      float x = asFloat(bytesX);
      float y = asFloat(bytesY);
      //float z = asFloat(bytesZ);
      //float w = asFloat(bytesW);
      if (!Float.isNaN(x) && !Float.isNaN(y)) { // && !Float.isNaN(z)) {
        PVector p = new PVector(x, y);
        points.add(p);
        println(p.x + " " + p.y);
      }
    }
    
    Stroke newStroke = new Stroke(index, c, points);

    boolean doReplace = false;
    int replaceIndex = 0;
    
    for (int i=0; i<strokesBuffer.size(); i++) {
      Stroke s = strokesBuffer.get(i);
      if (s.index == index) {
        replaceIndex = i;
        doReplace = true;
        break;
      }
    }
        
    if (doReplace) {
      strokesBuffer.set(replaceIndex, newStroke);
    } else {
      strokesBuffer.add(newStroke);
    }
  
    int time = millis();
    for (int i=0; i<strokesBuffer.size(); i++) {
      Stroke s = strokesBuffer.get(i);
      if (time > s.timestamp + s.lifespan) {
        strokesBuffer.remove(i);
      }
    }
    
    println(hostname + " " + " " + index);
    
    if (hostList.size() >= numHosts) {
      if (hostname.equals(hostList.get(0))) {
        //dot1 = new PVector(x * width, y * height);
      } else {
        //dot2 = new PVector(x * width, y * height);
      }
    } else {
      hostList.add(hostname);
    }
  }
}
