/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2009 - INRIA - Allan SIMON
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.xpad.utils;

import java.awt.Color;
import java.awt.Font;
import java.awt.Toolkit;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.List;
import java.util.Hashtable;
import java.util.Properties;
import java.util.Enumeration;
import java.util.Map;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.TransformerFactoryConfigurationError;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import javax.swing.KeyStroke;

import org.scilab.modules.console.GuiManagement;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

import org.scilab.modules.xpad.ScilabView;
import org.scilab.modules.xpad.TabManager;
import org.scilab.modules.xpad.MatchingBlockManager;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;


public final class ConfigXpadManager {
    private static final int BUFSIZE = 1024;

    private static final int MARGIN = 20;

    private static final String ERROR_READ = "Could not load file: ";
    private static final String ERROR_WRITE = "Could not save file: ";
    private static final String VALUE = "value";
    private static final String STYLE = "style";
    private static final String UNDERLINE = "underline";
    private static final String STROKE = "stroke";
    private static final String FONT_SIZE = "FontSize";
    private static final String FONT_STYLE = "FontStyle";
    private static final String FONT_NAME = "FontName";
    private static final String DEFAULT = "default";
    private static final String WIDTH = "width";
    private static final String HEIGHT = "height";
    private static final String XCOORD = "x";
    private static final String YCOORD = "y";
    private static final String MAINWINPOSITION = "MainWindowPosition";
    private static final String MAINWINSIZE = "MainWindowSize";
    private static final String AUTOINDENT = "AutoIndent";
    private static final String AUTOCOLORIZE = "AutoColorize";
    private static final String DEFAULTENCONDING = "DefaultEncoding";
    private static final String LINEHIGHLIGHTER = "LineHighlighter";
    private static final String HELPONTYPING = "HelpOnTyping";
    private static final String LINENUMBERING = "LineNumbering";

    private static final String FOREGROUNDCOLOR = "ForegroundColor";
    private static final String BACKGROUNDCOLOR = "BackgroundColor";
    private static final String LINECOLOR = "linecolor";
    private static final String CONTOURCOLOR = "contourcolor";
    private static final String COLORPREFIX = "#";

    private static final String NAME = "name";
    private static final String NULL = "null";

    private static final String PROFILE = "Profile";

    private static String RECENT_SEARCH = "recentSearch";
    private static String SEARCH = "search";
    private static String RECENT_REPLACE = "recentReplace";
    private static String REPLACE = "replace";
    private static String EXPRESSION = "exp";
    private static String REGULAR_EXPRESION = "regularExp";
    private static String WORD_WARP = "wordWarp";
    private static String WHOLE_WORD = "wholeWord";
    private static String CASE_SENSITIVE = "caseSensitive";
    private static String STATE_FLAG = "state";

    private static String SETTING = "Setting";
    private static String XPAD = "xpad";

    private static final String XPAD_CONFIG_FILE = System.getenv("SCI") + "/modules/xpad/etc/xpadConfiguration.xml";
    private static final String XPAD_CONFIG_KEYS_FILE = System.getenv("SCI") + "/modules/xpad/etc/keysConfiguration.xml";

    private static final String USER_XPAD_CONFIG_FILE = GuiManagement.getSCIHOME() + "/xpadConfiguration.xml";
    private static final String USER_XPAD_CONFIG_KEYS_FILE = GuiManagement.getSCIHOME() + "/keysConfiguration.xml";

    private static final int PLAIN = 0;
    private static final int BOLD =  1;
    private static final int ITALIC = 2;
    private static final int BOLDITALIC = 3;

    private static final int DEFAULT_WIDTH = 650;
    private static final int DEFAULT_HEIGHT = 550;

    private static final int MAX_RECENT_FILES = 10;
    private static final int MAX_RECENT_SEARCH = 20;
    private static final int MAX_RECENT_REPLACE = 20;

    private static final String packActionName = "org.scilab.modules.xpad.actions";

    private static Document document;
    private static Properties keysMap;

    /**
     * Constructor
     */
    private ConfigXpadManager() {
        throw new UnsupportedOperationException();
    }

    /**
     * Create a copy of Scilab configuration file in the user directory
     */
    public static void createUserCopy() {
        File fileConfig = new File(USER_XPAD_CONFIG_FILE);
        if (!fileConfig.exists() || (fileConfig.length() == 0)) {
            /* Create a local copy of the configuration file */
            copyFile(new File(XPAD_CONFIG_FILE), new File(USER_XPAD_CONFIG_FILE));
        }
        fileConfig = new File(USER_XPAD_CONFIG_KEYS_FILE);
        if (!fileConfig.exists() || (fileConfig.length() == 0)) {
            /* Create a local copy of the configuration file */
            copyFile(new File(XPAD_CONFIG_KEYS_FILE), new File(USER_XPAD_CONFIG_KEYS_FILE));
        }
    }
    /**
     * Get the name of the user configuration file
     * @return the name of the configuration file
     */
    public static String getUserConfigFile() {
        return USER_XPAD_CONFIG_FILE;
    }

    /**
     * Get all Style name
     * @return a array list of all style name
     */

