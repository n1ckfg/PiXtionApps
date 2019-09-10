import oscP5.*;
import netP5.*;

byte[] depthVideoBytes, rgbVideoBytes;
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
  if (msg.checkAddrPattern("/video")) {
    if (msg.checkTypetag("sb") || msg.checkTypetag("sbb")) {   
      String hostname = msg.get(0).stringValue();
      depthVideoBytes = msg.get(1).blobValue();
      if (depthVideoBytes != null) depth = fromJpeg(depthVideoBytes);
    }
    
    if (msg.checkTypetag("sbb")) {
      rgbVideoBytes = msg.get(2).blobValue();
      if (rgbVideoBytes != null) rgb = fromJpeg(rgbVideoBytes);
    }
  }
}
