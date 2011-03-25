/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FIGURE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTROL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_PUSHBUTTON__;

import java.util.HashMap;
import java.util.Map;

import org.flexdock.docking.DockingManager;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;

public class SwingView implements GraphicView {

    private static SwingView me = null;;
    
    public static void registerSwingView()
    {
        System.err.println("[DEBUG] calling registerSwingView()");
        if (me == null) {
            me = new SwingView();
        }
    }
    
    private SwingView() {
        GraphicController.getController().register(this);
        allObjects = new HashMap<String, TypedObject>();
    }
    
    private enum UielementType {
        Figure,
        PushButton
    } 
    
    private class TypedObject {
        private UielementType _type;
        private Object        _value;
        
        public TypedObject(UielementType _type, Object _value) {
            this._type = _type;
            this._value = _value;
        }
        
        public UielementType getType() {
            return _type;
        }
        
        public Object getValue() {
            return _value;
        }
    }
    
    private Map<String, TypedObject> allObjects;
    
    public void createObject(String id) {
        
        String objectType = (String) GraphicController.getController().getProperty(id, __GO_TYPE__);
        System.err.println("[SWINGVIEW] Object Created : " + id);
        System.err.println("[SWINGVIEW] with type : " + objectType);
        if(objectType.equals(__GO_FIGURE__)) {
            allObjects.put(id, CreateObjectFromType(objectType));
            return;
        }
        
        if(objectType.equals(__GO_UICONTROL__)) {
            String style = (String) GraphicController.getController().getProperty(id, __GO_STYLE__);
            allObjects.put(id, CreateObjectFromType(style));
        }
    }

    private UielementType StyleToEnum(String style) {
        if(style.equals(__GO_UI_PUSHBUTTON__)) {
            return UielementType.PushButton;
        }
        if(style.equals(__GO_FIGURE__)) {
            return UielementType.Figure;
        }
        return null;
    }

    private TypedObject CreateObjectFromType(String type) {
        UielementType enumType = StyleToEnum(type);
        return new TypedObject(enumType, CreateObjectFromType(enumType));
    }

    private Object CreateObjectFromType(UielementType type) {
        switch (type) {
        case Figure:
           SwingScilabWindow win = new SwingScilabWindow();
           SwingScilabTab tab = new SwingScilabTab("");
           DockingManager.dock(tab, win.getDockingPort());
           return tab;
        case PushButton:
            return new SwingScilabPushButton();
        default:
            return null;
        }
    }
    
    public void deleteObject(String id) {
        TypedObject requestedObject = allObjects.get(id);
        
        if (requestedObject) {
            
        }
    }

    public void updateObject(String id, String property) {
        
    }

}
