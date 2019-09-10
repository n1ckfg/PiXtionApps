// https://www.processing.org/discourse/beta/num_1215762729.html
// https://forum.processing.org/one/topic/converting-bufferedimage-to-pimage.html

import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import javax.imageio.ImageIO;

PImage fromJpeg(byte[] jpegBytes) {
  BufferedImage bImg = null;
  ByteArrayInputStream bis= new ByteArrayInputStream(jpegBytes);

  try {
    bImg = ImageIO.read(bis);
  } catch (Exception e) {
    //
  } finally {
    try {
      bis.close();
    } catch (Exception e) {
      //
    }
  }

  return bImageToPImage(bImg);
}

PImage bImageToPImage(BufferedImage bImg) {
  PImage img=new PImage(bImg.getWidth(), bImg.getHeight(), PConstants.ARGB);
  bImg.getRGB(0, 0, img.width, img.height, img.pixels, 0, img.width);
  img.updatePixels();
  return img;
}
