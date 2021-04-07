class Stroke {

  ArrayList<PVector> points;
  int index;
  int timestamp;
  int lifespan = 1000;
  color col;
  
  Stroke() {
    index = 0;
    col = color(255);
    points = new ArrayList<PVector>();
    timestamp = millis();
  }
  
  Stroke(int idx) {
    index = idx;
    col = color(255);
    points = new ArrayList<PVector>();
    timestamp = millis();
  }
  
  Stroke(int idx, ArrayList<PVector> pts) {
    index = idx;
    col = color(255);
    points = pts;
    timestamp = millis();
  }

  Stroke(int idx, color c, ArrayList<PVector> pts) {
    index = idx;
    col = c;
    points = pts;
    timestamp = millis();
  }
  
  void draw() {
    noFill();
    stroke(col);
    strokeWeight(4);
    beginShape();
    for (PVector p : points) {
      vertex(p.x, p.y, p.z);
    }
    endShape();
  }
}
