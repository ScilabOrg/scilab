/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.axis;

import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;
import org.scilab.modules.graphic_objects.graphicObject.IVisitor;
import org.scilab.modules.graphic_objects.textObject.Font;

import java.util.ArrayList;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_COLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_FRACTIONAL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FORMATN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_NUMBER_TICKS_LABELS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SUBTICKS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TICKS_COLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TICKS_DIRECTION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TICKS_LABELS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TICKS_SEGMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TICKS_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_X_NUMBER_TICKS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_X_TICKS_COORDS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_Y_NUMBER_TICKS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_Y_TICKS_COORDS__;

/**
 * Axis class
 * @author Manuel JULIACHS
 */
public class Axis extends ClippableContouredObject {
	/** Axis properties */
	private enum AxisProperty { TICKSDIRECTION, XNUMBERTICKS, YNUMBERTICKS, XTICKSCOORDS, YTICKSCOORDS, TICKSCOLOR, TICKSSEGMENT, TICKSSTYLE,
		SUBTICKS, NUMBERTICKSLABELS, TICKSLABELS, FORMATN, FONT };
		
	/** Default number of ticks */
	private static final int DEFAULT_NUMBER_OF_TICKS = 10;

	/** Ticks direction */
    public enum TicksDirection {TOP, BOTTOM, LEFT, RIGHT}
	
	/** Ticks direction */
	private TicksDirection ticksDirection;

	/** Ticks x-coordinate position vector */
	private double[] xTicksCoords; 

	/** Ticks y-coordinate position vector */
	private double[] yTicksCoords;
	
	/** Ticks color */
	private int ticksColor;

	/** Specifies whether the axis segment is drawn */
	private boolean ticksSegment;

	/** Specifies the ticks style (either 0, 1, or 2) */
	private int ticksStyle;

	/** Number of subticks */
	private int subticks;

	/** Ticks labels list */
	private ArrayList <String> ticksLabels;

	/** Label format */
	private String formatn;

	/** Font */
	private Font font;

	/** Constructor */
	public Axis() {
		super();
		ticksDirection = TicksDirection.TOP;
		xTicksCoords = new double[DEFAULT_NUMBER_OF_TICKS];
		yTicksCoords = new double[1];
		ticksColor = 0;
		ticksSegment = false;
		ticksLabels = new ArrayList<String>(DEFAULT_NUMBER_OF_TICKS);
		formatn = new String("");
		font = new Font();
	}

    @Override
    public void accept(IVisitor visitor) {
        visitor.visit(this);
    }

