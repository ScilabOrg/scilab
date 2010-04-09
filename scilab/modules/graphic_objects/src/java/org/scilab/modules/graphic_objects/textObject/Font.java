package org.scilab.modules.graphic_objects.textObject;


/**
 * Font class
 * @author juliachs
 */
public class Font {
	/** Font properties names */
	public enum FontProperty { STYLE, SIZE, COLOR, FRACTIONAL };
	
	/** Font style */
	private int style;

	/** Font size */
	private double size;

	/** Font color */
	private int color;

	/** Specifies whether fractional font sizes are used or not */
	private boolean fractional;

	/** Constructor */
	public Font() {
		this.style = 0;
		this.size = 0.0;
		this.color = 0;
		this.fractional = false;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp = null;

		if (propertyName.equals("Style")) {
			returnedProp =  FontProperty.STYLE;
		} else if (propertyName.equals("Size")) {
			returnedProp =  FontProperty.SIZE;
		} else if (propertyName.equals("Color")) {
			returnedProp =  FontProperty.COLOR;
		} else if (propertyName.equals("Fractional")) {
			returnedProp = FontProperty.FRACTIONAL;
		}
		
		return (Object) returnedProp;
	}

	/**
	 * Fast property get method
	 * @param property the property to get
	 * @return the property value
	 */
	public Object getPropertyFast(Object property) {
		Object returnedProp = null;

		if (property == FontProperty.STYLE) {
			returnedProp = getStyle();
		} else if (property == FontProperty.SIZE) {
			returnedProp = getSize();
		} else if (property == FontProperty.COLOR) {
			returnedProp = getColor();
		} else if (property == FontProperty.FRACTIONAL) {
			returnedProp = getFractional();
		}

		return returnedProp;
	}

	/**
	 * Fast property set method
	 * @param property the property to set
	 * @param value the property value
	 */
	public void setPropertyFast(Object property, Object value) {

		if (property == FontProperty.STYLE) {
			setStyle((Integer) value);
		} else if (property == FontProperty.SIZE) {
			setSize((Double) value);
		} else if (property == FontProperty.COLOR) {
			setColor((Integer) value);
		} else if (property == FontProperty.FRACTIONAL) {
			setFractional((Boolean) value);
		}
	}


	/**
	 * @return the color
	 */
	public Integer getColor() {
		return new Integer(color);
	}

	/**
	 * @param color the color to set
	 */
	public void setColor(Integer color) {
		this.color = color.intValue();
	}

	/**
	 * @return the fractional
	 */
	public Boolean getFractional() {
		return new Boolean(fractional);
	}

	/**
	 * @param fractional the fractional to set
	 */
	public void setFractional(Boolean fractional) {
		this.fractional = fractional.booleanValue();
	}

	/**
	 * @return the size
	 */
	public Double getSize() {
		return new Double(size);
	}

	/**
	 * @param size the size to set
	 */
	public void setSize(Double size) {
		this.size = size.doubleValue();
	}

	/**
	 * @return the style
	 */
	public Integer getStyle() {
		return new Integer(style);
	}

	/**
	 * @param style the style to set
	 */
	public void setStyle(Integer style) {
		this.style = style.intValue();
	}

	//	 Methods: to be done
}
