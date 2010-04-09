package org.scilab.modules.graphic_objects;


/**
 * Grayplot class
 * @author juliachs
 */
public class Grayplot extends Imageplot {
	/* TBD: properties relative to the data model */ 
	/** Grayplot properties names */
	private enum GrayplotProperty { DATAMAPPING };

	/** Data mapping */
	private enum DataMapping { SCALED, DIRECT };

	/** Specifies how colors are mapped to values */
	private DataMapping dataMapping;

	/** Constructor */
	public Grayplot() {
		super();
		dataMapping = DataMapping.DIRECT;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the associated property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("DataMapping")) {
			returnedProp =  GrayplotProperty.DATAMAPPING;
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

		if (property == GrayplotProperty.DATAMAPPING) {
			returnedProp = getDataMapping();
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
		if (propertyEnum == GrayplotProperty.DATAMAPPING) {
			setDataMapping((DataMapping) value);
		} else {
			super.setPropertyFast(propertyEnum, value);
		}
	}
	

	/**
	 * @return the dataMapping
	 */
	public DataMapping getDataMapping() {
		return dataMapping;
	}

	/**
	 * @param dataMapping the dataMapping to set
	 */
	public void setDataMapping(DataMapping dataMapping) {
		this.dataMapping = dataMapping;
	}

	//	 Methods: to be done
}
