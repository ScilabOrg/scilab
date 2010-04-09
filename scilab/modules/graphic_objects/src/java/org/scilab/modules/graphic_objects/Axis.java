package org.scilab.modules.graphic_objects;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.Arc.ArcDrawingMethod;

/**
 * Axis class
 * @author juliachs
 */
public class Axis extends ClippableContouredObject {
	/** Axis properties */
	private enum AxisProperty { TICKSDIRECTION, XTICKSCOORDS, YTICKSCOORDS, TICKSCOLOR, TICKSSEGMENT, TICKSLABELS,
		FORMATN, FONT, UNKNOWNPROP };

	/** Ticks direction */
	private enum TicksDirection { TOP, BOTTOM, LEFT, RIGHT };
	
	/** Ticks direction */
	private TicksDirection ticksDirection;

	/** Ticks x-coordinate position vector */
	private double [] xTicksCoords; 

	/** Ticks y-coordinate position vector */
	private double [] yTicksCoords;
	
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
		xTicksCoords = null;
		yTicksCoords = null;
		ticksColor = 0;
		ticksSegment = false;
		ticksLabels = null;
		formatn = null;
		font = null;
	}
	
	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the associated property enum
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
//					
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
			returnedProp = font.getStyle();
		} else if (property == Font.FontProperty.SIZE) {
			returnedProp = font.getSize();
		} else if (property == Font.FontProperty.COLOR) {
			returnedProp = font.getColor();
		} else if (property == Font.FontProperty.FRACTIONAL) {
			returnedProp = font.getFractional();
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
		if (propertyEnum == AxisProperty.TICKSDIRECTION) {
			setTicksDirection((TicksDirection) value);
		} else if (propertyEnum == AxisProperty.XTICKSCOORDS) {
			setXTicksCoords((double []) value);
		} else if (propertyEnum == AxisProperty.YTICKSCOORDS) {
			setYTicksCoords((double []) value);
		} else if (propertyEnum == AxisProperty.TICKSCOLOR) {
			setTicksColor((Integer) value);
		} else if (propertyEnum == AxisProperty.TICKSSEGMENT) {
			setTicksSegment((Boolean) value);
		} else if (propertyEnum == AxisProperty.TICKSLABELS) {
			setTicksLabels((ArrayList<String>) value);
		} else if (propertyEnum == AxisProperty.FORMATN) {
			setFormatn((String) value);
		} else if (propertyEnum == AxisProperty.FONT) {
			setFont((Font) value);
		} else if (propertyEnum == Font.FontProperty.STYLE) {
			font.setStyle((Integer) value);
		} else if (propertyEnum == Font.FontProperty.SIZE) {
			font.setSize((Double) value);
		} else if (propertyEnum == Font.FontProperty.COLOR) {
			font.setColor((Integer) value);
		} else if (propertyEnum == Font.FontProperty.FRACTIONAL) {
			font.setFractional((Boolean) value);
		} else if (propertyEnum == AxisProperty.UNKNOWNPROP) {
			System.out.format("UNKNOWN PROPERTY !");
		} else {
			super.setPropertyFast(propertyEnum, value);
			
			return;
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
	public double[] getXTicksCoords() {
		return xTicksCoords;
	}

	/**
	 * @param ticksCoords the xTicksCoords to set
	 */
	public void setXTicksCoords(double[] ticksCoords) {
		xTicksCoords = ticksCoords;
	}

	/**
	 * @return the yTicksCoords
	 */
	public double[] getYTicksCoords() {
		return yTicksCoords;
	}

	/**
	 * @param ticksCoords the yTicksCoords to set
	 */
	public void setYTicksCoords(double[] ticksCoords) {
		yTicksCoords = ticksCoords;
	}
	
	//	 Methods: to be done
}
