PImage depthImg, rgbImg;
PGraphics gfx;

void setup() {
  size(1280, 480, P2D);
  rgbImg = createImage(640, 480, RGB);
  depthImg = createImage(640, 480, RGB);
  gfx = createGraphics(1280, 480, P2D);
  
  oscSetup();
}

void draw() {
  background(0);
  if (videoBytes != null) depthImg = fromJpeg(videoBytes);
  gfx.beginDraw();
  gfx.background(0);
  gfx.image(depthImg, 0, 0);
  gfx.endDraw();
  
  image(gfx, 0, 0);
  
  surface.setTitle("" + frameRate);  
}
