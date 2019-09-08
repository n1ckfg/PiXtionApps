PImage img;

void setup() {
  size(1280, 480, P2D);
  img = createImage(640, 240, RGB);
  
  oscSetup();
}

void draw() {
  background(0);
  image(img, 0, 0);
  
  surface.setTitle("" + frameRate);  
}
