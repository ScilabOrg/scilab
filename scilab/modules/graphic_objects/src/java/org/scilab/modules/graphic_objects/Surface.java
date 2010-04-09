package org.scilab.modules.graphic_objects;


/**
 * Surface class
 * @author juliachs
 */
public abstract class Surface extends ClippableContouredObject {
	// TBD Data -> Data Model
	/* TBD: properties relative to the data model */
	/** Surface properties names */
	private enum SurfaceProperty { SURFACEMODE, COLORMODE, COLORFLAG };

	/** Specifies whether the surface is drawn or not */
	private boolean surfaceMode;

	/** Specifies the front face color (when colorFlag == 0) */
	private int colorMode;

	/** Specifies how facet colors are computed */
	private int colorFlag;

	/** Constructor */	
	public Surface() {
		super();
		surfaceMode = false;
		colorMode = 0;
		colorFlag = 0;
	}

	/**
	 * Return the enum associated to a property name
	 * @param propertyName the property name
	 * @return the associated property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("SurfaceMode")) {
			returnedProp =  SurfaceProperty.SURFACEMODE;
		} else if (propertyName.equals("ColorMode")) {
			returnedProp =  SurfaceProperty.COLORMODE;
		} else if (propertyName.equals("ColorFlag")) {
			returnedProp =  SurfaceProperty.COLORFLAG;
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
	
		if (property == SurfaceProperty.SURFACEMODE) {
			returnedProp = getSurfaceMode();
		} else if (property == SurfaceProperty.COLORMODE) {
			returnedProp = getColorMode();
		} else if (property == SurfaceProperty.COLORFLAG) {
			returnedProp = getColorFlag();
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
		if (propertyEnum == SurfaceProperty.SURFACEMODE) {
			setSurfaceMode((Boolean) value);
		} else if (propertyEnum == SurfaceProperty.COLORMODE) {
			setColorMode((Integer) value);
		} else if (propertyEnum == SurfaceProperty.COLORFLAG) {
			setColorFlag((Integer) value);
		} else {
			super.setPropertyFast(propertyEnum, value);
		}
	}	


	/**
	 * @return the colorFlag
	 */
	public int getColorFlag() {
		return colorFlag;
	}

	/**
	 * @param colorFlag the colorFlag to set
	 */
	public void setColorFlag(Integer colorFlag) {
		this.colorFlag = colorFlag.intValue();
	}

	/**
	 * @return the colorMode
	 */
	public int getColorMode() {
		return colorMode;
	}

	/**
	 * @param colorMode the colorMode to set
	 */
	public void setColorMode(Integer colorMode) {
		this.colorMode = colorMode.intValue();
	}

	/**
	 * @return the surfaceMode
	 */
	public boolean getSurfaceMode() {
		return surfaceMode;
	}

	/**
	 * @param surfaceMode the surfaceMode to set
	 */
	public void setSurfaceMode(Boolean surfaceMode) {
		this.surfaceMode = surfaceMode.booleanValue();
	}
	
	//	 Methods: to be done
}
