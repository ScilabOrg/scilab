/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_objects.uicontrol;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT_SET__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTROL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BACKGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_CHECKBOX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_EDIT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTNAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTSIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTUNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTWEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FOREGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_HORIZONTALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_IMAGE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOXTOP_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOXTOP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MAX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MIN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_POPUPMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_PUSHBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RADIOBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RELIEF__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDERSTEP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING_COLNB__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TEXT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TOOLTIPSTRING_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TOOLTIPSTRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_UNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VERTICALALIGNMENT__;

import java.util.Arrays;
import java.util.StringTokenizer;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.utils.LayoutType;

/**
 * @author Bruno JOFRET
 * @author Vincent COUVERT
 */
public class Uicontrol extends GraphicObject {

    protected static final String FLAT_RELIEF   = "flat";
    protected static final String RAISED_RELIEF = "raised";
    protected static final String SUNKEN_RELIEF = "sunken";
    protected static final String GROOVE_RELIEF = "groove";
    protected static final String RIDGE_RELIEF  = "ridge";
    protected static final String SOLID_RELIEF  = "solid";

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
    private static final String STRING_SEPARATOR = "|";

    private UicontrolStyle style;
    private Double[] backgroundColor = {DEFAULT_RED_BACKGROUND, DEFAULT_GREEN_BACKGROUND, DEFAULT_BLUE_BACKGROUND};
    private boolean enable = true;
    private String fontAngle = DEFAULTFONTANGLE;
    private String fontName = DEFAULTFONTNAME;
    private double fontSize = DEFAULTFONTSIZE;
    private String fontUnits = "points";
    private String fontWeight = DEFAULTFONTWEIGHT;
    private Double[] foregroundColor = {0.0, 0.0, 0.0};
    private String horizontalAlignment = "center";
    private Integer[] listboxTop;
    private double max = 1.0;
    private double min;
    private Double[] position = {DEFAULT_X, DEFAULT_Y, DEFAULT_WIDTH, DEFAULT_HEIGHT};
    private String relief = RAISED_RELIEF;
    private Double[] sliderStep = {0.01, 0.1};
    private String[] string = {""};
    private int stringColNb = 1; // Used for tables
    private String[] tooltipString = {""};
    private String units = "pixels";
    private Double[] value;
    private String verticalAlignment = "middle";
    private LayoutType layout = LayoutType.NONE;

    /**
     * All uicontrol properties
     */
    private enum UicontrolProperty {
        STYLE,
        BACKGROUNDCOLOR,
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
        STRING_COLNB,
        RELIEF,
        TOOLTIPSTRING,
        TOOLTIPSTRING_SIZE,
        UNITS,
        VALUE,
        VALUE_SIZE,
        VERTICALALIGNMENT,
        LAYOUT,
        LAYOUT_SET
    };

    /**
     * All uicontrol styles
     */
    private enum UicontrolStyle {
        CHECKBOX,
        EDIT,
        FRAME,
        IMAGE,
        LISTBOX,
        POPUPMENU,
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
        setVisible(false); /* To avoid to see the object rendered before all its properties to be set (See bug #10346) */
    }

    /**
     * Get style as a string
     * @param style the uicontrol style
     * @return the uicontrol style as a string
     */
    private int styleEnumToInt(UicontrolStyle style) {
        switch (style) {
            case CHECKBOX:
                return __GO_UI_CHECKBOX__;
            case EDIT:
                return __GO_UI_EDIT__;
            case FRAME:
                return __GO_UI_FRAME__;
            case IMAGE:
                return __GO_UI_IMAGE__;
            case LISTBOX:
                return __GO_UI_LISTBOX__;
            case POPUPMENU:
                return __GO_UI_POPUPMENU__;
            case PUSHBUTTON:
                return __GO_UI_PUSHBUTTON__;
            case RADIOBUTTON:
                return __GO_UI_RADIOBUTTON__;
            case SLIDER:
                return __GO_UI_SLIDER__;
            case TABLE:
                return __GO_UI_TABLE__;
            case TEXT:
                return __GO_UI_TEXT__;
            default :
                return -1;
        }
    }

