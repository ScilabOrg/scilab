package org.scilab.modules.graphic_objects.surface;

import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;

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
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
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
	 * @param property the property to set
	 * @param value the property value
	 */
	public void setPropertyFast(Object property, Object value) {

		if (property == SurfaceProperty.SURFACEMODE) {
			setSurfaceMode((Boolean) value);
		} else if (property == SurfaceProperty.COLORMODE) {
			setColorMode((Integer) value);
		} else if (property == SurfaceProperty.COLORFLAG) {
			setColorFlag((Integer) value);
		} else {
			super.setPropertyFast(property, value);
		}
	}	


	/**
	 * @return the colorFlag
	 */
	public Integer getColorFlag() {
		return new Integer(colorFlag);
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
	public Integer getColorMode() {
		return new Integer(colorMode);
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
	public Boolean getSurfaceMode() {
		return new Boolean(surfaceMode);
	}

	/**
	 * @param surfaceMode the surfaceMode to set
	 */
	public void setSurfaceMode(Boolean surfaceMode) {
		this.surfaceMode = surfaceMode.booleanValue();
	}
	
	//	 Methods: to be done
}
