package org.scilab.modules.graphic_objects;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.ContouredObject.ContouredObjectPropertyType;
import org.scilab.modules.graphic_objects.ClippableProperty.ClipStateType;

/**
 * Arc class
 * @author juliachs
 */
public class Arc extends ClippableContouredObject {
	/** Arc properties names */
	private enum ArcProperty { UPPERLEFTPOINT, WIDTH, HEIGHT, STARTANGLE, ENDANGLE, ARCDRAWINGMETHOD, UNKNOWNPROP };

	/** Arc properties names 2 */
	private enum ArcProperty2 { UPPERLEFTPOINT, WIDTH, HEIGHT, STARTANGLE, ENDANGLE, ARCDRAWINGMETHOD,
		 CLIPPROPERTY, CLIPSTATE, CLIPBOX,
		 LINE, FILLMODE, BACKGROUND, MARK,
		 PARENT, CHILDREN, VISIBLE, USERDATA
	};
	
	/** Arc drawing method */
	public enum ArcDrawingMethod { NURBS, LINES };

	/** Bounding box upper-left point (x,y,z) coordinates */
	private double [] upperLeftPoint;

	/** Bounding box width */
	private double width;

	/** Bounding box height */
	private double height;

	/** Start angle */
	private double startAngle;

	/** End angle */
	private double endAngle;
	
	/** Specifies which drawing method to use */
	private ArcDrawingMethod arcDrawingMethod;
	
	/**
	 * Constructor
	 */
	public Arc() {
		super();
		arcDrawingMethod = ArcDrawingMethod.LINES;
		upperLeftPoint = null;
		width = 0.0;
		height = 0.0;
		startAngle = 0.0;
		endAngle = 0.0;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the associated property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		ArcProperty returnedProp;
		Object contouredpropertytest;
		//System.out.format("property name: %s\n", propertyName);

			
		if (propertyName.equals("UpperLeftPoint")) {
			returnedProp =  ArcProperty.UPPERLEFTPOINT;
		} else if (propertyName.equals("Width")) {
			returnedProp =  ArcProperty.WIDTH;
		} else if (propertyName.equals("Height")) {
			returnedProp =  ArcProperty.HEIGHT;
		} else if (propertyName.equals("StartAngle")) {
			returnedProp =  ArcProperty.STARTANGLE;
		} else if (propertyName.equals("EndAngle")) {
			returnedProp =  ArcProperty.ENDANGLE;
		} else if (propertyName.equals("ArcDrawingMethod")) {
			returnedProp = ArcProperty.ARCDRAWINGMETHOD;
		} else {
			contouredpropertytest = super.getPropertyFromName(propertyName);

		//System.out.format("prop toString: %s\n", contouredpropertytest.toString());
		
			return (Object) contouredpropertytest;
		}
		
			return (Object) returnedProp;
	}

