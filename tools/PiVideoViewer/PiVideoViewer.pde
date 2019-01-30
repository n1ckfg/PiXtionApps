PImage img1, img2;

void setup() {
  size(1280, 480, P2D);
  img1 = createImage(640, 480, RGB);
  img2 = createImage(640, 480, RGB);
  
  oscSetup();
}

void draw() {
  background(0);
  if (videoBytes1 != null) img1 = fromJpeg(videoBytes1);
  if (videoBytes2 != null) img2 = fromJpeg(videoBytes2);
  image(img1, 0, 0);
  image(img2, width/2, 0);
  
  surface.setTitle("" + frameRate);  
}
