package org.scilab.modules.graphic_objects.contouredObject;

/**
 * Mark class
 * @author juliachs
 */
public class Mark {
	/** Mark properties */
	public enum MarkPropertyType { MODE, STYLE, MARKSIZEUNIT, FOREGROUND, BACKGROUND };

	/** Mark size unit type */
	public enum MarkSizeUnitType { POINT, TABULATED };

	/** Specifies whether marks must be drawn or not */
	private boolean mode;

	/** Mark style */
	private int style;

	/** Mark size unit */
	private MarkSizeUnitType markSizeUnit;

	/** Foreground color */
	private int foreground;

	/** Background color */
	private int background;

	/** Constructor */
	public Mark() {
		super();
		mode = false;
		style = 0;
		markSizeUnit = MarkSizeUnitType.POINT;
		foreground = 0;
		background = 0;
	}

	/**
	 * @return the background
	 */
	public Integer getBackground() {
		return new Integer(background);
	}

	/**
	 * @param background the background to set
	 */
	public void setBackground(Integer background) {
		this.background = background.intValue();
	}

	/**
	 * @return the foreground
	 */
	public Integer getForeground() {
		return new Integer(foreground);
	}

	/**
	 * @param foreground the foreground to set
	 */
	public void setForeground(Integer foreground) {
		this.foreground = foreground.intValue();
	}

	/**
	 * @return the markSizeUnit
	 */
	public MarkSizeUnitType getMarkSizeUnit() {
		return markSizeUnit;
	}

	/**
	 * @param markSizeUnit the markSizeUnit to set
	 */
	public void setMarkSizeUnit(MarkSizeUnitType markSizeUnit) {
		this.markSizeUnit = markSizeUnit;
	}

	/**
	 * @return the mode
	 */
	public Boolean getMode() {
		return new Boolean(mode);
	}

	/**
	 * @param mode the mode to set
	 */
	public void setMode(Boolean mode) {
		this.mode = mode.booleanValue();
	}

	/**
	 * @return the style
	 */
	public Integer getStyle() {
		return new Integer(style);
	}

	/**
	 * @param style the style to set
	 */
	public void setStyle(Integer style) {
		this.style = style.intValue();
	}
	
	//	 Methods: to be done
}
