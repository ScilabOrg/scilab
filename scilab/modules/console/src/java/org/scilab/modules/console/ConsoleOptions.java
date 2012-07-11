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

package org.scilab.modules.console;

import java.awt.Color;
import java.awt.Font;

import org.w3c.dom.Document;

import static org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;
import org.scilab.modules.commons.xml.XConfiguration;

/**
 * Console options
 * @author Calixte DENIZET
 */
public class ConsoleOptions {

    public static final String COLORSPATH = "//colors/body/desktop-colors";
    public static final String FONTPATH = "//fonts/body/fonts";
    public static final String DISPLAYPATH = "//console/body/display";
    public static final String KEYMAPPATH = "//general/shortcuts/body/actions/action-folder[@xconf-uid=\"console\"]/action";

    private static ConsoleOptions.ConsoleColor color;
    private static ConsoleOptions.ConsoleFont font;
    private static ConsoleOptions.ConsoleDisplay display;

    private static Document doc;

    @XConfAttribute
    public static class ConsoleDisplay {

        public int maxOutputLines;
        public int nbLines;
        public int nbColumns;
        public boolean adaptToDisplay;
        public boolean wrapLines;

        private ConsoleDisplay() { }

        @XConfAttribute(tag = "display", attributes = {"scroll-size", "lines-to-display", "columns-to-display", "adapt-to-display", "wrap-lines"})
        private void set(int maxOutputLines, int nbLines, int nbColumns, boolean adaptToDisplay, boolean wrapLines) {
            this.maxOutputLines = maxOutputLines;
            this.nbLines = nbLines;
            this.nbColumns = nbColumns;
            this.adaptToDisplay = adaptToDisplay;
            this.wrapLines = wrapLines;
        }
    }

    @XConfAttribute
    public static class ConsoleColor {

        public Color background;
        public Color cursor;
        public Color foreground;

        private ConsoleColor() { }

        @XConfAttribute(tag = "desktop-colors", attributes = {"background", "cursor", "text", "use-system-color"})
        private void set(Color background, Color cursor, Color foreground, boolean useSystemColor) {
            if (useSystemColor) {
                this.background = Color.WHITE;
                this.cursor = Color.BLACK;
                this.foreground = Color.BLACK;
            } else {
                this.background = background;
                this.cursor = cursor;
                this.foreground = foreground;
            }
        }
    }

    @XConfAttribute
    public static class ConsoleFont {

        public Font font;

        private ConsoleFont() { }

        @XConfAttribute(tag = "fonts", attributes = {"font-face", "font-name", "font-size", "system"})
        private void set(String fontFace, String fontName, int fontSize, boolean useSystemFont) {
            if (useSystemFont) {
                this.font = new Font("monospaced", Font.PLAIN, 13);
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

    public static void invalidate(ConsoleConfiguration.Conf conf) {
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

    public static final ConsoleOptions.ConsoleDisplay getConsoleDisplay() {
        if (display == null) {
            if (doc == null) {
                doc = XConfiguration.getXConfigurationDocument();
            }
            display = XConfiguration.get(ConsoleOptions.ConsoleDisplay.class, doc, DISPLAYPATH)[0];
        }

        return display;
    }

    public static final ConsoleOptions.ConsoleFont getConsoleFont() {
        if (font == null) {
            if (doc == null) {
                doc = XConfiguration.getXConfigurationDocument();
            }
            font = XConfiguration.get(ConsoleOptions.ConsoleFont.class, doc, FONTPATH)[0];
        }

        return font;
    }

    public static final ConsoleOptions.ConsoleColor getConsoleColor() {
        if (color == null) {
            if (doc == null) {
                doc = XConfiguration.getXConfigurationDocument();
            }
            color = XConfiguration.get(ConsoleOptions.ConsoleColor.class, doc, COLORSPATH)[0];
        }

        return color;
    }
}
