/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uiwidget.go;

import java.awt.Component;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.gui.uiwidget.ObjectToScilabConverters;
import org.scilab.modules.gui.uiwidget.ScilabTypeConverters;
import org.scilab.modules.gui.uiwidget.ScilabTypeMap;
import org.scilab.modules.gui.uiwidget.UIAccessTools;
import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIWidgetException;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabTypeEnum;

public class UIWidgetGraphicObject extends GraphicObject {

    private static final Map<Integer, UIWidgetGraphicObject> GO = new HashMap<Integer, UIWidgetGraphicObject>();

    protected UIComponent uicomponent;

    public UIWidgetGraphicObject(UIComponent uicomponent) {
        this.uicomponent = uicomponent;
	uicomponent.setGO(this);
    }

    public static void delete(final int objID) {
	final UIWidgetGraphicObject go = GO.get(objID);
	if (go != null && go.uicomponent != null) {
	    UIAccessTools.execOnEDT(new Runnable() {
                public void run() {
                    go.uicomponent.remove();
		    go.uicomponent = null;
		    GO.remove(objID);
                }
            });
	}
    }

    public static void requestFocus(int objID) {
	final UIWidgetGraphicObject go = GO.get(objID);
	UIAccessTools.execOnEDT(new Runnable() {
		public void run() {
		    try {
			go.uicomponent.getContainer().requestFocus();
		    } catch (Exception e) { }
		}
	    });
    }

    public static GraphicObject getGO(List<ScilabType> args) throws Exception {
	String[] names = args.size() > 0 ? ((ScilabString) args.get(args.size() - 1)).getData()[0] : new String[0];
        ScilabTypeMap attributes = new ScilabTypeMap();
	UIComponent ui = null;
	UIComponent parent = null;
	
	for (int i = 0; i < names.length; i++) {
	    if (!names[i].isEmpty()) {
		final String attr = names[i].toLowerCase();
		final ScilabType arg = args.get(i);
		if (attr.equals("id") || attr.equals("tag") || attr.equals("style")) {
                    if (arg.getType() != ScilabTypeEnum.sci_strings || arg.isEmpty()) {
                        throw new Exception(String.format("Invalid %d-th argument: A String expected", i));
                    }
                    attributes.put(attr, arg);
		} else {
		    attributes.put(names[i], arg);
		}
	    }
	}
	
        if (!attributes.containsKey("style")) {
	    attributes.put("style", new ScilabString("pushbutton"));
        }

        String[] style = ScilabTypeConverters.getObjectFromValue(String[].class, attributes.get("style"));
        attributes.remove("style");

	if (style.length == 1) {
	    ui = UIComponent.getUIComponent("org.scilab.modules.gui.uiwidget.components", style[0], attributes, parent, null);
	} else {
	    //ui = UIModele.get(style[0], style[1], parent, attributes, null);
	    if (ui == null) {
		throw new Exception("Invalid modele name or uri.");
	    }
	}

	final UIComponent _ui = ui;
	UIAccessTools.execOnEDT(new Runnable() {
                public void run() {
                    _ui.finish();
                }
            });
	
        return new UIWidgetGraphicObject(ui);
    }

    public UpdateStatus setIdentifier(Integer id) {
        super.setIdentifier(id);
	uicomponent.replaceUID(id);
	uicomponent.setObjectMapping("getcallbackobject(" + Integer.toString(id) + ")");
        GO.put(id, this);

	return UpdateStatus.Success;
    }

    public UpdateStatus setParent(Integer parent) {
        super.setParent(parent);
        final UIWidgetGraphicObject pgo = GO.get(parent);
        if (pgo != null) {
	    UIAccessTools.execOnEDT(new Runnable() {
		    public void run() {
			if (uicomponent != null && pgo.uicomponent != null) {
			    try {
				uicomponent.setParent(pgo.uicomponent);
				pgo.uicomponent.finish();
			    } catch (Exception e) { }
			}
		    }
		});
        }

	return UpdateStatus.Success;
    }

    public void getProperty(Object property, int stackPos) {
	String p = ((String) property).toLowerCase();

        // The properties root, path & parent need to be quickly retrieve (no invokeAndWait)
        if (p.equals("root")) {
            ObjectToScilabConverters.putOnScilabStack(uicomponent.getRoot().getGO(), stackPos);
        } else if (p.equals("path")) {
            ObjectToScilabConverters.putOnScilabStack(uicomponent.getPath(), stackPos);
        } else if (p.equals("parent")) {
	    ObjectToScilabConverters.putOnScilabStack(GO.get(getParent()), stackPos);
        } else if (p.equals("id") || p.equals("tag")) {
            ObjectToScilabConverters.putOnScilabStack(uicomponent.getId(), stackPos);
        } else {
	    try {
		Object o = uicomponent.getProperty((String) property);
		ObjectToScilabConverters.putOnScilabStack(o, stackPos);
	    } catch (UIWidgetException e) {
		e.printStackTrace();
	    }
	}
    }

    /**
     * {@inheritDoc}
     */
    public UpdateStatus setProperty(List<ScilabType> args) {
	if (args.size() > 0) {
	    String[] names = ((ScilabString) args.get(args.size() - 1)).getData()[0];
	    args.remove(args.size() - 1);
	    List<String> list = new ArrayList<String>(names.length);
	    for (int i = 0; i < names.length; i++) {
		list.add(names[i]);
	    }
	    
	    try {
		uicomponent.setProperty(list, args);
	    } catch (Exception e) {
		return UpdateStatus.Fail;
	    }
	}

	return UpdateStatus.Success;
    }

    public void accept(Visitor visitor) throws ObjectRemovedException { }

    /**
     * {@inheritDoc}
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_UICONTROL__;
    }

    /**
     * {@inheritDoc}
     */
    public Component getComponent() {
        return (Component) uicomponent.getComponent();
    }
}
