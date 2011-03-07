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

import org.scilab.modules.graphic_objects.contouredObject.ContouredObject;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_COLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_FRACTIONAL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FORMATTED_TEXT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TEXT_STRINGS__;


/**
 * TextObject class
 * @author Manuel JULIACHS
 */
public abstract class TextObject extends ContouredObject {
	/** TextObject properties names */
	private enum TextObjectProperty { TEXT, TEXT_ARRAY_DIMENSIONS };

	/**
	 * Formatted text objects array, row-major order.
	 * For now, elements are supposed to have identical font properties values,
	 * hence the current associated get/set methods implementation.
	 */
	private FormattedText[] text;

	/** Text array dimensions (number of rows, number of columns) */
	private int[] dimensions;

	/** Constructor */
	public TextObject() {
		super();

		dimensions = new int[2];
		dimensions[0] = 1;
		dimensions[1] = 1;

		text = new FormattedText[1];
		text[0] = new FormattedText();
	}

	/**
	 * Clone method
	 */
        public TextObject clone() {
		TextObject copy = (TextObject) super.clone();

		copy.dimensions = new int[2];
		copy.dimensions[0] = dimensions[0];
		copy.dimensions[1] = dimensions[1];
		copy.text = new FormattedText[dimensions[0]*dimensions[1]];

		/* Actually copies the FormattedText objects */
		copy.setText(text);

		return copy;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		if (propertyName.equals(__GO_FORMATTED_TEXT__)) {
			return TextObjectProperty.TEXT;
		} else if (propertyName.equals(__GO_TEXT_ARRAY_DIMENSIONS__)) {
			return TextObjectProperty.TEXT_ARRAY_DIMENSIONS;
		} else if (propertyName.equals(__GO_TEXT_STRINGS__)) {
			return FormattedText.FormattedTextProperty.TEXT;
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
		if (property == TextObjectProperty.TEXT) {
			return getText();
		} else if (property == TextObjectProperty.TEXT_ARRAY_DIMENSIONS) {
			return getTextArrayDimensions();
		} else if (property == FormattedText.FormattedTextProperty.TEXT) {
			return getTextStrings();
		} else if (property == Font.FontProperty.STYLE) {
			return getFontStyle();
		} else if (property == Font.FontProperty.SIZE) {
			return getFontSize();
		} else if (property == Font.FontProperty.COLOR) {
			return getFontColor();
		} else if (property == Font.FontProperty.FRACTIONAL) {
			return getFontFractional();
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
		if (property == TextObjectProperty.TEXT) {
			setText((FormattedText[]) value);
		} else if (property == TextObjectProperty.TEXT_ARRAY_DIMENSIONS) {
			setTextArrayDimensions((Integer[]) value);
		} else if (property == FormattedText.FormattedTextProperty.TEXT) {
			setTextStrings((String[]) value);
		} else if (property == Font.FontProperty.STYLE) {
			setFontStyle((Integer) value);
		} else if (property == Font.FontProperty.SIZE) {
			setFontSize((Double) value);
		} else if (property == Font.FontProperty.COLOR) {
			setFontColor((Integer) value);
		} else if (property == Font.FontProperty.FRACTIONAL) {
			setFontFractional((Boolean) value);
		} else {
			return super.setProperty(property, value);
		}

		return true;
	}

	/**
	 * @return the text array dimensions
	 */
	public Integer[] getTextArrayDimensions() {
		Integer retDimensions[] = new Integer[2];
		retDimensions[0] = dimensions[0];
		retDimensions[1] = dimensions[1];

		return retDimensions;
	}

	/**
	 * @param dimensions the text array dimensions to set
	 */
	public void setTextArrayDimensions(Integer[] dimensions) {
		if (dimensions[0]*dimensions[1] != this.dimensions[0]*this.dimensions[1]) {
			text = new FormattedText[dimensions[0]*dimensions[1]];

			for (int i = 0; i < dimensions[0]*dimensions[1]; i++) {
				text[i] = new FormattedText();
			}
		}

		this.dimensions[0] = dimensions[0];
		this.dimensions[1] = dimensions[1];
	}

	/**
	 * @return the text
	 */
	public FormattedText[] getText() {
		FormattedText[] retText = new FormattedText[dimensions[0]*dimensions[1]];

		for (int i = 0; i < dimensions[0]*dimensions[1]; i++) {
			retText[i] = new FormattedText(text[i]);
		}

		return retText;
	}

	/**
	 * @param textArray the textArray to set
	 */
	public void setText(FormattedText[] textArray) {
		for (int i = 0; i < dimensions[0]*dimensions[1]; i++) {
			text[i] = new FormattedText(textArray[i]);
		}
	}

	/**
	 * @return the text strings
	 */
	public String[] getTextStrings() {
		String[] textStrings = new String[dimensions[0]*dimensions[1]];

		for (int i = 0; i < dimensions[0]*dimensions[1]; i++) {
			textStrings[i] = new String(text[i].getText());
		}

		return textStrings;
	}

	/**
	 * @param textStrings the text strings array to set
	 */
	public void setTextStrings(String[] textStrings) {
		for (int i = 0; i < dimensions[0]*dimensions[1]; i++) {
			text[i].setText(textStrings[i]);
		}
	}

	/**
	 * @return the font
	 */
	public Font getFont() {
		return text[0].getFont();
	}

	/**
	 * @param font the font to set
	 */
	public void setFont(Font font) {
		for (int i = 0; i < dimensions[0]*dimensions[1]; i++) {
			text[i].setFont(font);
		}
	}

	/**
	 * @return the font style
	 */
	public Integer getFontStyle() {
		return text[0].getFont().getStyle();
	}

	/**
	 * @param style the font style to set
	 */
	public void setFontStyle(Integer style) {
		for (int i = 0; i < dimensions[0]*dimensions[1]; i++) {
			text[i].getFont().setStyle(style);
		}
	}

	/**
	 * @return the font color
	 */
	public Integer getFontColor() {
		return text[0].getFont().getColor();
	}

	/**
	 * @param color the font color to set 
	 */
	public void setFontColor(Integer color) {
		for (int i = 0; i < dimensions[0]*dimensions[1]; i++) {
			text[i].getFont().setColor(color);
		}
	}

	/**
	 * @return the font size
	 */
	public Double getFontSize() {
		return text[0].getFont().getSize();
	}

	/**
	 * @param size the font size to set
	 */
	public void setFontSize(Double size) {
		for (int i = 0; i < dimensions[0]*dimensions[1]; i++) {
			text[i].getFont().setSize(size);
		}
	}

	/**
	 * @return the font fractional
	 */
	public Boolean getFontFractional() {
		return text[0].getFont().getFractional();
	}

	/**
	 * @param fractional the font fractional to set
	 */
	public void setFontFractional(Boolean fractional) {
		for (int i = 0; i < dimensions[0]*dimensions[1]; i++) {
			text[i].getFont().setFractional(fractional);
		}
	}

}
