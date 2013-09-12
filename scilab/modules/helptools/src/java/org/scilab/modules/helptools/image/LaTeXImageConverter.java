/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.helptools.image;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.Map;

import org.scilab.forge.jlatexmath.TeXConstants;
import org.scilab.forge.jlatexmath.TeXFormula;
import org.scilab.forge.jlatexmath.TeXIcon;
import org.scilab.forge.jlatexmath.ParseException;
import org.scilab.modules.helptools.DocbookTagConverter;

/**
 * A LaTeX to PNG converter
 * @author Calixte DENIZET
 */
public class LaTeXImageConverter implements ExternalImageConverter {

    private final DocbookTagConverter conv;

    public LaTeXImageConverter(DocbookTagConverter conv) {
        this.conv = conv;
    }

    /**
     * {@inheritDoc}
     */
    public String getMimeType() {
        return "image/latex";
    }

    /**
     * {@inheritDoc}
     */
    public boolean mustRegenerate() {
        return true;
    }

    /**
     * {@inheritDoc}
     */
    public String convertToImage(String currentFile, String latex, Map<String, String> attributes, File imageFile, String imageName) {
        TeXIcon icon = null;
        try {
            TeXFormula formula = new TeXFormula(latex);
            String display = attributes.get("style");
            int style = TeXConstants.STYLE_DISPLAY;
            if (display != null && display.equals("text")) {
                style = TeXConstants.STYLE_TEXT;
            }
            String fs = attributes.get("fontsize");
            int ifs = 16;
            if (fs != null) {
                try {
                    ifs = Integer.parseInt(fs);
                } catch (Exception e) { }
            }

            icon = formula.createTeXIcon(style, ifs);

            Image img = new Image(icon, icon.getIconWidth(), icon.getIconHeight(), icon.getIconHeight() - icon.getIconDepth(), icon.getIconDepth());
            if (img != null && ImageConverter.convertIconToPNG(img.icon, imageFile)) {
                return conv.generateImageCode(img, conv.getBaseImagePath() + imageName, attributes);
            }

        } catch (ParseException e) {
            System.err.println("LaTeX code in\n" + currentFile + "\nhas generated an error: " + e.getMessage());
        }
        return null;
    }

    /**
     * {@inheritDoc}
     */
    public String convertToImage(File latex, Map<String, String> attributes, File imageFile, String imageName) {
        BufferedReader input = null;
        StringBuilder buffer = new StringBuilder();
        boolean ok = false;
        try {
            input = new BufferedReader(new FileReader(latex));
            String line = null;
            while ((line = input.readLine()) != null) {
                buffer.append(line);
                buffer.append("\n");
            }
            ok = true;
        } catch (IOException e) {
            System.err.println(e);
        }
        if (input != null) {
            try {
                input.close();
            } catch (IOException e) {
                System.err.println(e);
            }
        }

        if (ok) {
            return convertToImage(latex.getName(), buffer.toString(), attributes, imageFile, imageName);
        }

        return null;
    }
}