    /**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals(__GO_TICKS_DIRECTION__)) {
			return AxisProperty.TICKSDIRECTION;
		} else if (propertyName.equals(__GO_X_NUMBER_TICKS__)) {
			return AxisProperty.XNUMBERTICKS;
		} else if (propertyName.equals(__GO_Y_NUMBER_TICKS__)) {
			return AxisProperty.YNUMBERTICKS;
		} else if (propertyName.equals(__GO_X_TICKS_COORDS__)) {
			return AxisProperty.XTICKSCOORDS;
		} else if (propertyName.equals(__GO_Y_TICKS_COORDS__)) {
			return AxisProperty.YTICKSCOORDS;
		} else if (propertyName.equals(__GO_TICKS_COLOR__)) {
			return AxisProperty.TICKSCOLOR;
		} else if (propertyName.equals(__GO_TICKS_SEGMENT__)) {
			return AxisProperty.TICKSSEGMENT;
		} else if (propertyName.equals(__GO_TICKS_STYLE__)) {
			return AxisProperty.TICKSSTYLE;
		} else if (propertyName.equals(__GO_SUBTICKS__)) {
			return AxisProperty.SUBTICKS;
		} else if (propertyName.equals(__GO_NUMBER_TICKS_LABELS__)) {
			return AxisProperty.NUMBERTICKSLABELS;
		} else if (propertyName.equals(__GO_TICKS_LABELS__)) {
			return AxisProperty.TICKSLABELS;
		} else if (propertyName.equals(__GO_FORMATN__)) {
			return AxisProperty.FORMATN;
		} else if (propertyName.equals(__GO_FONT__)) {
			return AxisProperty.FONT;
		} else if (propertyName.equals(__GO_FONT_STYLE__)) {
			return Font.FontProperty.STYLE;
		} else if (propertyName.equals(__GO_FONT_SIZE__)) {
			return Font.FontProperty.SIZE;
		} else if (propertyName.equals(__GO_FONT_COLOR__)) {
			return Font.FontProperty.COLOR;
		} else if (propertyName.equals(__GO_FONT_FRACTIONAL__)) {
			return Font.FontProperty.FRACTIONAL;
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
		if (property == AxisProperty.TICKSDIRECTION) {
			return getTicksDirection();
		} else if (property == AxisProperty.XNUMBERTICKS) {
			return getXNumberTicks();
		} else if (property == AxisProperty.YNUMBERTICKS) {
			return getYNumberTicks();
		} else if (property == AxisProperty.XTICKSCOORDS) {
			return getXTicksCoords();
		} else if (property == AxisProperty.YTICKSCOORDS) {
			return getYTicksCoords();
		} else if (property == AxisProperty.TICKSCOLOR) {
			return getTicksColor();
		} else if (property == AxisProperty.TICKSSEGMENT) {
			return getTicksSegment();
		} else if (property == AxisProperty.TICKSSTYLE) {
			return getTicksStyle();
		} else if (property == AxisProperty.SUBTICKS) {
			return getSubticks();
		} else if (property == AxisProperty.NUMBERTICKSLABELS) {
			return getNumberTicksLabels();
		} else if (property == AxisProperty.TICKSLABELS) {
			return getTicksLabels();
		} else if (property == AxisProperty.FORMATN) {
			return getFormatn();
		} else if (property == AxisProperty.FONT) {
			return getFont();
		} else if (property == Font.FontProperty.STYLE) {
			return getStyle();
		} else if (property == Font.FontProperty.SIZE) {
			return getSize();
		} else if (property == Font.FontProperty.COLOR) {
			return getColor();
		} else if (property == Font.FontProperty.FRACTIONAL) {
			return getFractional();
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
		if (property == AxisProperty.TICKSDIRECTION) {
			setTicksDirection((Integer) value);
		} else if (property == AxisProperty.XTICKSCOORDS) {
			setXTicksCoords((Double[]) value);
		} else if (property == AxisProperty.YTICKSCOORDS) {
			setYTicksCoords((Double[]) value);
		} else if (property == AxisProperty.TICKSCOLOR) {
			setTicksColor((Integer) value);
		} else if (property == AxisProperty.TICKSSEGMENT) {
			setTicksSegment((Boolean) value);
		} else if (property == AxisProperty.TICKSSTYLE) {
			setTicksStyle((Integer) value);
		} else if (property == AxisProperty.SUBTICKS) {
			setSubticks((Integer) value);
		} else if (property == AxisProperty.TICKSLABELS) {
			setTicksLabels((String[]) value);
		} else if (property == AxisProperty.FORMATN) {
			setFormatn((String) value);
		} else if (property == AxisProperty.FONT) {
			setFont((Font) value);
		} else if (property == Font.FontProperty.STYLE) {
			setStyle((Integer) value);
		} else if (property == Font.FontProperty.SIZE) {
			setSize((Double) value);
		} else if (property == Font.FontProperty.COLOR) {
			setColor((Integer) value);
		} else if (property == Font.FontProperty.FRACTIONAL) {
			setFractional((Boolean) value);
		} else {
			return super.setProperty(property, value);
		}

		return true;
	}

	/**
	 * @return the font
	 */
	public Font getFont() {
		return font;
	}

	/**
	 * @param font the font to set
	 */
	public void setFont(Font font) {
		this.font = font;
	}

	/**
	 * @return the font style
	 */
	public Integer getStyle() {
		return font.getStyle();
	}

	/**
	 * @param style the font style to set
	 */
	public void setStyle(Integer style) {
		font.setStyle(style);
	}

	/**
	 * @return the font size
	 */
	public Double getSize() {
		return font.getSize();
	}

	/**
	 * @param size the font size to set
	 */
	public void setSize(Double size) {
		font.setSize(size);
	}

	/**
	 * @return the font color
	 */
	public Integer getColor() {
		return font.getColor();
	}

	/**
	 * @param color the font color to set
	 */
	public void setColor(Integer color) {
		font.setColor(color);
	}

	/**
	 * @return the font fractional
	 */
	public Boolean getFractional() {
		return font.getFractional();
	}

	/**
	 * @param fractional the font fractional to set
	 */
	public void setFractional(Boolean fractional) {
		font.setFractional(fractional);
	}

	/**
	 * @return the formatn
	 */
	public String getFormatn() {
		return formatn;
	}

	/**
	 * @param formatn the formatn to set
	 */
	public void setFormatn(String formatn) {
		this.formatn = formatn;
	}

	/**
	 * @return the ticksColor
	 */
	public Integer getTicksColor() {
		return ticksColor;
	}

	/**
	 * @param ticksColor the ticksColor to set
	 */
	public void setTicksColor(Integer ticksColor) {
		this.ticksColor = ticksColor;
	}

	/**
	 * @return the ticksDirection
	 */
	public Integer getTicksDirection() {
		return getTicksDirectionAsEnum().ordinal();
	}

	/**
	 * @return the ticksDirection
	 */
	public TicksDirection getTicksDirectionAsEnum() {
		return ticksDirection;
	}

