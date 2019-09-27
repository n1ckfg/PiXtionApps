class Host {

  Frame frame;
  
  String name;
  int w, h;
  
  Host(int _w, int _h, String _name) {
    w = _w;
    h = _h;
    name = _name;
       
    frame = new Frame(w, h);   
  }
    
  void update() {
    frame.updateDepth();
    frame.updateRgb();
  }
  
  void draw() {   
    image(frame.rgb, 0, 0);
    image(frame.depth, w, 0);
  }
 
  void run() {
    update();
    draw();
  }
  
}