    public static List<String> getAllStyleName() {
        List<String> stylesName = new ArrayList<String>();

        readDocument();

        Element root = document.getDocumentElement();
        NodeList styles = root.getElementsByTagName(STYLE);

        for (int i = 0; i < styles.getLength(); ++i) {
            Element style = (Element) styles.item(i);

            stylesName.add(style.getAttribute(NAME));


        }
        return stylesName;
    }

    /**
     * Get the font name
     * @return the name of the font
     */
    public static String getFontName() {

        /*load file*/
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList fontNameElement = xpadProfile.getElementsByTagName(FONT_NAME);
        Element fontName = (Element) fontNameElement.item(0);
        return fontName.getAttribute(VALUE);

    }

    /**
     * Get the font size
     * @return the font size
     */
    public static int getFontSize() {
        /*load file*/
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList fontSizeElement = xpadProfile.getElementsByTagName(FONT_SIZE);
        Element fontSize = (Element) fontSizeElement.item(0);
        return Integer.parseInt(fontSize.getAttribute(VALUE));

    }

    /**
     * @return true if help on typing is active
     */
    public static boolean getHelpOnTypingState() {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allSizeElements = xpadProfile.getElementsByTagName(HELPONTYPING);
        Element helpontyping = (Element) allSizeElements.item(0);

        return "true".equals(helpontyping.getAttribute(VALUE));
    }

    /**
     * Save help on typing
     * @param boolean active or not
     */
    public static void saveHelpOnTypingState(boolean activated) {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allSizeElements = xpadProfile.getElementsByTagName(HELPONTYPING);
        Element helpOnTyping = (Element) allSizeElements.item(0);
        if (helpOnTyping == null){
            Element help = document.createElement(HELPONTYPING);
            helpOnTyping.setAttribute(VALUE, new Boolean(activated).toString());
            helpOnTyping.appendChild((Node) help);
        } else {
            helpOnTyping.setAttribute(VALUE, new Boolean(activated).toString());
        }
        writeDocument();
    }

    /**
     * @return the default state
     */
    public static int getLineNumberingState() {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allSizeElements = xpadProfile.getElementsByTagName(LINENUMBERING);
        Element lineNumbering = (Element) allSizeElements.item(0);

        return Integer.parseInt(lineNumbering.getAttribute(VALUE));
    }

    /**
     * Save line numbering state
     * @param state the state
     */
    public static void saveLineNumberingState(int state) {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allSizeElements = xpadProfile.getElementsByTagName(LINENUMBERING);
        Element lineNumbering = (Element) allSizeElements.item(0);
        if (lineNumbering == null){
            Element line = document.createElement(LINENUMBERING);
            lineNumbering.setAttribute(VALUE, Integer.toString(state));
            lineNumbering.appendChild((Node) line);
        } else {
            lineNumbering.setAttribute(VALUE, Integer.toString(state));
        }
        writeDocument();
    }

    /**
     * @return true if highlighted line is active
     */
    public static boolean getHighlightState() {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allSizeElements = xpadProfile.getElementsByTagName(LINEHIGHLIGHTER);
        Element lineHighlight = (Element) allSizeElements.item(0);

        return "true".equals(lineHighlight.getAttribute(VALUE));
    }

    /**
     * Save highlight state
     * @param state the state
     */
    public static void saveHighlightState(boolean state) {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allSizeElements = xpadProfile.getElementsByTagName(LINEHIGHLIGHTER);
        Element lineHighlighter = (Element) allSizeElements.item(0);
        if (lineHighlighter == null){
            Element line = document.createElement(LINEHIGHLIGHTER);
            lineHighlighter.setAttribute(VALUE, Boolean.toString(state));
            lineHighlighter.appendChild((Node) line);
        } else {
            lineHighlighter.setAttribute(VALUE, Boolean.toString(state));
        }
        writeDocument();
    }

    /**
     * @return the color for the highlight and for the contour of the highlight
     */
    public static Color[] getHighlightColors() {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allSizeElements = xpadProfile.getElementsByTagName(LINEHIGHLIGHTER);
        Element lineHighlight = (Element) allSizeElements.item(0);
        Color[] arr = new Color[2];

        Color c;
        if (NULL.equals(lineHighlight.getAttribute(LINECOLOR))) {
            c = null;
        } else {
            c = Color.decode(lineHighlight.getAttribute(LINECOLOR));
        }

        arr[0] = c;

        if (NULL.equals(lineHighlight.getAttribute(CONTOURCOLOR))) {
            c = null;
        } else {
            c = Color.decode(lineHighlight.getAttribute(CONTOURCOLOR));
        }

        arr[1] = c;
        return arr;
    }

    /**
     * Get all font style
     * @return true if the font style is bold , false otherwise
     */
    public static Hashtable<String, Boolean> getAllisBold() {
        /*load file*/
        readDocument();
        Hashtable<String, Boolean > stylesIsBoldTable = new Hashtable<String, Boolean>();

        Element root = document.getDocumentElement();
        NodeList styles = root.getElementsByTagName(STYLE);

        for (int i = 0; i < styles.getLength(); ++i) {
            Element style = (Element) styles.item(i);


            NodeList fontStyleElement = style.getElementsByTagName(FONT_STYLE);
            Element fontStyle = (Element) fontStyleElement.item(0);
            int value = Integer.parseInt(fontStyle.getAttribute(VALUE));

            if (value  == BOLD || value == BOLDITALIC) {
                stylesIsBoldTable.put(style.getAttribute(NAME), true);
            } else {
                stylesIsBoldTable.put(style.getAttribute(NAME), false);
            }
        }
        return stylesIsBoldTable;
    }


