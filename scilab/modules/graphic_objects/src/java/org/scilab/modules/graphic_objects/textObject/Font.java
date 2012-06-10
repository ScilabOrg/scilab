/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.textObject;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Font class
 * @author Manuel JULIACHS
 */
public class Font {
    /** Font properties names */
    public enum FontProperty { STYLE, SIZE, COLOR, FRACTIONAL, ANTIALIASED, UNKNOWNPROPERTY };

    /** Font style */
    private int style;

    /** Font size */
    private double size;

    /** Font color */
    private int color;

    /** Specifies whether fractional font sizes are used or not */
    private boolean fractional;

    /** Specifies whether antialiased font is used */
    private boolean antialiased;

    /** Constructor */
    public Font() {
        this.style = 6;
        this.size = 1.0;
        this.color = -1;
        this.fractional = false;
        this.antialiased = true;
    }

    /**
     * Copy constructor
     * @param
     */
    public Font(Font font) {
        style = font.style;
        size = font.size;
        color = font.color;
        fractional = font.fractional;
        antialiased = font.antialiased;
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(String propertyName) {
        if (propertyName.equals(__GO_FONT_STYLE__)) {
            return FontProperty.STYLE;
        } else if (propertyName.equals(__GO_FONT_SIZE__)) {
            return FontProperty.SIZE;
        } else if (propertyName.equals(__GO_FONT_COLOR__)) {
            return FontProperty.COLOR;
        } else if (propertyName.equals(__GO_FONT_FRACTIONAL__)) {
            return FontProperty.FRACTIONAL;
        } else if (propertyName.equals(__GO_FONT_ANTIALIASED__)) {
            return FontProperty.ANTIALIASED;
        } else {
            return FontProperty.UNKNOWNPROPERTY;
        }
    }

    /**
     * Fast property get method
     * @param property the property to get
     * @return the property value
     */
    public Object getProperty(Object property) {
        if (property == FontProperty.STYLE) {
            return getStyle();
        } else if (property == FontProperty.SIZE) {
            return getSize();
        } else if (property == FontProperty.COLOR) {
            return getColor();
        } else if (property == FontProperty.FRACTIONAL) {
            return getFractional();
        } else if (property == FontProperty.ANTIALIASED) {
            return getAntialiased();
        } else {
            return null;
        }
    }

    /**
     * Fast property set method
     * @param property the property to set
     * @param value the property value
     * @return true if the property has been set, false otherwise
     */
    public boolean setProperty(Object property, Object value) {
        if (property == FontProperty.STYLE) {
            setStyle((Integer) value);
        } else if (property == FontProperty.SIZE) {
            setSize((Double) value);
        } else if (property == FontProperty.COLOR) {
            setColor((Integer) value);
        } else if (property == FontProperty.FRACTIONAL) {
            setFractional((Boolean) value);
        } else if (property == FontProperty.ANTIALIASED) {
            setAntialiased((Boolean) value);
        }

        return true;
    }

    /**
     * @return the color
     */
    public Integer getColor() {
        return color;
    }

    /**
     * @param color the color to set
     */
    public void setColor(Integer color) {
        this.color = color;
    }

    /**
     * @return the fractional
     */
    public Boolean getFractional() {
        return fractional;
    }

    /**
     * @param fractional the fractional to set
     */
    public void setFractional(Boolean fractional) {
        this.fractional = fractional;
    }

    /**
     * @return the antialiased
     */
    public Boolean getAntialiased() {
        return antialiased;
    }

    /**
     * @param antialiased the antialiased to set
     */
    public void setAntialiased(Boolean antialiased) {
        this.antialiased = antialiased;
    }

    /**
     * @return the size
     */
    public Double getSize() {
        return size;
    }

    /**
     * @param size the size to set
     */
    public void setSize(Double size) {
        this.size = size;
    }

    /**
     * @return the style
     */
    public Integer getStyle() {
        return style;
    }

    /**
     * @param style the style to set
     */
    public void setStyle(Integer style) {
        this.style = style;
    }

}
