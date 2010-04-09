package org.scilab.modules.graphic_objects.contouredObject;

/**
 * Line class
 * @author juliachs
 */
public class Line {
	/** Line properties */
	public enum LinePropertyType { MODE, LINESTYLE, THICKNESS, COLOR };
	
	/** Line style */
	enum LineType { SOLID, STYLE1, STYLE2, STYLE3, STYLE4, STYLE5, STYLE6 };
	
	/** Specifies whether the line is drawn or not */
	private boolean mode;

	/** Line style */
	private LineType lineStyle;

	/** Line thickness */
	private double thickness;

	/** Line color */
	private int color;

	/** Constructor */
	public Line() {
		mode = false;
		lineStyle = LineType.SOLID;
		thickness = 0.0;
		color = 0;
	}

	/**
	 * @return the color
	 */
	public Integer getColor() {
		return new Integer(color);
	}

	/**
	 * @param color the color to set
	 */
	public void setColor(Integer color) {
		this.color = color.intValue();
	}

	/**
	 * @return the lineStyle
	 */
	public LineType getLineStyle() {
		return lineStyle;
	}

	/**
	 * @param lineStyle the lineStyle to set
	 */
	public void setLineStyle(LineType lineStyle) {
		this.lineStyle = lineStyle;
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
	 * @return the thickness
	 */
	public Double getThickness() {
		return new Double(thickness);
	}

	/**
	 * @param thickness the thickness to set
	 */
	public void setThickness(Double thickness) {
		this.thickness = thickness.doubleValue();
	}
	
	//	 Methods: to be done
}