    /**
     * Get the font setting
     * @return the font
     */
    public static Font getFont() {

        /*load file*/
        readDocument();
        Font font;
        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList fontSizeElement = xpadProfile.getElementsByTagName(FONT_SIZE);
        Element fontSize = (Element) fontSizeElement.item(0);
        int size = Integer.parseInt(fontSize.getAttribute(VALUE));

        NodeList fontNameElement = xpadProfile.getElementsByTagName(FONT_NAME);
        Element fontName = (Element) fontNameElement.item(0);
        String name = fontName.getAttribute(VALUE);

        NodeList fontStyleElement = xpadProfile.getElementsByTagName(FONT_STYLE);
        Element fontStyle = (Element) fontStyleElement.item(0);
        int style = Integer.parseInt(fontStyle.getAttribute(VALUE));

        if (style == PLAIN) {
            font = new Font(name, Font.PLAIN, size);

        } else if (style == BOLD) {
            font = new Font(name, Font.BOLD, size);

        } else if (style == ITALIC) {
            font = new Font(name, Font.ITALIC, size);

        } else if (style == BOLDITALIC) {
            font = new Font(name, Font.BOLD | Font.ITALIC , size);

        } else {
            font = new Font(name, Font.PLAIN, size);
        }

        return font;
    }

    /**
     * Get Default Font Settings
     * @return the default font
     */

    public static Font getDefaultFont() {
        /*load file*/
        readDocument();

        Font font;
        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList fontSizeElement = xpadProfile.getElementsByTagName(FONT_SIZE);
        Element fontSize = (Element) fontSizeElement.item(0);
        int size = Integer.parseInt(fontSize.getAttribute(DEFAULT));

        NodeList fontNameElement = xpadProfile.getElementsByTagName(FONT_NAME);
        Element fontName = (Element) fontNameElement.item(0);
        String name = fontName.getAttribute(DEFAULT);

        NodeList fontStyleElement = xpadProfile.getElementsByTagName(FONT_STYLE);
        Element fontStyle = (Element) fontStyleElement.item(0);
        int style = Integer.parseInt(fontStyle.getAttribute(DEFAULT));

        if (style == PLAIN) {
            font = new Font(name, Font.PLAIN, size);

        } else if (style == BOLD) {
            font = new Font(name, Font.BOLD, size);

        } else if (style == ITALIC) {
            font = new Font(name, Font.ITALIC, size);

        } else if (style == BOLDITALIC) {
            font = new Font(name, Font.BOLD | Font.ITALIC , size);

        } else {
            font = new Font(name, Font.PLAIN, size);
        }

        return font;
    }

    /**
     * Save a new font setting
     * @param font the new font
     */
    public static void saveFont(Font font) {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList fontSizeElement = xpadProfile.getElementsByTagName(FONT_SIZE);
        Element fontSize = (Element) fontSizeElement.item(0);
        fontSize.setAttribute(VALUE, Integer.toString(font.getSize()));

        NodeList fontNameElement = xpadProfile.getElementsByTagName(FONT_NAME);
        Element fontName = (Element) fontNameElement.item(0);
        fontName.setAttribute(VALUE, font.getFontName());

        NodeList fontStyleElement = xpadProfile.getElementsByTagName(FONT_STYLE);
        Element fontStyle = (Element) fontStyleElement.item(0);

        if (!font.isBold() && !font.isItalic()) {
            fontStyle.setAttribute(VALUE, "0");
        } else if (font.isBold() && font.isItalic()) {
            fontStyle.setAttribute(VALUE, "3");
        } else if (font.isBold()) {
            fontStyle.setAttribute(VALUE, "1");
        } else {
            fontStyle.setAttribute(VALUE, "2");
        }

        /* Save changes */
        writeDocument();
    }