	/**
	 * Get property from name version 2
	 * @param propertyName the property name
	 * @return the property
	 */
	public Object getPropertyFromName2(String propertyName) {
		ArcProperty2 returnedProp  = null;

		if (propertyName.equals("UpperLeftPoint")) {
			returnedProp = ArcProperty2.UPPERLEFTPOINT;
		} else if (propertyName.equals("Width")) {
			returnedProp = ArcProperty2.WIDTH;
		} else if (propertyName.equals("Height")) {
			returnedProp = ArcProperty2.HEIGHT;
		} else if (propertyName.equals("StartAngle")) {
			returnedProp = ArcProperty2.STARTANGLE;
		} else if (propertyName.equals("EndAngle")) {
			returnedProp = ArcProperty2.ENDANGLE;
		} else if (propertyName.equals("ArcDrawingMethod")) {
			returnedProp = ArcProperty2.ARCDRAWINGMETHOD;
		} else if (propertyName.equals("ClipProperty")) {
			returnedProp = ArcProperty2.CLIPPROPERTY;
		} else if (propertyName.equals("ClipState")) {
			returnedProp = ArcProperty2.CLIPSTATE;
		} else if (propertyName.equals("ClipBox")) {
			returnedProp = ArcProperty2.CLIPBOX;
		} else if (propertyName.equals("Line")) {
			returnedProp = ArcProperty2.LINE;
		} else if (propertyName.equals("FillMode")) {
			returnedProp = ArcProperty2.FILLMODE;
		} else if (propertyName.equals("Background")) {
			returnedProp = ArcProperty2.BACKGROUND;
		} else if (propertyName.equals("Mark")) {
			returnedProp = ArcProperty2.MARK;
		} else if (propertyName.equals("Parent")) {
			returnedProp = ArcProperty2.PARENT;
		} else if (propertyName.equals("Children")) {
			returnedProp = ArcProperty2.CHILDREN;
		} else if (propertyName.equals("Visible")) {
			returnedProp = ArcProperty2.VISIBLE;
		} else if (propertyName.equals("UserData")) {
			returnedProp = ArcProperty2.USERDATA;
		}

		return (Object) returnedProp;
	}
	
	/**
	 * Fast property get method
	 * @param property the property
	 */
	public Object getPropertyFast(Object property) {
		Object returnedProp = null;
		Object prop = null;

		/*
		if(property instanceof ArcProperty) {
			//System.out.format("OK !\n");
			prop = (ArcProperty) property;
		}
		else {
			returnedProp = super.getPropertyFast(property);

			return returnedProp;
		}
		*/

		if (prop == ArcProperty.UPPERLEFTPOINT) {
			System.out.format("Upper left point !\n");
			returnedProp = getUpperLeftPoint();
		} else if (prop == ArcProperty.WIDTH) {
			System.out.format("Width !\n");
			returnedProp = getWidth();
		} else if (prop == ArcProperty.HEIGHT) {
			System.out.format("Height !\n");
			returnedProp = getHeight();
		} else if (prop == ArcProperty.STARTANGLE) {
			System.out.format("Start angle !\n");
			returnedProp = getStartAngle();
		} else if (prop == ArcProperty.ENDANGLE) {
			System.out.format("End angle !\n");
			returnedProp = getEndAngle();
		} else if (prop == ArcProperty.ARCDRAWINGMETHOD) {
			System.out.format("Arc drawing method !\n");
			returnedProp = getArcDrawingMethod();
		} else if (prop == ArcProperty.UNKNOWNPROP) {
			System.out.format("Unknown prop !\n");
			returnedProp = null;
		} else {
			prop = (Object) property;
			
			returnedProp = super.getPropertyFast(prop);	
		}

		return returnedProp;
	}
	
	/**
	 * Get the property version 2
	 * @param propertyEnum the property enum
	 * @return the property value to return
	 */
	public Object getPropertyFast2(Object propertyEnum) {
		Object returnedProp = null;
		//System.out.format("PropertyEnum.toString: %s\n", propertyEnum.toString());
		
		switch((ArcProperty2) propertyEnum) {
		case UPPERLEFTPOINT:
			returnedProp = getUpperLeftPoint();
			break;
		case WIDTH:
			returnedProp = getWidth();
			break;
		case HEIGHT:
			returnedProp = getHeight();
			break;
		case STARTANGLE:
			returnedProp = getStartAngle();
			break;
		case ENDANGLE:
			returnedProp = getEndAngle();
			break;
		case ARCDRAWINGMETHOD:
			returnedProp = getArcDrawingMethod();
			break;
		case CLIPPROPERTY:
			returnedProp = getClipProperty();
			break;
		case CLIPSTATE:
			returnedProp = getClipState();
			break;
		case CLIPBOX:
			returnedProp = getClipBox();
			break;
		case LINE:
			returnedProp = getLine();
			break;
		case FILLMODE:
			returnedProp = getFillMode();
			break;
		case BACKGROUND:
			returnedProp = getBackground();
			break;
		case MARK:
			returnedProp = getMark();
			break;
		case PARENT:
			returnedProp = getParent();
			break;
		//case CHILDREN:
			// TBD !!
	//		this.setChildren(setChildren((ArrayList<GraphicObject>) value);
			
		case VISIBLE:
			returnedProp = this.getVisible();
			break;
		case USERDATA:
			returnedProp = this.getUserData();
			break;
		default:
			returnedProp = null;
			break;
		}

		return returnedProp;

	}
	
