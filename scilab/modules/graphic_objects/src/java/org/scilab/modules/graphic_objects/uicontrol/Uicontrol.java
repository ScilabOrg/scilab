/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.uicontrol;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTROL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BACKGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_CHECKBOX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_COLUMNNAMES__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_COLUMNNAMES_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_EDIT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTNAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTSIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTUNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTWEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FOREGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_HORIZONTALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_IMAGERENDERER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOXTOP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOXTOP_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MIN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MAX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_PUSHBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RADIOBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RAISED_RELIEF__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RELIEF__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ROWNAMES__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ROWNAMES_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SCALE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SHEAR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDERSTEP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TABLEDATA__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TABLEDATA_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TEXT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_UNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VERTICALALIGNMENT__;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.IVisitor;

/**
 * @author Bruno JOFRET
 * @author Vincent COUVERT
 */
public class Uicontrol extends GraphicObject {

    private static final double DEFAULT_RED_BACKGROUND = 0.8;
    private static final double DEFAULT_GREEN_BACKGROUND = 0.8;
    private static final double DEFAULT_BLUE_BACKGROUND = 0.8;
    
    private static final double DEFAULT_X = 20.0;
    private static final double DEFAULT_Y = 40.0;
    private static final double DEFAULT_WIDTH = 40.0;
    private static final double DEFAULT_HEIGHT = 20.0;

    private static final double DEFAULTFONTSIZE = 10;
    private static final String DEFAULTFONTNAME = "helvetica";
    private static final String DEFAULTFONTWEIGHT = "normal";
    private static final String DEFAULTFONTANGLE = DEFAULTFONTWEIGHT;

    private static final String DEFAULTRELIEF = __GO_UI_RAISED_RELIEF__;

    private UicontrolStyle style;
	private Double[] backgroundColor = {DEFAULT_RED_BACKGROUND, DEFAULT_GREEN_BACKGROUND, DEFAULT_BLUE_BACKGROUND};
	private String[] columnnames = {""};
	private boolean enable = true;
	private String fontAngle = DEFAULTFONTANGLE;
	private String fontName = DEFAULTFONTNAME;
	private double fontSize = DEFAULTFONTSIZE;
	private String fontUnits = "points";
	private String fontWeight = DEFAULTFONTWEIGHT;
	private Double[] foregroundColor = {0.0, 0.0, 0.0};
	private String horizontalAlignment = "center";
	private Integer[] listboxTop; 
	private int max = 1; 
	private int min; 
	private Double[] position = {DEFAULT_X, DEFAULT_Y, DEFAULT_WIDTH, DEFAULT_HEIGHT};
	private String relief = DEFAULTRELIEF;
	private String[] rownames = {""};
	private Double[] scale = {1.0, 1.0};
	private Double[] shear = {0.0, 0.0};
	private Double[] sliderStep = {0.01, 0.1};
	private String[] string = {""};
	private String[] tabledata = {""};
	private String units = "pixels";
	private Integer[] value; 
	private String verticalAlignment = "middle";

	/**
	 * All uicontrol properties
	 */
	private enum UicontrolProperty {
		STYLE,
		BACKGROUNDCOLOR,
		COLUMNNAMES,
		COLUMNNAMES_SIZE,
		ENABLE,
		FONTANGLE,
		FONTNAME,
		FONTSIZE,
		FONTUNITS,
		FONTWEIGHT,
		FOREGROUNDCOLOR,
		HORIZONTALALIGNMENT,
		LISTBOXTOP,
		LISTBOXTOP_SIZE,
		MAX,
		MIN,
		POSITION,
		SCALE,
		SHEAR,
		SLIDERSTEP,
		STRING,
		STRING_SIZE,
		RELIEF,
		ROWNAMES,
		ROWNAMES_SIZE,
		TABLEDATA,
		TABLEDATA_SIZE,
		UNITS,
		VALUE,
		VALUE_SIZE,
		VERTICALALIGNMENT
	};

	/**
	 * All uicontrol styles
	 */
	private enum UicontrolStyle {
	    CHECKBOX,
	    EDIT,
	    FRAME,
		IMAGERENDERER,
		PUSHBUTTON,
		RADIOBUTTON,
		SLIDER,
		TABLE,
		TEXT
	};

	/**
	 * Constructor
	 */
	public Uicontrol() {
		super();
	}
	
