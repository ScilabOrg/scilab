package org.scilab.modules.graphic_objects;


/**
 * Arrow class
 * @author juliachs
 */
public class Arrow extends ContouredObject {
	/** Arrow properties names */
	private enum ArrowProperty { BASE, DIRECTION, ARROWSIZE };
	
	/** Arrow base (x,y,z) coordinates */
	private double [] base;

	/** Arrow vector (x,y,z) coordinates */
	private double [] direction;

	/** Arrow size */
	private double arrowSize;

	/** Constructor */
	public Arrow() {
		super();
		base = null;
		direction = null;
		arrowSize = 0.0;
	}
	
	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the associated property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("Base")) {
			returnedProp =  ArrowProperty.BASE;
		} else if (propertyName.equals("Direction")) {
			returnedProp =  ArrowProperty.DIRECTION;
		} else if (propertyName.equals("ArrowSize")) {
			returnedProp =  ArrowProperty.ARROWSIZE;
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
	
		if (property == ArrowProperty.BASE) {
			returnedProp = getBase();
		} else if (property == ArrowProperty.DIRECTION) {
			returnedProp = getDirection();
		} else if (property == ArrowProperty.ARROWSIZE) {
			returnedProp = getArrowSize();
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
		if (propertyEnum == ArrowProperty.BASE) {
			setBase((double []) value);
		} else if (propertyEnum == ArrowProperty.DIRECTION) {
			setDirection((double []) value);
		} else if (propertyEnum == ArrowProperty.ARROWSIZE) {
			setArrowSize((Double) value);
		} else {
			super.setPropertyFast(propertyEnum, value);
		}
	}

	/**
	 * @return the arrowSize
	 */
	public double getArrowSize() {
		return arrowSize;
	}

	/**
	 * @param arrowSize the arrowSize to set
	 */
	public void setArrowSize(Double arrowSize) {
		this.arrowSize = arrowSize.doubleValue();
	}

	/**
	 * @return the base
	 */
	public double[] getBase() {
		return base;
	}

	/**
	 * @param base the base to set
	 */
	public void setBase(double[] base) {
		this.base = base;
	}

	/**
	 * @return the direction
	 */
	public double[] getDirection() {
		return direction;
	}

	/**
	 * @param direction the direction to set
	 */
	public void setDirection(double[] direction) {
		this.direction = direction;
	}

	//	 Methods: to be done
}
