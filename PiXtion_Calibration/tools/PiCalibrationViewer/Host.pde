class Host {

  Frame frame;
  PGraphics depthGfx, rgbGfx;
  String name;
  int w, h;
  
  Host(int _w, int _h, String _name) {
    w = _w;
    h = _h;
    name = _name;
    
    depthGfx = createGraphics(w, h, P3D);
    rgbGfx = createGraphics(w, h, P2D);
    
    frame = new Frame();
    
    float fov = PI/3.0;
    float cameraZ = (h/2.0) / tan(fov/2.0);
    depthGfx.beginDraw();
    depthGfx.perspective(fov, float(depthGfx.width)/float(depthGfx.height), cameraZ/100.0, cameraZ*100.0);
    depthGfx.endDraw();
  }
  
  void draw() {
    depthGfx.beginDraw();
    depthGfx.background(0,0,50);    
    frame.draw(depthGfx, false, 4);
    depthGfx.endDraw();
  
    rgbGfx.beginDraw();
    rgbGfx.background(50,0,0);
    frame.draw(rgbGfx, true, 1);
    rgbGfx.endDraw();
    
    image(rgbGfx, 0, 0);
    image(depthGfx, w, 0);
  }
  
}
