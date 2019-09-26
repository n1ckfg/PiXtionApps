import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipOutputStream;
import java.util.zip.Inflater;
import java.util.zip.Deflater;
import java.util.Enumeration;

// FILE METHODS
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
      e.printStackTrace();
    }
  }
}
 
// MEMORY METHODS
// https://stackoverflow.com/questions/12521522/java-util-zip-dataformatexception-unknown-compression-method-at-java-util-zip-i
// https://www.experts-exchange.com/questions/23662007/How-do-I-unzip-and-read-a-file-in-memory-using-java.html
byte[] readZipFromMemory(byte[] input) { // ZLIB, NOT GZIP
  try {
    Inflater decompressor = new Inflater();
    decompressor.setInput(input);
   
    ByteArrayOutputStream bos = new ByteArrayOutputStream(input.length);
   
    byte[] buf = new byte[1024];
    while (!decompressor.finished()) {
      int count = decompressor.inflate(buf);
      bos.write(buf, 0, count);
    }
    bos.close();
    return bos.toByteArray();
  } catch (Exception e) {
    e.printStackTrace();
    return null;
  }
}

// http://www.java2s.com/Tutorial/Java/0180__File/CompressaByteArray.htm
byte[] writeZipToMemory(byte[] input) { // ZLIB, NOT GZIP
  try {
    Deflater compressor = new Deflater();
    compressor.setLevel(Deflater.BEST_COMPRESSION);
  
    compressor.setInput(input);
    compressor.finish();
  
    ByteArrayOutputStream bos = new ByteArrayOutputStream(input.length);
  
    byte[] buf = new byte[1024];
    while (!compressor.finished()) {
      int count = compressor.deflate(buf);
      bos.write(buf, 0, count);
    }
    bos.close();
    return bos.toByteArray();
  } catch (Exception e) {
    e.printStackTrace();
    return null;
  }
}

// https://blogs.sap.com/2012/12/11/zipping-byte-array-in-memory-java-snippet/
byte[] writeZipToMemoryAlt(byte[] input) {
  try {
    ByteArrayOutputStream bos = new ByteArrayOutputStream(); 
    ZipOutputStream zipfile = new ZipOutputStream(bos);
    ZipEntry zipentry = new ZipEntry("packedFileName");
  
    zipfile.putNextEntry(zipentry);
    zipfile.write(input); // or .getBytes() for other data types
    zipfile.close();
    return bos.toByteArray();
  } catch(IOException e) {
    e.printStackTrace();
    return null;
  }
}
