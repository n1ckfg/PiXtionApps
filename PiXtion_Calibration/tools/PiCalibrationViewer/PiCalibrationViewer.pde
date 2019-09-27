//import peasy.PeasyCam;

//PeasyCam cam;

ArrayList<Host> hosts;
int hostWidth = 320;
int hostHeight = 240;

void setup() {
  size(640, 480, P3D);
  frameRate(60);
  oscSetup();
  hosts = new ArrayList<Host>();
}

void draw() {
  background(0);
  
  for (int i=0; i<hosts.size(); i++) {
    pushMatrix();
    translate(0, i * hostHeight);
    hosts.get(i).run();
    popMatrix();
  }

  
  surface.setTitle("" + frameRate);
}