    /**
     * Get style as an enum element
     * @param style the uicontrol style
     * @return the uicontrol style as an enum element
     */
    private UicontrolStyle intToStyleEnum(int style) {
        switch (style) {
            case __GO_UI_CHECKBOX__ :
                return UicontrolStyle.CHECKBOX;
            case __GO_UI_EDIT__ :
                return UicontrolStyle.EDIT;
            case __GO_UI_FRAME__ :
                return UicontrolStyle.FRAME;
            case __GO_UI_IMAGE__ :
                return  UicontrolStyle.IMAGE;
            case __GO_UI_LISTBOX__ :
                return UicontrolStyle.LISTBOX;
            case __GO_UI_POPUPMENU__ :
                return UicontrolStyle.POPUPMENU;
            case __GO_UI_PUSHBUTTON__ :
                return UicontrolStyle.PUSHBUTTON;
            case __GO_UI_RADIOBUTTON__ :
                return UicontrolStyle.RADIOBUTTON;
            case __GO_UI_SLIDER__ :
                return UicontrolStyle.SLIDER;
            case __GO_UI_TABLE__ :
                return UicontrolStyle.TABLE;
            case __GO_UI_TEXT__ :
                return UicontrolStyle.TEXT;
            default :
                return null;
        }
    }

    /**
     * Get this object type
     * @return uicontrol
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#getType()
     */
    public Integer getType() {
        return __GO_UICONTROL__;
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_STYLE__ :
                return UicontrolProperty.STYLE;
            case __GO_UI_BACKGROUNDCOLOR__ :
                return UicontrolProperty.BACKGROUNDCOLOR;
            case __GO_UI_ENABLE__ :
                return UicontrolProperty.ENABLE;
            case __GO_UI_FONTANGLE__ :
                return UicontrolProperty.FONTANGLE;
            case __GO_UI_FONTNAME__ :
                return UicontrolProperty.FONTNAME;
            case __GO_UI_FONTSIZE__ :
                return UicontrolProperty.FONTSIZE;
            case __GO_UI_FONTUNITS__ :
                return UicontrolProperty.FONTUNITS;
            case __GO_UI_FONTWEIGHT__ :
                return UicontrolProperty.FONTWEIGHT;
            case __GO_UI_FOREGROUNDCOLOR__ :
                return UicontrolProperty.FOREGROUNDCOLOR;
            case __GO_UI_HORIZONTALALIGNMENT__ :
                return UicontrolProperty.HORIZONTALALIGNMENT;
            case __GO_UI_LISTBOXTOP__ :
                return UicontrolProperty.LISTBOXTOP;
            case __GO_UI_LISTBOXTOP_SIZE__ :
                return UicontrolProperty.LISTBOXTOP_SIZE;
            case __GO_UI_MAX__ :
                return UicontrolProperty.MAX;
            case __GO_UI_MIN__ :
                return UicontrolProperty.MIN;
            case __GO_POSITION__ :
                return UicontrolProperty.POSITION;
            case __GO_UI_RELIEF__ :
                return UicontrolProperty.RELIEF;
            case __GO_UI_SLIDERSTEP__ :
                return UicontrolProperty.SLIDERSTEP;
            case __GO_UI_STRING__ :
                return UicontrolProperty.STRING;
            case __GO_UI_STRING_SIZE__ :
                return UicontrolProperty.STRING_SIZE;
            case __GO_UI_STRING_COLNB__ :
                return UicontrolProperty.STRING_COLNB;
            case __GO_UI_TOOLTIPSTRING__ :
                return UicontrolProperty.TOOLTIPSTRING;
            case __GO_UI_TOOLTIPSTRING_SIZE__ :
                return UicontrolProperty.TOOLTIPSTRING_SIZE;
            case __GO_UI_UNITS__ :
                return UicontrolProperty.UNITS;
            case __GO_UI_VALUE__ :
                return UicontrolProperty.VALUE;
            case __GO_UI_VALUE_SIZE__ :
                return UicontrolProperty.VALUE_SIZE;
            case __GO_UI_VERTICALALIGNMENT__ :
                return UicontrolProperty.VERTICALALIGNMENT;
            case __GO_LAYOUT__ :
                return UicontrolProperty.LAYOUT;
            case __GO_LAYOUT_SET__ :
                return UicontrolProperty.LAYOUT_SET;
            default :
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
        } else if (property == UicontrolProperty.SLIDERSTEP) {
            return getSliderStep();
        } else if (property == UicontrolProperty.STRING) {
            return getString();
        } else if (property == UicontrolProperty.STRING_SIZE) {
            return getString().length;
        } else if (property == UicontrolProperty.STRING_COLNB) {
            return getStringColNb();
        } else if (property == UicontrolProperty.TOOLTIPSTRING) {
            return getTooltipString();
        } else if (property == UicontrolProperty.TOOLTIPSTRING_SIZE) {
            return getTooltipString().length;
        } else if (property == UicontrolProperty.UNITS) {
            return getUnits();
        } else if (property == UicontrolProperty.VALUE) {
            return getUiValue();
        } else if (property == UicontrolProperty.VALUE_SIZE) {
            return getUiValueSize();
        } else if (property == UicontrolProperty.VERTICALALIGNMENT) {
            return getVerticalAlignment();
        } else if (property == UicontrolProperty.LAYOUT) {
            return getLayout();
        } else if (property == UicontrolProperty.LAYOUT_SET) {
            return isLayoutSettable();
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
    public UpdateStatus setProperty(Object property, Object value) {
        if (!(property instanceof UicontrolProperty)) {
            return super.setProperty(property, value);
        }

        UicontrolProperty p = (UicontrolProperty) property;
        switch (p) {
            case STYLE:
                return setStyle((Integer) value);
            case BACKGROUNDCOLOR:
                return setBackgroundColor((Double[]) value);
            case ENABLE:
                return setEnable((Boolean) value);
            case FONTANGLE:
                return setFontAngle((String) value);
            case FONTNAME:
                return setFontName((String) value);
            case FONTSIZE:
                return setFontSize((Double) value);
            case FONTUNITS:
                return setFontUnits((String) value);
            case FONTWEIGHT:
                return setFontWeight((String) value);
            case FOREGROUNDCOLOR:
                return setForegroundColor((Double[]) value);
            case HORIZONTALALIGNMENT:
                return setHorizontalAlignment((String) value);
            case LISTBOXTOP:
                return setListboxTop((Integer[]) value);
            case MAX:
                return setMax((Double) value);
            case MIN:
                return setMin((Double) value);
            case POSITION:
                return setUiPosition((Double[]) value);
            case RELIEF:
                return setRelief((String) value);
            case SLIDERSTEP:
                return setSliderStep((Double[]) value);
            case STRING:
                return setString((String[]) value);
            case STRING_COLNB:
                return setStringColNb((Integer) value);
            case TOOLTIPSTRING:
                return setTooltipString((String[]) value);
            case UNITS:
                return setUnits((String) value);
            case VALUE:
                return setUiValue((Double[]) value);
            case VERTICALALIGNMENT:
                return setVerticalAlignment((String) value);
            case LAYOUT:
                return setLayout((Integer) value);
            default:
                return super.setProperty(property, value);
        }
    }

    /**
     * Get the style
     * @return the style
     */
    public Integer getStyle() {
        return styleEnumToInt(this.style);
    }

    /**
     * Set the style
     * @param style the style
     * @return TODO
     */
    public UpdateStatus setStyle(int style) {
        UicontrolStyle val =  intToStyleEnum(style);
        if (val == this.style) {
            return UpdateStatus.NoChange;
        }

        this.style = val;
        return UpdateStatus.Success;
    }

    /* Background Color */
    public Double[] getBackgroundColor() {
        return this.backgroundColor;
    }

    public UpdateStatus setBackgroundColor(Double[] colors) {
        if (Arrays.equals(colors, this.backgroundColor)) {
            return UpdateStatus.NoChange;
        }

        this.backgroundColor = colors;
        return UpdateStatus.Success;
    }

    /* Enable */
    public boolean getEnable() {
        return this.enable;
    }

    public UpdateStatus setEnable(boolean status) {
        if (this.enable == status) {
            return UpdateStatus.NoChange;
        }
        this.enable = status;
        return UpdateStatus.Success;
    }

    /* FontAngle */
    public String getFontAngle() {
        return this.fontAngle;
    }

    public UpdateStatus setFontAngle(String fontAngle) {
        if (this.fontAngle.equals(fontAngle)) {
            return UpdateStatus.NoChange;
        }
        this.fontAngle = fontAngle;
        return UpdateStatus.Success;
    }

    /* FontName */
    public String getFontName() {
        return this.fontName;
    }

    public UpdateStatus setFontName(String fontName) {
        if (this.fontName.equals(fontName)) {
            return UpdateStatus.NoChange;
        }
        this.fontName = fontName;
        return UpdateStatus.Success;
    }

    /* FontSize */
    public double getFontSize() {
        return this.fontSize;
    }

    public UpdateStatus setFontSize(double fontSize) {
        if (this.fontSize == fontSize) {
            return UpdateStatus.NoChange;
        }
        this.fontSize = fontSize;
        return UpdateStatus.Success;
    }

    /* FontUnits */
    public String getFontUnits() {
        return this.fontUnits;
    }

    public UpdateStatus setFontUnits(String fontUnits) {
        if (this.fontUnits.equals(fontUnits)) {
            return UpdateStatus.NoChange;
        }
        this.fontUnits = fontUnits;
        return UpdateStatus.Success;
    }

    /* FontWeight */
    public String getFontWeight() {
        return this.fontWeight;
    }

    public UpdateStatus setFontWeight(String fontWeight) {
        if (this.fontWeight.equals(fontWeight)) {
            return UpdateStatus.NoChange;
        }
        this.fontWeight = fontWeight;
        return UpdateStatus.Success;
    }

    /* Foreground Color */
    public Double[] getForegroundColor() {
        return this.foregroundColor;
    }

    public UpdateStatus setForegroundColor(Double[] colors) {
        if (Arrays.equals(this.foregroundColor, colors)) {
            return UpdateStatus.NoChange;
        }
        this.foregroundColor = colors;
        return UpdateStatus.Success;
    }

    /* Horizontal Alignment */
    public String getHorizontalAlignment() {
        return this.horizontalAlignment;
    }

    public UpdateStatus setHorizontalAlignment(String alignment) {
        if (this.horizontalAlignment.equals(alignment)) {
            return UpdateStatus.NoChange;
        }
        this.horizontalAlignment = alignment;
        return UpdateStatus.Success;
    }

    /* Listbox Top */
    public Integer getListboxTopSize() {
        return (listboxTop != null ? listboxTop.length : 0);
    }

    public Integer[] getListboxTop() {
        return this.listboxTop;
    }

    public UpdateStatus setListboxTop(Integer[] listboxTop) {
        if (Arrays.equals(this.listboxTop, listboxTop)) {
            return UpdateStatus.NoChange;
        }
        this.listboxTop = listboxTop;
        return UpdateStatus.Success;
    }

    /* Max */
    public Double getMax() {
        return this.max;
    }

    public UpdateStatus setMax(double max) {
        if (this.max == max) {
            return UpdateStatus.NoChange;
        }
        this.max = max;
        return UpdateStatus.Success;
    }

    /* Min */
    public Double getMin() {
        return this.min;
    }

    public UpdateStatus setMin(double min) {
        if (this.min == min) {
            return UpdateStatus.NoChange;
        }
        this.min = min;
        return UpdateStatus.Success;
    }

    /* Position */
    public Double[] getUiPosition() {
        return this.position;
    }

    public UpdateStatus setUiPosition(Double[] position) {
        if (Arrays.equals(this.position, position)) {
            return UpdateStatus.NoChange;
        }
        this.position = position;
        return UpdateStatus.Success;
    }

    /* Relief */
    public String getRelief() {
        return this.relief;
    }

    public UpdateStatus setRelief(String relief) {
        if (this.relief.equals(relief)) {
            return UpdateStatus.NoChange;
        }
        this.relief = relief;
        return UpdateStatus.Success;
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
    public UpdateStatus setString(String[] string) {
        if (this.style == UicontrolStyle.LISTBOX || this.style == UicontrolStyle.POPUPMENU) {
            /* String can be set using a|b|c|d */
            if (string.length == 1 & string[0].contains(STRING_SEPARATOR)) {
                StringTokenizer strTok = new StringTokenizer(string[0], STRING_SEPARATOR);
                String[] stringTab = new String[strTok.countTokens()];
                while (strTok.hasMoreTokens()) {
                    stringTab[stringTab.length - strTok.countTokens()] = strTok.nextToken();
                }
                this.string = stringTab;
                return UpdateStatus.Success;
            }
        }
        this.string = string;
        return UpdateStatus.Success;
    }

    /**
     * Get the string column number
     * @return the number of columns
     */
    public int getStringColNb() {
        return stringColNb;
    }

    /**
     * Set the string column number
     * @param stringColNb the number of columns
     */
    public UpdateStatus setStringColNb(Integer stringColNb) {
        if (this.stringColNb == stringColNb) {
            return UpdateStatus.NoChange;
        }
        this.stringColNb = stringColNb;
        return UpdateStatus.Success;
    }


    /**
     * Get the tooltip string
     * @return the tooltip string
     */
    public String[] getTooltipString() {
        return this.tooltipString;
    }

    /**
     * Set the tooltip string
     * @param tooltipString the tooltip string
     */
    public UpdateStatus setTooltipString(String[] tooltipString) {
        if (Arrays.equals(this.tooltipString, tooltipString)) {
            return UpdateStatus.NoChange;
        }
        this.tooltipString = tooltipString;
        return UpdateStatus.Success;
    }

    /* Slider Step */
    public Double[] getSliderStep() {
        return this.sliderStep;
    }

    public UpdateStatus setSliderStep(Double[] sliderStep) {
        if (Arrays.equals(this.sliderStep, sliderStep)) {
            return UpdateStatus.NoChange;
        }
        this.sliderStep = sliderStep;
        return UpdateStatus.Success;
    }

    /* Units */
    public String getUnits() {
        return this.units;
    }

    public UpdateStatus setUnits(String units) {
        if (this.units.equals(units)) {
            return UpdateStatus.NoChange;
        }
        this.units = units;
        return UpdateStatus.Success;
    }

    /* Value */
    public Integer getUiValueSize() {
        return (value != null ? value.length : 0);
    }

    public Double[] getUiValue() {
        return this.value;
    }

    public UpdateStatus setUiValue(Double[] value) {
        if (Arrays.equals(this.value, value)) {
            return UpdateStatus.NoChange;
        }
        this.value = value;
        return UpdateStatus.Success;
    }

    /* Vertical Alignment */
    public String getVerticalAlignment() {
        return this.verticalAlignment;
    }

    public UpdateStatus setVerticalAlignment(String alignment) {
        if (this.verticalAlignment.equals(alignment)) {
            return UpdateStatus.NoChange;
        }
        this.verticalAlignment = alignment;
        return UpdateStatus.Success;
    }

    public Integer getLayout() {
        return layout.ordinal();
    }

    public LayoutType getLayoutAsEnum() {
        return layout;
    }

    public boolean isLayoutSettable() {
        return (this.layout == LayoutType.NONE);
    }

    public UpdateStatus setLayout(Integer value) {
        return setLayout(LayoutType.intToEnum(value));
    }

    public UpdateStatus setLayout(LayoutType layout) {
        //avoid to set layout twice
        if (this.layout == LayoutType.NONE) {
            if (layout == LayoutType.NONE) {
                return UpdateStatus.NoChange;
            }

            this.layout = layout;
            return UpdateStatus.Success;
        }

        return UpdateStatus.Fail;
    }

    public void accept(Visitor visitor) {

    }

}
