PImage img;

void setup() {
  size(1280, 480, P2D);
  img = createImage(1280, 480, RGB);
  
  oscSetup();
}

void draw() {
  background(0);
  if (videoBytes != null) img = fromJpeg(videoBytes);
  image(img, 0, 0);
  
  surface.setTitle("" + frameRate);  
}
