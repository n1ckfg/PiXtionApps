//import peasy.PeasyCam;

//PeasyCam cam;

float dotSize = 20;
ArrayList<StrokeMulticolor> strokesBuffer;
boolean clicked = false;
boolean isDrawing = false;
int fps = 60;
int markTime = 0;
Frame frame;

PGraphics depthGfx, rgbGfx;

void setup() {
  size(1280, 480, P3D);
  frameRate(60);
  depthGfx = createGraphics(640, 480, P3D);
  rgbGfx = createGraphics(640, 480, P2D);
  
  //cam =  new PeasyCam(this, width/2, height/2, 400, 50);
  strokesBuffer = new ArrayList<StrokeMulticolor>();
  frame = new Frame(strokesBuffer);
  oscSetup();
  fps = int((1.0/float(fps)) * 1000);
  
  float fov = PI/3.0;
  float cameraZ = (height/2.0) / tan(fov/2.0);
  depthGfx.beginDraw();
  depthGfx.perspective(fov, float(depthGfx.width)/float(depthGfx.height), cameraZ/100.0, cameraZ*100.0);
  depthGfx.endDraw();
}

void draw() {
  background(0);
  
  int time = millis();
  if (time > markTime + fps) {
    markTime = time;
    frame = new Frame(strokesBuffer);
    //strokesBuffer = new ArrayList<Stroke>();
  }
  
  depthGfx.beginDraw();
  depthGfx.background(0,0,50);    
  frame.draw(depthGfx, false, 4);
  depthGfx.endDraw();

  rgbGfx.beginDraw();
  rgbGfx.background(50,0,0);
  frame.draw(rgbGfx, true, 1);
  rgbGfx.endDraw();
  
  image(rgbGfx, 0, 0);
  image(depthGfx, 640, 0);
  
  surface.setTitle("" + frameRate);
}
