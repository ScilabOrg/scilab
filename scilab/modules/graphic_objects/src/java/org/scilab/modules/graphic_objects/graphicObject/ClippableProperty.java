package org.scilab.modules.graphic_objects.graphicObject;


/**
 * ClippableProperty class
 * @author juliachs
 */
public class ClippableProperty {
	/** ClippableProperty properties */
	public enum ClippablePropertyType { CLIPSTATE, CLIPBOX };

	/** Indicates how clipping is performed */
	public static enum ClipStateType { OFF, CLIPGRF, ON };

	/** Clipping state */
	private ClipStateType clipState;

	/** Clip box (4- or 6-element array) */
	private double[] clipBox;
	
	/** Constructor */
	public ClippableProperty() {
		clipState = ClipStateType.OFF;
		clipBox = new double[4];
	}


	/**
	 * @return the clipBox
	 */
	public double[] getClipBox() {
		double[] retClipBox = new double[clipBox.length];

		for (int i = 0; i < clipBox.length; i++) {
			retClipBox[i] = clipBox[i];
		}
		
		return retClipBox;
	}

	/**
	 * @param clipBox the clipBox to set
	 */
	public void setClipBox(double[] clipBox) {
		for (int i = 0; i < clipBox.length; i++) {
			this.clipBox[i] = clipBox[i];
		}
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
