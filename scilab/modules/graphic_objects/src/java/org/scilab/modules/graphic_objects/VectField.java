package org.scilab.modules.graphic_objects;

import java.util.ArrayList;

/**
 * VectField class
 * @author juliachs
 */
public abstract class VectField extends GraphicClippableObject {
	/** VectField properties names */
	private enum VectFieldProperty { ARROWS };
	
	/** List of arrows composing the field */
	private ArrayList <Arrow> arrows;

	/** Constructor */
	public VectField() {
		super();
		arrows = null;
	}
	
	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the associated property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("Arrows")) {
			returnedProp =  VectFieldProperty.ARROWS;
		} else {
			returnedProp = super.getPropertyFromName(propertyName);
		//System.out.format("prop toString: %s\n", contouredpropertytest.toString());
		}
		
		return (Object) returnedProp;
	}

	/**
	 * Fast property get method
	 * @param property the property to get
	 * @return the property
	 */
	public Object getPropertyFast(Object property) {
		Object returnedProp;
	
		if (property == VectFieldProperty.ARROWS) {
			returnedProp = this.getArrows();
		} else {
			returnedProp = super.getPropertyFast(property);	
		}

		return returnedProp;
	}

	/**
	 * Fast property set method
	 * @param propertyEnum the property to set
	 * @param value the property value
	 */
	public void setPropertyFast(Object propertyEnum, Object value) {

//		System.out.format("prop to string: %s\n", prop.toString());
		if (propertyEnum == VectFieldProperty.ARROWS) {
			setArrows((ArrayList<Arrow>) value);
		} else {
			super.setPropertyFast(propertyEnum, value);
		}
	}

	/**
	 * @return the arrows
	 */
	public ArrayList<Arrow> getArrows() {
		return arrows;
	}

	/**
	 * @param arrows the arrows to set
	 */
	public void setArrows(ArrayList<Arrow> arrows) {
		this.arrows = arrows;
	}

	// Methods: to be done
}
