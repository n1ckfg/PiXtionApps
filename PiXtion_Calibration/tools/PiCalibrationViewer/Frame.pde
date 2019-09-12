class Frame {

  ArrayList<StrokeMulticolor> strokes;
  int timestamp;
  
  Frame(ArrayList<StrokeMulticolor> _strokes) {
    strokes = _strokes;
    timestamp = millis();
  }
  
  void draw() {      
    for (int i=0; i<strokes.size(); i++) {
      strokes.get(i).draw();
    }
  }
}
