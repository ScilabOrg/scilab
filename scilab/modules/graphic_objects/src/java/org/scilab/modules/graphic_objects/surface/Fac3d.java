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

package org.scilab.modules.graphic_objects.surface;

/**
 * Fac3d class
 * @author Manuel JULIACHS
 */
public class Fac3d extends Surface {
	/** Fac3d properties names */
	private enum Fac3dProperty { DATAMAPPING };
	
	/** Data mapping type */
	private enum DataMapping { SCALED, DIRECT;

		/**
		 * Converts an integer to the corresponding enum
		 * @param intValue the integer value
		 * @return the data mapping enum
		 */
		public static DataMapping intToEnum(Integer intValue) {
			switch (intValue) {
				case 0:
					return DataMapping.SCALED;
				case 1:
					return DataMapping.DIRECT;
				default:
					return null;
			}
		}
	}

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
		if (propertyName.equals("DataMapping")) {
			return Fac3dProperty.DATAMAPPING;
		} else {
			return super.getPropertyFromName(propertyName);
		}
	}

	/**
	 * Fast property get method
	 * @param property the property to get
	 * @return the property value
	 */
	public Object getProperty(Object property) {
		if (property == Fac3dProperty.DATAMAPPING) {
			return getDataMapping();
		} else {
			return super.getProperty(property);	
		}
	}

	/**
	 * Fast property set method
	 * @param property the property to set
	 * @param value the property value
	 * @return true if the property has been set, false otherwise
	 */
	public boolean setProperty(Object property, Object value) {
		if (property == Fac3dProperty.DATAMAPPING) {
			setDataMapping((Integer) value);
		} else {
			return super.setProperty(property, value);
		}

		return true;
	}

	/**
	 * @return the dataMapping
	 */
	public Integer getDataMapping() {
		return getDataMappingAsEnum().ordinal();
	}

	/**
	 * @return the dataMapping
	 */
	public DataMapping getDataMappingAsEnum() {
		return dataMapping;
	}

	/**
	 * @param dataMapping the dataMapping to set
	 */
	public void setDataMapping(Integer dataMapping) {
		setDataMappingAsEnum(DataMapping.intToEnum(dataMapping));
	}

	/**
	 * @param dataMapping the dataMapping to set
	 */
	public void setDataMappingAsEnum(DataMapping dataMapping) {
		this.dataMapping = dataMapping;
	}

}
