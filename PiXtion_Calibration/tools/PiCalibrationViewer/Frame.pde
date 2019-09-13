class Frame {

  ArrayList<StrokeMulticolor> strokes;
  int timestamp;

  Frame() {
    strokes = new ArrayList<StrokeMulticolor>();
    timestamp = millis();
  }

  Frame(ArrayList<StrokeMulticolor> _strokes) {
    strokes = _strokes;
    timestamp = millis();
  }

  void addStroke(StrokeMulticolor newStroke) {
    int index = newStroke.index;    
    boolean doReplace = false;
    int replaceIndex = 0;

    for (int i=0; i<strokes.size(); i++) {
      StrokeMulticolor s = strokes.get(i);
      if (s.index == index) {
        replaceIndex = i;
        doReplace = true;
        break;
      }
    }

    if (doReplace) {
      strokes.set(replaceIndex, newStroke);
    } else {
      strokes.add(newStroke);
    }

    int time = millis();
    for (int i=0; i<strokes.size(); i++) {
      StrokeMulticolor s = strokes.get(i);
      if (time > s.timestamp + s.lifespan) {
        strokes.remove(i);
      }
    }
  }

  void draw(PGraphics gfx, boolean flat, int weight) {      
    for (int i=0; i<strokes.size(); i++) {
      strokes.get(i).draw(gfx, flat, weight);
    }
  }
}
