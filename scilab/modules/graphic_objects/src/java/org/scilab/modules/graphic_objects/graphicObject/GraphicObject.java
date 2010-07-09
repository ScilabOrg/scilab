/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.graphicObject;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

import java.util.List;

/**
 * GraphicObject class
 * @author Manuel JULIACHS
 */
public abstract class GraphicObject {
	/** User data array default size */
	public static final int USER_DATA_DEFAULT_SIZE = 0;

	/** Graphic objects types */
	public enum Type { ARC, AXES, AXIS, CHAMP, COMPOUND, FAC3D, FEC, FIGURE, GRAYPLOT,
		LABEL, LEGEND, MATPLOT, PLOT3D, POLYLINE, RECTANGLE, SEGS, TEXT, UNKNOWNOBJECT };
	
	/** GraphicObject properties */
	public enum GraphicObjectPropertyType { PARENT, CHILDREN, VISIBLE, USERDATA, USERDATASIZE, UNKNOWNPROPERTY };

	/** Identifier */
	private String identifier;
	
	/** Parent object */
	private GraphicObject parent;

	/** Child objects list */
	private List <GraphicObject> children;

	/** Specifies whether the object is visible or not */
	private boolean visible;

	/** User data */
	private int[] userData;

	/** Constructor */
	public GraphicObject() {
		identifier = null;
		parent = null;
		children = null;
		visible = false;
		userData = new int[USER_DATA_DEFAULT_SIZE];
	}

	/**
     * Returns the enum associated to a type name
     * @param typeName the property name
     * @return the type enum
     */
    public static Type getTypeFromName(String typeName) {
        if (typeName.equals("arc")) {
            return Type.ARC;
        } else if (typeName.equals(__GO_AXES__)) {
            return Type.AXES;
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
		} else if (propertyName.equals(__GO_VISIBLE__)) {
			return GraphicObjectPropertyType.VISIBLE;
		} else if (propertyName.equals(__GO_USER_DATA__)) {
			return GraphicObjectPropertyType.USERDATA;
		} else if (propertyName.equals(__GO_USER_DATA_SIZE__)) {
			return GraphicObjectPropertyType.USERDATASIZE;
		} else {
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
		} else if (property == GraphicObjectPropertyType.VISIBLE) {
			return getVisible();
		} else if (property == GraphicObjectPropertyType.USERDATA) {
			return getUserData();
		} else if (property == GraphicObjectPropertyType.USERDATASIZE) {
			return getUserDataSize();
		} else if (property == GraphicObjectPropertyType.UNKNOWNPROPERTY) {
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
			setParent((GraphicObject) value);
		} else if (property == GraphicObjectPropertyType.CHILDREN) {
			setChildren((List<GraphicObject>) value);
		} else if (property == GraphicObjectPropertyType.VISIBLE) {
			setVisible((Boolean) value);
		} else if (property == GraphicObjectPropertyType.USERDATA) {
			setUserData((Integer[]) value);
		} else if (property == GraphicObjectPropertyType.USERDATASIZE) {
			return false;
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
		// TBD
		return null;
	}

	/* TBD */
	/**
	 * Void property set method
	 * @param property the property name
	 * @param value the property value
	 */
	public void setPropertyVoid(String property, Object value) {
		// TBD
	}
	
	/**
	 * @return the children
	 */
	public List<GraphicObject> getChildren() {
		return children;
	}

	/**
	 * @param children the children to set
	 */
	public void setChildren(List<GraphicObject> children) {
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
	public GraphicObject getParent() {
		return parent;
	}

	/**
	 * @param parent the parent to set
	 */
	public void setParent(GraphicObject parent) {
		this.parent = parent;
	}

	/**
	 * @return the userData
	 */
	public Integer[] getUserData() {
		Integer[] returnedData = new Integer[userData.length];

		for (int i = 0; i < userData.length; i++) {
			returnedData[i] = userData[i];
		}

		return returnedData;
	}

	/**
	 * @param userData the userData to set
	 */
	public void setUserData(Integer[] userData) {
		if (userData.length != this.userData.length) {
			this.userData = new int[userData.length];
		}

		for (int i = 0; i < userData.length; i++) {
			this.userData[i] = userData[i];
		}
	}

	/**
	 * @return the userDataSize
	 */
	public Integer getUserDataSize() {
		return userData.length;
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

}
