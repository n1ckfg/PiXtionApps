import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.IOException;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipOutputStream;
import java.util.Enumeration;

String readZip(String fileName) {
  try {
    String url = new File(dataPath(""), fileName).toString();
    ZipFile zipFile = new ZipFile(url);
  
    InputStream stream = zipFile.getInputStream(zipFile.getEntry(getFileNameNoExt(fileName) + ".json"));

    String newLine = System.getProperty("line.separator");
    BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
    StringBuilder result = new StringBuilder();
    boolean flag = false;
    for (String line; (line = reader.readLine()) != null; ) {
      result.append(flag? newLine: "").append(line);
      flag = true;
    }
    zipFile.close();
    return result.toString();  
  } catch (Exception e) {
    println(e);
    return "";
  }
}

void writeZip(String fileName, ArrayList<String> s) {
  String url = sketchPath("") + fileName;
  
  if (getExtFromFileName(fileName).equals("json")) {
    saveStrings(url, s.toArray(new String[s.size()]));
  } else {      
    try {
      File f = new File(url);
      ZipOutputStream out = new ZipOutputStream(new FileOutputStream(f));
      ZipEntry e = new ZipEntry(getFileNameNoExt(fileName) + ".json");
      out.putNextEntry(e);
      
      byte[] data = String.join("\n", s.toArray(new String[s.size()])).getBytes();
      out.write(data, 0, data.length);
      out.closeEntry();
      
      out.close();
    } catch (Exception e) {
      //
    }
  }
}
 
