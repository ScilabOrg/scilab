package org.scilab.modules.graphic_objects;

import org.scilab.modules.graphic_objects.Box.BoxType;
//import org.scilab.modules.graphic_objects.Text.TextProperty;
import org.scilab.modules.graphic_objects.Camera.ViewType;

/**
 * Axes class
 * TBD: (AxesProperties plus title) get/set methods
 * @author juliachs
 */
public class Axes extends GraphicObject {
	/** Axes properties names */
	private enum AxesProperty { AXES, GRIDPOSITION, TITLE, AUTOCLEAR, FILLED, CAMERA, BOX, MARGINS, AXESBOUNDS };
	
	/** Specifies the grid position relative to the graphics entities */
	private static enum GridPosition { FOREGROUND, BACKGROUND };

	/** 3-element array (properties of the X, Y and Z axes) */
	private AxisProperty [] axes;

	/** Grid position */
	private GridPosition gridPosition;
	
	/** Title label */
	private Label title;

	/** Specifies whether the Axes subwindow is cleared when a new plot command is performed */ 
	private boolean autoClear;

	/** Specifies whether the Axes background is filled or not  */
	private boolean filled;

	/** Camera */
	private Camera camera;

	/** Box: properties related to the data bounds */
	private Box box;
	
	/** Margins enclosing the drawing area (left, right, top, bottom) */
	private double [] margins;
	
	/**
	 * Axes bounds relative to their parent figure's
	 * drawing area (x, y, width, height)
	 */
	private double [] axesBounds;

