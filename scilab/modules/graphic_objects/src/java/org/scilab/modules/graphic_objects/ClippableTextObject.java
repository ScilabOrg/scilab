package org.scilab.modules.graphic_objects;


/**
 * ClippableTextObject
 * @author juliachs
 */
public abstract class ClippableTextObject extends TextObject {
	/** ClippableTextObject properties names */
	private enum ClippableTextObjectProperty { CLIPPROPERTY };
	
	/** Clipping state */
	private ClippableProperty clipProperty;

	/**  Constructor */
	public ClippableTextObject() {
		clipProperty = null;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the associated property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("ClipProperty")) {
			returnedProp =  ClippableTextObjectProperty.CLIPPROPERTY;
		} else if (propertyName.equals("ClipState")) {
				returnedProp =  ClippableProperty.ClippablePropertyType.CLIPSTATE;
		} else if (propertyName.equals("ClipBox")) {
			returnedProp =  ClippableProperty.ClippablePropertyType.CLIPBOX;
		} else {
			returnedProp = super.getPropertyFromName(propertyName);
		}
		
		return (Object) returnedProp;
	}

	/**
	 * @return the clipProperty
	 */
	public ClippableProperty getClipProperty() {
		return clipProperty;
	}

	/**
	 * @param clipProperty the clipProperty to set
	 */
	public void setClipProperty(ClippableProperty clipProperty) {
		this.clipProperty = clipProperty;
	}

	// Methods: to be done
}
