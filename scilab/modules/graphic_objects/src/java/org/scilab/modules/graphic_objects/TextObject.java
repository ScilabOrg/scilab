package org.scilab.modules.graphic_objects;


/**
 * TextObject class
 * @author juliachs
 */
public class TextObject extends ContouredObject {
	/** TextObject properties names */
	private enum TextObjectProperty { TEXT };
	
	/** Formatted text object */
	private FormattedText text;

	/** Constructor */
	public TextObject() {
		super();
		text = null;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the associated property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("Text")) {
			returnedProp =  TextObjectProperty.TEXT;
		} else if (propertyName.equals("TextString")) { // To be modified
			returnedProp = FormattedText.FormattedTextProperty.TEXT;
		} else if (propertyName.equals("Font")) {
			returnedProp = FormattedText.FormattedTextProperty.FONT;
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

		if (property == TextObjectProperty.TEXT) {
			returnedProp = getText();
		} else if (property == FormattedText.FormattedTextProperty.TEXT) {
			returnedProp = getTextString();
		} else if (property == FormattedText.FormattedTextProperty.FONT) {
			returnedProp = getFont();
		} else if (property == Font.FontProperty.STYLE) {
			returnedProp = getFontStyle();
		} else if (property == Font.FontProperty.SIZE) {
			returnedProp = getFontSize();
		} else if (property == Font.FontProperty.COLOR) {
			returnedProp = getFontColor();
		} else if (property == Font.FontProperty.FRACTIONAL) {
			returnedProp = getFontFractional();
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

//		System.out.format("prop to string: %s\n", prop.toString());
		if (property == TextObjectProperty.TEXT) {
			setText((FormattedText) value);
		} else if (property == FormattedText.FormattedTextProperty.TEXT) {
			setTextString((String) value);
		} else if (property == FormattedText.FormattedTextProperty.FONT) {
			setFont((Font) value);
		} else if (property == Font.FontProperty.STYLE) {
			setFontStyle((Integer) value);
		} else if (property == Font.FontProperty.SIZE) {
			setFontSize((Double) value);
		} else if (property == Font.FontProperty.COLOR) {
			setFontColor((Integer) value);
		} else if (property == Font.FontProperty.FRACTIONAL) {
			setFontFractional((Boolean) value);
		} else {
			super.setPropertyFast(property, value);
		}
	}

	/**
	 * @return the text
	 */
	public FormattedText getText() {
		return text;
	}

	/**
	 * @param text the text to set
	 */
	public void setText(FormattedText text) {
		this.text = text;
	}

	/**
	 * @return the text string
	 */
	public String getTextString() {
		return text.getText();
	}
	
	/**
	 * @param text the text string to set
	 */
	public void setTextString(String text) {
		this.text.setText(text);
	}

	/**
	 * @return the font
	 */
	public Font getFont() {
		return text.getFont();
	}
	
	/**
	 * @param font the font to set
	 */
	public void setFont(Font font) {
		text.setFont(font);
	}

	/**
	 * @return the font style
	 */
	public int getFontStyle() {
		return text.getFont().getStyle();
	}
	
	/**
	 * @param style the font style to set
	 */
	public void setFontStyle(Integer style) {
		text.getFont().setStyle(style);
	}

	/**
	 * @return the font color
	 */
	public int getFontColor() {
		return text.getFont().getColor();
	}

	/**
	 * @param color the font color to set 
	 */
	public void setFontColor(Integer color) {
		text.getFont().setColor(color);
	}

	/**
	 * @return the font size
	 */
	public double getFontSize() {
		return text.getFont().getSize();
	}

	/**
	 * @param size the font size to set
	 */
	public void setFontSize(Double size) {
		text.getFont().setSize(size);
	}
	
	/**
	 * @return the font fractional
	 */
	public boolean getFontFractional() {
		return text.getFont().getFractional();
	}

	/**
	 * @param fractional the font fractional to set
	 */
	public void setFontFractional(Boolean fractional) {
		text.getFont().setFractional(fractional);
	}
	
	//	 Methods: to be done
}
