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

package org.scilab.modules.graphic_objects.fec;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;

/**
 * Fec class
 * @author Manuel JULIACHS
 */
public class Fec extends ClippableContouredObject {
	// TBD: Data
	// + triangles	-> Data Model
	// TBD properties relative to the data model
	/** Fec properties names */
	private enum FecProperty { ZBOUNDS, OUTSIDECOLOR, COLORRANGE  };

	/** Z Bounds: 2-element array */
	private double[] zBounds;

	/** 2-element array */
	private int[] outsideColor;

	/** 2-element array */
	private int[] colorRange;

	/** Constructor */
	public Fec() {
		super();
		zBounds = new double[2];
		outsideColor = new int[2];
		colorRange = new int[2];
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals("ZBounds")) {
			return FecProperty.ZBOUNDS;
		} else if (propertyName.equals("OutsideColor")) {
			return FecProperty.OUTSIDECOLOR;
		} else if (propertyName.equals("ColorRange")) {
			return FecProperty.COLORRANGE;
		} else {
			return super.getPropertyFromName(propertyName);
		}
	}

	/**
	 * Fast property get method
	 * @param property the property to get
	 * @return the property value
	 */
	public Object getPropertyFast(Object property) {
		if (property == FecProperty.ZBOUNDS) {
			return getZBounds();
		} else if (property == FecProperty.OUTSIDECOLOR) {
			return getOutsideColor();
		} else if (property == FecProperty.COLORRANGE) {
			return getColorRange();
		} else {
			return super.getPropertyFast(property);
		}
	}

	/**
	 * Fast property set method
	 * @param property the property to set
	 * @param value the property value
	 */
	public void setPropertyFast(Object property, Object value) {
		if (property == FecProperty.ZBOUNDS) {
			setZBounds((Double[]) value);
		} else if (property == FecProperty.OUTSIDECOLOR) {
			setOutsideColor((Integer[]) value);
		} else if (property == FecProperty.COLORRANGE) {
			setColorRange((Integer[]) value);
		} else {
			super.setPropertyFast(property, value);
		}
	}

	/**
	 * @return the colorRange
	 */
	public Integer[] getColorRange() {
		Integer[] retColorRange = new Integer[2];
		retColorRange[0] = colorRange[0];
		retColorRange[1] = colorRange[1];
		
		return retColorRange;
	}

	/**
	 * @param colorRange the colorRange to set
	 */
	public void setColorRange(Integer[] colorRange) {
		this.colorRange[0] = colorRange[0];
		this.colorRange[1] = colorRange[1];
	}

	/**
	 * @return the outsideColor
	 */
	public Integer[] getOutsideColor() {
		Integer[] retOutsideColor = new Integer[2];
		retOutsideColor[0] = outsideColor[0];
		retOutsideColor[1] = outsideColor[1];

		return retOutsideColor;
	}

	/**
	 * @param outsideColor the outsideColor to set
	 */
	public void setOutsideColor(Integer[] outsideColor) {
		this.outsideColor[0] = outsideColor[0];
		this.outsideColor[1] = outsideColor[1];
	}

	/**
	 * @return the zBounds
	 */
	public Double[] getZBounds() {
		Double[] retZBounds = new Double[2];
		retZBounds[0] = zBounds[0];
		retZBounds[1] = zBounds[1];

		return retZBounds;
	}

	/**
	 * @param bounds the zBounds to set
	 */
	public void setZBounds(Double[] zBounds) {
		this.zBounds[0] = zBounds[0];
		this.zBounds[1] = zBounds[1];
	}

}
