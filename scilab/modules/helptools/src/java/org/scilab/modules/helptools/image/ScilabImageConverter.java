/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.helptools.image;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStream;
import java.util.Map;

import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.helptools.HTMLDocbookTagConverter;

/**
 * Scilab code to PNG converter
 */
public class ScilabImageConverter implements ExternalImageConverter {

    private static ScilabImageConverter instance;
    private final StringBuilder buffer;
    private final HTMLDocbookTagConverter.GenerationType type;

    private ScilabImageConverter(HTMLDocbookTagConverter.GenerationType type) {
        buffer = new StringBuilder(8192);
        this.type = type;
    }

    public String getMimeType() {
        return "image/scilab";
    }

    public static String getFileWithScilabCode() {
        if (instance.buffer.length() != 0) {
            try {
                File f = File.createTempFile("help-", ".sce", new File(ScilabCommons.getTMPDIR()));
                BufferedOutputStream out = new BufferedOutputStream(new FileOutputStream(f));
                byte[] arr = instance.buffer.toString().getBytes();
                out.write(arr, 0, arr.length);
                out.flush();
                out.close();

                return f.getAbsolutePath();
            } catch (Exception e) {
                System.err.println("Cannot generate the file with Scilab code to execute:\n" + e);
            } finally {
                instance = null;
            }
        }

        instance = null;

        return null;
    }

    /**
     * Since this a singleton class...
     * @return this
     */
    public static ScilabImageConverter getInstance(HTMLDocbookTagConverter.GenerationType type) {
        if (instance == null) {
            instance = new ScilabImageConverter(type);
        }

        return instance;
    }

    /**
     * {@inheritDoc}
     */
    public String convertToImage(String currentFile, String code, Map<String, String> attributes, File imageFile, String imageName) {
        return convertToPNG(currentFile, code, imageFile, imageName);
    }

    /**
     * {@inheritDoc}
     */
    public String convertToImage(File code, Map<String, String> attributes, File imageFile, String imageName) {
        try {
            BufferedReader in = new BufferedReader(new FileReader(code));
            StringBuilder buffer = new StringBuilder(8192);
            String line;

            while ((line = in.readLine()) != null) {
                buffer.append(line).append("\n");
            }

            in.close();

            return convertToPNG(code.getName(), buffer.toString(), imageFile, imageName);
        } catch (Exception e) {
            System.err.println("Problem when exporting Scilab code to " + imageFile + "!\n" + e.toString());
        }

        return null;
    }

    private final String convertToPNG(String currentFile, String code, File imageFile, String imageName) {
        buffer.append("__olddrv__=driver();\n");
        buffer.append("disp(\"Generate image " + imageName + " from Scilab code in file " + new File(currentFile).getName() + "\");\n");
        buffer.append("driver(\"png\");\n");
        buffer.append("xinit(\"").append(imageFile.getAbsolutePath()).append("\");\n");
        buffer.append(code).append("\n");
        buffer.append("xend();\n");
        buffer.append("driver(__olddrv__);\n");

        if (type == HTMLDocbookTagConverter.GenerationType.WEB) {
            /* Prepare the code for the html inclusion */
            code = code.trim().replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;").replace("\n", "<br />");
            /* Provide a tooltip */
            return "<div rel='tooltip' title='" + code + "'><img src=\'" + imageName + "\'/></div>";
        } else {
            /* No tooltip in the javahelp browser ...
             * too limited html capabilities */
            return "<img src=\'" + imageName + "\'/>";
        }

    }
}
