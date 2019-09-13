class StrokeMulticolor {

  ArrayList<PVector> points;
  ArrayList col;
  int index;
  int timestamp;
  int lifespan = 10000;
  
  StrokeMulticolor(int idx, ArrayList c, ArrayList<PVector> pts) {
    index = idx;
    col = c;
    points = pts;
    timestamp = millis();
  }
  
  void draw(PGraphics gfx, boolean flat, int weight) {
    gfx.noFill();    
    gfx.strokeWeight(weight);
    
    gfx.beginShape(POINTS);
    for (int i=0; i<points.size(); i++) {
      PVector p = points.get(i);
      color c = color((int) col.get(i));
      gfx.stroke(c);
      if (flat) {
        gfx.vertex(i, index);
      } else {
        gfx.vertex(p.x, p.y, p.z);
      }
    }
    gfx.endShape();
  }
}
