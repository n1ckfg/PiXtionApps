import peasy.PeasyCam;

PeasyCam cam;

float dotSize = 20;
ArrayList<Stroke> strokesBuffer;
boolean clicked = false;
boolean isDrawing = false;
int fps = 60;
int markTime = 0;
Frame frame;

void setup() {
  size(640, 480, P3D);
  frameRate(60);
  cam =  new PeasyCam(this, width/2, height/2, 400, 50);
  strokesBuffer = new ArrayList<Stroke>();
  frame = new Frame(strokesBuffer);
  oscSetup();
  fps = int((1.0/float(fps)) * 1000);
}

void draw() {
  background(0);
  
  int time = millis();
  if (time > markTime + fps) {
    markTime = time;
    frame = new Frame(strokesBuffer);
    //strokesBuffer = new ArrayList<Stroke>();
  }
  
  frame.draw();
  
  surface.setTitle("" + frameRate);
}
