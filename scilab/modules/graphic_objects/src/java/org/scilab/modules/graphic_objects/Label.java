package org.scilab.modules.graphic_objects;

/**
 * Label class
 * @author juliachs
 */
public class Label extends TextObject {
	/** Label properties names */
	public enum LabelProperty { FONTANGLE, AUTOPOSITION, POSITION, AUTOROTATION };
	
	/** Text orientation */
	private double fontAngle;
	
	 /** Specifies whether the label position is automatically computed or not */
	private boolean autoPosition;

	/** 3-element array: (x, y, z) position */
	private double [] position;

	/** Specifies whether the label orientation is automatically computed or not */
	private boolean autoRotation;

	/** Constructor */
	public Label() {
		super();
		fontAngle = 0.0;
		autoPosition = false;
		position = null;
		autoRotation = false;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the associated property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("FontAngle")) {
			returnedProp =  LabelProperty.FONTANGLE;
		} else if (propertyName.equals("AutoPosition")) {
			returnedProp =  LabelProperty.AUTOPOSITION;
		} else if (propertyName.equals("Position")) {
			returnedProp =  LabelProperty.POSITION;
		} else if (propertyName.equals("AutoRotation")) {
			returnedProp =  LabelProperty.AUTOROTATION;
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
	
		if (property == LabelProperty.FONTANGLE) {
			returnedProp = getFontAngle();
		} else if (property == LabelProperty.AUTOPOSITION) {
			returnedProp = getAutoPosition();
		} else if (property == LabelProperty.POSITION) {
			returnedProp = getPosition();
		} else if (property == LabelProperty.AUTOROTATION) {
			returnedProp = getAutoRotation();
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

//		System.out.format("prop to string: %s\n", prop.toString());
		if (property == LabelProperty.FONTANGLE) {
			setFontAngle((Double) value);
		} else if (property == LabelProperty.AUTOPOSITION) {
			setAutoPosition((Boolean) value);
		} else if (property == LabelProperty.POSITION) {
			setPosition((double []) value);
		} else if (property == LabelProperty.AUTOROTATION) {
			setAutoRotation((Boolean) value);
		} else {
			super.setPropertyFast(property, value);
		}
	}
	
	/**
	 * @return the autoPosition
	 */
	public boolean getAutoPosition() {
		return autoPosition;
	}

	/**
	 * @param autoPosition the autoPosition to set
	 */
	public void setAutoPosition(Boolean autoPosition) {
		this.autoPosition = autoPosition.booleanValue();
	}

	/**
	 * @return the autoRotation
	 */
	public boolean getAutoRotation() {
		return autoRotation;
	}

	/**
	 * @param autoRotation the autoRotation to set
	 */
	public void setAutoRotation(Boolean autoRotation) {
		this.autoRotation = autoRotation.booleanValue();
	}

	/**
	 * @return the fontAngle
	 */
	public double getFontAngle() {
		return fontAngle;
	}

	/**
	 * @param fontAngle the fontAngle to set
	 */
	public void setFontAngle(Double fontAngle) {
		this.fontAngle = fontAngle.doubleValue();
	}

	/**
	 * @return the position
	 */
	public double[] getPosition() {
		return position;
	}

	/**
	 * @param position the position to set
	 */
	public void setPosition(double[] position) {
		this.position = position;
	}

	//	 Methods: to be done
}
