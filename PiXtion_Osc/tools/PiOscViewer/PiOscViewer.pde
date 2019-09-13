PImage depth, rgb;

void setup() {
  size(1280, 480, P2D);
  depth = createImage(width/2, height, RGB);
  rgb = createImage(width/2, height, RGB);
  
  oscSetup();
}

void draw() {
  background(0);
  image(depth, 0, 0);
  image(rgb, width/2, 0);
  
  surface.setTitle("" + frameRate);  
}
