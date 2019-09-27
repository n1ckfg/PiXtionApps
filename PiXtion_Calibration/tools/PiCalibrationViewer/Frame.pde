class Frame {

  ArrayList<StrokeMulticolor> strokes;
  int w, h, timestamp;
  PGraphics depth, rgb;
  PVector[] depthMap;
  
  Frame(int _w, int _h) {
    strokes = new ArrayList<StrokeMulticolor>();
    init(_w, _h);
  }

  Frame(int _w, int _h, ArrayList<StrokeMulticolor> _strokes) {
    strokes = _strokes;
    init(_w, _h);
  }

  void init(int _w, int _h) {
    w = _w;
    h = _h;
    timestamp = millis();
    depthMap = new PVector[w * h];
    depth = createGraphics(w, h, P3D);
    rgb = createGraphics(w, h, P2D);
    
    float fov = PI/3.0;
    float cameraZ = (h/2.0) / tan(fov/2.0);
    depth.beginDraw();
    depth.perspective(fov, float(depth.width)/float(depth.height), cameraZ/100.0, cameraZ*100.0);
    depth.endDraw();
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
    
    for (int x=0; x<w; x++) {
      int loc = x + newStroke.index * w;
      depthMap[loc] = newStroke.points.get(x);
    }
    
    int time = millis();
    for (int i=0; i<strokes.size(); i++) {
      StrokeMulticolor s = strokes.get(i);
      if (time > s.timestamp + s.lifespan) {
        strokes.remove(i);
      }
    }
  }

  void update(PGraphics gfx, boolean flat, int weight) {      
    for (int i=0; i<strokes.size(); i++) {
      strokes.get(i).draw(gfx, flat, weight);
    }
  }
  
  void updateRgb() {
    rgb.beginDraw();
    rgb.background(50,0,0);
    update(rgb, true, 1);
    rgb.endDraw();
  }
  
  void updateDepth() {
    depth.beginDraw();
    depth.background(0,0,50);   
    update(depth, false, 4);
    depth.endDraw();
  }
  
}
