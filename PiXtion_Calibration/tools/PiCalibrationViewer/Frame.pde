class Frame {

  ArrayList<StrokeMulticolor> strokes;
  int timestamp;
  
  Frame(ArrayList<StrokeMulticolor> _strokes) {
    strokes = _strokes;
    timestamp = millis();
  }
  
  void draw(PGraphics gfx, boolean flat, int weight) {      
    for (int i=0; i<strokes.size(); i++) {
      strokes.get(i).draw(gfx, flat, weight);
    }
  }
}
