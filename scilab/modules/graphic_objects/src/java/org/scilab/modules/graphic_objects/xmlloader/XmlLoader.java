/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_objects.xmlloader;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXES__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHILDREN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FIGURE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_CHECKBOX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_EDIT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_IMAGE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LAYER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_POPUPMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_PUSHBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RADIOBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TAB__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TEXT__;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.AbstractMap.SimpleEntry;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.Stack;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.graphic_objects.ScilabNativeView;
import org.scilab.modules.graphic_objects.builder.Builder;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.xml.sax.Attributes;
import org.xml.sax.Locator;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public class XmlLoader extends DefaultHandler {

    private String filename = "";
    private Locator locator;
    private Integer uid = 0;
    private String namespace = null;
    private boolean isWaitingModelName = true;
    private String myURI = "";
    private boolean isFirst = false;

    private static HashMap<String, Integer> nameToGO = new HashMap<String, Integer>();
    private static HashMap<String, HashMap<String, Entry<Integer, Map<String, String>>>> models = new HashMap<String, HashMap<String, Entry<Integer, Map<String, String>>>>();
    private Stack<Integer> stackGO = new Stack<Integer>();
    private String currentPath = "";

    private GraphicController controller;

    static {
        // init map to convert control name to id
        nameToGO.put("UIScilabWindow", __GO_FIGURE__);

        nameToGO.put("UIPanel", __GO_UI_FRAME__);
        nameToGO.put("UIFrame", __GO_UI_FRAME__);
        nameToGO.put("UILayer", __GO_UI_LAYER__);
        nameToGO.put("UIButton", __GO_UI_PUSHBUTTON__);
        nameToGO.put("UILabel", __GO_UI_TEXT__);
        nameToGO.put("UILaTeXLabel", -1);
        nameToGO.put("UICheckbox", __GO_UI_CHECKBOX__);
        nameToGO.put("UITextfield", __GO_UI_EDIT__);
        nameToGO.put("UISplashScreen", __GO_UI_IMAGE__);
        nameToGO.put("UIList", __GO_UI_LISTBOX__);
        nameToGO.put("UIComboBox", __GO_UI_POPUPMENU__);
        nameToGO.put("UIRadio", __GO_UI_RADIOBUTTON__);
        nameToGO.put("UISlider", __GO_UI_SLIDER__);
        nameToGO.put("UITab", __GO_UI_TAB__);
        nameToGO.put("UIScilabPlot", __GO_AXES__);

        /** sdsdf*/
        nameToGO.put("UITextarea", __GO_UI_PUSHBUTTON__);
        nameToGO.put("UIComboColor", __GO_UI_POPUPMENU__);

    }

    public XmlLoader(String filename) {
        controller = GraphicController.getController();
        this.filename = filename;
    }

    public int parse() throws SAXException {
        int ret = 0;
        if (filename != null && filename.equals("") == false) {
            ret = parse(filename);
        }

        // clean model before leave.
        Set<String> entries = models.keySet();
        for (String key : entries) {
            HashMap<String, Entry<Integer, Map<String, String>>> map = models.get(key);
            Set<String> entries2 = map.keySet();
            for (String key2 : entries2) {
                deleteObject(map.get(key2).getKey());
            }
        }

        return ret;
    }

    public int parse(String filename) throws SAXException {
        this.filename = filename;
        File f = new File(filename);
        if (f.exists()) {
            //add filename filepath in ScilabSwingUtilities paths
            String absoluteFilePath = f.getAbsolutePath();
            String path = absoluteFilePath.substring(0, absoluteFilePath.lastIndexOf(File.separator));
            currentPath = path;
            FindIconHelper.addThemePath(path);
        } else {
            //try to find file in currentPath
            if (f.isAbsolute()) {
                //failed
                return 1;
            }

            f = new File(currentPath + File.separator + filename);
            if (f.exists() == false) {
                return 1;
            }
        }

        FileInputStream in = null;
        try {
            in = new FileInputStream(f);
        } catch (FileNotFoundException e1) {
            e1.printStackTrace();
            return 1;
        }

        SAXParser parser;
        try {
            SAXParserFactory factory;
            factory = SAXParserFactory.newInstance();
            factory.setValidating(false);
            factory.setNamespaceAware(true);
            factory.setFeature("http://xml.org/sax/features/namespace-prefixes", true);
            parser = factory.newSAXParser();
        } catch (Exception e) {
            try {
                in.close();
            } catch (IOException e1) {
                e1.printStackTrace();
                return 1;
            }
            throw new SAXException(String.format("Cannot initialize the XML parser: %s", e.getMessage()));
        }

        try {
            parser.parse(in, this);
        } catch (SAXException e) {
            if (locator != null) {
                throw new SAXException(String.format("Parse error at line %d: %s", locator.getLineNumber(), e.getMessage()));
            } else {
                throw new SAXException(String.format("Parse error: %s", e.getMessage()));
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                in.close();
            } catch (IOException e) {
            }
        }

        return uid;
    }

    public void characters(char[] ch, int start, int length) throws SAXException {
        super.characters(ch, start, length);
    }

    public void endDocument() throws SAXException {
    }

    public void endElement(String uri, String localName, String qName) throws SAXException {

        if (localName.equals("modele")) {
            namespace = null;
            isWaitingModelName = false;
        }

        if (stackGO.size() != 0) {
            Integer go = stackGO.pop();
            if (go == null) {
            } else if (stackGO.size() > 0) {
                Integer parent = stackGO.peek();

                controller.setGraphicObjectRelationship(parent, go);
                //controller.setProperty(go, __GO_VISIBLE__, true);
            } else {
                uid = go;
            }
        }
    }

    public void setDocumentLocator(Locator locator) {
        this.locator = locator;
    }

    public void startDocument() throws SAXException {
        isFirst = true;
    }

    public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {

        if (localName.equals("interface")) {
            if (isFirst) {
                myURI = attributes.getValue("xmlns");
                isFirst = false;
            }
        } else if (localName.equals("include")) {
            String path = attributes.getValue("path");
            parse(path);
        } else if (localName.equals("modele")) {
            namespace = attributes.getValue("namespace");
            isWaitingModelName = true;
        } else {
            Integer uitype = getTypeFromName(localName);
            Integer go = 0;
            if (uitype != null) {
                if (uitype == __GO_FIGURE__) {
                    // never create a new figure, clone figure model !
                    go = GOBuilder.figureBuilder(controller, attributes);
                } else if (uitype == __GO_AXES__) {
                    go = GraphicController.getController().askObject(Type.AXES);
                } else {
                    int parent = 0;
                    if (stackGO.isEmpty() == false) {
                        parent = stackGO.peek();
                    }
                    go = GOBuilder.uicontrolBuilder(controller, uitype, attributes, parent);
                }
            } else { // namespace or bad name ...
                if (myURI.equals(uri)) {
                    // bad name
                } else {
                    HashMap<String, Entry<Integer, Map<String, String>>> m = models.get(uri);
                    if (m == null) {
                        // bad namespace
                    } else {
                        Entry<Integer, Map<String, String>> entry = m.get(localName);
                        go = entry.getKey();
                        // need to clone object and children
                        Integer newgo = cloneObject(go);
                        GOBuilder.uicontrolUpdater(controller, newgo, attributes, stackGO.peek(), entry.getValue());
                        go = newgo;

                    }
                }
            }

            // we are in a model
            // the first child must have field "model-name"
            if (namespace != null) {
                if (isWaitingModelName) {
                    String name = attributes.getValue("modele-name");
                    if (name == null) {
                        // unmaned model ? :s
                    } else {
                        if (models.get(namespace) == null) {
                            models.put(namespace, new HashMap<String, Entry<Integer, Map<String, String>>>());
                        }

                        // copy attributes to a Map, i do not know why a can use
                        // attributes ? Oo
                        Map<String, String> attrib = new HashMap<String, String>();
                        for (int i = 0; i < attributes.getLength(); i++) {
                            attrib.put(attributes.getLocalName(i), attributes.getValue(i));
                        }

                        // add root element in map model
                        models.get(namespace).put(name, new SimpleEntry<Integer, Map<String, String>>(go, attrib));
                    }
                }
            }

            stackGO.add(go);
        }
    }

    Integer cloneObject(Integer root) {
        Integer newGo = controller.cloneObject(root);
        Integer[] children = (Integer[]) controller.getProperty(root, __GO_CHILDREN__);
        for (int i = 0; i < children.length; i++) {
            if ((Integer)controller.getProperty(children[i], __GO_TYPE__) == __GO_AXES__) {
                Integer go = controller.cloneObject(GraphicModel.getAxesModel().getIdentifier());
                Builder.createLabel(go, GraphicObjectProperties.__GO_X_AXIS_LABEL__);
                Builder.createLabel(go, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);
                Builder.createLabel(go, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);
                Builder.createLabel(go, GraphicObjectProperties.__GO_TITLE__);

                controller.setGraphicObjectRelationship(newGo, go);
                controller.setProperty(newGo, GraphicObjectProperties.__GO_SELECTED_CHILD__, go);

                ScilabNativeView.ScilabNativeView__setCurrentSubWin(go);
                ScilabNativeView.ScilabNativeView__setCurrentObject(go);
            } else {
                Integer newChild = cloneObject(children[i]);
                controller.setGraphicObjectRelationship(newGo, newChild);
            }
        }

        return newGo;
    }

    private void deleteObject(Integer root) {
        Integer[] children = (Integer[]) controller.getProperty(root, __GO_CHILDREN__);
        for (int i = 0; i < children.length; i++) {
            deleteObject(children[i]);
        }

        controller.deleteObject(root);
    }

    private Integer getTypeFromName(String localName) {
        return nameToGO.get(localName);
    }
}
