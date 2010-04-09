package org.scilab.modules.graphic_objects.axis;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.arc.Arc.ArcDrawingMethod;
import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;
import org.scilab.modules.graphic_objects.textObject.Font;
import org.scilab.modules.graphic_objects.textObject.Font.FontProperty;

/**
 * Axis class
 * @author juliachs
 */
public class Axis extends ClippableContouredObject {
	/** Axis properties */
	private enum AxisProperty { TICKSDIRECTION, XTICKSCOORDS, YTICKSCOORDS, TICKSCOLOR, TICKSSEGMENT, TICKSLABELS,
		FORMATN, FONT };
		
	/** Default number of ticks */
	private static final int DEFAULT_NUMBER_OF_TICKS = 10;

	/** Ticks direction */
	private enum TicksDirection { TOP, BOTTOM, LEFT, RIGHT };
	
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
		yTicksCoords = null;
		ticksColor = 0;
		ticksSegment = false;
		ticksLabels = new ArrayList<String>(DEFAULT_NUMBER_OF_TICKS);
		formatn = null;
		font = null;
	}
	
	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("TicksDirection")) {
			returnedProp =  AxisProperty.TICKSDIRECTION;
		} else if (propertyName.equals("XTicksCoords")) {
			returnedProp =  AxisProperty.XTICKSCOORDS;
		} else if (propertyName.equals("YTicksCoords")) {
			returnedProp =  AxisProperty.YTICKSCOORDS;
		} else if (propertyName.equals("TicksColor")) {
			returnedProp =  AxisProperty.TICKSCOLOR;
		} else if (propertyName.equals("TicksSegment")) {
			returnedProp =  AxisProperty.TICKSSEGMENT;
		} else if (propertyName.equals("TicksLabels")) {
			returnedProp = AxisProperty.TICKSLABELS;
		} else if (propertyName.equals("Formatn")) {
			returnedProp = AxisProperty.FORMATN;
		} else if (propertyName.equals("Font")) {
			returnedProp = AxisProperty.FONT;
		} else if (propertyName.equals("Style")) {
			returnedProp = Font.FontProperty.STYLE;
		} else if (propertyName.equals("Size")) {
			returnedProp = Font.FontProperty.SIZE;
		} else if (propertyName.equals("Color")) {
			returnedProp = Font.FontProperty.COLOR;
		} else if (propertyName.equals("Fractional")) {
			returnedProp = Font.FontProperty.FRACTIONAL;
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
	
		if (property == AxisProperty.TICKSDIRECTION) {
			returnedProp = getTicksDirection();
		} else if (property == AxisProperty.XTICKSCOORDS) {
			returnedProp = getXTicksCoords();
		} else if (property == AxisProperty.YTICKSCOORDS) {
			returnedProp = getYTicksCoords();
		} else if (property == AxisProperty.TICKSCOLOR) {
			returnedProp = getTicksColor();
		} else if (property == AxisProperty.TICKSSEGMENT) {
			returnedProp = getTicksSegment();
		} else if (property == AxisProperty.TICKSLABELS) {
			returnedProp = getTicksLabels();
		} else if (property == AxisProperty.FORMATN) {
			returnedProp = getFormatn();
		} else if (property == AxisProperty.FONT) {
			returnedProp = getFont();
		} else if (property == Font.FontProperty.STYLE) {
			returnedProp = getStyle();
		} else if (property == Font.FontProperty.SIZE) {
			returnedProp = getSize();
		} else if (property == Font.FontProperty.COLOR) {
			returnedProp = getColor();
		} else if (property == Font.FontProperty.FRACTIONAL) {
			returnedProp = getFractional();
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

		if (property == AxisProperty.TICKSDIRECTION) {
			setTicksDirection((TicksDirection) value);
		} else if (property == AxisProperty.XTICKSCOORDS) {
			setXTicksCoords((Double[]) value);
		} else if (property == AxisProperty.YTICKSCOORDS) {
			setYTicksCoords((Double[]) value);
		} else if (property == AxisProperty.TICKSCOLOR) {
			setTicksColor((Integer) value);
		} else if (property == AxisProperty.TICKSSEGMENT) {
			setTicksSegment((Boolean) value);
		} else if (property == AxisProperty.TICKSLABELS) {
			setTicksLabels((ArrayList<String>) value);
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
			super.setPropertyFast(property, value);
		}

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
		return new Integer(ticksColor);
	}

	/**
	 * @param ticksColor the ticksColor to set
	 */
	public void setTicksColor(Integer ticksColor) {
		this.ticksColor = ticksColor.intValue();
	}

	/**
	 * @return the ticksDirection
	 */
	public TicksDirection getTicksDirection() {
		return ticksDirection;
	}

	/**
	 * @param ticksDirection the ticksDirection to set
	 */
	public void setTicksDirection(TicksDirection ticksDirection) {
		this.ticksDirection = ticksDirection;
	}

	/**
	 * @return the ticksLabels
	 */
	public ArrayList<String> getTicksLabels() {
		return ticksLabels;
	}

	/**
	 * @param ticksLabels the ticksLabels to set
	 */
	public void setTicksLabels(ArrayList<String> ticksLabels) {
		this.ticksLabels = ticksLabels;
	}

	/**
	 * @return the ticksSegment
	 */
	public Boolean getTicksSegment() {
		return new Boolean(ticksSegment);
	}

	/**
	 * @param ticksSegment the ticksSegment to set
	 */
	public void setTicksSegment(Boolean ticksSegment) {
		this.ticksSegment = ticksSegment;
	}

	/**
	 * @return the xTicksCoords
	 */
	public Double[] getXTicksCoords() {
		Double[] retXTicksCoords = new Double[xTicksCoords.length];

		for (int i = 0; i < xTicksCoords.length; i++) {
			retXTicksCoords[i] = new Double(xTicksCoords[i]);
		}

		return retXTicksCoords;
	}

	/**
	 * @param ticksCoords the xTicksCoords to set
	 */
	public void setXTicksCoords(Double[] ticksCoords) {
		for (int i = 0; i < xTicksCoords.length; i++) {
			xTicksCoords[i] = ticksCoords[i].doubleValue();
		}
	}

	/**
	 * @return the yTicksCoords
	 */
	public Double[] getYTicksCoords() {
		Double[] retYTicksCoords = new Double[yTicksCoords.length];

		for (int i = 0; i < yTicksCoords.length; i++) {
			retYTicksCoords[i] = new Double(yTicksCoords[i]);
		}

		return retYTicksCoords;
	}

	/**
	 * @param ticksCoords the yTicksCoords to set
	 */
	public void setYTicksCoords(Double[] ticksCoords) {
		for (int i = 0; i < yTicksCoords.length; i++) {
			yTicksCoords[i] = ticksCoords[i].doubleValue();
		}
	}
	
	//	 Methods: to be done
}
