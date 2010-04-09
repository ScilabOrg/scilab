package org.scilab.modules.graphic_objects;

import java.util.ArrayList;
import java.lang.reflect.Method;
import java.rmi.server.UID;

/**
 * GraphicObject class
 * @author juliachs
 */
public abstract class GraphicObject {
	/** Graphics objects types */
	public enum Type { ARC, AXES, AXIS, CHAMP, COMPOUND, FAC3D, FEC, FIGURE, GRAYPLOT, LABEL, LEGEND,
		MATPLOT, PLOT3D, POLYLINE, RECTANGLE, SEGS, TEXT, UNKNOWNOBJECT };
	
	/** GraphicObject properties */
	public enum GraphicObjectPropertyType { PARENT, CHILDREN, VISIBLE, USERDATA, UNKNOWNPROP };
	
	/** Parent object */
	private GraphicObject parent;

	/** Child objects list */
	private ArrayList <GraphicObject> children;

	/** Specifies whether the object is visible or not */
	private boolean visible;

	/** User data */
	private byte [] userData;

	/** Constructor */
	public GraphicObject() {
		this.parent = null;
		this.children = null;
		this.visible = false;
		this.userData = null;
	}

	/**
	 * Fast property get method
	 * @param property the property to get
	 * @return the property value
	 */
	public Object getPropertyFast(Object property) {
		Object returnedProp = null;
		
		if (property == GraphicObjectPropertyType.PARENT) {
			returnedProp = getParent();
		} else if (property == GraphicObjectPropertyType.CHILDREN) {
			returnedProp = getChildren();
		} else if (property == GraphicObjectPropertyType.VISIBLE) {
			returnedProp = getVisible();
		} else if (property == GraphicObjectPropertyType.USERDATA) {
			returnedProp = getUserData();
		} else if (property == GraphicObjectPropertyType.UNKNOWNPROP) {
			returnedProp = null;
		}

		return returnedProp;
	}

	/**
	 * Fast property get method
	 * @param property the property
	 * @return the property value
	 */
	public Object getPropertyFast2(Object property) {
		return null; 
	}

	/**
	 * Get property from name
	 * @param propertyName the property name string
	 * @return the property type
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp = null;
		
		if (propertyName.equals("Parent")) {
			returnedProp =  GraphicObjectPropertyType.PARENT;
		} else if (propertyName.equals("Children")) {
			returnedProp = GraphicObjectPropertyType.CHILDREN;
		} else if (propertyName.equals("Visible")) {
			returnedProp = GraphicObjectPropertyType.VISIBLE;
		} else if (propertyName.equals("UserData")) {
			returnedProp = GraphicObjectPropertyType.USERDATA;
		} else {
			returnedProp = GraphicObjectPropertyType.UNKNOWNPROP;
		}

		return returnedProp;
	}
	
	/**
	 * Get property from name, version 2
	 * @param propertyName the property name string
	 * @return the property
	 */
	public Object getPropertyFromName2(String propertyName) {
		return null;
	}
	
	/**
	 * Fast property set method
	 * @param property the property
	 * @param value the property value
	 */
	public void setPropertyFast(Object property, Object value) {
		
		if (property == GraphicObjectPropertyType.PARENT) {
			setParent((GraphicObject) value);
		} else if (property == GraphicObjectPropertyType.CHILDREN) {
			setChildren((ArrayList<GraphicObject>) value);
		} else if (property == GraphicObjectPropertyType.VISIBLE) {
			setVisible((Boolean) value);
		} else if (property == GraphicObjectPropertyType.USERDATA) {
			setUserData((byte []) value);
		} else {
			// Unknown prop
		}
	}
	
	/**
	 * Experimental fast set property method
	 * @param property the property
	 * @param value the property value
	 */
	public void setPropertyFastHack(Object property, Object value) {
		//
	}
	
	/**
	 * Fast property set method version 2
	 * @param property the property
	 * @param value the property value
	 */
	public void setPropertyFast2(Object property, Object value) {
		// Do nothing
	}
	
	/**
	 * Set property method
	 * @param property property name
	 * @param value property value
	 */
    public void setProperty(String property, Object value) {
    	try {
    	//	System.out.format("before calling getMethod: %s\n", property);
    	    Method setter = this.getClass().getMethod("set"+property, value.getClass());
    	//  System.out.format("before setter.invoke: %s\n", setter.toString());
    	    setter.invoke(this, value);
    	} catch (Exception e) {
    	    System.err.println("Got Exception "+e.getMessage());
    	    e.printStackTrace();
    	}

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
	 * Get property method
	 * @param property the property name string
	 * @return the property value
	 */
	public Object getProperty(String property) {
		try {
			//System.out.format("before calling getMethod: %s\n", property);
			Method getter = this.getClass().getMethod("get" + property, (Class[]) null);
			//System.out.format("before getter.invoke: %s\n", property);
			return getter.invoke(this, (Object [])null);
		} catch (Exception e) {
			System.err.println("Got Exception "+e.getMessage());
		}// TODO Auto-generated method stub
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
	public ArrayList<GraphicObject> getChildren() {
		return children;
	}

	/**
	 * @param children the children to set
	 */
	public void setChildren(ArrayList<GraphicObject> children) {
		this.children = children;
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
	public byte[] getUserData() {
		return userData;
	}

	/**
	 * @param userData the userData to set
	 */
	public void setUserData(byte[] userData) {
		this.userData = userData;
	}

	/**
	 * @return the visible
	 */
	public Boolean isVisible() {
		return new Boolean(visible);
	}

	/**
	 * Get visible method
	 * @return the visible
	 */
	public Boolean getVisible() {
		return new Boolean(visible);
	}

	/**
	 * Set visible method
	 * @param visible the visible to set
	 */
	public void setVisible(Boolean visible) {
		this.visible = visible.booleanValue();
	}

	// Methods: to be done
}
