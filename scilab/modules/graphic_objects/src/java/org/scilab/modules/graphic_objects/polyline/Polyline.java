package org.scilab.modules.graphic_objects.polyline;

import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;

/**
 * Polyline class
 * @author juliachs
 */
public class Polyline extends ClippableContouredObject {
	/** TBD: data */
	// Data data -> Data Model
	/* TBD: properties relative to the data model */
	/** Polyline properties names */
	private enum PolylineProperty { CLOSED, ARROWSIZEFACTOR, POLYLINESTYLE, INTERPCOLORVECTOR, INTERPCOLORMODE,
		XSHIFT, YSHIFT, ZSHIFT, BARWIDTH };

	/** Specifies whether the polyline is closed */
	private boolean closed;
	
	/** Determines the arrow size */
	private double arrowSizeFactor;

	/** Polyline drawing style (normal, staircase, bar, etc.) */
	private int polylineStyle;
	
	/** Interpolation color vector (3- or 4-element array) */
	private int[] interpColorVector;

	/** Specifies whether interpolated shading is used */
	private boolean interpColorMode;

	/** Shift applied to the line points along the x-axis */
	private double[] xShift;

	/** Shift applied to the line points along the y-axis */
	private double[] yShift;

	/** Shift applied to the line points along the z-axis */
	private double[] zShift;

	/** Bar width */
	private double barWidth;

	/** Constructor */
	public Polyline() {
		super();
		closed = false;
		arrowSizeFactor = 0.0;
		polylineStyle = 0;
		interpColorVector = null;
		interpColorMode = false;
		xShift = null;
		yShift = null;
		zShift = null;
		barWidth = 0.0;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("Closed")) {
			returnedProp =  PolylineProperty.CLOSED;
		} else if (propertyName.equals("ArrowSizeFactor")) {
			returnedProp =  PolylineProperty.ARROWSIZEFACTOR;
		} else if (propertyName.equals("PolylineStyle")) {
			returnedProp =  PolylineProperty.POLYLINESTYLE;
		} else if (propertyName.equals("InterpColorVector")) {
			returnedProp =  PolylineProperty.INTERPCOLORVECTOR;
		} else if (propertyName.equals("InterpColorMode")) {
			returnedProp =  PolylineProperty.INTERPCOLORMODE;
		} else if (propertyName.equals("XShift")) {
			returnedProp = PolylineProperty.XSHIFT;
		} else if (propertyName.equals("YShift")) {
			returnedProp = PolylineProperty.YSHIFT;
		} else if (propertyName.equals("ZShift")) {
			returnedProp = PolylineProperty.ZSHIFT;
		} else if (propertyName.equals("BarWidth")) {
			returnedProp = PolylineProperty.BARWIDTH;
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

		if (property == PolylineProperty.CLOSED) {
			returnedProp = getClosed();
		} else if (property == PolylineProperty.ARROWSIZEFACTOR) {
			returnedProp = getArrowSizeFactor();
		} else if (property == PolylineProperty.POLYLINESTYLE) {
			returnedProp = getPolylineStyle();
		} else if (property == PolylineProperty.INTERPCOLORVECTOR) {
			returnedProp = getInterpColorVector();
		} else if (property == PolylineProperty.INTERPCOLORMODE) {
			returnedProp = getInterpColorMode();
		} else if (property == PolylineProperty.XSHIFT) {
			returnedProp = getXShift();
		} else if (property == PolylineProperty.YSHIFT) {
			returnedProp = getYShift();
		} else if (property == PolylineProperty.ZSHIFT) {
			returnedProp = getZShift();
		} else if (property == PolylineProperty.BARWIDTH) {
			returnedProp = getBarWidth();
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
		if (property == PolylineProperty.CLOSED) {
			setClosed((Boolean) value);
		} else if (property == PolylineProperty.ARROWSIZEFACTOR) {
			setArrowSizeFactor((Double) value);
		} else if (property == PolylineProperty.POLYLINESTYLE) {
			setPolylineStyle((Integer) value);
		} else if (property == PolylineProperty.INTERPCOLORVECTOR) {
			setInterpColorVector((Integer[]) value);
		} else if (property == PolylineProperty.INTERPCOLORMODE) {
			setInterpColorMode((Boolean) value);
		} else if (property == PolylineProperty.XSHIFT) {
			setXShift((double[]) value);
		} else if (property == PolylineProperty.YSHIFT) {
			setYShift((double[]) value);
		} else if (property == PolylineProperty.ZSHIFT) {
			setZShift((double[]) value);
		} else if (property == PolylineProperty.BARWIDTH) {
			setBarWidth((Double) value);
		} else {
			super.getPropertyFast(property);	
		}
	}


	/**
	 * @return the arrowSizeFactor
	 */
	public Double getArrowSizeFactor() {
		return new Double(arrowSizeFactor);
	}

	/**
	 * @param arrowSizeFactor the arrowSizeFactor to set
	 */
	public void setArrowSizeFactor(Double arrowSizeFactor) {
		this.arrowSizeFactor = arrowSizeFactor.doubleValue();
	}

	/**
	 * @return the barWidth
	 */
	public Double getBarWidth() {
		return new Double(barWidth);
	}

	/**
	 * @param barWidth the barWidth to set
	 */
	public void setBarWidth(Double barWidth) {
		this.barWidth = barWidth.doubleValue();
	}

	/**
	 * @return the closed
	 */
	public Boolean getClosed() {
		return new Boolean(closed);
	}

	/**
	 * @param closed the closed to set
	 */
	public void setClosed(Boolean closed) {
		this.closed = closed.booleanValue();
	}

	/**
	 * @return the interpColorMode
	 */
	public Boolean getInterpColorMode() {
		return new Boolean(interpColorMode);
	}

	/**
	 * @param interpColorMode the interpColorMode to set
	 */
	public void setInterpColorMode(Boolean interpColorMode) {
		this.interpColorMode = interpColorMode.booleanValue();
	}

	/**
	 * @return the interpColorVector
	 */
	public Integer[] getInterpColorVector() {
		Integer[] retVector = new Integer[interpColorVector.length];
		for (int i = 0; i < interpColorVector.length; i++) {
			retVector[i] = new Integer(interpColorVector[i]);
		}

		return retVector;
	}

	/**
	 * @param interpColorVector the interpColorVector to set
	 */
	public void setInterpColorVector(Integer[] interpColorVector) {
		for(int i = 0; i < interpColorVector.length; i++) {
			this.interpColorVector[i] = interpColorVector[i].intValue();
		}
	}

	/**
	 * @return the polylineStyle
	 */
	public Integer getPolylineStyle() {
		return new Integer(polylineStyle);
	}

	/**
	 * @param polylineStyle the polylineStyle to set
	 */
	public void setPolylineStyle(Integer polylineStyle) {
		this.polylineStyle = polylineStyle.intValue();
	}

	/**
	 * @return the xShift
	 */
	public double[] getXShift() {
		return xShift;
	}

	/**
	 * @param shift the xShift to set
	 */
	public void setXShift(double[] shift) {
		xShift = shift;
	}

	/**
	 * @return the yShift
	 */
	public double[] getYShift() {
		return yShift;
	}

	/**
	 * @param shift the yShift to set
	 */
	public void setYShift(double[] shift) {
		yShift = shift;
	}

	/**
	 * @return the zShift
	 */
	public double[] getZShift() {
		return zShift;
	}

	/**
	 * @param shift the zShift to set
	 */
	public void setZShift(double[] shift) {
		zShift = shift;
	}
	
	//	 Methods: to be done
}
