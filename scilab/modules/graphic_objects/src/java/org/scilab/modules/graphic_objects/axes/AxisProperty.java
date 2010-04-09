package org.scilab.modules.graphic_objects.axes;

import org.scilab.modules.graphic_objects.label.Label;

/**
 * Axis property class
 * @author juliachs
 */
public class AxisProperty {
	/**
	 * AxisProperty properties names
	 */
	public enum AxisPropertyProperty { VISIBLE, REVERSE, GRIDCOLOR, LABEL, AXISLOCATION,
		TICKS, LOGFLAG };

	/**
	 * Axis location type
	 * BOTTOM, TOP, MIDDLE, ORIGIN are allowed for an x-axis,
	 * LEFT, RIGHT, MIDDLE or ORIGIN are allowed for a y-axis
	 */
	private static enum AxisLocation { BOTTOM, TOP, MIDDLE, ORIGIN, LEFT, RIGHT };
	
	/** Specifies whether the axis is visible or not */
	private boolean visible;
	
	/** Specifies whether the axis is reversed or not */
	private boolean reverse;
	
	/** Grid color */
	private int gridColor;

	/** Axis label */
	private Label label;

	/** Axis location */
	private AxisLocation axisLocation;

	/** Ticks property */
	private TicksProperty ticks;

	/** Specifies whether logarithmic coordinates are used or not */
	private boolean logFlag;

	/** Constructor */
	public AxisProperty() {
		visible = false;
		reverse = false;
		gridColor = 0;
		label = new Label();
		axisLocation = null;
		ticks = new TicksProperty();
		logFlag = false;
	}
	
	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp = null;

		if (propertyName.equals("Visible")) {
			returnedProp =  AxisPropertyProperty.VISIBLE;
		} else if (propertyName.equals("Reverse")) {
			returnedProp =  AxisPropertyProperty.REVERSE;
		} else if (propertyName.equals("GridColor")) {
			returnedProp =  AxisPropertyProperty.GRIDCOLOR;
		} else if (propertyName.equals("Label")) {
			returnedProp =  AxisPropertyProperty.LABEL;
		} else if (propertyName.equals("AxisLocation")) {
			returnedProp =  AxisPropertyProperty.AXISLOCATION;
		} else if (propertyName.equals("Ticks")) {
			returnedProp = AxisPropertyProperty.TICKS;
		} else if (propertyName.equals("LogFlag")) {
			returnedProp = AxisPropertyProperty.LOGFLAG;
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

		if (property == AxisPropertyProperty.VISIBLE) {
			returnedProp = getVisible();
		} else if (property == AxisPropertyProperty.REVERSE) {
			returnedProp = getReverse();
		} else if (property == AxisPropertyProperty.GRIDCOLOR) {
			returnedProp = getGridColor();
		} else if (property == AxisPropertyProperty.LABEL) {
			returnedProp = getLabel();
		} else if (property == AxisPropertyProperty.AXISLOCATION) {
			returnedProp = getAxisLocation();
		} else if (property == AxisPropertyProperty.TICKS) {
			returnedProp = getTicks();
		} else if (property == AxisPropertyProperty.LOGFLAG) {
			returnedProp = getLogFlag();
		}

		return returnedProp;
	}

	/**
	 * Fast property set method
	 * @param property the property to set
	 * @param value the property value
	 */
	public void setPropertyFast(Object property, Object value) {

		if (property == AxisPropertyProperty.VISIBLE) {
			setVisible((Boolean) value);
		} else if (property == AxisPropertyProperty.REVERSE) {
			setReverse((Boolean) reverse);
		} else if (property == AxisPropertyProperty.GRIDCOLOR) {
			setGridColor((Integer) value);
		} else if (property == AxisPropertyProperty.LABEL) {
			setLabel((Label) value);
		} else if (property == AxisPropertyProperty.AXISLOCATION) {
			setAxisLocation((AxisLocation) value);
		} else if (property == AxisPropertyProperty.TICKS) {
			setTicks((TicksProperty) value);
		} else if (property == AxisPropertyProperty.LOGFLAG) {
			setLogFlag((Boolean) value);
		}

	}
	
	/**
	 * @return the axisLocation
	 */
	public AxisLocation getAxisLocation() {
		return axisLocation;
	}

	/**
	 * @param axisLocation the axisLocation to set
	 */
	public void setAxisLocation(AxisLocation axisLocation) {
		this.axisLocation = axisLocation;
	}

	/**
	 * @return the gridColor
	 */
	public int getGridColor() {
		return gridColor;
	}

	/**
	 * @param gridColor the gridColor to set
	 */
	public void setGridColor(int gridColor) {
		this.gridColor = gridColor;
	}

	/**
	 * @return the label
	 */
	public Label getLabel() {
		return label;
	}

	/**
	 * @param label the label to set
	 */
	public void setLabel(Label label) {
		this.label = label;
	}

	/**
	 * @return the logFlag
	 */
	public Boolean getLogFlag() {
		return new Boolean(logFlag);
	}

	/**
	 * @param logFlag the logFlag to set
	 */
	public void setLogFlag(Boolean logFlag) {
		this.logFlag = logFlag.booleanValue();
	}

	/**
	 * @return the reverse
	 */
	public Boolean getReverse() {
		return new Boolean(reverse);
	}

	/**
	 * @param reverse the reverse to set
	 */
	public void setReverse(Boolean reverse) {
		this.reverse = reverse.booleanValue();
	}

	/**
	 * @return the ticks
	 */
	public TicksProperty getTicks() {
		return ticks;
	}

	/**
	 * @param ticks the ticks to set
	 */
	public void setTicks(TicksProperty ticks) {
		this.ticks = ticks;
	}

	/**
	 * @return the visible
	 */
	public Boolean getVisible() {
		return new Boolean(visible);
	}

	/**
	 * @param visible the visible to set
	 */
	public void setVisible(Boolean visible) {
		this.visible = visible.booleanValue();
	}
	
	//	 Methods: to be done
}
