import oscP5.*;
import netP5.*;

PVector dot1 = new PVector(0,0);
PVector dot2 = new PVector(0,0);

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
}

void oscEvent(OscMessage msg) {
  println(msg);
  if (msg.checkAddrPattern("/points") && msg.checkTypetag("sibb")) {    
    String hostname = msg.get(0).stringValue();
    int index = msg.get(1).intValue();
    byte[] readColorBytes = msg.get(2).blobValue();
    byte[] readPointsBytes = msg.get(3).blobValue();
      
    ArrayList colors = new ArrayList();
    for (int i = 0; i < readColorBytes.length; i += 12) { 
      byte[] bytesR = { readColorBytes[i], readColorBytes[i+1], readColorBytes[i+2], readColorBytes[i+3] };
      byte[] bytesG = { readColorBytes[i+4], readColorBytes[i+5], readColorBytes[i+6], readColorBytes[i+7] };
      byte[] bytesB = { readColorBytes[i+8], readColorBytes[i+9], readColorBytes[i+10], readColorBytes[i+11] };
       
      float r = asFloat(bytesR);
      float g = asFloat(bytesG);
      float b = asFloat(bytesB);
      color c = color(r, g, b);
      
      colors.add(c);
    }
 
    ArrayList<PVector> points = new ArrayList<PVector>();
    for (int i = 0; i < readPointsBytes.length; i += 12) { 
      byte[] bytesX = { readPointsBytes[i], readPointsBytes[i+1], readPointsBytes[i+2], readPointsBytes[i+3] };
      byte[] bytesY = { readPointsBytes[i+4], readPointsBytes[i+5], readPointsBytes[i+6], readPointsBytes[i+7] };
      byte[] bytesZ = { readPointsBytes[i+8], readPointsBytes[i+9], readPointsBytes[i+10], readPointsBytes[i+11] };

      float x = asFloat(bytesX);
      float y = asFloat(bytesY);
      float z = asFloat(bytesZ);    
      PVector p = new PVector(-x, -y, -z);
      
      points.add(p);
    }
    
    StrokeMulticolor newStroke = new StrokeMulticolor(index, colors, points);
    
    boolean addNewHost = true;
    for (Host host : hosts) {
      if (host.name.equals(hostname)) {
        host.frame.addStroke(newStroke);
        addNewHost = false;
      }
    }
    if (addNewHost) {
      hosts.add(new Host(hostWidth, hostHeight, hostname));
      Host host = hosts.get(hosts.size()-1);
      host.frame.addStroke(newStroke);
    }
    println("Sent line " + index + " to host " + hostname);
  }
}
