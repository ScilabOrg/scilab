package org.scilab.modules.graphic_objects;

import java.util.ArrayList;


/**
 * Fec class
 * @author juliachs
 */
public class Fec extends ClippableContouredObject {
	// TBD: Data
	// + triangles	-> Data Model
	// TBD properties relative to the data model
	/** Fec properties names */
	private enum FecProperty { ZBOUNDS, OUTSIDECOLOR, COLORRANGE, UNKNOWNPROP };

	/** Z Bounds: 2-element array */
	private double [] zBounds;
	
	/** 2-element array */
	private int [] outsideColor;

	/** 2-element array */
	private int [] colorRange;

	/** Constructor */
	public Fec() {
		super();
		zBounds = null;
		outsideColor = null;
		colorRange = null;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the associated property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("ZBounds")) {
			returnedProp =  FecProperty.ZBOUNDS;
		} else if (propertyName.equals("OutsideColor")) {
			returnedProp =  FecProperty.OUTSIDECOLOR;
		} else if (propertyName.equals("ColorRange")) {
			returnedProp =  FecProperty.COLORRANGE;
		} else {
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

		if (property == FecProperty.ZBOUNDS) {
			returnedProp = getZBounds();
		} else if (property == FecProperty.OUTSIDECOLOR) {
			returnedProp = getOutsideColor();
		} else if (property == FecProperty.COLORRANGE) {
			returnedProp = getColorRange();
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
		if (propertyEnum == FecProperty.ZBOUNDS) {
			setZBounds((double []) value);
		} else if (propertyEnum == FecProperty.OUTSIDECOLOR) {
			setOutsideColor((int []) value);
		} else if (propertyEnum == FecProperty.COLORRANGE) {
			setColorRange((int []) value);
		} else {
			super.setPropertyFast(propertyEnum, value);
		}
	}

	/**
	 * @return the colorRange
	 */
	public int[] getColorRange() {
		return colorRange;
	}

	/**
	 * @param colorRange the colorRange to set
	 */
	public void setColorRange(int[] colorRange) {
		this.colorRange = colorRange;
	}

	/**
	 * @return the outsideColor
	 */
	public int[] getOutsideColor() {
		return outsideColor;
	}

	/**
	 * @param outsideColor the outsideColor to set
	 */
	public void setOutsideColor(int[] outsideColor) {
		this.outsideColor = outsideColor;
	}

	/**
	 * @return the zBounds
	 */
	public double[] getZBounds() {
		return zBounds;
	}

	/**
	 * @param bounds the zBounds to set
	 */
	public void setZBounds(double[] bounds) {
		zBounds = bounds;
	}

	// 	Methods: to be done
}
