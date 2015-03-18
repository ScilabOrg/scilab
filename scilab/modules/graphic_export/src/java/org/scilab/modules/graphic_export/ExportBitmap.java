/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_export;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Iterator;

import javax.imageio.IIOImage;
import javax.imageio.ImageIO;
import javax.imageio.ImageWriteParam;
import javax.imageio.ImageWriter;
import javax.imageio.stream.FileImageOutputStream;

/**
 * Class which allows to export screen-shots in format Bitmap (BMP, GIF, JPG, PNG and PPM)
 * @author Sylvestre Koumar
 * @author Calixte DENIZET
 *
 */
public class ExportBitmap {

    private static final int MAX_ATTEMPT = 100;
    private static final int SLEEP_TIME = 10;

    /**
     * Write the image in the file. If the file is locked (cf bug 9542), then retry.
     * @param image the image
     * @param ext the extension
     * @param file the file
     */
    public static final int writeFile(BufferedImage image, String ext, File file) {
        int ret = Export.IOEXCEPTION_ERROR;
        if (file.isFile() && file.canWrite()) {
            FileOutputStream fos;
            for (int i = 0; i < MAX_ATTEMPT && ret != Export.SUCCESS; i++) {
                try {
                    fos = new FileOutputStream(file);
                    boolean writerFound = ImageIO.write(image, ext, fos);
                    if (writerFound == false) {
                        return Export.NOWRITER_ERROR;
                    }
                    fos.close();
                    ret = Export.SUCCESS;
                } catch (FileNotFoundException e) {
                    try {
                        Thread.sleep(SLEEP_TIME);
                    } catch (InterruptedException ex) { }
                } catch (IOException e) {
                    return ret;
                }
            }
        }

        return ret;
    }

    /**
     * @param image the image to compress
     * @param compressionQuality the compression rate
     * @param file the output file
     */
    public static int writeJPEG(BufferedImage image, float compressionQuality, File file) throws IOException {
        Iterator iter = ImageIO.getImageWritersByFormatName("jpeg");
        ImageWriter writer;
        if (iter.hasNext()) {
            writer = (ImageWriter) iter.next();
        } else {
            return Export.NOWRITER_ERROR;
        }
        ImageWriteParam param = writer.getDefaultWriteParam();
        param.setCompressionMode(ImageWriteParam.MODE_EXPLICIT);
        param.setCompressionQuality(compressionQuality);
        FileImageOutputStream output = new FileImageOutputStream(file);
        writer.setOutput(output);
        try {
            writer.write(null, new IIOImage(image, null, null), param);
        } catch (IOException e) {
            return Export.NOWRITER_ERROR;
        }
        writer.dispose();
        output.close();
        return Export.SUCCESS;
    }
}
