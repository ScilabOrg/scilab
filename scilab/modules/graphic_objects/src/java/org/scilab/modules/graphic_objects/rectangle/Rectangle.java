package org.scilab.modules.graphic_objects.rectangle;

import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;


/**
 * Class Rectangle
 * @author juliachs
 */
public class Rectangle extends ClippableContouredObject {
	/** Rectangle properties names */
	private enum RectangleProperty { UPPERLEFTPOINT, WIDTH, HEIGHT };
	
	/** Upper-left corner (x,y,z) coordinates */
	private double[] upperLeftPoint;
	
	/** Width */
	private double width;

	/** Height */
	private double height;

	/** Constructor */
	public Rectangle() {
		super();
		upperLeftPoint = new double[3];
		width = 0.0;
		height = 0.0;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("UpperLeftPoint")) {
			returnedProp =  RectangleProperty.UPPERLEFTPOINT;
		} else if (propertyName.equals("Width")) {
			returnedProp =  RectangleProperty.WIDTH;
		} else if (propertyName.equals("Height")) {
			returnedProp =  RectangleProperty.HEIGHT;
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
	
		if (property == RectangleProperty.UPPERLEFTPOINT) {
			returnedProp = getUpperLeftPoint();
		} else if (property == RectangleProperty.WIDTH) {
			returnedProp = getWidth();
		} else if (property == RectangleProperty.HEIGHT) {
			returnedProp = getHeight();
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
		if (property == RectangleProperty.UPPERLEFTPOINT) {
			setUpperLeftPoint((Double[]) value);
		} else if (property == RectangleProperty.WIDTH) {
			setWidth((Double) value);
		} else if (property == RectangleProperty.HEIGHT) {
			setHeight((Double) value);
		} else {
			super.setPropertyFast(property, value);
		}
	}

	/**
	 * @return the height
	 */
	public Double getHeight() {
		return new Double(height);
	}

	/**
	 * @param height the height to set
	 */
	public void setHeight(Double height) {
		this.height = height.doubleValue();
	}

	/**
	 * @return the upperLeftPoint
	 */
	public Double[] getUpperLeftPoint() {
		Double[] retPoint = new Double[3];

		retPoint[0] = new Double(upperLeftPoint[0]);
		retPoint[1] = new Double(upperLeftPoint[1]);
		retPoint[2] = new Double(upperLeftPoint[2]);

		return retPoint;
	}

	/**
	 * @param upperLeftPoint the upperLeftPoint to set
	 */
	public void setUpperLeftPoint(Double[] upperLeftPoint) {
		this.upperLeftPoint[0] = upperLeftPoint[0].doubleValue();
		this.upperLeftPoint[1] = upperLeftPoint[1].doubleValue();
		this.upperLeftPoint[2] = upperLeftPoint[2].doubleValue();
	}

	/**
	 * @return the width
	 */
	public Double getWidth() {
		return new Double(width);
	}

	/**
	 * @param width the width to set
	 */
	public void setWidth(Double width) {
		this.width = width.doubleValue();
	}
	
	//	 Methods: to be done
}
