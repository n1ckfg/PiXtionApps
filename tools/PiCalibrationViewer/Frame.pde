class Frame {
  
  PShape shp;
  boolean ready;
   
  Frame() {
    ready = false;
  }
  
  void draw() {      
    if (ready) shape(shp, 0, 0);
  }
  
  void init(PImage _rgb, ArrayList<PVector> _points) {
    _rgb.loadPixels();
    
    shp = createShape();
    shp.beginShape(POINTS);
    for (int x = 0; x < 320; x++) {
      for (int y = 0; y < 240; y++) {
        int loc = x + y * 320;
        color c = _rgb.pixels[loc];
        PVector p = _points.get(loc);
        stroke(255);
        vertex(p.x * width, p.y * height, p.z);
      }
    }
    shp.endShape();
    ready = true;
  }
  
}
