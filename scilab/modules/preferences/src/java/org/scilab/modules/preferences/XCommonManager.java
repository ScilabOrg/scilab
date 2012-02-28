/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 -         Pierre GRADIT
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.preferences;

import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Frame;
import java.io.File;
import java.io.FilenameFilter;
import java.io.FileFilter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;

import javax.swing.JDialog;
import javax.swing.SwingUtilities;
import javax.swing.border.Border;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.TransformerFactoryConfigurationError;
import javax.xml.transform.dom.DOMResult;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

import org.w3c.dom.Document;
import org.w3c.dom.DocumentFragment;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

import org.scilab.modules.localization.Messages;
import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.commons.xml.ScilabDocumentBuilderFactory;
import org.scilab.modules.commons.xml.ScilabTransformerFactory;
import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.gui.console.ScilabConsole;

/* This class is the common ancestor to both X_manager.
 */
public abstract class XCommonManager {

    /** XConfiguration management verbosity.*/
    public static final boolean performances = true;

    /** XConfiguration management verbosity.*/
    public static final boolean differential = true;

    /** Message for read error.*/
    protected static final String ERROR_READ = "Could not load file: ";

    /** Message for write error.*/
    protected static final String ERROR_WRITE = "Could not save file: ";

    /** Buffer size.*/
    protected static final int BUFSIZE = 1024;

    /** Main dialog.*/
    protected static JDialog dialog;

    /** DOM Document.*/
    protected static Document document;

    /** DOM Document.*/
    protected static String documentAddress;

    /** Up-to-date flag.*/
    protected static boolean  updated = false;

    /** Top level DOM Node.*/
    protected static Node topDOM;

    /** Top level Swing container.*/
    protected static Container topSwing;

    /** Container-Sentinel correspondence.*/
    protected static Map<Component, XSentinel> correspondance;

    /** Last visitor.*/
    protected static XUpdateVisitor visitor;

    /** Last current time.*/
    protected static long time = System.currentTimeMillis();

    /**
     * XSL Transformer factory.
     */
    protected static TransformerFactory factory = ScilabTransformerFactory.newInstance();

    /**
     * XML Document builder factory.
     */
    protected static DocumentBuilderFactory builder = ScilabDocumentBuilderFactory.newInstance();

    /**
     * XSL Transformer.
     */
    protected static Transformer transformer = null;

    private static final String SCI = System.getenv("SCI");

    /** Scilab configuration file.*/
    private static final String SCILAB_CONFIG_FILE = SCI + "/modules/preferences/etc/XConfiguration.xml";

    private static String XSLCODE;

    /**
     * Monitor time between calls.
     */
    public static void printTimeStamp(final String msg) {
        long nextTime  = System.currentTimeMillis();
        long deltaTime = nextTime - time;
        if (performances) {
            System.out.println((msg.startsWith("*")?"":" |  ") + msg + " in " + deltaTime + " ms.");
        }
        time = nextTime;
    }

    /** Launch swing hierarchy update.
     *
     * @return whether XSL return a node or not.
     */
    public static boolean refreshDisplay() {
        topDOM = generateViewDOM().getNode().getFirstChild();
        if (topDOM == null) {
            System.err.println("XSL does not give a node!");
            return false;
        }

        // Refresh correspondence
        //    TODO top layout changes
        visitor = new XUpdateVisitor(correspondance);
        visitor.visit(topSwing, topDOM);
        setDimension(dialog, topDOM);
        dialog.pack();

        return true;
    }

    /**
     * Horizontal space between parent and child in String representations.
     */
    public static final String INCREMENT = "    ";

    /**
     * Compute recursive string representation of DOM view tree.
     * @param node : current node
     * @param indent : current indentation
     * @return node representation
     *
     */
    private static String viewDOM(final Node node, final String indent) {
        String signature = indent;
        if (node.hasAttributes()) {
            signature += XSentinel.signature(node, new String[0]);
        } else {
            String single = node.getNodeName() + ": " + node.getNodeValue();
            signature += single.replaceAll("[ \t\n]+", " ");
        }
        signature += "\n";
        NodeList nodelist = node.getChildNodes();
        for (int i = 0; i < nodelist.getLength(); i++) {
            Node item  = nodelist.item(i);
            signature += viewDOM(item, indent + INCREMENT);
        }

        return signature;
    }

    /**
     * Compute top-level string representation of DOM view tree.
     * @return document representation
     */
    protected static String viewDOM() {
        return viewDOM(topDOM, "");
    }

