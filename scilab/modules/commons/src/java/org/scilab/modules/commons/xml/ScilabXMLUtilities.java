/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.commons.xml;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.util.Map;
import java.util.StringTokenizer;

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

import org.w3c.dom.Attr;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import org.xml.sax.SAXException;

import org.scilab.modules.localization.Messages;

/**
 * Class to provide different tools to manage xml.
 *
 * @author Calixte DENIZET
 */
public class ScilabXMLUtilities {

    private static final String ENCODING = "UTF-8";
    private static final String ERROR_WRITE = Messages.gettext("Could not save file: ");
    private static final String ERROR_READ = Messages.gettext("Could not load file: ");
    private static final Integer INDENTATION = new Integer(4);

    private static Transformer transformer;

    /**
     * Read a xml file and parse it
     * @param fileToRead the file to read
     * @return the corresponding document
     */
    public static Document readDocument(String fileToRead) {
        Document doc = null;
        try {
            DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
            DocumentBuilder docBuilder = factory.newDocumentBuilder();
            doc = docBuilder.parse(new File(fileToRead));
        } catch (ParserConfigurationException e) {
            System.err.println(ERROR_READ + fileToRead);
        } catch (SAXException e) {
            System.err.println(ERROR_READ + fileToRead);
        } catch (IOException e) {
            System.err.println(ERROR_READ + fileToRead);
        }

        return doc;
    }

    /**
     * Write a Document in a file. The empty lines are removed and the xml code is indented
     * @param doc the document to write
     * @param fileName the file name
     */
    public static void writeDocument(Document doc, String fileName) {
        if (transformer == null) {
            initTransformer();
        }

        if (transformer != null) {
            removeEmptyLines(doc.getDocumentElement());

            FileOutputStream fos = null;
            OutputStreamWriter osw = null;

            try {
                /* FileOutputStream & OutputStreamWriter are needed to be sure that
                   the indentation will be correct (known Java bug) */

                fos = new FileOutputStream(fileName, false);
                osw = new OutputStreamWriter(fos, ENCODING);
                transformer.transform(new DOMSource(doc), new StreamResult(osw));
            } catch (FileNotFoundException e) {
                System.err.println(ERROR_WRITE + fileName);
                System.err.println(e);
            } catch (UnsupportedEncodingException e) {
                System.err.println(ERROR_WRITE + fileName);
                System.err.println(e);
            } catch (TransformerException e) {
                System.err.println(ERROR_WRITE + fileName);
                System.err.println(e);
            } finally {
                if (osw != null) {
                    try {
                        osw.close();
                    } catch (IOException ex) {
                        System.err.println(ex);
                    }
                }
            }
        }
    }

    /**
     * Remove empty lines which are a descendant of node
     * @param node the initial node
     */
    public static void removeEmptyLines(Node node) {
        if (node != null) {
            if (node.getNodeType() == Node.TEXT_NODE) {
                node.getParentNode().removeChild(node);
            } else {
                NodeList list = node.getChildNodes();
                int length = 0;
                try {
                    length = list.getLength();
                } catch (NullPointerException e) {
                    /* Avoid Java bug */
                }

                for (int i = 0; i < length; i++) {
                    removeEmptyLines(list.item(i));
                }
            }
        }
    }

    /**
     * Create a new node with parent element
     * @param parent the parent element
     * @param nodeName the node name
     * @param attr an array containing attribute name followed by its value: "attr1", 1, "attr2", true, ...
     * @return the created element
     */
    public static Element createNode(Document doc, Element parent, String nodeName, Object[] attr) {
        Element elem = doc.createElement(nodeName);
        for (int i = 0; i < attr.length; i += 2) {
            elem.setAttribute(attr[i].toString(), attr[i + 1].toString());
        }
        parent.appendChild(elem);

        return elem;
    }

