package org.scilab.modules.graphic_objects;

import org.scilab.modules.graphic_objects.ClippableProperty.ClipStateType;

/**
 * GraphicClippableObject class 
 * @author juliachs
 */
public abstract class GraphicClippableObject extends GraphicObject {
	/** GraphicClippableObject properties names */
	private enum GraphicClippableObjectProperty { CLIPPROPERTY };
	
	/** Stores the actual clipping state */
	private ClippableProperty clipProperty;
	
	/** Default constructor */
	GraphicClippableObject() {
		super();
		clipProperty = null;
	}
	
	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the associated property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("ClipProperty")) {
			returnedProp =  GraphicClippableObjectProperty.CLIPPROPERTY;
		} else {
			/* experimental */
			returnedProp = clipProperty.getPropertyFromName(propertyName);

			if(returnedProp != ClippableProperty.ClippablePropertyType.UNKNOWNPROP) {
				return returnedProp;
			}

			returnedProp = super.getPropertyFromName(propertyName);
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
	
		if (property == GraphicClippableObjectProperty.CLIPPROPERTY) {
			returnedProp = getClipProperty();
		} else if (property == ClippableProperty.ClippablePropertyType.CLIPSTATE) {
			returnedProp = clipProperty.getClipState();
		} else if (property == ClippableProperty.ClippablePropertyType.CLIPBOX) {
			returnedProp = clipProperty.getClipBox();
			
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
		if (propertyEnum == GraphicClippableObjectProperty.CLIPPROPERTY) {
			setClipProperty((ClippableProperty) value);
		} else if (propertyEnum == ClippableProperty.ClippablePropertyType.CLIPSTATE) {
			clipProperty.setClipState((ClipStateType) value);
		} else if (propertyEnum == ClippableProperty.ClippablePropertyType.CLIPBOX) {
			clipProperty.setClipBox((double []) value);
		} else {
			super.setPropertyFast(propertyEnum, value);
		}
	}

	/**
	 * @return the clipProperty
	 */
	public ClippableProperty getClipProperty() {
		return clipProperty;
	}

	/**
	 * @param clipProperty the clipProperty to set
	 */
	public void setClipProperty(ClippableProperty clipProperty) {
		this.clipProperty = clipProperty;
	}

	//	 Methods: to be done
}