	/**
	 * Get style as a string
	 * @param style the uicontrol style
	 * @return the uicontrol style as a string
	 */
	private String styleEnumToString(UicontrolStyle style) {
	    String stringStyle = null;
	    switch(style) {
	    case CHECKBOX:
	        stringStyle = __GO_UI_CHECKBOX__;
	        break;
        case EDIT:
            stringStyle = __GO_UI_EDIT__;
            break;
        case FRAME:
            stringStyle = __GO_UI_FRAME__;
            break;
	    case IMAGERENDERER:
	        stringStyle = __GO_UI_IMAGERENDERER__;
            break;
	    case PUSHBUTTON:
	        stringStyle = __GO_UI_PUSHBUTTON__;
            break;
        case RADIOBUTTON:
            stringStyle = __GO_UI_RADIOBUTTON__;
            break;
        case SLIDER:
            stringStyle = __GO_UI_SLIDER__;
            break;
        case TABLE:
            stringStyle = __GO_UI_TABLE__;
            break;
	    case TEXT:
	        stringStyle = __GO_UI_TEXT__;
            break;
	    default:
	       stringStyle = null;
           break;
	    }
	    return stringStyle;
	}

	/**
	 * Get style as an enum element
	 * @param style the uicontrol style
	 * @return the uicontrol style as an enum element
	 */
	private UicontrolStyle stringToStyleEnum(String style) {
	    UicontrolStyle enumStyle = null;
		if (style.equals(__GO_UI_CHECKBOX__)) {
		    enumStyle = UicontrolStyle.CHECKBOX;
        } else if (style.equals(__GO_UI_EDIT__)) {
            enumStyle = UicontrolStyle.EDIT;
        } else if (style.equals(__GO_UI_FRAME__)) {
            enumStyle = UicontrolStyle.FRAME;
		} else if (style.equals(__GO_UI_IMAGERENDERER__)) {
		    enumStyle = UicontrolStyle.IMAGERENDERER;
		} else if (style.equals(__GO_UI_PUSHBUTTON__)) {
		    enumStyle = UicontrolStyle.PUSHBUTTON;
        } else if (style.equals(__GO_UI_RADIOBUTTON__)) {
            enumStyle = UicontrolStyle.RADIOBUTTON;
        } else if (style.equals(__GO_UI_SLIDER__)) {
            enumStyle = UicontrolStyle.SLIDER;
        } else if (style.equals(__GO_UI_TABLE__)) {
                enumStyle = UicontrolStyle.TABLE;
		} else if (style.equals(__GO_UI_TEXT__)) {
            enumStyle = UicontrolStyle.TEXT;
        }
		return enumStyle;
	}
	