	/** Constructor */
	public Axes() {
		super();
		axes = null;
		gridPosition = GridPosition.FOREGROUND;
		title = null;
		autoClear = false;
		filled = false;
		camera = null;
		box = null;
		margins = null;
		axesBounds = null;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the associated property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;
		//System.out.format("property name: %s\n", propertyName);

		if (propertyName.equals("Axes")) {
			returnedProp =  AxesProperty.AXES;
		} else if (propertyName.equals("GridPosition")) {
			returnedProp =  AxesProperty.GRIDPOSITION;
		} else if (propertyName.equals("Title")) {
			returnedProp =  AxesProperty.TITLE;
		} else if (propertyName.equals("AutoClear")) {
			returnedProp =  AxesProperty.AUTOCLEAR;
		} else if (propertyName.equals("Filled")) {
			returnedProp =  AxesProperty.FILLED;
		} else if (propertyName.equals("Camera")) {
			returnedProp =  AxesProperty.CAMERA;
		} else if (propertyName.equals("View")) {
			returnedProp = Camera.CameraProperty.VIEW;
		} else if (propertyName.equals("IsoView")) {
			returnedProp = Camera.CameraProperty.ISOVIEW;
		} else if (propertyName.equals("CubeScaling")) {
			returnedProp = Camera.CameraProperty.CUBESCALING;
		} else if (propertyName.equals("RotationAngles")) {
			returnedProp = Camera.CameraProperty.ROTATIONANGLES;
		} else if (propertyName.equals("Box")) {
			returnedProp =  AxesProperty.BOX;
		} else if (propertyName.equals("BoxType")) {
			returnedProp = Box.BoxProperty.BOX;			
		} else if (propertyName.equals("HiddenAxisColor")) {
			returnedProp = Box.BoxProperty.HIDDENAXISCOLOR;
		} else if (propertyName.equals("TightLimits")) {
			returnedProp = Box.BoxProperty.TIGHTLIMITS;
		} else if (propertyName.equals("DataBounds")) {
			returnedProp = Box.BoxProperty.DATABOUNDS;
		} else if (propertyName.equals("RealDataBounds")) {
			returnedProp = Box.BoxProperty.REALDATABOUNDS;
		} else if (propertyName.equals("ZoomBox")) {
			returnedProp = Box.BoxProperty.ZOOMBOX;
		} else if (propertyName.equals("AutoScale")) {
			returnedProp = Box.BoxProperty.AUTOSCALE;
		} else if (propertyName.equals("Margins")) {
			returnedProp =  AxesProperty.MARGINS;
		} else if (propertyName.equals("AxesBounds")) {
			returnedProp =  AxesProperty.AXESBOUNDS;
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

		if (property == AxesProperty.AXES) {
			returnedProp = getAxes();
		} else if (property == AxesProperty.GRIDPOSITION) {
			returnedProp = getGridPosition();
		} else if (property == AxesProperty.TITLE) {
			returnedProp = getTitle();
		} else if (property == AxesProperty.AUTOCLEAR) {
			returnedProp = getAutoClear();
		} else if (property == AxesProperty.FILLED) {
			returnedProp = getFilled();
		} else if (property == AxesProperty.CAMERA) {
			returnedProp = getCamera();
		} else if (property == Camera.CameraProperty.VIEW) {
			returnedProp = getViewType();
		} else if (property == Camera.CameraProperty.ISOVIEW) {
			returnedProp = getIsoView();
		} else if (property == Camera.CameraProperty.CUBESCALING) {
			returnedProp = getCubeScaling();
		} else if (property == Camera.CameraProperty.ROTATIONANGLES) {
			returnedProp = getRotationAngles();
		} else if (property == AxesProperty.BOX) {
			returnedProp = getBox();
		} else if (property == Box.BoxProperty.BOX) {
			returnedProp = getBoxType();
		} else if (property == Box.BoxProperty.HIDDENAXISCOLOR) {
			returnedProp = getHiddenAxisColor();
		} else if (property == Box.BoxProperty.TIGHTLIMITS) {
			returnedProp = getTightLimits();
		} else if (property == Box.BoxProperty.DATABOUNDS) {
			returnedProp = getDataBounds();
		} else if (property == Box.BoxProperty.REALDATABOUNDS) {
			returnedProp = getRealDataBounds();
		} else if (property == Box.BoxProperty.ZOOMBOX) {
			returnedProp = getZoomBox();
		} else if (property == Box.BoxProperty.AUTOSCALE) {
			returnedProp = getAutoScale();
		} else if (property == AxesProperty.MARGINS) {
			returnedProp = getMargins();
		} else if (property == AxesProperty.AXESBOUNDS) {
			returnedProp = getAxesBounds();
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
		if (property == AxesProperty.AXES) {
			setAxes((AxisProperty []) value);
		} else if (property == AxesProperty.GRIDPOSITION) {
			setGridPosition((GridPosition) value);
		} else if (property == AxesProperty.TITLE) {
			setTitle((Label) value);
		} else if (property == AxesProperty.AUTOCLEAR) {
			setAutoClear((Boolean) value);
		} else if (property == AxesProperty.FILLED) {
			setFilled((Boolean) value);
		} else if (property == AxesProperty.CAMERA) {
			setCamera((Camera) value);	
		} else if (property == Camera.CameraProperty.VIEW) {
			setViewType((ViewType) value);
		} else if (property == Camera.CameraProperty.ISOVIEW) {
			setIsoView((Boolean) value);
		} else if (property == Camera.CameraProperty.CUBESCALING) {
			setCubeScaling((Boolean) value);
		} else if (property == Camera.CameraProperty.ROTATIONANGLES) {
			setRotationAngles((double []) value);
		} else if (property == AxesProperty.BOX) {
			setBox((Box) value);
		} else if (property == Box.BoxProperty.BOX) {
			setBoxType((BoxType) value);
		} else if (property == Box.BoxProperty.HIDDENAXISCOLOR) {
			setHiddenAxisColor((Integer) value);
		} else if (property == Box.BoxProperty.TIGHTLIMITS) {
			setTightLimits((Boolean) value);
		} else if (property == Box.BoxProperty.DATABOUNDS) {
			setDataBounds((double []) value);				
		} else if (property == AxesProperty.MARGINS) {
			setMargins((double []) value);
		} else if (property == AxesProperty.AXESBOUNDS) {
			setAxesBounds((double []) value);
		} else {
			super.setPropertyFast(property, value);
		}
	}

	/**
	 * @return the autoClear
	 */
	public boolean getAutoClear() {
		return autoClear;
	}

	/**
	 * @param autoClear the autoClear to set
	 */
	public void setAutoClear(boolean autoClear) {
		this.autoClear = autoClear;
	}

	/**
	 * @return the axes
	 */
	public AxisProperty[] getAxes() {
		return axes;
	}

	/**
	 * @param axes the axes to set
	 */
	public void setAxes(AxisProperty[] axes) {
		this.axes = axes;
	}

	/**
	 * @return the axesBounds
	 */
	public double[] getAxesBounds() {
		return axesBounds;
	}

	/**
	 * @param axesBounds the axesBounds to set
	 */
	public void setAxesBounds(double[] axesBounds) {
		this.axesBounds = axesBounds;
	}

	/**
	 * @return the box
	 */
	public Box getBox() {
		return box;
	}

	/**
	 * @param box the box to set
	 */
	public void setBox(Box box) {
		this.box = box;
	}

	/**
	 * @return the box type
	 */
	public BoxType getBoxType() {
		return box.getBox();
	}
	
	/**
	 * @param box the BoxType to set
	 */
	public void setBoxType(BoxType box) {
		this.box.setBox(box);
	}

	/**
	 * @return the hidden axis color
	 */
	public int getHiddenAxisColor() {
		return box.getHiddenAxisColor();
	}

	/**
	 * @param color the hidden axis color to set
	 */
	public void setHiddenAxisColor(Integer color) {
		box.setHiddenAxisColor(color.intValue());
	}
	

	/**
	 * @return the tight limits
	 */
	public boolean getTightLimits() {
		return box.getTightLimits();
	}

	/**
	 * @param tightLimits the tightLimits to set 
	 */
	public void setTightLimits(Boolean tightLimits) {
		box.setTightLimits(tightLimits.booleanValue());
	}

	/**
	 * @return the data bounds
	 */
	public double[] getDataBounds() {
		return box.getDataBounds();
	}

	/**
	 * @param dataBounds the data bounds to set
	 */
	public void setDataBounds(double [] dataBounds) {
		box.setDataBounds(dataBounds);
	}

	/**
	 * @return the real data bounds
	 */
	public double[] getRealDataBounds() {
		return box.getRealDataBounds();
	}

	/**
	 * @param realDataBounds the real data bounds to set
	 */
	public void setRealDataBounds(double [] realDataBounds) {
		box.setRealDataBounds(realDataBounds);
	}

	/**
	 * @return the zoom box
	 */
	public double[] getZoomBox() {
		return box.getZoomBox();
	}

	/**
	 * @param zoomBox the zoom box to set
	 */
	public void setZoomBox(double [] zoomBox) {
		box.setZoomBox(zoomBox);
	}

	/**
	 * @return the autoscale
	 */
	public boolean getAutoScale() {
		return box.getAutoScale();
	}

	/**
	 * @param autoScale the autoscale to set 
	 */
	public void setAutoScale(Boolean autoScale) {
		box.setAutoScale(autoScale);
	}

	/**
	 * @return the camera
	 */
	public Camera getCamera() {
		return camera;
	}

	/**
	 * @param camera the camera to set
	 */
	public void setCamera(Camera camera) {
		this.camera = camera;
	}

	/**
	 * @return the view type
	 */
	public ViewType getViewType() {
		return camera.getView();
	}

	/**
	 * @param view the ViewType to set
	 */
	public void setViewType(ViewType view) {
		camera.setView(view);
	}

	/**
	 * @return the isoview
	 */
	public boolean getIsoView() {
		return camera.getIsoview();
	}

	/**
	 * @param isoView the isoview to set
	 */
	public void setIsoView(Boolean isoView) {
		camera.setIsoview(isoView.booleanValue());
	}
	
	/**
	 * @return the cubescaling
	 */
	public boolean getCubeScaling() {
		return camera.getCubeScaling();
	}

	/**
	 * @param cubeScaling the cubescaling to set
	 */
	public void setCubeScaling(Boolean cubeScaling) {
		camera.setCubeScaling(cubeScaling.booleanValue());
	}

	/**
	 * @return the rotation angles
	 */
	public double [] getRotationAngles() {
		return camera.getRotationAngles();
	}

	/**
	 * @param rotationAngles the rotation angles to set
	 */
	public void setRotationAngles(double [] rotationAngles) {
		camera.setRotationAngles(rotationAngles);
	}

	/**
	 * @return the filled
	 */
	public boolean getFilled() {
		return filled;
	}

	/**
	 * @param filled the filled to set
	 */
	public void setFilled(boolean filled) {
		this.filled = filled;
	}

	/**
	 * @return the gridPosition
	 */
	public GridPosition getGridPosition() {
		return gridPosition;
	}

	/**
	 * @param gridPosition the gridPosition to set
	 */
	public void setGridPosition(GridPosition gridPosition) {
		this.gridPosition = gridPosition;
	}

	/**
	 * @return the margins
	 */
	public double[] getMargins() {
		return margins;
	}

	/**
	 * @param margins the margins to set
	 */
	public void setMargins(double[] margins) {
		this.margins = margins;
	}

	/**
	 * @return the title
	 */
	public Label getTitle() {
		return title;
	}

	/**
	 * @param title the title to set
	 */
	public void setTitle(Label title) {
		this.title = title;
	}

	//	 Methods: to be done
}
