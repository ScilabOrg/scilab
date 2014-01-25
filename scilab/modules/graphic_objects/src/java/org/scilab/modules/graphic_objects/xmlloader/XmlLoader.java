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

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHILDREN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FIGURE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_CHECKBOX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_EDIT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_IMAGE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_POPUPMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_PUSHBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RADIOBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TABGROUP__;
//import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TAB__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TEXT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import java.io.File;
import java.io.IOException;
import java.util.AbstractMap.SimpleEntry;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.Stack;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
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
    private static HashMap<String, HashMap<String, Entry<Integer, String>>> models = new HashMap<String, HashMap<String, Entry<Integer, String>>>();
    private Stack<Integer> stackGO = new Stack<Integer>();

    private GraphicController controller;

    static {
        //init map to convert control name to id
        nameToGO.put("UIScilabWindow", __GO_FIGURE__);
        nameToGO.put("Figure", __GO_FIGURE__);

        nameToGO.put("UIPanel", __GO_UI_FRAME__);
        nameToGO.put("UIFrame", __GO_UI_FRAME__);
        nameToGO.put("UILayer", __GO_UI_FRAME__);
        nameToGO.put("Panel", __GO_UI_FRAME__);
        nameToGO.put("Frame", __GO_UI_FRAME__);
        nameToGO.put("Layer", __GO_UI_FRAME__);

        nameToGO.put("UIButton", __GO_UI_PUSHBUTTON__);
        nameToGO.put("PushButton", __GO_UI_PUSHBUTTON__);
        nameToGO.put("Button", __GO_UI_PUSHBUTTON__);

        nameToGO.put("UILabel", __GO_UI_TEXT__);
        nameToGO.put("Label", __GO_UI_TEXT__);
        nameToGO.put("Text", __GO_UI_TEXT__);

        nameToGO.put("UICheckbox", __GO_UI_CHECKBOX__);
        nameToGO.put("CheckBox", __GO_UI_CHECKBOX__);

        nameToGO.put("UITextfield", __GO_UI_EDIT__);
        nameToGO.put("Textfield", __GO_UI_EDIT__);
        nameToGO.put("EditBox", __GO_UI_EDIT__);
        nameToGO.put("Edit", __GO_UI_EDIT__);

        nameToGO.put("UISplashScreen", __GO_UI_IMAGE__);
        nameToGO.put("Image", __GO_UI_IMAGE__);
        nameToGO.put("Picture", __GO_UI_IMAGE__);

        nameToGO.put("UIList", __GO_UI_LISTBOX__);
        nameToGO.put("List", __GO_UI_LISTBOX__);
        nameToGO.put("ListBox", __GO_UI_LISTBOX__);

        nameToGO.put("UICombobox", __GO_UI_POPUPMENU__);
        nameToGO.put("ComboBox", __GO_UI_POPUPMENU__);
        nameToGO.put("PopupMenu", __GO_UI_POPUPMENU__);

        nameToGO.put("UIRadio", __GO_UI_RADIOBUTTON__);
        nameToGO.put("RadioButton", __GO_UI_RADIOBUTTON__);
        nameToGO.put("Radio", __GO_UI_RADIOBUTTON__);

        nameToGO.put("UISlider", __GO_UI_SLIDER__);
        nameToGO.put("Slider", __GO_UI_SLIDER__);

        nameToGO.put("Table", __GO_UI_TABLE__);

        nameToGO.put("UILabel", __GO_UI_TEXT__);
        nameToGO.put("Text", __GO_UI_TEXT__);
        nameToGO.put("Label", __GO_UI_TEXT__);

        nameToGO.put("UITab", __GO_UI_TABGROUP__);

        //UiPanel -> __GO_UI_TAB__ or __GO_UI_FRAME__
    }

    public XmlLoader(String filename) {
        controller = GraphicController.getController();
        this.filename = filename;
    }

    public int parse() throws SAXException {
        if (filename != null && filename.equals("") == false) {
            return parse(filename);
        }

        return 0;
    }

    public int parse(String filename) throws SAXException {
        this.filename = filename;
        File f = new File(filename);

        SAXParser parser;
        try {
            SAXParserFactory factory;
            factory = SAXParserFactory.newInstance();
            factory.setValidating(false);
            factory.setNamespaceAware(true);
            factory.setFeature("http://xml.org/sax/features/namespace-prefixes", true);
            parser = factory.newSAXParser();
        } catch (Exception e) {
            throw new SAXException(String.format("Cannot initialize the XML parser: %s", e.getMessage()));
        }

        try {
            parser.parse(f, this);
        } catch (SAXException e) {
            if (locator != null) {
                throw new SAXException(String.format("Parse error at line %d: %s", locator.getLineNumber(), e.getMessage()));
            } else {
                throw new SAXException(String.format("Parse error: %s", e.getMessage()));
            }
        } catch (IOException e) {
            e.printStackTrace();
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
        }

        if (stackGO.size() != 0) {
            Integer go = stackGO.pop();
            if (go == null) {
                disp("heu ....");
            } else if (stackGO.size() > 0) {
                Integer parent = stackGO.peek();
                disp(go + " -> " + parent);
                controller.setGraphicObjectRelationship(parent, go);
                controller.setProperty(go,  __GO_VISIBLE__, true);
            } else {
                disp("end : " + go);
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

    public void startElement(String uri, String localName, String qName,
                             Attributes attributes) throws SAXException {

        if (localName.equals("interface")) {
            if (isFirst) {
                myURI =  attributes.getValue("xmlns");
                isFirst = false;
            }
        } else if (localName.equals("include")) {
            String path = attributes.getValue("path");
            parse(path);
        } else if (localName.equals("modele")) {
            namespace = attributes.getValue("namespace");
            disp("namespace : " + namespace);
            isWaitingModelName = true;
        } else {
            Integer uitype = getTypeFromName(localName);
            Integer go = 0;
            if (uitype != null && uitype.intValue() > 0) {
                if (uitype == __GO_FIGURE__) {
                    //never create a new figure, clone figure model !
                    go = GOBuilder.figureBuilder(controller, attributes);
                } else {
                    int parent = 0;
                    if (stackGO.isEmpty() == false) {
                        parent = stackGO.peek();
                    }
                    go = GOBuilder.uicontrolBuilder(controller, uitype, attributes, parent);
                }
            } else { //namespace or bad name ...
                if (myURI.equals(uri)) {
                    //bad name
                } else {
                    HashMap<String, Entry<Integer, String>> m = models.get(uri);
                    if (m == null) {
                        //bad namespace
                    } else {
                        Entry<Integer, String> entry = m.get(localName);
                        go = entry.getKey();
                        //need to clone object and children
                        Integer newgo = cloneObject(go);
                        GOBuilder.uicontrolUpdater(controller, newgo, attributes, stackGO.peek(), entry.getValue());
                        disp("clone " + go + " from model to " + newgo);
                        go = newgo;

                    }
                }
            }

            disp("create " + localName + " id " + go);

            // we are in a model
            // the first child must have field "model-name"
            if (namespace != null) {
                if (isWaitingModelName) {
                    String name = attributes.getValue("modele-name");
                    if (name == null) {
                        disp("KABOOM !");
                    } else {
                        disp("add model : " + name);

                        if (models.get(namespace) == null) {
                            models.put(namespace, new HashMap<String, Entry<Integer, String>>());
                        }
                        // add root element in map model
                        disp("add to model : (" + namespace + "] = (" + name + ", " + go + ")");

                        disp("constraint to model : " + attributes.getValue("constraint"));
                        models.get(namespace).put(name, new SimpleEntry<Integer, String>(go, attributes.getValue("constraint")));
                        isWaitingModelName = false;
                    }
                }
            }

            stackGO.add(go);
        }
    }

    Integer cloneObject(Integer root) {
        Integer newGo = controller.cloneObject(root);
        Integer[] children = (Integer[])controller.getProperty(root, __GO_CHILDREN__);
        for (int i = 0 ; i < children.length ; i++) {
            Integer newChild = cloneObject(children[i]);
            //disp("clone " + children[i] + " to " + newChild);
            controller.setGraphicObjectRelationship(newGo, newChild);
        }

        return newGo;
    }

    private Integer getTypeFromName(String localName) {
        return nameToGO.get(localName);
    }


    void disp(String s) {
        disp(stackGO.size(), s);
    }

    void disp(Integer tabs, String s) {
        //		for(int i = 0 ; i < tabs ; i++) {
        //			System.out.print("    ");
        //		}
        //
        //		System.out.println(s);
    }
}
