package org.scilab.modules.graphic_objects;


/**
 * ClippableProperty class
 * @author juliachs
 */
public class ClippableProperty {
	/** ClippableProperty properties */
	public enum ClippablePropertyType { CLIPSTATE, CLIPBOX, UNKNOWNPROP };
	
	/** Indicates how clipping is performed */
	public static enum ClipStateType { OFF, CLIPGRF, ON };

	/** Clipping state */
	private ClipStateType clipState;

	/** Clip box (4- or 6-element array) */
	private double [] clipBox;
	
	/** Constructor */
	public ClippableProperty() {
		clipState = ClipStateType.OFF;
		clipBox = null;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the associated property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		ClippablePropertyType returnedProp;

		if (propertyName.equals("ClipState")) {
			returnedProp =  ClippablePropertyType.CLIPSTATE;
		} else if (propertyName.equals("ClipBox")) {
			returnedProp =  ClippablePropertyType.CLIPBOX;
		} else {
			returnedProp =  ClippablePropertyType.UNKNOWNPROP;
		}

		return returnedProp;
	}

	/**
	 * Fast property get method
	 * @param property the property to get
	 * @return the property
	 */
	// Will be deleted
	public Object getPropertyFast(Object property) {
		Object returnedProp = null;
	
		if (property == ClippablePropertyType.CLIPSTATE) {
			returnedProp = getClipState();
		} else if (property == ClippablePropertyType.CLIPBOX) {
			returnedProp = getClipBox();
		}

		return returnedProp;
	}



	/**
	 * @return the clipBox
	 */
	public double[] getClipBox() {
		return clipBox;
	}

	/**
	 * @param clipBox the clipBox to set
	 */
	public void setClipBox(double[] clipBox) {
		this.clipBox = clipBox;
	}

	/**
	 * @return the clipState
	 */
	public ClipStateType getClipState() {
		return clipState;
	}

	/**
	 * @param clipState the clipState to set
	 */
	public void setClipState(ClipStateType clipState) {
		this.clipState = clipState;
	}

	// Methods: to be done
}
