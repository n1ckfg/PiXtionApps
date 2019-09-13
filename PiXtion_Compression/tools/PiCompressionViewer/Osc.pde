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
  println(msg);
  if (msg.checkAddrPattern("/points") && msg.checkTypetag("sibb")) {    
    String hostname = msg.get(0).stringValue();
    int index = msg.get(1).intValue();
    byte[] readColorBytes = msg.get(2).blobValue();
    byte[] readPointsBytes = msg.get(3).blobValue();
    
    rgb = fromJpeg(readColorBytes);
    depth = fromJpeg(readPointsBytes);
    
    ArrayList<PVector> points = new ArrayList<PVector>();
    for (int i = 0; i < depth.pixels.length; i += 4) { 
      byte[] bytesX = { (byte) red(depth.pixels[i]), (byte) green(depth.pixels[i]), (byte) blue(depth.pixels[i]), (byte) red(depth.pixels[i+1]) };
      byte[] bytesY = { (byte) green(depth.pixels[i+1]), (byte) blue(depth.pixels[i+1]), (byte) red(depth.pixels[i+2]), (byte) green(depth.pixels[i+2]) };
      byte[] bytesZ = { (byte) blue(depth.pixels[i+2]), (byte) red(depth.pixels[i+3]), (byte) green(depth.pixels[i+3]), (byte) blue(depth.pixels[i+3]) };

      float x = asFloat(bytesX);
      float y = asFloat(bytesY);
      float z = asFloat(bytesZ);    
      PVector p = new PVector(-x, -y, -z);
      
      points.add(p);
    }
    
    StrokeMulticolor newStroke = new StrokeMulticolor(index, rgb.pixels, points);

    boolean doReplace = false;
    int replaceIndex = 0;
    
    for (int i=0; i<strokesBuffer.size(); i++) {
      StrokeMulticolor s = strokesBuffer.get(i);
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
      StrokeMulticolor s = strokesBuffer.get(i);
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