	/**
	 * Get this object type
	 * @return uicontrol
	 * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#getType()
	 */
	public String getType() {
		return __GO_UICONTROL__;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals(__GO_STYLE__)) {
			return UicontrolProperty.STYLE;
		} else if (propertyName.equals(__GO_UI_BACKGROUNDCOLOR__)) {
			return UicontrolProperty.BACKGROUNDCOLOR;
		} else if (propertyName.equals(__GO_UI_COLUMNNAMES__)) {
			return UicontrolProperty.COLUMNNAMES;
		} else if (propertyName.equals(__GO_UI_COLUMNNAMES_SIZE__)) {
			return UicontrolProperty.COLUMNNAMES_SIZE;
		} else if (propertyName.equals(__GO_UI_ENABLE__)) {
			return UicontrolProperty.ENABLE;
		} else if (propertyName.equals(__GO_UI_FONTANGLE__)) {
			return UicontrolProperty.FONTANGLE;
		} else if (propertyName.equals(__GO_UI_FONTNAME__)) {
			return UicontrolProperty.FONTNAME;
		} else if (propertyName.equals(__GO_UI_FONTSIZE__)) {
			return UicontrolProperty.FONTSIZE;
		} else if (propertyName.equals(__GO_UI_FONTUNITS__)) {
			return UicontrolProperty.FONTUNITS;
		} else if (propertyName.equals(__GO_UI_FONTWEIGHT__)) {
			return UicontrolProperty.FONTWEIGHT;
		} else if (propertyName.equals(__GO_UI_FOREGROUNDCOLOR__)) {
			return UicontrolProperty.FOREGROUNDCOLOR;
		} else if (propertyName.equals(__GO_UI_HORIZONTALALIGNMENT__)) {
			return UicontrolProperty.HORIZONTALALIGNMENT;
		} else if (propertyName.equals(__GO_UI_LISTBOXTOP__)) {
			return UicontrolProperty.LISTBOXTOP;
		} else if (propertyName.equals(__GO_UI_LISTBOXTOP_SIZE__)) {
			return UicontrolProperty.LISTBOXTOP_SIZE;
		} else if (propertyName.equals(__GO_UI_MAX__)) {
			return UicontrolProperty.MAX;
		} else if (propertyName.equals(__GO_UI_MIN__)) {
			return UicontrolProperty.MIN;
		} else if (propertyName.equals(__GO_POSITION__)) {
			return UicontrolProperty.POSITION;
		} else if (propertyName.equals(__GO_UI_RELIEF__)) {
			return UicontrolProperty.RELIEF;
		} else if (propertyName.equals(__GO_UI_ROWNAMES__)) {
			return UicontrolProperty.ROWNAMES;
		} else if (propertyName.equals(__GO_UI_ROWNAMES_SIZE__)) {
			return UicontrolProperty.ROWNAMES_SIZE;
		} else if (propertyName.equals(__GO_UI_SCALE__)) {
			return UicontrolProperty.SCALE;
		} else if (propertyName.equals(__GO_UI_SHEAR__)) {
			return UicontrolProperty.SHEAR;
		} else if (propertyName.equals(__GO_UI_SLIDERSTEP__)) {
			return UicontrolProperty.SLIDERSTEP;
		} else if (propertyName.equals(__GO_UI_STRING__)) {
			return UicontrolProperty.STRING;
		} else if (propertyName.equals(__GO_UI_STRING_SIZE__)) {
			return UicontrolProperty.STRING_SIZE;
		} else if (propertyName.equals(__GO_UI_TABLEDATA__)) {
			return UicontrolProperty.TABLEDATA;
		} else if (propertyName.equals(__GO_UI_TABLEDATA_SIZE__)) {
			return UicontrolProperty.TABLEDATA_SIZE;
		} else if (propertyName.equals(__GO_UI_UNITS__)) {
			return UicontrolProperty.UNITS;
		} else if (propertyName.equals(__GO_UI_VALUE__)) {
			return UicontrolProperty.VALUE;
		} else if (propertyName.equals(__GO_UI_VALUE_SIZE__)) {
			return UicontrolProperty.VALUE_SIZE;
		} else if (propertyName.equals(__GO_UI_VERTICALALIGNMENT__)) {
			return UicontrolProperty.VERTICALALIGNMENT;
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
		if (property == UicontrolProperty.STYLE) {
			return getStyle();
		} else if (property == UicontrolProperty.BACKGROUNDCOLOR) {
			return getBackgroundColor();
		} else if (property == UicontrolProperty.COLUMNNAMES) {
			return getColumnNames();
		} else if (property == UicontrolProperty.COLUMNNAMES_SIZE) {
			return getColumnNames().length;
		} else if (property == UicontrolProperty.ENABLE) {
			return getEnable();
		} else if (property == UicontrolProperty.FONTANGLE) {
			return getFontAngle();
		} else if (property == UicontrolProperty.FONTNAME) {
			return getFontName();
		} else if (property == UicontrolProperty.FONTSIZE) {
			return getFontSize();
		} else if (property == UicontrolProperty.FONTUNITS) {
			return getFontUnits();
		} else if (property == UicontrolProperty.FONTWEIGHT) {
			return getFontWeight();
		} else if (property == UicontrolProperty.FOREGROUNDCOLOR) {
			return getForegroundColor();
		} else if (property == UicontrolProperty.HORIZONTALALIGNMENT) {
			return getHorizontalAlignment();
		} else if (property == UicontrolProperty.LISTBOXTOP) {
			return getListboxTop();
		} else if (property == UicontrolProperty.LISTBOXTOP_SIZE) {
			return getListboxTopSize();
		} else if (property == UicontrolProperty.MAX) {
			return getMax();
		} else if (property == UicontrolProperty.MIN) {
			return getMin();
		} else if (property == UicontrolProperty.POSITION) {
			return getUiPosition();
		} else if (property == UicontrolProperty.RELIEF) {
			return getRelief();
		} else if (property == UicontrolProperty.ROWNAMES) {
			return getRowNames();
		} else if (property == UicontrolProperty.ROWNAMES_SIZE) {
			return getRowNames().length;
		} else if (property == UicontrolProperty.SCALE) {
			return getScale();
		} else if (property == UicontrolProperty.SHEAR) {
			return getShear();
		} else if (property == UicontrolProperty.SLIDERSTEP) {
			return getSliderStep();
		} else if (property == UicontrolProperty.STRING) {
			return getString();
		} else if (property == UicontrolProperty.STRING_SIZE) {
			return getString().length;
		} else if (property == UicontrolProperty.TABLEDATA) {
			return getTableData();
		} else if (property == UicontrolProperty.TABLEDATA_SIZE) {
			return getTableData().length;
		} else if (property == UicontrolProperty.UNITS) {
			return getUnits();
		} else if (property == UicontrolProperty.VALUE) {
			return getUiValue();
		} else if (property == UicontrolProperty.VALUE_SIZE) {
			return getUiValueSize();
		} else if (property == UicontrolProperty.VERTICALALIGNMENT) {
			return getVerticalAlignment();
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
		if (property == UicontrolProperty.STYLE) {
			setStyle((String) value);
		} else if (property == UicontrolProperty.BACKGROUNDCOLOR) {
			setBackgroundColor((Double[]) value);
		} else if (property == UicontrolProperty.COLUMNNAMES) {
			setColumnNames((String[]) value);
		} else if (property == UicontrolProperty.ENABLE) {
			setEnable((Boolean) value);
		} else if (property == UicontrolProperty.FONTANGLE) {
			setFontAngle((String) value);
		} else if (property == UicontrolProperty.FONTNAME) {
			setFontName((String) value);
		} else if (property == UicontrolProperty.FONTSIZE) {
			setFontSize((Double) value);
		} else if (property == UicontrolProperty.FONTUNITS) {
			setFontUnits((String) value);
		} else if (property == UicontrolProperty.FONTWEIGHT) {
			setFontWeight((String) value);
		} else if (property == UicontrolProperty.FOREGROUNDCOLOR) {
			setForegroundColor((Double[]) value);
		} else if (property == UicontrolProperty.HORIZONTALALIGNMENT) {
			setHorizontalAlignment((String) value);
		} else if (property == UicontrolProperty.LISTBOXTOP) {
			setListboxTop((Integer[]) value);
		} else if (property == UicontrolProperty.MAX) {
			setMax((Integer) value);
		} else if (property == UicontrolProperty.MIN) {
			setMin((Integer) value);
		} else if (property == UicontrolProperty.POSITION) {
			setUiPosition((Double[]) value);
		} else if (property == UicontrolProperty.RELIEF) {
			setRelief((String) value);
		} else if (property == UicontrolProperty.ROWNAMES) {
			setRowNames((String[]) value);
		} else if (property == UicontrolProperty.SCALE) {
			setScale((Double[]) value);
		} else if (property == UicontrolProperty.SHEAR) {
			setShear((Double[]) value);
		} else if (property == UicontrolProperty.SLIDERSTEP) {
			setSliderStep((Double[]) value);
		} else if (property == UicontrolProperty.STRING) {
			setString((String[]) value);
		} else if (property == UicontrolProperty.TABLEDATA) {
			setTableData((String[]) value);
		} else if (property == UicontrolProperty.UNITS) {
			setUnits((String) value);
		} else if (property == UicontrolProperty.VALUE) {
			setUiValue((Integer[]) value);
		} else if (property == UicontrolProperty.VERTICALALIGNMENT) {
			setVerticalAlignment((String) value);
		} else {
			return super.setProperty(property, value);
		}

		return true;
	}

	/**
	 * Get the style
	 * @return the style
	 */
	public String getStyle() {
		return styleEnumToString(this.style);
	}

	/**
	 * Set the style
	 * @param style the style
	 */
	public void setStyle(String style) {
		this.style = stringToStyleEnum(style);
	}

	/* Background Color */
	public Double[] getBackgroundColor() {
		return this.backgroundColor;
	}

	public void setBackgroundColor(Double[] colors) {
		this.backgroundColor = colors;
	}

	/**
	 * Get the column names
	 * @return the column names
	 */
	public String[] getColumnNames() {
		return this.columnnames;
	}

	/**
	 * Set the column names
	 * @param columnnames the columnnames
	 */
	public void setColumnNames(String[] columnnames) {
		this.columnnames = columnnames;
	}

	/* Enable */
	public boolean getEnable() {
		return this.enable;
	}

	public void setEnable(boolean status) {
		this.enable = status;
	}

	/* FontAngle */
	public String getFontAngle() {
		return this.fontAngle;
	}

	public void setFontAngle(String fontAngle) {
		this.fontAngle = fontAngle;
	}

	/* FontName */
	public String getFontName() {
		return this.fontName;
	}

	public void setFontName(String fontName) {
		this.fontName = fontName;
	}

	/* FontSize */
	public double getFontSize() {
		return this.fontSize;
	}

	public void setFontSize(double fontSize) {
		this.fontSize = fontSize;
	}

	/* FontUnits */
	public String getFontUnits() {
		return this.fontUnits;
	}

	public void setFontUnits(String fontUnits) {
		this.fontUnits = fontUnits;
	}

	/* FontWeight */
	public String getFontWeight() {
		return this.fontWeight;
	}

	public void setFontWeight(String fontWeight) {
		this.fontWeight = fontWeight;
	}

	/* Foreground Color */
	public Double[] getForegroundColor() {
		return this.foregroundColor;
	}

	public void setForegroundColor(Double[] colors) {
		this.foregroundColor = colors;
	}

	/* Horizontal Alignment */
	public String getHorizontalAlignment() {
		return this.horizontalAlignment;
	}

	public void setHorizontalAlignment(String alignment) {
		this.horizontalAlignment = alignment;
	}

	/* Listbox Top */
	public Integer getListboxTopSize() {
		return (listboxTop != null ? listboxTop.length : 0); 
	}

	public Integer[] getListboxTop() {
		return this.listboxTop;
	}

	public void setListboxTop(Integer[] listboxTop) {
		this.listboxTop = listboxTop;
	}

	/* Max */
	public Integer getMax() {
		return this.max;
	}

	public void setMax(Integer max) {
		this.max = max;
	}

	/* Min */
	public Integer getMin() {
		return this.min;
	}

	public void setMin(Integer min) {
		this.min = min;
	}
	
	/* Position */
	public Double[] getUiPosition() {
		return this.position;
	}

	public void setUiPosition(Double[] position) {
		this.position = position;
	}

	/* Relief */
	public String getRelief() {
		return this.relief;
	}

	public void setRelief(String relief) {
		this.relief = relief;
	}

	/**
	 * Get the row names
	 * @return the row names
	 */
	public String[] getRowNames() {
		return this.rownames;
	}

	/**
	 * Set the row names
	 * @param rownames the rownames
	 */
	public void setRowNames(String[] rownames) {
		this.rownames = rownames;
	}

	/**
	 * Get the string
	 * @return the string
	 */
	public String[] getString() {
		return this.string;
	}

	/**
	 * Set the string
	 * @param string the string
	 */
	public void setString(String[] string) {
		this.string = string;
	}

	/**
	 * Get the scale
	 * @return the scale
	 **/
	public Double[] getScale() {
		return this.scale;
	}

	/**
	 * Set the scale
	 * @param scale the scale
	 **/
	public void setScale(Double[] scale) {
		this.scale = scale;
	}

	/**
	 * Get the shear
	 * @return the shear
	 **/
	public Double[] getShear() {
		return this.shear;
	}

	/**
	 * Set the shear
	 * @param shear the shear
	 **/
	public void setShear(Double[] shear) {
		this.shear = shear;
	}

	/* Slider Step */
	public Double[] getSliderStep() {
		return this.sliderStep;
	}

	public void setSliderStep(Double[] sliderStep) {
		this.sliderStep = sliderStep;
	}

	/**
	 * Get the table data
	 * @return the table data
	 */
	public String[] getTableData() {
		return this.tabledata;
	}

	/**
	 * Set the table data
	 * @param tabledata the table data
	 */
	public void setTableData(String[] tabledata) {
		this.tabledata = tabledata;
	}

	/* Units */
	public String getUnits() {
		return this.units;
	}

	public void setUnits(String units) {
		this.units = units;
	}

	/* Value */
	public Integer getUiValueSize() {
		return (value != null ? value.length : 0); 
	}
	
	public Integer[] getUiValue() {
		return this.value;
	}

	public void setUiValue(Integer[] value) {
		this.value = value;
	}

	/* Vertical Alignment */
	public String getVerticalAlignment() {
		return this.verticalAlignment;
	}

	public void setVerticalAlignment(String alignment) {
		this.verticalAlignment = alignment;
	}

	public void accept(IVisitor visitor) {

	}

}
