import oscP5.*;
import netP5.*;

byte[] videoBytes1, videoBytes2;
ArrayList<String> hostList;
int numHosts = 2;

String ipNumber = "127.0.0.1";
int sendPort = 9998;
int receivePort = 7110;
int datagramSize = 100000;

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
  if (msg.checkAddrPattern("/video") && msg.checkTypetag("sb")) {
    
    String hostname = msg.get(0).stringValue();
    byte[] bytes = msg.get(1).blobValue();
    
    println(hostname + " " + bytes.length);
    
    if (hostList.size() >= numHosts) {
      if (hostname.equals(hostList.get(0))) {
        videoBytes1 = bytes;
      } else {
        videoBytes2 = bytes;
      }
    } else {
      hostList.add(hostname);
    }
  }
}
