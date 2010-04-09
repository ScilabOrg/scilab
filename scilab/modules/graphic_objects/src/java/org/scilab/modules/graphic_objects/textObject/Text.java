package org.scilab.modules.graphic_objects.textObject;



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
	private double[] position;

	/** Text alignment */
	private Alignment alignment;

	/** Specifies whether a box is drawn */
	private boolean box;

	/** Text box dimensions (2-element array) */
	private double[] textBox;

	/** Text box mode */
	private TextBoxMode textBoxMode;

	/** Auto-dimensioning */
	private boolean autoDimensioning;

	/** Constructor */
	public Text() {
		super();
		fontAngle = 0.0;
		position = new double[3];
		alignment = null;
		box = false;
		textBox = new double[2];
		textBoxMode = null;
		autoDimensioning = false;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the property enum
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

		if (property == TextProperty.FONTANGLE) {
			setFontAngle((Double) value);
		} else if (property == TextProperty.POSITION) {
			setPosition((Double[]) value);
		} else if (property == TextProperty.ALIGNMENT) {
			setAlignment((Alignment) value);
		} else if (property == TextProperty.BOX) {
			setBox((Boolean) value);
		} else if (property == TextProperty.TEXTBOX) {
			setTextBox((Double[]) value);
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
	public Boolean getAutoDimensioning() {
		return new Boolean(autoDimensioning);
	}

	/**
	 * @param autoDimensioning the autoDimensioning to set
	 */
	public void setAutoDimensioning(Boolean autoDimensioning) {
		this.autoDimensioning = autoDimensioning.booleanValue();
	}

	/**
	 * @return the box
	 */
	public Boolean getBox() {
		return new Boolean(box);
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
	public Double getFontAngle() {
		return new Double(fontAngle);
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
	public Double[] getPosition() {
		Double[] retPosition = new Double[3];
		retPosition[0] = new Double(position[0]);
		retPosition[1] = new Double(position[1]);
		retPosition[2] = new Double(position[2]);

		return retPosition;
	}

	/**
	 * @param position the position to set
	 */
	public void setPosition(Double[] position) {
		this.position[0] = position[0].doubleValue();
		this.position[1] = position[1].doubleValue();
		this.position[2] = position[2].doubleValue();
	}

	/**
	 * @return the textBox
	 */
	public Double[] getTextBox() {
		Double[] retTextBox = new Double[2];
		retTextBox[0] = new Double(textBox[0]);
		retTextBox[1] = new Double(textBox[2]);

		return retTextBox;
	}

	/**
	 * @param textBox the textBox to set
	 */
	public void setTextBox(Double[] textBox) {
		this.textBox[0] = textBox[0].doubleValue();
		this.textBox[1] = textBox[1].doubleValue();
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
