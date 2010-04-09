package org.scilab.modules.graphic_objects.surface;


/**
 * Fac3d class
 * @author juliachs
 */
public class Fac3d extends Surface {
	/** Fac3d properties names */
	private enum Fac3dProperty { DATAMAPPING };
	
	/** Data mapping type */
	private enum DataMapping { SCALED, DIRECT };

	/** Specifies how colors are mapped to scalar values */
	private DataMapping dataMapping;

	/** Constructor */
	public Fac3d() {
		super();
		dataMapping = DataMapping.DIRECT;
	}
	
	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("DataMapping")) {
			returnedProp =  Fac3dProperty.DATAMAPPING;
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
	
		if (property == Fac3dProperty.DATAMAPPING) {
			returnedProp = getDataMapping();
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
		if (property == Fac3dProperty.DATAMAPPING) {
			setDataMapping((DataMapping) value);
		} else {
			super.setPropertyFast(property, value);
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

	// Methods: to be done
}
