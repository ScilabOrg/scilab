/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.Color;
import java.awt.Font;
import java.awt.Toolkit;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactoryConfigurationError;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.commons.xml.ScilabTransformerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

/**
 * Management of the Console configuration file
 * @author Vincent COUVERT
 */
public final class ConfigManager {

    private static final int BUFSIZE = 1024;

    private static final int MARGIN = 20;

    private static final String ERROR_READ = "Could not load file: ";
    private static final String ERROR_WRITE = "Could not save file: ";
    private static final String VERSION = "version";
    private static final String VALUE = "value";
    private static final String WIDTH = "width";
    private static final String HEIGHT = "height";
    private static final String XCOORD = "x";
    private static final String YCOORD = "y";
    private static final String MAINWINPOSITION = "MainWindowPosition";
    private static final String MAINWINSIZE = "MainWindowSize";
    private static final String HELPWINPOSITION = "HelpWindowPosition";
    private static final String HELPWINSIZE = "HelpWindowSize";
    private static final String HELPFONTSIZE = "HelpFontSize";
    private static final String PROFILE = "Profile";
    private static final String FOREGROUNDCOLOR = "ForegroundColor";
    private static final String BACKGROUNDCOLOR = "BackgroundColor";
    private static final String COLORPREFIX = "#";
    private static final String MAXOUTPUTSIZE = "MaxOutputSize";
    private static final String LASTOPENEDDIR = "LastOpenedDirectory";

    private static final String SCILAB_CONFIG_FILE = System.getenv("SCI") + "/modules/console/etc/configuration.xml";

    private static final String USER_CONFIG_FILE = ScilabCommons.getSCIHOME() + "/configuration.xml";

    private static final int DEFAULT_WIDTH = 650;
    private static final int DEFAULT_HEIGHT = 550;

    private static final int DEFAULT_MAXOUTPUTSIZE = 10000;
    private static final int DEFAULT_HELPFONTSIZE = 2;

    private static Document document;

    private static boolean updated;

    /**
     * Constructor
     */
    private ConfigManager() {
        throw new UnsupportedOperationException();
    }

    /**
     * Create a copy of Scilab configuration file in the user directory
     */
    public static void createUserCopy() {
        File fileConfig = new File(USER_CONFIG_FILE);
        if (!fileConfig.exists() || (fileConfig.length() == 0) || checkVersion()) {
            /* Create a local copy of the configuration file */
            try {
                copyFile(new File(SCILAB_CONFIG_FILE), new File(USER_CONFIG_FILE));
                updated = true;
            } catch (FileNotFoundException e) {
                System.out.println(ERROR_READ + USER_CONFIG_FILE);
            }
        }
    }

    // TODO: remove this f...g method (see in SwingScilabConsole and in checkVersion)
    public static String workaroundForVersionBugWith5_3_1() {
	File fileConfig = new File(USER_CONFIG_FILE);
	if (fileConfig.exists()) {
            Document doc = readDocument(USER_CONFIG_FILE);
            Element setting = doc.getDocumentElement();
            String str = setting.getAttribute(VERSION);
	    if (str == null || str.isEmpty()) {
		return SCILAB_CONFIG_FILE;
	    }
	}
	
	return USER_CONFIG_FILE;
    }

    /**
     * @return true if configuration.xml in etc has a version different of the version in home
     */
    public static boolean checkVersion() {
        if (updated || !workaroundForVersionBugWith5_3_1().equals(USER_CONFIG_FILE)) {
            return false;
        }

        File fileConfig = new File(USER_CONFIG_FILE);
        if (fileConfig.exists()) {
            Document doc = readDocument(USER_CONFIG_FILE);
            Element setting = doc.getDocumentElement();
            String str = ((Element) setting).getAttribute(VERSION);
            if (str != null && str.length() != 0) {
                float userVersion = Float.parseFloat(str);
                doc = readDocument(SCILAB_CONFIG_FILE);
                setting = doc.getDocumentElement();
                str = ((Element) setting).getAttribute(VERSION);

                if (str != null && str.length() != 0) {
                    float scilabVersion = Float.parseFloat(str);
                    return userVersion != scilabVersion;
                }
            }
        }

        return true;
    }