	/**
	 * @param ticksDirection the ticksDirection to set
	 */
	public void setTicksDirection(Integer ticksDirection) {
		setTicksDirectionAsEnum(TicksDirection.values()[ticksDirection]);
	}

	/**
	 * @param ticksDirection the ticksDirection to set
	 */
	public void setTicksDirectionAsEnum(TicksDirection ticksDirection) {
		this.ticksDirection = ticksDirection;
	}

	/**
	 * @return the number of ticks labels
	 */
	public Integer getNumberTicksLabels() {
		return ticksLabels.size();
	}

	/**
	 * @return the ticksLabels
	 */
	public String[] getTicksLabels() {
		String[] labels = new String[ticksLabels.size()];

		for (int i = 0; i < ticksLabels.size(); i++) {
			labels[i] = new String(ticksLabels.get(i));
		}

		return labels;
	}

	/**
	 * @return the ticksLabels
	 */
	public ArrayList<String> getTicksLabelsAsArrayList() {
		return ticksLabels;
	}

	/**
	 * @param ticksLabels the ticksLabels to set
	 */
	public void setTicksLabels(String[] ticksLabels) {
		if (!this.ticksLabels.isEmpty()) {
			this.ticksLabels.clear();
		}

		for (int i = 0; i < ticksLabels.length; i++) {
			this.ticksLabels.add(new String(ticksLabels[i]));
		}

	}

	/**
	 * @param ticksLabels the ticksLabels to set
	 */
	public void setTicksLabelsAsArrayList(ArrayList<String> ticksLabels) {
		if (!this.ticksLabels.isEmpty()) {
			this.ticksLabels.clear();
		}

		for (int i = 0; i < ticksLabels.size(); i++) {
			this.ticksLabels.add(new String(ticksLabels.get(i)));
		}

	}

	/**
	 * @return the ticksSegment
	 */
	public Boolean getTicksSegment() {
		return ticksSegment;
	}

	/**
	 * @param ticksSegment the ticksSegment to set
	 */
	public void setTicksSegment(Boolean ticksSegment) {
		this.ticksSegment = ticksSegment;
	}

	/**
	 * @return the ticksStyle
	 */
	public Integer getTicksStyle() {
		return ticksStyle;
	}

	/**
	 * @param ticksStyle the ticksStyle to set
	 */
	public void setTicksStyle(Integer ticksStyle) {
		this.ticksStyle = ticksStyle;
	}

	/**
	 * @return the subticks
	 */
	public Integer getSubticks() {
		return subticks;
	}

	/**
	 * @param subticks the subticks to set
	 */
	public void setSubticks(Integer subticks) {
		this.subticks = subticks;
	}

	/**
	 * @return the number of X ticks
	 */
	public Integer getXNumberTicks() {
		return xTicksCoords.length;
	}

	/**
	 * @return the number of X ticks
	 */
	public Integer getYNumberTicks() {
		return yTicksCoords.length;
	}

	/**
	 * @return the xTicksCoords
	 */
	public Double[] getXTicksCoords() {
		Double[] retXTicksCoords = new Double[xTicksCoords.length];

		for (int i = 0; i < xTicksCoords.length; i++) {
			retXTicksCoords[i] = xTicksCoords[i];
		}

		return retXTicksCoords;
	}

	/**
	 * Sets the x ticks coordinates
	 * Resizes the array if required
     * TODO : we should use format_n to fill ticks label.
	 * @param ticksCoords the xTicksCoords to set
	 */
	public void setXTicksCoords(Double[] ticksCoords) {
		if (ticksCoords.length != xTicksCoords.length) {
			xTicksCoords = new double[ticksCoords.length];
		}

		for (int i = 0; i < xTicksCoords.length; i++) {
			xTicksCoords[i] = ticksCoords[i];
		}
	}

	/**
	 * @return the yTicksCoords
	 */
	public Double[] getYTicksCoords() {
		Double[] retYTicksCoords = new Double[yTicksCoords.length];

		for (int i = 0; i < yTicksCoords.length; i++) {
			retYTicksCoords[i] = yTicksCoords[i];
		}

		return retYTicksCoords;
	}

	/**
	 * Sets the y ticks coordinates
	 * Resizes the array if required
     * TODO : we should use format_n to fill ticks label.
	 * @param ticksCoords the yTicksCoords to set
	 */
	public void setYTicksCoords(Double[] ticksCoords) {
		if (ticksCoords.length != yTicksCoords.length) {
			yTicksCoords = new double[ticksCoords.length];
		}

		for (int i = 0; i < yTicksCoords.length; i++) {
			yTicksCoords[i] = ticksCoords[i];
		}
	}

	/**
	 * @return Type as String
	 */
	public String getType() {
		return "Axis";
	}

}