    /**
     * Compute recursive string representation of Swing tree.
     * @param component : current component
     * @param indent : current indentation
     * @return component representation
     *
     */
    private static String swingComposite(final Component component, final String indent) {
        String signature = indent;
        signature += component.toString() + "\n";
        if (component instanceof Container) {
            Container container = (Container) component;
            Component [] components = container.getComponents();
            for (int i = 0; i < components.length; i++) {
                Component child = components[i];
                signature += swingComposite(child, indent + INCREMENT);
            }
        }
        return signature;
    }

    /**
     * Compute top-level string representation of swing composite.
     * @return Graphical interface representation
     */
    protected static String swingComposite() {
        return swingComposite(topSwing, "");
    }

    private static List<File> getEtcDir() {
        List<File> list = new ArrayList<File>();
        File modulesDir = new File(SCI + "/modules/");
        File[] modules = modulesDir.listFiles(new FileFilter() {
                public boolean accept(File f) {
                    return f.isDirectory();
                }
            });

        for (File module : modules) {
            File etc = new File(module, "/etc/");
            if (etc.exists() && etc.isDirectory()) {
                list.add(etc);
            }
        }

        return list;
    }

    /**
     * Create a XSL string in using the XConfiguration-*.xsl found in SCI/modules/MODULE_NAME/etc/
     * @return the buit XSL string.
     */
    protected static String createXSLFile() {
        if (XSLCODE == null) {
            List<File> etcs = XConfiguration.getEtcDir();

            StringBuilder buffer = new StringBuilder("<?xml version='1.0' encoding='utf-8'?>\n");
            buffer.append("<xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">\n");
            buffer.append("<xsl:import href=\"").append(SCI).append("/modules/preferences/src/xslt/XConfiguration.xsl").append("\"/>\n");

            for (File etc : etcs) {
                File[] xsls = etc.listFiles(new FilenameFilter() {
                        public boolean accept(File dir, String name) {
                            return name.endsWith(".xsl") && name.startsWith("XConfiguration");
                        }
                    });
                for (File xsl : xsls) {
                    try {
                        buffer.append("<xsl:import href=\"").append(xsl.getCanonicalPath()).append("\"/>\n");
                    } catch (IOException e) {
                        buffer.append("<xsl:import href=\"").append(xsl.getAbsolutePath()).append("\"/>\n");
                    }
                }
            }
            buffer.append("</xsl:stylesheet>");

            XSLCODE = buffer.toString();
        }

        return XSLCODE;
    }

    /**
     * Load XSL as XSL Transformer.
     */
    protected static void reloadTransformer(String address) {
        try {
            StreamSource source = new StreamSource(new StringReader(createXSLFile()));
            transformer = factory.newTransformer(source);
        } catch (TransformerConfigurationException e1) {
            System.err.println(ERROR_READ + address);
        } catch (TransformerFactoryConfigurationError e1) {
            System.err.println(ERROR_READ + address);
        }
    }

    /**
     * Generate view by application of XSL on XConfiguration file.
     * @return View DOM.
     */
    private static DOMResult generateViewDOM() {
        DOMResult result = new DOMResult();
        DOMSource source = new DOMSource(document);
        try {
            transformer.transform(source, result);
        } catch (TransformerException e) {
            // Just highlight clear transformer output.
            System.out.println("\n");
        }
        return result;
    }

    /**
     * Identify an element with its context string.
     * @see XConfiguration.xsl#context
     * @param context : the context string used to catch the element.
     * @return the corresponding node
     */
    public static Element getElementByContext(final String context) {
        String[] ids = context.split("/");
        Element element = (Element) document.getDocumentElement();
        for (int i = 0; i < ids.length; i++) {
            int index = Integer.parseInt(ids[i]);
            // get the element with corresponding index (filter text nodes)
            NodeList childNodes = element.getChildNodes();
            Node node = null;
            for (int j = 0; index > 0 && j < childNodes.getLength(); j++) {
                node = childNodes.item(j);
                if (!node.getNodeName().equals("#text") && !node.getNodeName().equals("#comment")) {
                    index--;
                }
            }
            if (index == 0) {
                element = (Element) node;
            } else {
                System.err.println("'" + context + "' out of document!");
                return null;
            }
        }
        return element;
    }

    /**
     * Interpret action.
     * @param action : to be interpreted.
     * @param source : component source of the action (only class is needed).
     * @return if the event is handled here
     */
    protected static boolean generixEvent(final Node[] actions, final Component source) {
        if (actions.length == 0) {
            return false;
        }

        Node action = actions[0];
        // All actions must be of the same kind.

        if (!getAttribute(action, "set").equals(NAV)) {
            for (int i = 0; i < actions.length; i++) {
                action = actions[i];
                String context = getAttribute(action, "context");
                Element element = getElementByContext(context);
                String value = getAttribute(action, "value");
                String attribute = getAttribute(action, "set");
                if (element != null) {
                    element.setAttribute(attribute, value);
                }
            }
            refreshDisplay();
            updated = true;
            return true;
        }

        if (!getAttribute(action, "insert").equals(NAV)) {
            for (int i = 0; i < actions.length; i++) {
                action = actions[i];
                String context = getAttribute(action, "context");
                Element element = getElementByContext(context);
                String insertValue = XCommonManager.getAttribute(action, "insert");
                int insert = 0;
                try {
                    insert = Integer.decode(insertValue);
                } catch (NumberFormatException e) {
                    XChooser chooser = (XChooser) source;
                    insertValue = chooser.choose().toString();
                    insert = Integer.decode(insertValue) + 1;
                }
                Node hook = null;
                NodeList nodelist = element.getChildNodes();
                for (int xi = 0; xi < nodelist.getLength(); xi++) {
                    Node node = nodelist.item(xi);
                    if (!node.getNodeName().equals("#text") && !node.getNodeName().equals("#comment")) {
                        if (insert == 1) {
                            hook = node;
                            break;
                        }
                        insert --;
                    }
                }
                DocumentFragment fragment = document.createDocumentFragment();
                while (action.hasChildNodes()) {
                    Node transferred = action.getFirstChild();
                    action.removeChild(transferred);
                    transferred = document.importNode(transferred, true);
                    fragment.appendChild(transferred);
                }
                if (element != null) {
                    element.insertBefore(fragment, hook);
                }
            }
            refreshDisplay();
            updated = true;

            return true;
        }

        if (!getAttribute(action, "delete").equals(NAV)) {
            for (int i = 0; i < actions.length; i++) {
                action = actions[i];
                String context = getAttribute(action, "context");
                Element element = getElementByContext(context);
                String xDelete = XCommonManager.getAttribute(action, "delete");
                int delete = 0;
                try {
                    delete = Integer.decode(xDelete);
                } catch (NumberFormatException e) {
                    if (source == null) {
                        return false;
                    }
                    XChooser chooser = (XChooser) source;
                    xDelete = chooser.choose().toString();
                    delete = Integer.decode(xDelete);
                }
                Node deleted = null;
                NodeList nodelist = element.getChildNodes();
                for (int xi = 0; xi < nodelist.getLength(); xi++) {
                    Node node = nodelist.item(xi);
                    if (!node.getNodeName().equals("#text") && !node.getNodeName().equals("#comment")) {
                        if (delete == 1) {
                            deleted = node;
                            break;
                        }
                        delete --;
                    }
                }
                if (element != null && deleted != null) {
                    element.removeChild(deleted);
                }
            }
            refreshDisplay();
            updated = true;

            return true;
        }

        if (!getAttribute(action, "choose").equals(NAV)) {
            String context = getAttribute(action, "context");
            Element element = getElementByContext(context);
            if (source == null) {
                return false;
            }
            if (source instanceof XChooser) {
                XChooser chooser = (XChooser) source;
                Object value = chooser.choose();
                if (value != null) {
                    String attribute = getAttribute(action, "choose");
                    if (element != null) {
                        if (value instanceof String[]) {
                            String[] values = (String[]) value;
                            StringTokenizer toks = new StringTokenizer(attribute, ",");
                            int n = Math.min(toks.countTokens(), values.length);
                            for (int i = 0; i < n; i++) {
                                String attr = toks.nextToken().trim();
                                element.setAttribute(attr, values[i]);
                            }
                        } else {
                            element.setAttribute(attribute, value.toString());
                        }
                    }
                    refreshDisplay();
                    updated = true;
                }
            } else {
                System.err.println("@choose attribute only valid on choosers " + "(SELECT, COLOR, FILE, ENTRY,...)");
            }
            return true;
        }
        return false;
    }

    /**
     * Sentinel string for attribute consulting.
     */
    public static final String NAV = "\"not an value'";

    /**
     * Attribute consulting with default.
     * @param node : consulted node
     * @param name : attribute key
     * @param value : default value
     * @return the consulted value
     */
    public static String getAttribute(final Node node, final String name, final String value) {
        String response = getAttribute(node, name);
        if (response == NAV) {
            return value;
        }

        return response;
    }

    /**
     * Attribute consulting without default.
     * @param node : consulted node.
     * @param name : attribute key.
     * @return the consulted value (or NAV if attribute key is absent)
     */
    public static String getAttribute(final Node node, final String name) {
        NamedNodeMap attrs = node.getAttributes();
        if (attrs == null) {
            return NAV;
        }
        Node attr = attrs.getNamedItem(name);
        if (attr == null) {
            return NAV;
        }

        return attr.getNodeValue();
    }

    /**
     * Typed attribute consulting with default.
     * @param node : consulted node.
     * @param name : attribute key.
     * @param value : default value.
     * @return the value.
     */
    public static int getInt(final Node node, final String name, final int value) {
        String response = getAttribute(node, name);
        if (response.equals(NAV) || response.equals("")) {
            return value;
        }

        try {
            return Integer.parseInt(response);
        } catch (NumberFormatException e) {
            return 0;
        }
    }

    /**
     * Typed attribute consulting with default.
     * @param node : consulted node.
     * @param name : attribute key.
     * @param value : default value.
     * @return the value.
     */
    public static double getDouble(final Node node, final String name, final double value) {
        String response = getAttribute(node, name);
        if (response.equals(NAV) || response.equals("")) {
            return value;
        }

        try {
            return Double.parseDouble(response);
        } catch (NumberFormatException e) {
            return 0;
        }
    }

    /**
     * Manage color representation.
     * @param source : the color.
     * @return the string representation.
     */
    public static String getColor(final Color source) {
        if (source == null) {
            return "#000000";
        }

        String hexStr = Integer.toHexString(source.getRGB());
        return "#" + hexStr.substring(2);
    }

    /**
     * Manage color representation.
     * @param source : the string representation.
     * @return the corresponding color
     */
    public static Color getColor(final String source) {
        return Color.decode(source);
    }

    /**
     * Get top level window for correct dialog opening.
     * @return top-level frame.
     */
    public static Frame getTopLevel() {
        Container main = (Container) ScilabConsole.getConsole().getAsSimpleConsole();
        return (Frame) SwingUtilities.getAncestorOfClass(Frame.class, main);
    }

    /**
     * Set a dimension for a component.
     * @param component : the resized component.
     * @param peer : the node having the dimension information.
     */
    public static void setDimension(
        final Component component,
        final Node peer) {
        int height = XConfigManager.getInt(peer, "height", 0);
        int width = XConfigManager.getInt(peer, "width",  0);
        if (height > 0 && width > 0) {
            component.setPreferredSize(new Dimension(width, height));
        }
    }

    /**
     * Create a copy of Scilab configuration file in the user directory.
     */
    public static void createUserCopy(String original, String copy) {
        File fileConfig = new File(copy);
        if (!fileConfig.exists()) {
            refreshUserCopy(original, copy);
        }
    }

    /**
     * Refresh configuration file in the user directory with Scilab defaults.
     */
    public static void refreshUserCopy(String original, String copy) {
        /* Create a local copy of the configuration file */
        try {
            copyFile(new File(original), new File(copy));
        } catch (FileNotFoundException e) {
            System.err.println(ERROR_READ + copy);
        }
    }

    /**
     * Copy a file
     * @param in src file
     * @param out dest file
     * @throws FileNotFoundException
     */
    private static void copyFile(final File in, final File out) throws FileNotFoundException {
        FileInputStream fis = new FileInputStream(in);
        FileOutputStream fos = new FileOutputStream(out);

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
     * Read the file to modify
     */
    protected static Document readDocument(final String fileName) {
        File xml = new File(fileName);
        DocumentBuilder docBuilder = null;

        try {
            DocumentBuilderFactory factory = ScilabDocumentBuilderFactory.newInstance();
            docBuilder = factory.newDocumentBuilder();
            return docBuilder.parse(xml);
        } catch (ParserConfigurationException pce) {
            System.err.println(ERROR_READ + fileName);
        } catch (SAXException se) {
            System.err.println(ERROR_READ + fileName);
        } catch (IOException ioe) {
            System.err.println(ERROR_READ + fileName);
        }
        return null;
    }

    /**
     * Save the modifications
     */
    protected static void writeDocument(String filename, Node written) {
        Transformer transformer = null;
        try {
            transformer = ScilabTransformerFactory.newInstance().newTransformer();
        } catch (TransformerConfigurationException e1) {
            System.err.println(ERROR_WRITE + filename);
        } catch (TransformerFactoryConfigurationError e1) {
            System.err.println(ERROR_WRITE + filename);
        }
        transformer.setOutputProperty(OutputKeys.INDENT, "yes");

        StreamResult result = new StreamResult(new File(filename));
        DOMSource source = new DOMSource(written);
        try {
            transformer.transform(source, result);
        } catch (TransformerException e) {
            System.err.println(ERROR_WRITE + filename);
        }
    }
}
