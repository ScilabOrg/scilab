package org.scilab.modules.graphic_objects.arc;

import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;

/**
 * Arc class
 * @author juliachs
 */
public class Arc extends ClippableContouredObject {
	/** Arc properties names */
	private enum ArcProperty { UPPERLEFTPOINT, WIDTH, HEIGHT, STARTANGLE, ENDANGLE, ARCDRAWINGMETHOD  };
	
	/** Arc drawing method */
	public enum ArcDrawingMethod { NURBS, LINES };

	/** Bounding box upper-left point (x,y,z) coordinates */
	private double[] upperLeftPoint;

	/** Bounding box width */
	private double width;

	/** Bounding box height */
	private double height;

	/** Start angle */
	private double startAngle;

	/** End angle */
	private double endAngle;
	
	/** Specifies which drawing method to use */
	private ArcDrawingMethod arcDrawingMethod;
	
	/**
	 * Constructor
	 */
	public Arc() {
		super();
		arcDrawingMethod = ArcDrawingMethod.LINES;
		upperLeftPoint = new double[3];
		width = 0.0;
		height = 0.0;
		startAngle = 0.0;
		endAngle = 0.0;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;
			
		if (propertyName.equals("UpperLeftPoint")) {
			returnedProp =  ArcProperty.UPPERLEFTPOINT;
		} else if (propertyName.equals("Width")) {
			returnedProp =  ArcProperty.WIDTH;
		} else if (propertyName.equals("Height")) {
			returnedProp =  ArcProperty.HEIGHT;
		} else if (propertyName.equals("StartAngle")) {
			returnedProp =  ArcProperty.STARTANGLE;
		} else if (propertyName.equals("EndAngle")) {
			returnedProp =  ArcProperty.ENDANGLE;
		} else if (propertyName.equals("ArcDrawingMethod")) {
			returnedProp = ArcProperty.ARCDRAWINGMETHOD;
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
		Object returnedProp = null;

		if (property == ArcProperty.UPPERLEFTPOINT) {
			returnedProp = getUpperLeftPoint();
		} else if (property == ArcProperty.WIDTH) {
			returnedProp = getWidth();
		} else if (property == ArcProperty.HEIGHT) {
			returnedProp = getHeight();
		} else if (property == ArcProperty.STARTANGLE) {
			returnedProp = getStartAngle();
		} else if (property == ArcProperty.ENDANGLE) {
			returnedProp = getEndAngle();
		} else if (property == ArcProperty.ARCDRAWINGMETHOD) {
			returnedProp = getArcDrawingMethod();
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
		if (property == ArcProperty.UPPERLEFTPOINT) {
			setUpperLeftPoint((Double[]) value);
		} else if (property == ArcProperty.WIDTH) {
			setWidth((Double) value);
		} else if (property == ArcProperty.HEIGHT) {
			setHeight((Double) value);
		} else if (property == ArcProperty.STARTANGLE) {
			setStartAngle((Double) value);
		} else if (property == ArcProperty.ENDANGLE) {
			setEndAngle((Double) value);
		} else if (property == ArcProperty.ARCDRAWINGMETHOD) {
			setArcDrawingMethod((ArcDrawingMethod) value);
		} else {
			super.setPropertyFast(property, value);
		}

	}

	/**
	 * @return the arcDrawingMethod
	 */
	public ArcDrawingMethod getArcDrawingMethod() {
		return arcDrawingMethod;
	}

	/**
	 * @param arcDrawingMethod the arcDrawingMethod to set
	 */
	public void setArcDrawingMethod(ArcDrawingMethod arcDrawingMethod) {
		this.arcDrawingMethod = arcDrawingMethod;
	}

	/**
	 * @return the endAngle
	 */
	public Double getEndAngle() {
		return new Double(endAngle);
	}

	/**
	 * @param endAngle the endAngle to set
	 */
	public void setEndAngle(Double endAngle) {
		this.endAngle = endAngle.doubleValue();
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
	 * @return the startAngle
	 */
	public Double getStartAngle() {
		return new Double(startAngle);
	}

	/**
	 * @param startAngle the startAngle to set
	 */
	public void setStartAngle(Double startAngle) {
		this.startAngle = startAngle.doubleValue();
	}

	/**
	 * @return the upperLeftPoint
	 */
	public Double[] getUpperLeftPoint() {
		Double[] returnedPoint = new Double[3];

		returnedPoint[0] = new Double(upperLeftPoint[0]);
		returnedPoint[1] = new Double(upperLeftPoint[1]);
		returnedPoint[2] = new Double(upperLeftPoint[2]);

		return returnedPoint;
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
