package org.scilab.modules.graphic_objects;


/**
 * Class Rectangle
 * @author juliachs
 */
public class Rectangle extends ClippableContouredObject {
	/** Rectangle properties names */
	private enum RectangleProperty { UPPERLEFTPOINT, WIDTH, HEIGHT };
	
	/** Upper-left corner (x,y,z) coordinates */
	private double [] upperLeftPoint;
	
	/** Width */
	private double width;

	/** Height */
	private double height;

	/** Constructor */
	public Rectangle() {
		super();
		upperLeftPoint = null;
		width = 0.0;
		height = 0.0;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the associated property enum
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
	 * @param propertyEnum the property to set
	 * @param value the property value
	 */
	public void setPropertyFast(Object propertyEnum, Object value) {

//		System.out.format("prop to string: %s\n", prop.toString());
		if (propertyEnum == RectangleProperty.UPPERLEFTPOINT) {
			setUpperLeftPoint((double []) value);
		} else if (propertyEnum == RectangleProperty.WIDTH) {
			setWidth((Double) value);
		} else if (propertyEnum == RectangleProperty.HEIGHT) {
			setHeight((Double) value);
		} else {
			super.setPropertyFast(propertyEnum, value);
		}
	}

	/**
	 * @return the height
	 */
	public double getHeight() {
		return height;
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
	public double[] getUpperLeftPoint() {
		return upperLeftPoint;
	}

	/**
	 * @param upperLeftPoint the upperLeftPoint to set
	 */
	public void setUpperLeftPoint(double[] upperLeftPoint) {
		this.upperLeftPoint = upperLeftPoint;
	}

	/**
	 * @return the width
	 */
	public double getWidth() {
		return width;
	}

	/**
	 * @param width the width to set
	 */
	public void setWidth(Double width) {
		this.width = width.doubleValue();
	}
	
	//	 Methods: to be done
}
