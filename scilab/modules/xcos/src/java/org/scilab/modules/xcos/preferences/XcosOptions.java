/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises -Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.preferences;

import java.awt.Font;

import org.scilab.modules.commons.ScilabGeneralPrefs;
import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;
import org.w3c.dom.Document;

/**
 * Xcos options
 */
public class XcosOptions {

    public static final String COLORSPATH = "//colors/body/desktop-colors";
    public static final String CONSOLEFONTPATH = "//fonts/body/fonts/item[@xconf-uid=\"console-font\"]";
    public static final String FONTPATH = "//fonts/body/fonts";
    public static final String DISPLAYPATH = "//console/body/display";
    public static final String KEYMAPPATH = "//general/shortcuts/body/actions/action-folder[@xconf-uid=\"xcos\"]/action";

    ;
    private static XcosOptions.Edition edition;
    private static XcosOptions.Simulation simulation;

    private static Document doc;

    @XConfAttribute
    public static class Edition {

        private Edition() {
        }

        @XConfAttribute(tag = "display", attributes = {})
        private void set() {
        }
    }

    @XConfAttribute
    public static class Simulation {

        public Font font;

        private Simulation() {
        }

        @XConfAttribute(tag = "item", attributes = { "font-face", "font-name", "font-size", "desktop" })
        private void set(String fontFace, String fontName, int fontSize, boolean desktopFont) {
            if (desktopFont) {
                this.font = ScilabGeneralPrefs.getDesktopFont();
            } else {
                this.font = new Font(fontName, Font.PLAIN, fontSize);
                int style = Font.PLAIN;
                if (fontFace.contains("bold")) {
                    style = style | Font.BOLD;
                } else if (fontFace.contains("italic")) {
                    style = style | Font.ITALIC;
                }
                if (style != Font.PLAIN) {
                    this.font = this.font.deriveFont(style);
                }
            }
        }
    }

    public static void invalidate(XcosConfiguration.Conf conf) {
        if (conf.font) {
            font = null;
            doc = null;
        }
        if (conf.color) {
            color = null;
            doc = null;
        }
        if (conf.display) {
            display = null;
            doc = null;
        }
    }

    public static final XcosOptions.ConsoleDisplay getConsoleDisplay() {
        if (display == null) {
            if (doc == null) {
                doc = XConfiguration.getXConfigurationDocument();
            }
            display = XConfiguration.get(XcosOptions.ConsoleDisplay.class, doc, DISPLAYPATH)[0];
        }

        return display;
    }

    public static final XcosOptions.ConsoleFont getConsoleFont() {
        if (font == null) {
            if (doc == null) {
                doc = XConfiguration.getXConfigurationDocument();
            }
            font = XConfiguration.get(XcosOptions.ConsoleFont.class, doc, CONSOLEFONTPATH)[0];
        }

        return font;
    }

    public static final XcosOptions.ConsoleColor getConsoleColor() {
        if (color == null) {
            if (doc == null) {
                doc = XConfiguration.getXConfigurationDocument();
            }
            color = XConfiguration.get(XcosOptions.ConsoleColor.class, doc, COLORSPATH)[0];
        }

        return color;
    }
}
