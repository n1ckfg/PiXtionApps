class Frame {
  
  PShape shp;
  boolean ready;
  boolean firstRun;
   
  Frame() {
    ready = false;
    firstRun = true;
  }
  
  void draw() {      
    strokeWeight(20);
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
        PVector p = _points.get(loc).mult(10.1);
        shp.stroke(255);        
        shp.vertex(p.x, p.y, p.z);
      }
    }
    shp.endShape();
    ready = true;
    firstRun = false;
  }
  
}