    /**
     * Copy a file
     * @param in src file
     * @param out dest file
     */
    private static void copyFile(File in, File out) {
        FileInputStream fis = null;
        try {
            fis = new FileInputStream(in);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        FileOutputStream fos = null;
        try {
            fos = new FileOutputStream(out);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        byte[] buf = new byte[BUFSIZE];
        int i = 0;
        try {
            while ((i = fis.read(buf)) != -1) {
                fos.write(buf, 0, i);
            }
            fis.close();
            fos.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Retrieve from xpadConfiguration.xml the infos about a tabulation
     * @return a Tabulation containing infos
     */
    public static TabManager.Tabulation getDefaultTabulation() {
        /* <style name="Tabulation" rep="vertical" value="4" white="false"> */
        readDocument();

        Element root = document.getDocumentElement();
        NodeList styles = root.getElementsByTagName(STYLE);

        for (int i = 0; i < styles.getLength(); ++i) {
            Element style = (Element) styles.item(i);
            if ("Tabulation".equals(style.getAttribute(NAME))) {
                String rep = style.getAttribute("rep").toLowerCase();
                int type = ScilabView.TABNOTHING;
                char rrep = ' ';
                if ("vertical".equals(rep)) {
                    type = ScilabView.TABVERTICAL;
                } else if ("horizontal".equals(rep)) {
                    type = ScilabView.TABHORIZONTAL;
                } else if ("doublechevrons".equals(rep)) {
                    type = ScilabView.TABDOUBLECHEVRONS;
                } else if ("none".equals(rep)) {
                    type = ScilabView.TABNOTHING;
                } else if (rep.length() >= 1) {
                    type = ScilabView.TABCHARACTER;
                    rrep = rep.charAt(0);
                }

                char one;
                int value = Integer.parseInt(style.getAttribute("value"));
                String white = style.getAttribute("white").toLowerCase();
                if ("false".equals(white)) {
                    one = '\t';
                } else {
                    one = ' ';
                }

                return new TabManager.Tabulation(one, value, type, rrep);
            }
        }
        return null;
    }

    /**
     * Retrieve form xpadConfiguration.xml the infos the matchers
     * @param kind should be "KeywordsHighlighter" or "OpenCloseHighlighter"
     * @return an Object containing infos
     */
    public static MatchingBlockManager.Parameters getDefaultForMatcher(String kind) {
        /* <KeywordsHighlighter color="#fff3d2" inside="true" type="filled"/> */

        readDocument();

        Element root = document.getDocumentElement();
        NodeList matching = root.getElementsByTagName("Matching");
        Element elem = (Element) matching.item(0);
        boolean onmouseover = "true".equals(elem.getAttribute("onmouseover"));

        NodeList mat = elem.getElementsByTagName(kind);
        Element matcher = (Element) mat.item(0);

        Color color = Color.decode(matcher.getAttribute("color"));
        boolean inside = "true".equals(matcher.getAttribute("inside"));
        String stype = matcher.getAttribute("type");
        int type = 0;
        if ("filled".equals(stype)) {
            type = MatchingBlockManager.ScilabKeywordsPainter.FILLED;
        } else if ("underlined".equals(stype)) {
            type = MatchingBlockManager.ScilabKeywordsPainter.UNDERLINED;
        } else if ("framed".equals(stype)) {
            type = MatchingBlockManager.ScilabKeywordsPainter.FRAMED;
        }

        return new MatchingBlockManager.Parameters(color, inside, type, onmouseover);
    }

    /**
     * Get the background Color
     * @return the background Color
     */
    public static Color getXpadBackgroundColor() {
        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allSizeElements = xpadProfile.getElementsByTagName(BACKGROUNDCOLOR);
        Element xpadBackground = (Element) allSizeElements.item(0);

        /*direct create a Color with "#FF00FF" string from the xml */
        return Color.decode(xpadBackground.getAttribute(VALUE));
    }

    /**
     * Save Xpad BackgroundColor
     * @param color the new Color
     */
    public static void saveXpadBackground(Color color) {

        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allSizeElements = xpadProfile.getElementsByTagName(FOREGROUNDCOLOR);
        Element xpadForeground = (Element) allSizeElements.item(0);

        String rgb = Integer.toHexString(color.getRGB());
        xpadForeground.setAttribute(VALUE, COLORPREFIX + rgb.substring(2, rgb.length()));

        /* Save changes */
        writeDocument();

    }

    /**
     * Save Xpad autoIndent or not
     * @param boolean if autoIndent should be used or not
     */
    public static void saveAutoIndent(boolean activated) {

        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allSizeElements = xpadProfile.getElementsByTagName(AUTOINDENT);
        Element xpadAutoIndent = (Element) allSizeElements.item(0);
        if (xpadAutoIndent == null){
            Element autoIndent = document.createElement(AUTOINDENT);

            autoIndent.setAttribute(VALUE, new Boolean(activated).toString());

            xpadProfile.appendChild((Node) autoIndent);
        } else {
            xpadAutoIndent.setAttribute(VALUE, new Boolean(activated).toString()  );
        }
        /* Save changes */
        writeDocument();

    }


    /**
     * Save Xpad autoIndent or not
     * @param boolean if autoIndent should be used or not
     */
    public static boolean getAutoIndent() {

        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allSizeElements = xpadProfile.getElementsByTagName(AUTOINDENT);
        Element autoIndent = (Element) allSizeElements.item(0);

        if(autoIndent == null){
            return true;
        } else {
            return new Boolean(autoIndent.getAttribute(VALUE));
        }
    }


    /**
     * Save Xpad autoColorize or not
     * @param boolean if autoIndent should be used or not
     */
    public static void saveAutoColorize(boolean activated) {

        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allSizeElements = xpadProfile.getElementsByTagName(AUTOCOLORIZE);
        Element xpadAutoIndent = (Element) allSizeElements.item(0);
        if (xpadAutoIndent == null){
            Element autoColorize = document.createElement(AUTOCOLORIZE);

            autoColorize.setAttribute(VALUE, new Boolean(activated).toString());

            xpadProfile.appendChild((Node) autoColorize);
        } else {
            xpadAutoIndent.setAttribute(VALUE, new Boolean(activated).toString()  );
        }
        /* Save changes */
        writeDocument();

    }


    /**
     * Save Xpad autoIndent or not
     * @param boolean if autoIndent should be used or not
     */
    public static boolean getAutoColorize() {

        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allSizeElements = xpadProfile.getElementsByTagName(AUTOCOLORIZE);
        Element autoColorize = (Element) allSizeElements.item(0);

        if(autoColorize == null){
            return true;
        } else {
            return new Boolean(autoColorize.getAttribute(VALUE));
        }
    }


    /**
     * Save Xpad autoIndent or not
     * @param boolean if autoIndent should be used or not
     */
    public static void saveDefaultEncoding(String encoding) {

        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allSizeElements = xpadProfile.getElementsByTagName(DEFAULTENCONDING);
        Element xpadAutoIndent = (Element) allSizeElements.item(0);
        if (xpadAutoIndent == null){
            Element defaultEncoding = document.createElement(DEFAULTENCONDING);

            defaultEncoding.setAttribute(VALUE, encoding);

            xpadProfile.appendChild((Node) defaultEncoding);
        } else {
            xpadAutoIndent.setAttribute(VALUE, encoding  );
        }
        /* Save changes */
        writeDocument();

    }


    /**
     * Save Xpad autoIndent or not
     * @param boolean if autoIndent should be used or not
     */
    public static String getDefaultEncoding() {

        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allSizeElements = xpadProfile.getElementsByTagName(DEFAULTENCONDING);
        Element defaultEncoding = (Element) allSizeElements.item(0);

        if (defaultEncoding == null || defaultEncoding.getAttribute(VALUE).equals("")) {
            // If no default encoding read then used system default
            saveDefaultEncoding(Charset.defaultCharset().name());
            return Charset.defaultCharset().name();
        } else {
            return defaultEncoding.getAttribute(VALUE);
        }
    }

    /**
     * Get all the foreground Colors
     * @return a Hashtable with the styles and the associated colors.
     */

    public static Hashtable<String, Color> getAllForegroundColors() {
        /* Load file */
        readDocument();

        Hashtable<String, Color> stylesColorsTable = new Hashtable<String, Color>();

        Element root = document.getDocumentElement();
        NodeList styles = root.getElementsByTagName(STYLE);

        for (int i = 0; i < styles.getLength(); ++i) {
            Element style = (Element) styles.item(i);

            NodeList allForegroundElements = style.getElementsByTagName(FOREGROUNDCOLOR);
            Element styleForeground = (Element) allForegroundElements.item(0);
            Color styleColor = Color.decode(styleForeground.getAttribute(VALUE));
            stylesColorsTable.put(style.getAttribute(NAME), styleColor);
        }

        return stylesColorsTable;
    }

    public static Hashtable<String, Font> getAllFontStyle() {
        return getAllFontStyle(getFont());
    }

    public static Hashtable<String, Font> getAllFontStyle(Font font) {
        /* Load file */
        readDocument();

        Hashtable<String, Font> stylesFontsTable = new Hashtable<String, Font>();

        Element root = document.getDocumentElement();
        NodeList styles = root.getElementsByTagName(STYLE);

        for (int i = 0; i < styles.getLength(); ++i) {
            Element fstyles = (Element) styles.item(i);

            NodeList allFontStyleElements = fstyles.getElementsByTagName(FONT_STYLE);
            Element fontStyle = (Element) allFontStyleElements.item(0);
            int style = Integer.parseInt(fontStyle.getAttribute(VALUE));
            String name = font.getName();
            int size = font.getSize();
            if (style == PLAIN) {
                font = new Font(name, Font.PLAIN, size);

            } else if (style == BOLD) {
                font = new Font(name, Font.BOLD, size);

            } else if (style == ITALIC) {
                font = new Font(name, Font.ITALIC, size);

            } else if (style == BOLDITALIC) {
                font = new Font(name, Font.BOLD | Font.ITALIC , size);

            } else {
                font = new Font(name, Font.PLAIN, size);
            }

            stylesFontsTable.put(fstyles.getAttribute(NAME), font);
        }

        return stylesFontsTable;
    }


    /**
     * get all default foreground colors of xpad
     * @return a Hashtable with the styles and the associated default colors.
     */
    public static Hashtable<String, Color> getAllDefaultForegroundColors() {
        /* Load file */
        readDocument();

        Hashtable<String, Color> stylesDefaultColorsTable = new Hashtable<String, Color>();

        Element root = document.getDocumentElement();
        NodeList styles = root.getElementsByTagName(STYLE);

        for (int i = 0; i < styles.getLength(); ++i) {
            Element style = (Element) styles.item(i);

            NodeList allForegroundElements = style.getElementsByTagName(FOREGROUNDCOLOR);
            Element styleForeground = (Element) allForegroundElements.item(0);
            Color styleColor = Color.decode(styleForeground.getAttribute(DEFAULT));

            stylesDefaultColorsTable.put(style.getAttribute(NAME), styleColor);
        }

        return stylesDefaultColorsTable;
    }

    public static Hashtable<String, Integer> getAllAttributes() {
        /* Load file */
        readDocument();

        Hashtable<String, Integer> attr = new Hashtable<String, Integer>();

        Element root = document.getDocumentElement();
        NodeList styles = root.getElementsByTagName(STYLE);

        for (int i = 0; i < styles.getLength(); ++i) {
            Element style = (Element) styles.item(i);
            int at = 0;
            String underline = style.getAttribute(UNDERLINE);
            if ("true".equals(underline)) {
                at = 1;
            }
            String stroke = style.getAttribute(STROKE);
            if ("true".equals(stroke)) {
                at += 2;
            }
            attr.put(style.getAttribute(NAME), at);
        }

        return attr;
    }

    /**
     * save all foreground colors
     *@param stylesColorsTable a hashtable containing styles and the associated colors
     */
    public static void saveAllForegroundColors(Hashtable<String, Color> stylesColorsTable) {
        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();
        NodeList styles = root.getElementsByTagName(STYLE);

        for (int i = 0; i < styles.getLength(); ++i) {
            Element style = (Element) styles.item(i);

            String styleName = style.getAttribute(NAME);
            NodeList allForegroundElements = style.getElementsByTagName(FOREGROUNDCOLOR);
            Element styleForeground = (Element) allForegroundElements.item(0);

            Color color = stylesColorsTable.get(styleName);

            String rgb = Integer.toHexString(color.getRGB());
            styleForeground.setAttribute(VALUE, COLORPREFIX + rgb.substring(2, rgb.length()));


        }
        /* Save changes */
        writeDocument();
    }

    /**
     * Get the position of Xpad Main Window
     * @return the position
     */
    public static Position getMainWindowPosition() {
        /* Load file */
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allPositionElements = xpadProfile.getElementsByTagName(MAINWINPOSITION);
        Element mainWindowPosition = (Element) allPositionElements.item(0);
        if (mainWindowPosition != null) {
            int x = Integer.parseInt(mainWindowPosition.getAttribute(XCOORD));
            int y = Integer.parseInt(mainWindowPosition.getAttribute(YCOORD));
            /* Avoid Xpad Main Window to be out of the screen */
            if (x <= (Toolkit.getDefaultToolkit().getScreenSize().width - MARGIN)
                && y <= (Toolkit.getDefaultToolkit().getScreenSize().height - MARGIN)) {
                return new Position(x, y);
            } else {
                return new Position(0, 0);
            }
        } else {
            return new Position(0, 0);
        }
    }


    /**
     * Save the position of Xpad Main Window
     * @param position the position of Xpad main Window
     */
    public static void saveMainWindowPosition(Position position) {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allPositionElements = xpadProfile.getElementsByTagName(MAINWINPOSITION);
        Element mainWindowPosition = (Element) allPositionElements.item(0);


        mainWindowPosition.setAttribute(XCOORD, Integer.toString(position.getX()));
        mainWindowPosition.setAttribute(YCOORD, Integer.toString(position.getY()));

        /* Save changes */
        writeDocument();
    }


    /**
     * Save the size of Xpad Main Window
     * @param size the size of Xpad main Window
     */
    public static void saveMainWindowSize(Size size) {
        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allPositionElements = xpadProfile.getElementsByTagName(MAINWINSIZE);
        Element mainWindowSize = (Element) allPositionElements.item(0);


        mainWindowSize.setAttribute(WIDTH, Integer.toString(size.getWidth()));
        mainWindowSize.setAttribute(HEIGHT, Integer.toString(size.getHeight()));

        /* Save changes */
        writeDocument();

    }

    /**
     * Get the size of Xpad Main Window
     * @return the size
     */
    public static Size getMainWindowSize() {

        readDocument();

        Element root = document.getDocumentElement();

        NodeList profiles = root.getElementsByTagName(PROFILE);
        Element xpadProfile = (Element) profiles.item(0);

        NodeList allSizeElements = xpadProfile.getElementsByTagName(MAINWINSIZE);
        Element mainWindowSize = (Element) allSizeElements.item(0);
        if (mainWindowSize != null) {
            return new Size(Integer.parseInt(mainWindowSize.getAttribute(WIDTH)), Integer.parseInt(mainWindowSize.getAttribute(HEIGHT)));
        } else {
            return new Size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
        }
    }

    /**
     * Get all the  recent opened files
     * @return a array of uri
     */

    public static ArrayList<File> getAllRecentOpenedFiles() {
        ArrayList<File> files = new ArrayList<File>();

        readDocument();


        Element root = (Element)document.getDocumentElement().getElementsByTagName("recentFiles").item(0);
        if (root != null) {
            NodeList recentFiles = root.getElementsByTagName("document");

            for (int i = 0; i < recentFiles.getLength(); ++i) {
                Element style = (Element) recentFiles.item(i);

                File temp = new File(style.getAttribute("path"));

                if (temp.exists()) {
                    files.add(temp);
                } else {
                    root.removeChild((Node) style);
                }

                /* Save changes */
                writeDocument();
            }
        }
        return files;
    }

    /**
     * Add a file to recent Opened Files
     * @param filePath the path of the files to add
     */
    public static void saveToRecentOpenedFiles(String filePath) {

        readDocument();

        Element root = (Element) document.getDocumentElement().getElementsByTagName("recentFiles").item(0);
        NodeList recentFiles = root.getElementsByTagName("document");
        int numberOfFiles = recentFiles.getLength();

        // we remove all the duplicate
        for (int i = 0; i < recentFiles.getLength();  ++i) {
            Element style = (Element) recentFiles.item(i);


            if (filePath.equals(style.getAttribute("path"))) {
                root.removeChild((Node) style);
                numberOfFiles--;
            }

        }

        // if we have reached the maximun , we remove the oldest files
        while (recentFiles.getLength() >= MAX_RECENT_FILES) {
            root.removeChild(root.getFirstChild());
        }

        Element newFile =  document.createElement("document");

        newFile.setAttribute("path", filePath);

        root.appendChild((Node) newFile);

        /* Save changes */
        writeDocument();
    }

    /**
     * Read the file to modify
     */
    private static void readDocument() {

        File xml = null;
        DocumentBuilder docBuilder = null;

        try {
            if (document == null) {
                DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
                docBuilder = factory.newDocumentBuilder();

                // read content of a XML file with DOM
                xml = new File(USER_XPAD_CONFIG_FILE);
                document = docBuilder.parse(xml);
            }
        } catch (ParserConfigurationException pce) {
            System.err.println(ERROR_READ + USER_XPAD_CONFIG_FILE);
        } catch (SAXException se) {
            System.err.println(ERROR_READ + USER_XPAD_CONFIG_FILE);
        } catch (IOException ioe) {
            System.err.println(ERROR_READ + USER_XPAD_CONFIG_FILE);
        }

        try {
            if (keysMap == null) {
                xml = new File(USER_XPAD_CONFIG_KEYS_FILE);
                keysMap = new Properties();
                keysMap.loadFromXML(new FileInputStream(xml));
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void addMapActionNameKeys(Map map) {
        for (Enumeration action = keysMap.propertyNames(); action.hasMoreElements();) {
            String name = (String) action.nextElement();
            KeyStroke ks = KeyStroke.getKeyStroke(keysMap.getProperty(name));
            map.put(name, ks);
        }
    }


    /**
     * Save the modifications
     */
    private static void writeDocument() {

        Transformer transformer = null;
        try {
            transformer = TransformerFactory.newInstance().newTransformer();
        } catch (TransformerConfigurationException e1) {
            System.out.println(ERROR_WRITE + USER_XPAD_CONFIG_FILE);
        } catch (TransformerFactoryConfigurationError e1) {
            System.out.println(ERROR_WRITE + USER_XPAD_CONFIG_FILE);
        }
        transformer.setOutputProperty(OutputKeys.INDENT, "yes");

        StreamResult result = new StreamResult(new File(USER_XPAD_CONFIG_FILE));
        DOMSource source = new DOMSource(document);
        try {
            transformer.transform(source, result);
        } catch (TransformerException e) {
            System.out.println(ERROR_WRITE + USER_XPAD_CONFIG_FILE);
        }


    }
    /**
     * Add a file to recent Opened Files
     *
     * @param exp
     *            the path of the files to add
     */
    public static void saveRecentSearch(String exp) {

        Node root = getXcosRoot();
        if(root == null || exp == null || exp.compareTo("") == 0) {
            return;
        }

        Node recents = getNodeChild(root, RECENT_SEARCH);
        if(recents == null) {
            recents = document.createElement(RECENT_SEARCH);
            root.appendChild(recents);
        }

        ArrayList<Node> search = getNodeChildren(recents, SEARCH);

        while(search.size() >= MAX_RECENT_SEARCH) {
            removeRecentSearch(((Element)search.get(0)).getAttribute(EXPRESSION));
            search = getNodeChildren(recents, SEARCH);
        }
        //if path already in file no need to add it
        for(Node item : search) {
            if (exp.compareTo(((Element)item).getAttribute(EXPRESSION)) == 0) {
                return;
            }
        }

        Element newSearch = document.createElement(SEARCH);

        newSearch.setAttribute(EXPRESSION, exp);

        recents.appendChild((Node) newSearch);

        /* Save changes */
        writeDocument();
    }

    public static void removeRecentSearch(String exp) {

        Node root = getXcosRoot();
        if(root == null) {
            return;
        }

        Node recent = getNodeChild(root, RECENT_SEARCH);
        ArrayList<Node> search = getNodeChildren(recent, SEARCH);

        // remove node if exists
        for(Node file : search) {
            if (exp.compareTo(((Element)file).getAttribute(EXPRESSION)) == 0) {
                recent.removeChild(file);
                break;
            }

        }
        /* Save changes */
        writeDocument();

    }

    public static ArrayList<String> getRecentSearch() {
        ArrayList<String> files = new ArrayList<String>();

        Node root = getXcosRoot();
        if(root == null) {
            return files;
        }

        Node recent = getNodeChild(root, RECENT_SEARCH);
        ArrayList<Node> searches = getNodeChildren(recent, SEARCH);
        for(Node search : searches) {
            String exp = ((Element)search).getAttribute(EXPRESSION);
            if(exp != null && exp.compareTo("") != 0) {
                files.add(exp);
            }
        }

        return files;
    }

    public static void saveRecentReplace(String exp) {

        Node root = getXcosRoot();
        if(root == null || exp == null || exp.compareTo("") == 0) {
            return;
        }

        Node recent = getNodeChild(root, RECENT_REPLACE);
        if(recent == null) {
            recent = document.createElement(RECENT_REPLACE);
            root.appendChild(recent);
        }

        ArrayList<Node> replace = getNodeChildren(recent, REPLACE);

        while(replace.size() >= MAX_RECENT_REPLACE) {
            removeRecentReplace(((Element)replace.get(0)).getAttribute(EXPRESSION));
            replace = getNodeChildren(recent, REPLACE);
        }
        //if path already in file no need to add it
        for(Node item : replace) {
            if (exp.compareTo(((Element)item).getAttribute(EXPRESSION)) == 0) {
                return;
            }
        }

        Element newReplace = document.createElement(REPLACE);

        newReplace.setAttribute(EXPRESSION, exp);

        recent.appendChild((Node) newReplace);

        /* Save changes */
        writeDocument();
    }

    public static void removeRecentReplace(String filePath) {

        Node root = getXcosRoot();
        if(root == null) {
            return;
        }

        Node recent = getNodeChild(root, RECENT_REPLACE);
        ArrayList<Node> replace = getNodeChildren(recent, REPLACE );

        // remove node if exists
        for(Node exp : replace) {
            if (filePath.compareTo(((Element)exp).getAttribute(EXPRESSION)) == 0) {
                recent.removeChild(exp);
                break;
            }

        }
        /* Save changes */
        writeDocument();

    }

    public static ArrayList<String> getRecentReplace() {
        ArrayList<String> exps = new ArrayList<String>();

        Node root = getXcosRoot();
        if(root == null) {
            return exps;
        }

        Node recent = getNodeChild(root, RECENT_REPLACE);
        ArrayList<Node> replace = getNodeChildren(recent, REPLACE);
        for(Node file : replace) {
            String exp = ((Element)file).getAttribute(EXPRESSION);
            if(exp != null && exp.compareTo("") != 0) {
                exps.add(exp);
            }
        }

        return exps;
    }

    public static boolean getRegularExpression() {
        return getBooleanAttribute(REGULAR_EXPRESION, STATE_FLAG, false);
    }

    public static void saveRegularExpression(boolean regualExp) {
        saveBooleanAttribute(REGULAR_EXPRESION, STATE_FLAG, regualExp);
    }

    public static boolean getWholeWord() {
        return getBooleanAttribute(WHOLE_WORD, STATE_FLAG, false);
    }

    public static void saveWholeWord(boolean wholeWord) {
        saveBooleanAttribute(WHOLE_WORD, STATE_FLAG, wholeWord);
    }

    public static boolean getWordWarp() {
        return getBooleanAttribute(WORD_WARP, STATE_FLAG, true);
    }

    public static void saveWordWarp(boolean wordWarp) {
        saveBooleanAttribute(WORD_WARP, STATE_FLAG, wordWarp);
    }

    public static boolean getCaseSensitive() {
        return getBooleanAttribute(CASE_SENSITIVE, STATE_FLAG, false);
    }

    public static void saveCaseSensitive(boolean caseSensitive) {
        saveBooleanAttribute(CASE_SENSITIVE, STATE_FLAG, caseSensitive);
    }

    private static boolean getBooleanAttribute(String node, String attrib, boolean defaultValue) {
        boolean flag = false;
        Node root = getXcosRoot();
        if(root == null) {
            return flag;
        }
        Node recent = getNodeChild(root, node);
        if(recent != null) {
            String exp = ((Element)recent).getAttribute(attrib);
            if(exp.compareTo("true") == 0) {
                flag = true;
            }
        } else {
            return defaultValue;
        }
        return flag;
    }

    private static void saveBooleanAttribute(String node, String attrib, boolean state) {
        Node root = getXcosRoot();
        if(root == null) {
            return;
        }

        Node recent = getNodeChild(root, node);
        if(recent == null) {
            recent = document.createElement(node);
            root.appendChild(recent);
        }


        ((Element)recent).setAttribute(attrib, new Boolean(state).toString());

        root.appendChild((Node) recent);

        /* Save changes */
        writeDocument();
    }

    private static Node getNodeChild(Node parent, String nodeName) {

        if(parent == null) {
            if(document == null) {
                readDocument();
                if(document == null) {
                    return null;
                }
            }
            parent = document;
        }

        Node currentNode = parent.getFirstChild();
        while(currentNode != null) {
            if(currentNode.getNodeName().compareTo(nodeName) == 0){
                return currentNode;
            }
            currentNode = currentNode.getNextSibling();
        }
        return currentNode;
    }

    private static ArrayList<Node> getNodeChildren(Node parent, String childName) {
        ArrayList<Node> nodes = new ArrayList<Node>();

        if(parent == null) {
            if(document == null) {
                readDocument();
                if(document == null) {
                    return nodes;
                }
            }
            parent = document;
        }

        Node currentNode = parent.getFirstChild();
        while(currentNode != null) {
            if(currentNode.getNodeName().compareTo(childName) == 0){
                nodes.add(currentNode);
            }
            currentNode = currentNode.getNextSibling();
        }
        return nodes;

    }

    private static Node getXcosRoot() {

        if(document == null) {
            readDocument();
            if(document == null) {
                return null;
            }
        }

        Node setting = getNodeChild(null, SETTING);

        if(setting != null) {
            ArrayList<Node> nodes = getNodeChildren(setting, PROFILE);
            for(Node node : nodes) {
                if(((Element)node).getAttribute(NAME).compareTo(XPAD) == 0) {
                    return node;
                }
            }
        }
        return null;
    }
}