    /**
     * Get the name of the user configuration file
     * @return the name of the configuration file
     */
    public static String getUserConfigFile() {
        return USER_CONFIG_FILE;
    }

    /**
     * Save a new font setting
     * @param font the new font
     */
    public static void saveFont(Font font) {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList fontSizeElement = scilabProfile.getElementsByTagName("FontSize");
            Element fontSize = (Element) fontSizeElement.item(0);
            fontSize.setAttribute(VALUE, Integer.toString(font.getSize()));

            NodeList fontNameElement = scilabProfile.getElementsByTagName("FontName");
            Element fontName = (Element) fontNameElement.item(0);
            fontName.setAttribute(VALUE, font.getFontName());

            NodeList fontStyleElement = scilabProfile.getElementsByTagName("FontStyle");
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
    }

    /**
     * Copy a file
     * @param in src file
     * @param out dest file
     * @throws FileNotFoundException
     */
    private static void copyFile(File in, File out) throws FileNotFoundException {
        FileInputStream fis = new FileInputStream(in);
        FileOutputStream fos = new FileOutputStream(out);;

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
     * Get the maximum number of lines to keep in the output
     * @return the nulber of lines
     */
    public static int getMaxOutputSize() {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(MAXOUTPUTSIZE);
            Element maxOutputSize = (Element) allPositionElements.item(0);
            if (maxOutputSize != null) {
                return Integer.parseInt(maxOutputSize.getAttribute(VALUE));
            }
        }
        return DEFAULT_MAXOUTPUTSIZE;
    }

