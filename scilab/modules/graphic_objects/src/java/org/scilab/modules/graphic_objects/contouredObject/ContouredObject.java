package org.scilab.modules.graphic_objects.contouredObject;

import org.scilab.modules.graphic_objects.contouredObject.Line.LinePropertyType;
import org.scilab.modules.graphic_objects.contouredObject.Line.LineType;
import org.scilab.modules.graphic_objects.contouredObject.Mark.MarkPropertyType;
import org.scilab.modules.graphic_objects.contouredObject.Mark.MarkSizeUnitType;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;

/**
 * Contoured object class 
 * @author juliachs
 */
public abstract class ContouredObject extends GraphicObject {
	/** ContouredObject properties */
	public enum ContouredObjectPropertyType { LINE, FILLMODE, BACKGROUND, MARK };
	
	/** Line property */
	private Line line;

	/** Specifies whether the object is filled or not */
	private boolean fillMode;

	/** Background (fill) color */
	private int background;

	/** Mark property */
	private Mark mark;

	/** Default constructor */
	public ContouredObject() {
		line = new Line();
		fillMode = false;
		background = 0;
		mark = new Mark();
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;
		
		if (propertyName.equals("Line")) {
			returnedProp =  ContouredObjectPropertyType.LINE;
		} else if (propertyName.equals("LineMode")) {
			returnedProp = LinePropertyType.MODE;
		} else if (propertyName.equals("LineStyle")) {
			returnedProp = LinePropertyType.LINESTYLE;
		} else if (propertyName.equals("LineThickness")) {
			returnedProp = LinePropertyType.THICKNESS;
		} else if (propertyName.equals("LineColor")) {
			returnedProp = LinePropertyType.COLOR;
		} else if (propertyName.equals("FillMode")) {
			returnedProp =  ContouredObjectPropertyType.FILLMODE;
		} else if (propertyName.equals("Background")) {
			returnedProp =  ContouredObjectPropertyType.BACKGROUND;
		} else if (propertyName.equals("Mark")) {
			returnedProp =  ContouredObjectPropertyType.MARK;
		} else if (propertyName.equals("MarkMode")) {
			returnedProp = MarkPropertyType.MODE;
		} else if (propertyName.equals("MarkStyle")) {
			returnedProp = MarkPropertyType.STYLE;
		} else if (propertyName.equals("MarkSizeUnit")) {
			returnedProp = MarkPropertyType.MARKSIZEUNIT;
		} else if (propertyName.equals("MarkForeground")) {
			returnedProp = MarkPropertyType.FOREGROUND;
		} else if (propertyName.equals("MarkBackground")) {
			returnedProp = MarkPropertyType.BACKGROUND;
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
		Object returnedProp = null;
		
		if (property == ContouredObjectPropertyType.LINE) {
			returnedProp = getLine();
		} else if (property == LinePropertyType.MODE) {
			returnedProp = getLineMode();
		} else if (property == LinePropertyType.LINESTYLE) {
			returnedProp = getLineStyle();
		} else if (property == LinePropertyType.THICKNESS) {
			returnedProp = getThickness();
		} else if (property == LinePropertyType.COLOR) {
			returnedProp = getColor();
		} else if (property == ContouredObjectPropertyType.FILLMODE) {
			returnedProp = getFillMode();
		} else if (property == ContouredObjectPropertyType.BACKGROUND) {
			returnedProp = getBackground();
		} else if (property == ContouredObjectPropertyType.MARK) {
			returnedProp = getMark();
		} else if (property == MarkPropertyType.MODE) {
			returnedProp = getMarkMode();
		} else if (property == MarkPropertyType.STYLE) {
			returnedProp = getMarkStyle();
		} else if (property == MarkPropertyType.MARKSIZEUNIT) {
			returnedProp = getMarkSizeUnit();
		} else if (property == MarkPropertyType.FOREGROUND) {
			returnedProp = getMarkForeground();
		} else if (property == MarkPropertyType.BACKGROUND) {
			returnedProp = getMarkBackground();
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

		if (property == ContouredObjectPropertyType.LINE) {
			setLine((Line) value);
		} else if (property == LinePropertyType.MODE) {
			setLineMode((Boolean) value);
		} else if (property == LinePropertyType.LINESTYLE) {
			setLineStyle((LineType) value);
		} else if (property == LinePropertyType.THICKNESS) {
			setThickness((Double) value);
		} else if (property == LinePropertyType.COLOR) {
			setColor((Integer) value);
		} else if (property == ContouredObjectPropertyType.FILLMODE) {
			setFillMode((Boolean) value);
		} else if (property == ContouredObjectPropertyType.BACKGROUND) {
			setBackground((Integer) value);
		} else if (property == ContouredObjectPropertyType.MARK) {
			setMark((Mark) value);
		} else if (property == MarkPropertyType.MODE) {
			setMarkMode((Boolean) value);
		} else if (property == MarkPropertyType.STYLE) {
			setMarkStyle((Integer) value);
		} else if (property == MarkPropertyType.MARKSIZEUNIT) {
			setMarkSizeUnit((MarkSizeUnitType) value);
		} else if (property == MarkPropertyType.FOREGROUND) {
			this.setMarkForeground((Integer) value);
		} else if (property == MarkPropertyType.BACKGROUND) {
			this.setMarkBackground((Integer) value);
		} else {
			super.setPropertyFast(property, value);
		}
	
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
	 * @return the fillMode
	 */
	public Boolean getFillMode() {
		return new Boolean(fillMode);
	}

	/**
	 * @param fillMode the fillMode to set
	 */
	public void setFillMode(Boolean fillMode) {
		this.fillMode = fillMode.booleanValue();
	}

	/**
	 * @return the line
	 */
	public Line getLine() {
		return line;
	}

	/**
	 * @param line the line to set
	 */
	public void setLine(Line line) {
		this.line = line;
	}

	/**
	 * Get the line color
	 * @return the color
	 */
	public Integer getColor() {
		return line.getColor();
	}

	/**
	 * Set the line color
	 * @param color the color to set
	 */
	public void setColor(Integer color) {
		line.setColor(color);
	}

	/**
	 * Get the line style
	 * @return the lineStyle
	 */
	public LineType getLineStyle() {
		return line.getLineStyle();
	}

	/**
	 * Set the line style
	 * @param lineStyle the lineStyle to set
	 */
	public void setLineStyle(LineType lineStyle) {
		line.setLineStyle(lineStyle);
	}

	/**
	 * Return the line mode
	 * @return the mode
	 */
	public Boolean getLineMode() {
		return line.getMode();
	}

	/**
	 * Set the line mode
	 * @param mode the mode to set
	 */
	public void setLineMode(Boolean mode) {
		line.setMode(mode);
	}

	/**
	 * Get the line thickness
	 * @return the thickness
	 */
	public Double getThickness() {
		return line.getThickness();
	}

	/**
	 * Set the line thickness
	 * @param thickness the thickness to set
	 */
	public void setThickness(Double thickness) {
		line.setThickness(thickness);
	}
	
	/**
	 * @return the mark
	 */
	public Mark getMark() {
		return mark;
	}

	/**
	 * @param mark the mark to set
	 */
	public void setMark(Mark mark) {
		this.mark = mark;
	}
	
	/**
	 * Get the mark background
	 * @return the background
	 */
	public Integer getMarkBackground() {
		return new Integer(mark.getBackground());
	}

	/**
	 * Set the mark background
	 * @param background the background to set
	 */
	public void setMarkBackground(Integer background) {
		mark.setBackground(background);
	}

	/**
	 * Get the mark foreground
	 * @return the foreground
	 */
	public Integer getMarkForeground() {
		return mark.getForeground();
	}

	/**
	 * Set the mark foreground
	 * @param foreground the foreground to set
	 */
	public void setMarkForeground(Integer foreground) {
		mark.setForeground(foreground);
	}

	/**
	 * Get the mark size unit
	 * @return the markSizeUnit
	 */
	public MarkSizeUnitType getMarkSizeUnit() {
		return mark.getMarkSizeUnit();
	}

	/**
	 * Set the mark size unit
	 * @param markSizeUnit the markSizeUnit to set
	 */
	public void setMarkSizeUnit(MarkSizeUnitType markSizeUnit) {
		mark.setMarkSizeUnit(markSizeUnit);
	}

	/**
	 * Get the mark mode
	 * @return the mode
	 */
	public Boolean getMarkMode() {
		return new Boolean(mark.getMode());
	}

	/**
	 * Set the mark mode
	 * @param mode the mode to set
	 */
	public void setMarkMode(Boolean mode) {
		mark.setMode(mode);
	}

	/**
	 * Get the mark style
	 * @return the style
	 */
	public Integer getMarkStyle() {
		return mark.getStyle();
	}

	/**
	 * Set the mark style
	 * @param style the style to set
	 */
	public void setMarkStyle(Integer style) {
		mark.setStyle(style);
	}

	
	// Methods: to be done
}
