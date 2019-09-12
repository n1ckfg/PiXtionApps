class StrokeMulticolor {

  ArrayList<PVector> points;
  ArrayList col;
  int index;
  int timestamp;
  int lifespan = 1000;
  
  StrokeMulticolor(int idx, ArrayList c, ArrayList<PVector> pts) {
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
      color c = color((int) col.get(i));
      stroke(c);
      vertex(p.x, p.y, p.z);
    }
    endShape();
  }
}
