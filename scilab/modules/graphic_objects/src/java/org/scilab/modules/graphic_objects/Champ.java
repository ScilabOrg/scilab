package org.scilab.modules.graphic_objects;

import java.util.ArrayList;

/**
 * Champ class
 * @author juliachs
 */
public class Champ extends VectField {
	/** Champ properties names */
	private enum ChampProperty { COLORED };
	
	/** Specifies whether vectors are colored or not */
	private boolean colored;

	/** Constructor */
	public Champ() {
		super();
		colored = false;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the associated property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("Colored")) {
			returnedProp =  ChampProperty.COLORED;
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
	
		if (property == ChampProperty.COLORED) {
			returnedProp = this.getColored();
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
		if (propertyEnum == ChampProperty.COLORED) {
			setColored((Boolean) value);
		} else {
			super.setPropertyFast(propertyEnum, value);
		}
	}


	/**
	 * @return the colored
	 */
	public boolean getColored() {
		return colored;
	}

	/**
	 * @param colored the colored to set
	 */
	public void setColored(Boolean colored) {
		this.colored = colored.booleanValue();
	}

	// Methods: to be done
}