    /**
     * Get the font size in the help viewer
     * @return the font size
     */
    public static int getHelpFontSize() {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(HELPFONTSIZE);
            Element helpFontSize = (Element) allPositionElements.item(0);
            if (helpFontSize != null) {
                return Integer.parseInt(helpFontSize.getAttribute(VALUE));
            }
        }
        return DEFAULT_HELPFONTSIZE;
    }

    /**
     * Get the font size in the help viewer
     * @return the font size
     */
    public static void setHelpFontSize(int size) {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(HELPFONTSIZE);
            Element helpFontSize = (Element) allPositionElements.item(0);
            if (helpFontSize == null) {
                helpFontSize = document.createElement(HELPFONTSIZE);
                scilabProfile.appendChild(helpFontSize);
            }

            helpFontSize.setAttribute(VALUE, Integer.toString(size));
            writeDocument();
        }
    }

    /**
     * Get the position of Scilab Main Window
     * @return the position
     */
    public static Position getMainWindowPosition() {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(MAINWINPOSITION);
            Element mainWindowPosition = (Element) allPositionElements.item(0);
            if (mainWindowPosition != null) {
                int x = Integer.parseInt(mainWindowPosition.getAttribute(XCOORD));
                int y = Integer.parseInt(mainWindowPosition.getAttribute(YCOORD));
                /* Avoid Scilab Main Window to be out of the screen */
                if (x <= (Toolkit.getDefaultToolkit().getScreenSize().width - MARGIN)
                    && y <= (Toolkit.getDefaultToolkit().getScreenSize().height - MARGIN)) {
                    return new Position(x, y);
                }
            }
        }
        return new Position(0, 0);
    }

    /**
     * Save the position of Scilab Main Window
     * @param position the position of Scilab main Window
     */
    public static void saveMainWindowPosition(Position position) {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(MAINWINPOSITION);
            Element mainWindowPosition = (Element) allPositionElements.item(0);

            // Ascendant compatibility
            if (mainWindowPosition == null) {
                mainWindowPosition = document.createElement(MAINWINPOSITION);
                scilabProfile.appendChild(mainWindowPosition);
            }

            mainWindowPosition.setAttribute(XCOORD, Integer.toString(position.getX()));
            mainWindowPosition.setAttribute(YCOORD, Integer.toString(position.getY()));

            /* Save changes */
            writeDocument();
        }
    }

    /**
     * Save the size of Scilab Main Window
     * @param size the size of Scilab main Window
     */
    public static void saveMainWindowSize(Size size) {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(MAINWINSIZE);
            Element mainWindowSize = (Element) allPositionElements.item(0);

            // Ascendant compatibility
            if (mainWindowSize == null) {
                mainWindowSize = document.createElement(MAINWINSIZE);
                scilabProfile.appendChild(mainWindowSize);
            }

            mainWindowSize.setAttribute(WIDTH, Integer.toString(size.getWidth()));
            mainWindowSize.setAttribute(HEIGHT, Integer.toString(size.getHeight()));

            /* Save changes */
            writeDocument();
        }
    }

    /**
     * Get the size of Scilab Main Window
     * @return the size
     */
    public static Size getMainWindowSize() {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allSizeElements = scilabProfile.getElementsByTagName(MAINWINSIZE);
            Element mainWindowSize = (Element) allSizeElements.item(0);
            if (mainWindowSize != null) {
                return new Size(Integer.parseInt(mainWindowSize.getAttribute(WIDTH)), Integer.parseInt(mainWindowSize.getAttribute(HEIGHT)));
            }
        }
        return new Size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    }

    /**
     * Get the position of Scilab Help Window
     * @return the position
     */
    public static Position getHelpWindowPosition() {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(HELPWINPOSITION);
            Element helpWindowPosition = (Element) allPositionElements.item(0);
            if (helpWindowPosition != null) {
                int x = Integer.parseInt(helpWindowPosition.getAttribute(XCOORD));
                int y = Integer.parseInt(helpWindowPosition.getAttribute(YCOORD));
                /* Avoid Scilab Help Window to be out of the screen */
                if (x <= (Toolkit.getDefaultToolkit().getScreenSize().width - MARGIN)
                    && y <= (Toolkit.getDefaultToolkit().getScreenSize().height - MARGIN)) {
                    return new Position(x, y);
                }
            }
        }
        return new Position(0, 0);
    }

    /**
     * Save the position of Scilab Help Window
     * @param position the position of Scilab help Window
     */
    public static void saveHelpWindowPosition(Position position) {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(HELPWINPOSITION);
            Element helpWindowPosition = (Element) allPositionElements.item(0);

            // Ascendant compatibility
            if (helpWindowPosition == null) {
                helpWindowPosition = document.createElement(HELPWINPOSITION);
                scilabProfile.appendChild(helpWindowPosition);
            }

            helpWindowPosition.setAttribute(XCOORD, Integer.toString(position.getX()));
            helpWindowPosition.setAttribute(YCOORD, Integer.toString(position.getY()));

            /* Save changes */
            writeDocument();
        }
    }

    /**
     * Save the size of Scilab Help Window
     * @param size the size of Scilab help Window
     */
    public static void saveHelpWindowSize(Size size) {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allPositionElements = scilabProfile.getElementsByTagName(HELPWINSIZE);
            Element helpWindowSize = (Element) allPositionElements.item(0);

            // Ascendant compatibility
            if (helpWindowSize == null) {
                helpWindowSize = document.createElement(HELPWINSIZE);
                scilabProfile.appendChild(helpWindowSize);
            }

            helpWindowSize.setAttribute(WIDTH, Integer.toString(size.getWidth()));
            helpWindowSize.setAttribute(HEIGHT, Integer.toString(size.getHeight()));

            /* Save changes */
            writeDocument();
        }
    }

    /**
     * Get the size of Scilab Help Window
     * @return the size
     */
    public static Size getHelpWindowSize() {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allSizeElements = scilabProfile.getElementsByTagName(HELPWINSIZE);
            Element helpWindowSize = (Element) allSizeElements.item(0);
            if (helpWindowSize != null) {
                return new Size(Integer.parseInt(helpWindowSize.getAttribute(WIDTH)), Integer.parseInt(helpWindowSize.getAttribute(HEIGHT)));
            }
        }
        return new Size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    }

    /**
     * Save the Last Opened Directory in Scilab
     * @param the directory's path
     */

    public static void saveLastOpenedDirectory(String path ){
        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allSizeElements = scilabProfile.getElementsByTagName(LASTOPENEDDIR);
            Element lastOpenedDir = (Element) allSizeElements.item(0);

            lastOpenedDir.setAttribute(VALUE, path);

            writeDocument();
        }
    }

    /**
     * Get the Last Opened Directory in Scilab
     * @return the directory's path
     */

    public static String getLastOpenedDirectory(){
        /* Load file */
        /*System.getProperty("user.dir") if no path*/
        readDocument();
        String path = new String() ;

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allSizeElements = scilabProfile.getElementsByTagName(LASTOPENEDDIR);
            Element lastOpenedDir = (Element) allSizeElements.item(0);

            if (lastOpenedDir != null){

                path = lastOpenedDir.getAttribute(VALUE);

                if (path.length() == 0){
                    path = System.getProperty("user.dir") ;
                }
            }else{
                Element newLastOpenedDir =  document.createElement(LASTOPENEDDIR);
                path = System.getProperty("user.dir") ;
                newLastOpenedDir.setAttribute("useCache","true");
                newLastOpenedDir.setAttribute(VALUE, path);

                scilabProfile.appendChild(newLastOpenedDir);

                writeDocument();
            }
        }
        return path ;
    }

    /**
     * Save the console Foreground Color
     * @param color the new Color
     */
    public static void saveConsoleForeground(Color color) {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allSizeElements = scilabProfile.getElementsByTagName(FOREGROUNDCOLOR);
            Element consoleForeground = (Element) allSizeElements.item(0);

            String rgb = Integer.toHexString(color.getRGB());
            consoleForeground.setAttribute(VALUE, COLORPREFIX + rgb.substring(2, rgb.length()));

            /* Save changes */
            writeDocument();
        }
    }

    /**
     * Save the console Background Color
     * @param color the new Color
     */
    public static void saveConsoleBackground(Color color) {

        /* Load file */
        readDocument();

        if (document != null) {
            Element racine = document.getDocumentElement();

            NodeList profiles = racine.getElementsByTagName(PROFILE);
            Element scilabProfile = (Element) profiles.item(0);

            NodeList allSizeElements = scilabProfile.getElementsByTagName(BACKGROUNDCOLOR);
            Element consoleBackground = (Element) allSizeElements.item(0);

            String rgb = Integer.toHexString(color.getRGB());
            consoleBackground.setAttribute(VALUE, COLORPREFIX + rgb.substring(2, rgb.length()));

            /* Save changes */
            writeDocument();
        }
    }

    /**
     * Read the file to modify
     */
    private static Document readDocument(String fileName) {
        File xml = null;
        DocumentBuilder docBuilder = null;

        try {
            DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
            docBuilder = factory.newDocumentBuilder();

            // lecture du contenu d'un fichier XML avec DOM
            xml = new File(fileName);
            return docBuilder.parse(xml);
        } catch (ParserConfigurationException pce) {
            System.out.println(ERROR_READ + fileName);
        } catch (SAXException se) {
            System.out.println(ERROR_READ + fileName);
        } catch (IOException ioe) {
            System.out.println(ERROR_READ + fileName);
        }
        return null;
    }

    /**
     * Read the file to modify
     */
    private static void readDocument() {
        createUserCopy();
        document = readDocument(USER_CONFIG_FILE);
    }

    /**
     * Save the modifications
     */
    private static void writeDocument() {
        Transformer transformer = null;
        try {
            transformer = ScilabTransformerFactory.newInstance().newTransformer();
        } catch (TransformerConfigurationException e1) {
            System.out.println(ERROR_WRITE + USER_CONFIG_FILE);
        } catch (TransformerFactoryConfigurationError e1) {
            System.out.println(ERROR_WRITE + USER_CONFIG_FILE);
        }
        transformer.setOutputProperty(OutputKeys.INDENT, "yes");

        StreamResult result = new StreamResult(new File(USER_CONFIG_FILE));
        DOMSource source = new DOMSource(document);
        try {
            transformer.transform(source, result);
        } catch (TransformerException e) {
            System.out.println(ERROR_WRITE + USER_CONFIG_FILE);
        }

    }

}