    /**
     * Create a new node with parent element
     * @param parent the parent element
     * @param nodeName the node name
     * @param map a map containing {attributes -> value}, the method value.toString() will be used.
     * @return the created element
     */
    public static Element createNode(Document doc, Element parent, String nodeName, Map<String, Object> map) {
        Element elem = doc.createElement(nodeName);
        for (String name : map.keySet()) {
            elem.setAttribute(name, map.get(name).toString());
        }
        parent.appendChild(elem);

        return elem;
    }

    /**
     * Read the attributes of elem.
     * Map must be like this: "attr1" -> integer.class, "attr2" -> boolean.class, ...
     * Map will be filled with the value (as Object) of the different attributes
     * @param elem the element to analyze
     * @param map the map containing the attributes type.
     */
    public static void readNodeAttributes(Element elem, Map<String, Object> map) {
        NamedNodeMap attrMap = elem.getAttributes();
        for (int i = 0; i < attrMap.getLength(); i++) {
            Attr attr = (Attr) attrMap.item(i);
            String key = attr.getName();
            if (map.containsKey(key)) {
                map.put(key, convert(attr.getValue(), (Class) map.get(key)));
            }
        }
    }

    /**
     * Read the attributes of elem.
     * Map must be like this: Object[]{"attr1", integer.class, "attr2", boolean.class, ...}
     * Map will be filled with the value (as Object) of the different attributes
     * @param elem the element to analyze
     * @param map the map containing the attributes type.
     */
    public static void readNodeAttributes(Element elem, Object[] map) {
        NamedNodeMap attrMap = elem.getAttributes();
        for (int i = 0; i < attrMap.getLength(); i++) {
            Attr attr = (Attr) attrMap.item(i);
            String key = attr.getName();
            for (int j = 0; j < map.length; j += 2) {
		if (map[j].equals(key)) {
		    map[j + 1] = convert(attr.getValue(), (Class) map[j + 1]);
		}
	    }
        }
    }

    /**
     * Convert a value (as String) into an object according to its class type giving in clazz
     * @param value the value to convert
     * @param clazz the class type of the value
     * @return the converted object
     */
    private static final Object convert(String value, Class clazz) {
        if (clazz == int.class) {
            return Integer.parseInt(value);
        } else if (clazz == float.class) {
            return Float.parseFloat(value);
        } else if (clazz == boolean.class) {
            return Boolean.parseBoolean(value);
        } else if (clazz == double.class) {
            return Double.parseDouble(value);
        } else if (clazz == short.class) {
            return Short.parseShort(value);
        } else if (clazz == long.class) {
            return Long.parseLong(value);
        } else if (clazz == byte.class) {
            return Byte.parseByte(value);
        } else if (clazz == char.class) {
            return value.length() == 0 ? '\0' : value.charAt(0);
        } else if (clazz == String[].class) {
	    return getArray(value, ";");
        }

        return value;
    }

    /**
     * Get an array from a string containing strings separated with a delimitor
     * @param value the string to parse
     * @param delimitor the delimitor
     * @return an array of String
     */
    private static final String[] getArray(String value, String delimitor) {
	StringTokenizer tokens = new StringTokenizer(value, delimitor);
	String[] arr = new String[tokens.countTokens()];
	for (int i = 0; i < arr.length; i++) {
	    arr[i] = tokens.nextToken();
	}
	
	return arr;
    }

    /**
     * Init the transformer
     */
    private static final void initTransformer() {
        try {
            TransformerFactory factory = ScilabTransformerFactory.newInstance();
            factory.setAttribute("indent-number", INDENTATION);
            transformer = factory.newTransformer();
            transformer.setOutputProperty(OutputKeys.INDENT, "yes");
            transformer.setOutputProperty(OutputKeys.OMIT_XML_DECLARATION, "no");
            transformer.setOutputProperty(OutputKeys.ENCODING, ENCODING);
        } catch (TransformerConfigurationException e) {
            System.err.println("Problem to init the Transformer to write xml files");
            System.err.println(e);
        } catch (TransformerFactoryConfigurationError e) {
            System.err.println("Problem to init the Transformer to write xml files");
            System.err.println(e);
        }
    }
}
