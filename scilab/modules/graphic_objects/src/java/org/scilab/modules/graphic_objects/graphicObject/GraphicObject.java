/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Manuel JULIACHS
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.graphicObject;

import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.legend.Legend;

import java.util.LinkedList;
import java.util.List;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * GraphicObject class
 * @author Manuel JULIACHS
 * @author Vincent COUVERT
 */
public abstract class GraphicObject implements Cloneable {
    /** User data array default size */
    public static final int USER_DATA_DEFAULT_SIZE = 0;

    /** Graphic objects types */
    public enum Type { ARC, AXES, AXESMODEL, AXIS, CHAMP, COMPOUND, FAC3D, FEC, FIGURE, FIGUREMODEL, GRAYPLOT,
        LABEL, LEGEND, MATPLOT, PLOT3D, POLYLINE, RECTANGLE, SEGS, TEXT, CHECKBOX, EDIT, FRAME,
        IMAGERENDERER, LISTBOX, POPUPMENU, PUSHBUTTON, RADIOBUTTON, CONSOLE, JAVACONSOLE, SLIDER, TABLE, UITEXT, UIMENU, UNKNOWNOBJECT 
    };

    /** GraphicObject properties */
    public enum GraphicObjectPropertyType { PARENT, CHILDREN, CHILDREN_COUNT, HIDDEN, VISIBLE, USERDATA, USERDATASIZE, TYPE, REFERENCED, VALID, DATA,
        PARENT_FIGURE, PARENT_AXES, HASLEGENDCHILD, LEGENDCHILD, SELECTEDCHILD, TAG, CALLBACK, CALLBACKTYPE, UNKNOWNPROPERTY 
    };

    /** Identifier */
    private String identifier;

    /** Parent object is known by its UID */
    private String parent;

    /** Child objects list. Known by their UID */
    private List <String> children;

    /** Specifies whether the object is visible or not */
    private boolean visible;

    /** Specifies if the "handle" is referenced in scilab */
    private boolean referenced;

    /** Specifies if the "handle" is valid, i.e included in a rendered object */
    private boolean valid;

    /** Specifies if the "handle" is hidden, i.e not listed as children in Scilab view */
    private boolean hidden;

    /** User data */
    private Object userData;

    /** Tag */
    private String tag;

    /** Callback */
    private CallBack callback;

    /**
     * Identifier of the selected child
     * This was previously implemented as a list, but is used in practice
     * to store only the identifier of the currently selected child.
     * To do: use a list if required
     */
    private String selectedChild;

    /** Constructor */
    public GraphicObject() {
        identifier = null;
        parent = "";
        children = new LinkedList<String>();
        visible = true;
        userData = null;
        valid = true;
        referenced = false;
        selectedChild = "";
        tag = "";
        callback = new CallBack("");
    }

