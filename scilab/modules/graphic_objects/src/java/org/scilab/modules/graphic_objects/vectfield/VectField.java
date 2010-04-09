package org.scilab.modules.graphic_objects.vectfield;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.graphicObject.GraphicClippableObject;

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
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("Arrows")) {
			returnedProp =  VectFieldProperty.ARROWS;
		} else {
			returnedProp = super.getPropertyFromName(propertyName);
		}
		
		return (Object) returnedProp;
	}

	/**
	 * Fast property get method
	 * @param property the property to get
	 * @return the property value
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
	 * @param property the property to set
	 * @param value the property value
	 */
	public void setPropertyFast(Object property, Object value) {
		if (property == VectFieldProperty.ARROWS) {
			setArrows((ArrayList<Arrow>) value);
		} else {
			super.setPropertyFast(property, value);
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
