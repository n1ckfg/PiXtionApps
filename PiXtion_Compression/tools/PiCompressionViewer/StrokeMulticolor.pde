class StrokeMulticolor {

  ArrayList<PVector> points;
  color[] col;
  int index;
  int timestamp;
  int lifespan = 1000;
  
  StrokeMulticolor(int idx, color[] c, ArrayList<PVector> pts) {
    index = idx;
    col = c;
    points = pts;
    timestamp = millis();
  }
  
  void draw() {
    noFill();
    strokeWeight(4);
    beginShape(POINTS);
    for (int i=0; i<points.size(); i++) {
      PVector p = points.get(i);
      color c = col[i];
      stroke(c);
      vertex(p.x, p.y, p.z);
    }
    endShape();
  }
}