	/**
	 * Set the property
	 * @param propertyEnum the enum
	 * @param value the actual property value
	 */
	public void setPropertyFast(Object propertyEnum, Object value) {
		Object prop = null;

		prop = propertyEnum;
//		System.out.format("prop to string: %s\n", prop.toString());
		if (prop == ArcProperty.UPPERLEFTPOINT) {
			setUpperLeftPoint((Double []) value);
		} else if (prop == ArcProperty.WIDTH) {
			setWidth((Double) value);
		} else if (prop == ArcProperty.HEIGHT) {
			setHeight((Double) value);
		} else if (prop == ArcProperty.STARTANGLE) {
			setStartAngle((Double) value);
		} else if (prop == ArcProperty.ENDANGLE) {
			setEndAngle((Double) value);
		} else if (prop == ArcProperty.ARCDRAWINGMETHOD) {
			setArcDrawingMethod((ArcDrawingMethod) value);
		} else if (prop == ArcProperty.UNKNOWNPROP) {
			System.out.format("UNKNOWN PROPERTY !");
		} else {
	//		System.out.format("ContouredObjectPropertyType !\n");
			prop = (Object) propertyEnum;

			super.setPropertyFast(prop, value);
			
			return;
		}


	}

	/**
	 * Experimental fast set property method
	 * @param propertyEnum the property
	 * @param value the property value
	 */
	public void setPropertyFastHack(Object propertyEnum, Object value) {
		Object returnedProp = null;
		Object prop = null;
 
		prop = propertyEnum;
		//	System.out.format("prop to string: %s\n", prop.toString());
			if (prop == ArcProperty2.UPPERLEFTPOINT) {
				setUpperLeftPoint((Double []) value);
			} else if (prop == ArcProperty2.WIDTH) {
				setWidth((Double) value);
			} else if (prop == ArcProperty2.HEIGHT) {
				setHeight((Double) value);
			} else if (prop == ArcProperty2.STARTANGLE) {
				setStartAngle((Double) value);
			} else if (prop == ArcProperty2.ENDANGLE) {
				setEndAngle((Double) value);
			} else if (prop == ArcProperty2.ARCDRAWINGMETHOD) {
				setArcDrawingMethod((ArcDrawingMethod) value);
			} else {
	//		System.out.format("ContouredObjectPropertyType !\n");
			prop = (Object) propertyEnum;

			super.setPropertyFast(prop, value);
			
			return;
		}

	//	System.out.format("setPropertyFast Arc !\n");
		//prop = (ArcProperty) propertyEnum;
		

	}
	
