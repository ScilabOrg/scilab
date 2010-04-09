package org.scilab.modules.graphic_objects.textObject;

import org.scilab.modules.graphic_objects.graphicObject.ClippableProperty;
import org.scilab.modules.graphic_objects.graphicObject.ClippableProperty.ClipStateType;

/**
 * ClippableTextObject
 * @author juliachs
 */
public abstract class ClippableTextObject extends TextObject {
	/** ClippableTextObject properties names */
	private enum ClippableTextObjectProperty { CLIPPROPERTY };
	
	/** Clipping state */
	private ClippableProperty clipProperty;

	/**  Constructor */
	public ClippableTextObject() {
		clipProperty = new ClippableProperty();
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("ClipProperty")) {
			returnedProp =  ClippableTextObjectProperty.CLIPPROPERTY;
		} else if (propertyName.equals("ClipState")) {
				returnedProp =  ClippableProperty.ClippablePropertyType.CLIPSTATE;
		} else if (propertyName.equals("ClipBox")) {
			returnedProp =  ClippableProperty.ClippablePropertyType.CLIPBOX;
		} else {
			returnedProp = super.getPropertyFromName(propertyName);
		}
		
		return returnedProp;
	}

	/**
	 * Fast property get method
	 * @param property the property to get
	 * @return the property value
	 */
	public Object getPropertyFast(Object property) {
		Object returnedProp;

		if (property == ClippableTextObjectProperty.CLIPPROPERTY) {
			returnedProp = getClipProperty();
		} else if (property == ClippableProperty.ClippablePropertyType.CLIPSTATE) {
			returnedProp = getClipState();
		} else if (property == ClippableProperty.ClippablePropertyType.CLIPBOX) {
			returnedProp = getClipBox();
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
		if(property == ClippableTextObjectProperty.CLIPPROPERTY) {
			setClipProperty((ClippableProperty) value);
		} else if (property == ClippableProperty.ClippablePropertyType.CLIPSTATE) {
			setClipBox((Double[]) value);
		} else if (property == ClippableProperty.ClippablePropertyType.CLIPBOX) {
			setClipState((ClipStateType) value);
		} else {
			super.setPropertyFast(property, value);
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

	/**
	 * @return the clipBox
	 */
	public Double[] getClipBox() {
		double[] cbox = clipProperty.getClipBox();
		Double[] clipBox = new Double[cbox.length];
		for (int i = 0; i < clipBox.length; i++) {
			clipBox[i] = new Double(cbox[i]);
		}

		return clipBox;
	}

	/**
	 * @param clipBox the clipBox to set
	 */
	public void setClipBox(Double[] clipBox) {
		double[] cBox = new double[clipBox.length];
		for (int i = 0; i < clipBox.length; i++) {
			cBox[i] = clipBox[i].doubleValue();
		}

		clipProperty.setClipBox(cBox);
	}

	/**
	 * @return the clipState
	 */
	public ClipStateType getClipState() {
		return clipProperty.getClipState();
	}

	/**
	 * @param clipState the clipState to set
	 */
	public void setClipState(ClipStateType clipState) {
		clipProperty.setClipState(clipState);
	}

	// Methods: to be done
}
