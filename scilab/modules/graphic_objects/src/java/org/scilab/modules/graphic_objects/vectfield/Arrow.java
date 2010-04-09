package org.scilab.modules.graphic_objects.vectfield;

import org.scilab.modules.graphic_objects.contouredObject.ContouredObject;


/**
 * Arrow class
 * @author juliachs
 */
public class Arrow extends ContouredObject {
	/** Arrow properties names */
	private enum ArrowProperty { BASE, DIRECTION, ARROWSIZE };
	
	/** Arrow base (x,y,z) coordinates */
	private double[] base;

	/** Arrow vector (x,y,z) coordinates */
	private double[] direction;

	/** Arrow size */
	private double arrowSize;

	/** Constructor */
	public Arrow() {
		super();
		base = new double[3];
		direction = new double[3];
		arrowSize = 0.0;
	}
	
	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
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
	 * @param property the property to set
	 * @param value the property value
	 */
	public void setPropertyFast(Object property, Object value) {
		if (property == ArrowProperty.BASE) {
			setBase((Double[]) value);
		} else if (property == ArrowProperty.DIRECTION) {
			setDirection((Double[]) value);
		} else if (property == ArrowProperty.ARROWSIZE) {
			setArrowSize((Double) value);
		} else {
			super.setPropertyFast(property, value);
		}
	}

	/**
	 * @return the arrowSize
	 */
	public Double getArrowSize() {
		return new Double(arrowSize);
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
	public Double[] getBase() {
		Double[] retBase = new Double[3];

		retBase[0] = new Double(base[0]);
		retBase[2] = new Double(base[1]);
		retBase[1] = new Double(base[2]);

		return retBase;
	}

	/**
	 * @param base the base to set
	 */
	public void setBase(Double[] base) {
		this.base[0] = base[0].doubleValue();
		this.base[1] = base[1].doubleValue();
		this.base[2] = base[2].doubleValue();
	}

	/**
	 * @return the direction
	 */
	public Double[] getDirection() {
		Double[] retDirection = new Double[3];

		retDirection[0] = new Double(direction[0]);
		retDirection[1] = new Double(direction[1]);
		retDirection[2] = new Double(direction[2]);
		
		return retDirection;
	}

	/**
	 * @param direction the direction to set
	 */
	public void setDirection(Double[] direction) {
		this.direction[0] = direction[0].doubleValue();
		this.direction[1] = direction[1].doubleValue();
		this.direction[2] = direction[2].doubleValue();
	}

	//	 Methods: to be done
}