    /**
     * Clone
     * @return clone
     * @see java.lang.Object#clone()
     */
    public GraphicObject clone() {
        GraphicObject copy = null;

        try {
            copy = (GraphicObject) super.clone();
        } catch (CloneNotSupportedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        /*
         * Creating an empty list is done to avoid
         * still referencing the original object's own list,
         * which occurs when the Figure model is cloned.
         */
        copy.setChildren(new LinkedList<String>());

        /*
         * Avoids keeping the Figure model as a parent
         * when the Axes model is cloned.
         */
        copy.setParent("");

        /*
         * Sets no object as the selected child.
         */
        copy.setSelectedChild("");

        return (GraphicObject) copy;
    }

    abstract public void accept(IVisitor visitor);

    /**
     * Returns the enum associated to a type name
     * @param typeName the property name
     * @return the type enum
     */
    public static Type getTypeFromName(String typeName) {
        if (typeName.equals(__GO_ARC__)) {
            return Type.ARC;
        } else if (typeName.equals(__GO_AXES__)) {
            return Type.AXES;
        } else if (typeName.equals(__GO_AXESMODEL__)) {
            return Type.AXESMODEL;
        } else if (typeName.equals(__GO_AXIS__)) {
            return Type.AXIS;
        } else if (typeName.equals(__GO_CHAMP__)) {
            return Type.CHAMP;
        } else if (typeName.equals(__GO_COMPOUND__)) {
            return Type.COMPOUND;
        } else if (typeName.equals(__GO_FAC3D__)) {
            return Type.FAC3D;
        } else if (typeName.equals(__GO_FEC__)) {
            return Type.FEC;
        } else if (typeName.equals(__GO_FIGURE__)) {
            return Type.FIGURE;
        } else if (typeName.equals(__GO_FIGUREMODEL__)) {
            return Type.FIGUREMODEL;
        } else if (typeName.equals(__GO_GRAYPLOT__)) {
            return Type.GRAYPLOT;
        } else if (typeName.equals(__GO_LABEL__)) {
            return Type.LABEL;
        } else if (typeName.equals(__GO_LEGEND__)) {
            return Type.LEGEND;
        } else if (typeName.equals(__GO_MATPLOT__)) {
            return Type.MATPLOT;
        } else if (typeName.equals(__GO_PLOT3D__)) {
            return Type.PLOT3D;
        } else if (typeName.equals(__GO_POLYLINE__)) {
            return Type.POLYLINE;
        } else if (typeName.equals(__GO_RECTANGLE__)) {
            return Type.RECTANGLE;
        } else if (typeName.equals(__GO_SEGS__)) {
            return Type.SEGS;
        } else if (typeName.equals(__GO_TEXT__)) {
            return Type.TEXT;
        } else if (typeName.equals(__GO_UI_CHECKBOX__)) {
            return Type.CHECKBOX;
        } else if (typeName.equals(__GO_UI_EDIT__)) {
            return Type.EDIT;
        } else if (typeName.equals(__GO_UI_FRAME__)) {
            return Type.FRAME;
        } else if (typeName.equals(__GO_UI_IMAGERENDERER__)) {
            return Type.IMAGERENDERER;
        } else if (typeName.equals(__GO_UI_LISTBOX__)) {
            return Type.LISTBOX;
        } else if (typeName.equals(__GO_UI_POPUPMENU__)) {
            return Type.POPUPMENU;
        } else if (typeName.equals(__GO_UI_PUSHBUTTON__)) {
            return Type.PUSHBUTTON;
        } else if (typeName.equals(__GO_UI_RADIOBUTTON__)) {
            return Type.RADIOBUTTON;
        } else if (typeName.equals(__GO_UI_SLIDER__)) {
            return Type.SLIDER;
        } else if (typeName.equals(__GO_UI_TABLE__)) {
            return Type.TABLE;
        } else if (typeName.equals(__GO_UI_TEXT__)) {
            return Type.UITEXT;
        } else if (typeName.equals(__GO_UIMENU__)) {
            return Type.UIMENU;
        } else {
            return Type.UNKNOWNOBJECT;
        }
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(String propertyName) {
        if (propertyName.equals(__GO_PARENT__)) {
            return  GraphicObjectPropertyType.PARENT;
        } else if (propertyName.equals(__GO_CHILDREN__)) {
            return GraphicObjectPropertyType.CHILDREN;
        } else if (propertyName.equals(__GO_CHILDREN_COUNT__)) {
            return GraphicObjectPropertyType.CHILDREN_COUNT;
        } else if (propertyName.equals(__GO_HIDDEN__)) {
            return GraphicObjectPropertyType.HIDDEN;
        } else if (propertyName.equals(__GO_VISIBLE__)) {
            return GraphicObjectPropertyType.VISIBLE;
        } else if (propertyName.equals(__GO_USER_DATA__)) {
            return GraphicObjectPropertyType.USERDATA;
        } else if (propertyName.equals(__GO_USER_DATA_SIZE__)) {
            return GraphicObjectPropertyType.USERDATASIZE;
        } else if (propertyName.equals(__GO_REFERENCED__)) {
            return GraphicObjectPropertyType.REFERENCED;
        } else if (propertyName.equals(__GO_VALID__)) {
            return GraphicObjectPropertyType.VALID;
        } else if (propertyName.equals(__GO_PARENT_FIGURE__)) {
            return GraphicObjectPropertyType.PARENT_FIGURE;
        } else if (propertyName.equals(__GO_PARENT_AXES__)) {
            return GraphicObjectPropertyType.PARENT_AXES;
        } else if (propertyName.equals(__GO_PARENT_AXES__)) {
            return GraphicObjectPropertyType.PARENT_AXES;
        } else if (propertyName.equals(__GO_HAS_LEGEND_CHILD__)) {
            return GraphicObjectPropertyType.HASLEGENDCHILD;
        } else if (propertyName.equals(__GO_LEGEND_CHILD__)) {
            return GraphicObjectPropertyType.LEGENDCHILD;
        } else if (propertyName.equals(__GO_SELECTED_CHILD__)) {
            return GraphicObjectPropertyType.SELECTEDCHILD;
        } else if (propertyName.equals(__GO_TYPE__)) {
            return GraphicObjectPropertyType.TYPE;
        }  else if (propertyName.equals(__GO_DATA_MODEL__)) {
            return GraphicObjectPropertyType.DATA;
        }  else if (propertyName.equals(__GO_TAG__)) {
            return GraphicObjectPropertyType.TAG;
        }  else if (propertyName.equals(__GO_CALLBACK__)) {
            return GraphicObjectPropertyType.CALLBACK;
        }  else if (propertyName.equals(__GO_CALLBACKTYPE__)) {
            return GraphicObjectPropertyType.CALLBACKTYPE;
        }  else {
            return GraphicObjectPropertyType.UNKNOWNPROPERTY;
        }
    }

    /**
     * Fast property get method
     * @param property the property to get
     * @return the property value
     */
    public Object getProperty(Object property) {
        if (property == GraphicObjectPropertyType.PARENT) {
            return getParent();
        } else if (property == GraphicObjectPropertyType.CHILDREN) {
            return getChildren();
        } else if (property == GraphicObjectPropertyType.CHILDREN_COUNT) {
            return getChildren().length;
        } else if (property == GraphicObjectPropertyType.VALID) {
            return isValid();
        } else if (property == GraphicObjectPropertyType.HIDDEN) {
            return isHidden();
        } else if (property == GraphicObjectPropertyType.VISIBLE) {
            return getVisible();
        } else if (property == GraphicObjectPropertyType.USERDATA) {
            return getUserData();
        } else if (property == GraphicObjectPropertyType.USERDATASIZE) {
            return getUserDataSize();
        } else if (property == GraphicObjectPropertyType.PARENT_FIGURE) {
            return getParentFigure();
        } else if (property == GraphicObjectPropertyType.PARENT_AXES) {
            return getParentAxes();
        } else if (property == GraphicObjectPropertyType.HASLEGENDCHILD) {
            return getHasLegendChild();
        } else if (property == GraphicObjectPropertyType.LEGENDCHILD) {
            return getLegendChild();
        } else if (property == GraphicObjectPropertyType.SELECTEDCHILD) {
            return getSelectedChild();
        } else if (property == GraphicObjectPropertyType.TYPE) {
            return getType();
        }  else if (property == GraphicObjectPropertyType.DATA) {
            return getIdentifier();
        }  else if (property == GraphicObjectPropertyType.TAG) {
            return getTag();
        }  else if (property == GraphicObjectPropertyType.CALLBACK) {
            return getCallbackString();
        }  else if (property == GraphicObjectPropertyType.CALLBACKTYPE) {
            return getCallbackType();
        }  else if (property == GraphicObjectPropertyType.UNKNOWNPROPERTY) {
            return null;
        } else {
            return null;
        }
    }

    /**
     * Fast property set method
     * @param property the property to set
     * @param value the property value
     * @return true if the property has been set, false otherwise
     */
    public boolean setProperty(Object property, Object value) {
        if (property == GraphicObjectPropertyType.PARENT) {
            setParent((String) value);
        } else if (property == GraphicObjectPropertyType.CHILDREN) {
            setChildren((List<String>) value);
        } else if (property == GraphicObjectPropertyType.VALID) {
            setValid((Boolean) value);
        } else if (property == GraphicObjectPropertyType.HIDDEN) {
            setHidden((Boolean) value);
        } else if (property == GraphicObjectPropertyType.VISIBLE) {
            setVisible((Boolean) value);
        } else if (property == GraphicObjectPropertyType.USERDATA) {
            setUserData(value);
        } else if (property == GraphicObjectPropertyType.USERDATASIZE) {
            return false;
        } else if (property == GraphicObjectPropertyType.SELECTEDCHILD) {
            setSelectedChild((String) value);
        } else if (property == GraphicObjectPropertyType.DATA) {
            return true;
        } else if (property == GraphicObjectPropertyType.TAG) {
            setTag((String) value);
        } else if (property == GraphicObjectPropertyType.CALLBACK) {
            setCallbackString((String) value);
        } else if (property == GraphicObjectPropertyType.CALLBACKTYPE) {
            setCallbackType((Integer) value);
        } else if (property == GraphicObjectPropertyType.UNKNOWNPROPERTY) {
            return false;
        }

        return true;
    }

    /**
     * Returns a null property
     * @param property property name
     * @return null property
     */
    public Object getNullProperty(String property) {
        return null;
    }

    /**
     * Void property get method
     * @param property the property name
     */
    public Object getPropertyVoid(String property) {
        // TODO
        return null;
    }

    /* TODO */
    /**
     * Void property set method
     * @param property the property name
     * @param value the property value
     */
    public void setPropertyVoid(String property, Object value) {
        // TODO
    }

    /**
     * @return the children
     */
    public String[] getChildren() {
        return children.toArray(new String[children.size()]);
    }

    /**
     * Adds a child.
     * @param child the identifier of the added child.
     */
    public void addChild(String child) {
        if (!children.contains(child)) {
            children.add(0, child);
        }
    }

    /**
     * Removes a child.
     * @param child the identifier of the removed child.
     */
    public void removeChild(String child) {
        children.remove(child);
    }

    /**
     * @param children the children to set
     */
    public void setChildren(List<String> children) {
        this.children = children;
    }

    /**
     * @return the identifier
     */
    public String getIdentifier() {
        return identifier;
    }

    /**
     * @param identifier the identifier to set
     */
    public void setIdentifier(String identifier) {
        this.identifier = identifier;
    }

    /**
     * @return the parent
     */
    public String getParent() {
        return parent;
    }

    /**
     * @param parent the parent to set
     */
    public void setParent(String parent) {
        this.parent = parent;
    }

    /**
     * @return the userData
     */
    public Object getUserData() {
        return userData;
    }

    /**
     * @param userData the userData to set
     */
    public void setUserData(Object userData) {
        this.userData = userData;
    }

    /**
     * @return the userDataSize
     */
    public Integer getUserDataSize() {
        return 0;
    }

    /**
     * @return the tag
     */
    public String getTag() {
        return tag;
    }

    /**
     * @param tag the tag to set
     */
    public void setTag(String tag) {
        this.tag = tag;
    }

    /**
     * @return the callback
     */
    public String getCallbackString() {
        return callback.getCommand();
    }

    /**
     * @param callback the callback to set
     */
    public void setCallbackString(String callback) {
        this.callback.setCommand(callback);
    }

    /**
     * @return the callbackType
     */
    public Integer getCallbackType() {
        return callback.getCommandType();
    }

    /**
     * @param callbackType the callbackType to set
     */
    public void setCallbackType(Integer callbackType) {
        this.callback.setCommandType(callbackType);
    }

    /**
     * Get parent Figure method
     * Returns the identifier of the object's parent Figure
     * If the object is a Figure, then returns its own identifier.
     * To be done: use a member variable storing the up-to-date current parent Figure,
     * returned instead of recursively ascending the hierarchy at each call.
     * @return the parent Figure identifier
     */
    public String getParentFigure() {
        if (this instanceof Figure) {
            return getIdentifier();
        } else {
            if (getParent() != null && GraphicController.getController().getObjectFromId(getParent()) != null) {
                return GraphicController.getController().getObjectFromId(getParent()).getParentFigure();
            } else {
                return null;
            }
        }
    }

    /**
     * Get parent Axes method
     * Returns the identifier of the object's parent Axes
     * If the object is an Axes, then returns its own identifier.
     * To be done: use a member variable storing the up-to-date current parent Axes,
     * returned instead of recursively ascending the hierarchy at each call.
     * @return the parent Axes identifier
     */
    public String getParentAxes() {
        if (this instanceof Axes) {
            return getIdentifier();
        } else {
            if (getParent() != null && GraphicController.getController().getObjectFromId(getParent()) != null) {
                return GraphicController.getController().getObjectFromId(getParent()).getParentAxes();
            } else {
                return null;
            }
        }
    }

    /**
     * Get has legend child method
     * Returns a boolean indicating whether one of the object's direct children
     * is a Legend object. Only one Legend is supposed to be present in the list.
     * To be done: storing the property and updating it only when a Legend object
     * is inserted or deleted instead of searching the children list when the
     * property is queried.
     * @return a Boolean indicating whether the object has a child Legend object or not
     */
    public Boolean getHasLegendChild() {
        for (int i = 0; i < children.size(); i++) {
            GraphicObject currentObject = GraphicController.getController().getObjectFromId(children.get(i));

            if (currentObject instanceof Legend) {
                return true;
            }
        }

        return false;
    }

    /**
     * Returns the identifier of the object's direct child that is a Legend object.
     * It returns an empty string if the object has no Legend in its children list
     * (one Legend is supposed to be present at most).
     * @return the object's child Legend object identifier, or an empty string if no child Legend found.
     */
    public String getLegendChild() {
        for (int i = 0; i < children.size(); i++) {
            GraphicObject currentObject = GraphicController.getController().getObjectFromId(children.get(i));

            if (currentObject instanceof Legend) {
                return currentObject.getIdentifier();
            }
        }

        /* No child legend found */
        return "";
    }

    /**
     * Get selected child method
     * @return the selected child
     */
    public String getSelectedChild() {
        return selectedChild;
    }

    /**
     * Set selected child method
     * @param selectedChild the selected child to set
     */
    public void setSelectedChild(String selectedChild) {
        this.selectedChild = selectedChild;
    }

    /**
     * Get visible method
     * @return the visible
     */
    public Boolean getVisible() {
        return visible;
    }

    /**
     * Set visible method
     * @param visible the visible to set
     */
    public void setVisible(Boolean visible) {
        this.visible = visible;
    }

    /**
     * Each type should name itself
     * @return Type as String
     */
    // TODO : Should be public abstract.
    //public abstract String getType();
    public String getType() {
        return "???";
    }

    /**
     * isValid method
     * @return valid
     */
    public Boolean isValid() {
        return valid;
    }

    /**
     * Set hidden method
     * @param hidden the value to set
     */
    public void setHidden(Boolean hidden) {
        this.hidden = hidden;
    }

    /**
     * isHidden method
     * @return hidden
     */
    public Boolean isHidden() {
        return hidden;
    }

    /**
     * Set valid method
     * @param valid the validity to set
     */
    public void setValid(Boolean valid) {
        this.valid = valid;
    }

    /**
     * isReferenced method
     * @return referenced 
     */
    public Boolean isReferenced() {
        return referenced;
    }

    /**
     * Set referenced method
     * @param referenced the reference status to set
     */
    public void setReferenced(Boolean referenced) {
        this.referenced = referenced;
    }
}