	/**
	 * Fast property set method version 2
	 * @param propertyEnum the property
	 * @param value the property value
	 */
	public void setPropertyFast2(Object propertyEnum, Object value) {
			if (propertyEnum == ArcProperty2.UPPERLEFTPOINT) {
				setUpperLeftPoint((Double []) value);
			} else if (propertyEnum == ArcProperty2.WIDTH) {
				setWidth((Double) value);
			} else if (propertyEnum == ArcProperty2.HEIGHT) {
				setHeight((Double) value);
			} else if (propertyEnum == ArcProperty2.STARTANGLE) {
				setStartAngle((Double) value);
			} else if (propertyEnum == ArcProperty2.ENDANGLE) {
				setEndAngle((Double) value);
			} else if (propertyEnum == ArcProperty2.ARCDRAWINGMETHOD) {
				setArcDrawingMethod((ArcDrawingMethod) value);
			} else if (propertyEnum == ArcProperty2.CLIPPROPERTY) {
				this.setClipProperty((ClippableProperty) value);
			} else if (propertyEnum == ArcProperty2.CLIPSTATE) {
				this.setClipState((ClipStateType) value);
			} else if (propertyEnum == ArcProperty2.CLIPBOX) {
				this.setClipBox((Double []) value);
			} else if (propertyEnum == ArcProperty2.LINE) {
				this.setLine((Line) value);
			} else if (propertyEnum == ArcProperty2.FILLMODE) {
				this.setFillMode((Boolean) value);
			} else if (propertyEnum == ArcProperty2.BACKGROUND) {
				this.setBackground((Integer) value);
			} else if (propertyEnum == ArcProperty2.MARK) {
				this.setMark((Mark) value);
			} else if (propertyEnum == ArcProperty2.PARENT) {
				this.setParent((GraphicObject) value);
			} else if (propertyEnum == ArcProperty2.CHILDREN) {
				// TBD
		//		this.setChildren(setChildren((ArrayList<GraphicObject>) value);
			} else if (propertyEnum == ArcProperty2.VISIBLE) {
				this.setVisible((Boolean) value);
			} else if (propertyEnum == ArcProperty2.USERDATA) {
				this.setUserData((byte[]) value);
			}
	}


	/**
	 * @return the arcDrawingMethod
	 */
	public ArcDrawingMethod getArcDrawingMethod() {
		return arcDrawingMethod;
	}

	/**
	 * @param arcDrawingMethod the arcDrawingMethod to set
	 */
	public void setArcDrawingMethod(ArcDrawingMethod arcDrawingMethod) {
		this.arcDrawingMethod = arcDrawingMethod;
	}

	/**
	 * @return the endAngle
	 */
	public Double getEndAngle() {
		return new Double(endAngle);
	}

	/**
	 * @param endAngle the endAngle to set
	 */
	public void setEndAngle(Double endAngle) {
		this.endAngle = endAngle.doubleValue();
	}

	/**
	 * @return the height
	 */
	public Double getHeight() {
		return new Double(height);
	}

	/**
	 * @param height the height to set
	 */
	public void setHeight(Double height) {
		this.height = height.doubleValue();
	}

	/**
	 * @return the startAngle
	 */
	public Double getStartAngle() {
		return new Double(startAngle);
	}

	/**
	 * @param startAngle the startAngle to set
	 */
	public void setStartAngle(Double startAngle) {
		this.startAngle = startAngle.doubleValue();
	}

	/**
	 * @return the upperLeftPoint
	 */
	public Double [] getUpperLeftPoint() {
		//System.out.println("getUpperLeftPoint !\n");
		
		Double [] returnedPoint = new Double[3];
		returnedPoint[0] = new Double(upperLeftPoint[0]);
		returnedPoint[1] = new Double(upperLeftPoint[1]);
		returnedPoint[2] = new Double(upperLeftPoint[2]);
		return returnedPoint;
	}

	/**
	 * @param upperLeftPoint the upperLeftPoint to set
	 */
	public void setUpperLeftPoint(Double [] upperLeftPoint) {
		if(this.upperLeftPoint == null) {
			this.upperLeftPoint = new double [3];
		}

		this.upperLeftPoint[0] = upperLeftPoint[0].doubleValue();
		this.upperLeftPoint[1] = upperLeftPoint[1].doubleValue();
		this.upperLeftPoint[2] = upperLeftPoint[2].doubleValue();
	}

	/**
	 * @return the width
	 */
	public Double getWidth() {
		return new Double(width);
	}

	/**
	 * @param width the width to set
	 */
	public void setWidth(Double width) {
		this.width = width.doubleValue();
	}

	//	 Methods: to be done
}
