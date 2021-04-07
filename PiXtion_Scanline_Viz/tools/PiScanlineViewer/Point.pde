class Point {
  
  PVector pos;
  PVector col;
  
  Point() {
    pos = new PVector(0,0,0);
    col = new PVector(255, 255, 255);
  }
  
  Point(PVector p) {
    pos = p;
    col = new PVector(255, 255, 255);
  }
  
  Point(PVector p, PVector c) {
    pos = p;
    col = c;
  }
  
}
