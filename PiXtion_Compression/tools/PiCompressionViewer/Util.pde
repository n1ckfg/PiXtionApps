import java.util.regex.Pattern;

boolean hitDetect3D(PVector p1, PVector p2, float s) { 
  if (PVector.dist(p1, p2) < s) {
    return true;
  } else {
    return false;
  }
}
 
float rounder(float _val, float _places){
  _val *= pow(10,_places);
  _val = round(_val);
  _val /= pow(10,_places);
  return _val;
}
  
String getFileNameNoExt(String s) {
  String returns = "";
  String[] temp = s.split(Pattern.quote("."));
  if (temp.length > 1) {
    for (int i=0; i<temp.length-1; i++) {
      if (i > 0) returns += ".";
      returns += temp[i];
    }
  } else {
    return s;
  }
  return returns;
}

String getExtFromFileName(String s) {
  String returns = "";
  String[] temp = s.split(Pattern.quote("."));
  returns = temp[temp.length-1];
  return returns;
}

void fixClipping() {
  float fov = PI/3.0;
  float cameraZ = (height/2.0) / tan(fov/2.0);
  perspective(fov, float(width)/float(height), cameraZ/100.0, cameraZ*100.0);
}
