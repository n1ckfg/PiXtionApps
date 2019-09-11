import peasy.PeasyCam;

PeasyCam cam;

float dotSize = 20;
boolean clicked = false;
boolean isDrawing = false;
int markTime = 0;
Frame frame;

void setup() {
  size(640, 480, P3D);
  frameRate(60);
  cam =  new PeasyCam(this, width/2, height/2, 400, 50);
  frame = new Frame();
  oscSetup();
  
  float fov = PI/3.0;
  float cameraZ = (height/2.0) / tan(fov/2.0);
  perspective(fov, float(width)/float(height), cameraZ/100.0, cameraZ*100.0);

}

void draw() {
  background(0);
  
  frame.draw();
  image(rgb, 0, 0);
  image(depth, 320, 0);
  
  surface.setTitle("" + frameRate);
}
