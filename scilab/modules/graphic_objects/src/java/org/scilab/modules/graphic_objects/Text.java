package org.scilab.modules.graphic_objects;


/**
 * Text class
 * @author juliachs
 */
public class Text extends ClippableTextObject {
	/** Text properties names */
	private enum TextProperty { FONTANGLE, POSITION, ALIGNMENT, BOX, TEXTBOX, TEXTBOXMODE, AUTODIMENSIONING };

	/** Alignement */
	private enum Alignment { LEFT, CENTER, RIGHT };

	/** Text box mode */
	private enum TextBoxMode { OFF, CENTERED, FILLED };

	/** Text angle */
	private double fontAngle;

	/** 3D coordinates position (3-element array)*/
	private double [] position;

	/** Text alignment */
	private Alignment alignment;

	/** Specifies whether a box is drawn */
	private boolean box;

	/** Text box dimensions (2-element array) */
	private double [] textBox;

	/** Text box mode */
	private TextBoxMode textBoxMode;

	/** Auto-dimensioning */
	private boolean autoDimensioning;

	/** Constructor */
	public Text() {
		super();
		fontAngle = 0.0;
		position = null;
		alignment = null;
		box = false;
		textBox = null;
		textBoxMode = null;
		autoDimensioning = false;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the associated property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("FontAngle")) {
			returnedProp =  TextProperty.FONTANGLE;
		} else if (propertyName.equals("Position")) {
			returnedProp =  TextProperty.POSITION;
		} else if (propertyName.equals("Alignment")) {
			returnedProp =  TextProperty.ALIGNMENT;
		} else if (propertyName.equals("Box")) {
			returnedProp =  TextProperty.BOX;
		} else if (propertyName.equals("TextBox")) {
			returnedProp =  TextProperty.TEXTBOX;
		} else if (propertyName.equals("TextBoxMode")) {
			returnedProp =  TextProperty.TEXTBOXMODE;
		} else if (propertyName.equals("AutoDimensioning")) {
			returnedProp =  TextProperty.AUTODIMENSIONING;
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
	
		if (property == TextProperty.FONTANGLE) {
			returnedProp = getFontAngle();
		} else if (property == TextProperty.POSITION) {
			returnedProp = getPosition();
		} else if (property == TextProperty.ALIGNMENT) {
			returnedProp = getAlignment();
		} else if (property == TextProperty.BOX) {
			returnedProp = getBox();
		} else if (property == TextProperty.TEXTBOX) {
			returnedProp = getTextBox();
		} else if (property == TextProperty.TEXTBOXMODE) {
			returnedProp = getTextBoxMode();
		} else if (property == TextProperty.AUTODIMENSIONING) {
			returnedProp = getAutoDimensioning();
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
		if (property == TextProperty.FONTANGLE) {
			setFontAngle((Double) value);
		} else if (property == TextProperty.POSITION) {
			setPosition((double []) value);
		} else if (property == TextProperty.ALIGNMENT) {
			setAlignment((Alignment) value);
		} else if (property == TextProperty.BOX) {
			setBox((Boolean) value);
		} else if (property == TextProperty.TEXTBOX) {
			setTextBox((double []) value);
		} else if (property == TextProperty.TEXTBOXMODE) {
			setTextBoxMode((TextBoxMode) value);
		} else if (property == TextProperty.AUTODIMENSIONING) {
			setAutoDimensioning((Boolean) value);
		} else {
			super.setPropertyFast(property, value);
		}
	}

	/**
	 * @return the alignment
	 */
	public Alignment getAlignment() {
		return alignment;
	}

	/**
	 * @param alignment the alignment to set
	 */
	public void setAlignment(Alignment alignment) {
		this.alignment = alignment;
	}

	/**
	 * @return the autoDimensioning
	 */
	public boolean getAutoDimensioning() {
		return autoDimensioning;
	}

	/**
	 * @param autoDimensioning the autoDimensioning to set
	 */
	public void setAutoDimensioning(Boolean autoDimensioning) {
		this.autoDimensioning = autoDimensioning;
	}

	/**
	 * @return the box
	 */
	public boolean getBox() {
		return box;
	}

	/**
	 * @param box the box to set
	 */
	public void setBox(Boolean box) {
		this.box = box.booleanValue();
	}

	/**
	 * @return the fontAngle
	 */
	public double getFontAngle() {
		return fontAngle;
	}

	/**
	 * @param fontAngle the fontAngle to set
	 */
	public void setFontAngle(Double fontAngle) {
		this.fontAngle = fontAngle.doubleValue();
	}

	/**
	 * @return the position
	 */
	public double[] getPosition() {
		return position;
	}

	/**
	 * @param position the position to set
	 */
	public void setPosition(double[] position) {
		this.position = position;
	}

	/**
	 * @return the textBox
	 */
	public double[] getTextBox() {
		return textBox;
	}

	/**
	 * @param textBox the textBox to set
	 */
	public void setTextBox(double[] textBox) {
		this.textBox = textBox;
	}

	/**
	 * @return the textBoxMode
	 */
	public TextBoxMode getTextBoxMode() {
		return textBoxMode;
	}

	/**
	 * @param textBoxMode the textBoxMode to set
	 */
	public void setTextBoxMode(TextBoxMode textBoxMode) {
		this.textBoxMode = textBoxMode;
	}
	
	//	 Methods: to be done
}
