package org.scilab.modules.graphic_objects;


/**
 * Figure class
 * @author juliachs
 */
public class Figure extends GraphicObject {
	/** Figure properties names */
	private enum FigureProperty { DIMENSIONS, CANVAS, NAME, INFOMESSAGE, COLORMAP, RENDERINGMODE,
		BACKGROUND, EVENTHANDLER, TAG, ROTATIONTYPE
	};
	
	/** Specifies whether rotation applies to a single subwindow or to all the figure's subwindows */
	private enum RotationType { UNARY, MULTIPLE };
	
	/** Pixel drawing logical operations */
	private enum PixelDrawingMode { CLEAR, AND, ANDREVERSE, COPY, ANDINVERTED, NOOP, XOR, OR,
		EQUIV, INVERT, ORREVERSE, COPYINVERTED, ORINVERTED, NAND, SET };

	/* TBM */
	public enum FigureDimensionsProperty { POSITION, SIZE };
	private class FigureDimensions {
		/** Position (x,y) of the upper-left corner in the top-level window */
		private int [] position;

		/** Dimensions (w, h) of the figure window */
		private int [] size;
	}

	/* TBM */
	public enum CanvasProperty { AUTORESIZE, VIEWPORT, AXESSIZE };
	private class Canvas {
		/** Specifies whether automatic resizing is used */
		private boolean autoResize;

		/** Viewport position (x,y) */
		private int [] viewPort;

		/** Rendering canvas (w,h) dimensions */
		private int [] axesSize;
	}

	/* TBM */
	public enum FigureNameProperty { NAME, ID };
	private class FigureName {
		/** Name */
		private String name;

		/** Identifier */
		private int id;
	}

	/* TBM */
	public enum RenderingModeProperty { PIXMAP, ANTIALIASING, IMMEDIATEDRAWING };
	private class RenderingMode {
		/** Specifies rendering into a pixmap */
		private boolean pixmap;

		/** Antialising level (0 == off) */
		private int antialiasing;

		/** Specifies whether immediate drawing is used or not */
		private boolean immediateDrawing;
	}

	/* TBM */
	public enum EventHandlerProperty { EVENTHANDLER };
	private class EventHandler{
		/** Event handler string */
		private String eventHandler;
	}

	/** Figure dimensions */
	private FigureDimensions dimensions;

	/** Canvas property */
	private Canvas canvas;

	/** Figure name */
	private FigureName figureName;

	/** Info message */
	private String infoMessage;

	/**
	 * Default colormap: (3 x N) matrix, where N is the
	 * number of colors
	 */
	private int [][] colorMap;

	/** Rendering mode */
	private RenderingMode renderingMode;

	/** Background color */
	private int background;

	/** Event handler */
	private EventHandler eventHandler;

	/** Tag */
	private int tag;

	/** Rotation type */
	private RotationType rotation;

	/** Constructor */
	public Figure() {
		super();
		dimensions = new FigureDimensions();
		canvas = new Canvas();
		figureName = new FigureName();
		infoMessage = null;
		colorMap = null;
		renderingMode = new RenderingMode();
		background = 0;
		eventHandler = null;
		tag = 0;
		rotation = null;
	}

	/**
	 * Returns the enum associated to a property name
	 * @param propertyName the property name
	 * @return the associated property enum
	 */
	public Object getPropertyFromName(String propertyName) {
		Object returnedProp;

		if (propertyName.equals("Dimensions")) {
			returnedProp =  FigureProperty.DIMENSIONS;
		} else if (propertyName.equals("Position")) {
			returnedProp = FigureDimensionsProperty.POSITION;
		} else if (propertyName.equals("Size")) {
			returnedProp = FigureDimensionsProperty.SIZE;
		} else if (propertyName.equals("Canvas")) {
			returnedProp =  FigureProperty.CANVAS;
		} else if (propertyName.equals("AutoResize")) {
			returnedProp = CanvasProperty.AUTORESIZE;
		} else if (propertyName.equals("Viewport")) {
			returnedProp = CanvasProperty.VIEWPORT;
		} else if (propertyName.equals("AxesSize")) {
			returnedProp = CanvasProperty.AXESSIZE;
		} else if (propertyName.equals("FigureName")) {
			returnedProp =  FigureProperty.NAME;
		} else if (propertyName.equals("Name")) {
			returnedProp =  FigureNameProperty.NAME;
		} else if (propertyName.equals("Id")) {
			returnedProp =  FigureNameProperty.ID;
		} else if (propertyName.equals("InfoMessage")) {
			returnedProp =  FigureProperty.INFOMESSAGE;
		} else if (propertyName.equals("ColorMap")) {
			returnedProp =  FigureProperty.COLORMAP;
		} else if (propertyName.equals("RenderingMode")) {
			returnedProp = FigureProperty.RENDERINGMODE;
		} else if (propertyName.equals("Pixmap")) {
			returnedProp = RenderingModeProperty.PIXMAP;
		} else if (propertyName.equals("Antialiasing")) {
			returnedProp = RenderingModeProperty.ANTIALIASING;
		} else if (propertyName.equals("ImmediateDrawing")) {
			returnedProp = RenderingModeProperty.IMMEDIATEDRAWING;
		} else if (propertyName.equals("Background")) {
			returnedProp = FigureProperty.BACKGROUND;
		} else if (propertyName.equals("EventHandler")) {
			returnedProp = FigureProperty.EVENTHANDLER;
		} else if (propertyName.equals("EventHandlerName")) {
			returnedProp = EventHandlerProperty.EVENTHANDLER;
		} else if (propertyName.equals("Tag")) {
			returnedProp = FigureProperty.TAG;
		} else if (propertyName.equals("RotationType")) {
			returnedProp =  FigureProperty.ROTATIONTYPE;
		} else {
			returnedProp = super.getPropertyFromName(propertyName);
		}
		
		return returnedProp;
	}


	/**
	 * Fast property get method
	 * @param property the property to get
	 * @return the property
	 */
	public Object getPropertyFast(Object property) {
		Object returnedProp;

		if (property == FigureProperty.DIMENSIONS) {
			returnedProp = getDimensions();
		} else if (property == FigureDimensionsProperty.POSITION) {
			returnedProp = getPosition();
		} else if (property == FigureDimensionsProperty.SIZE) {
			returnedProp = getSize();
		} else if (property == FigureProperty.CANVAS) {
			returnedProp = getCanvas();
		} else if (property == CanvasProperty.AUTORESIZE) {
			returnedProp = getAutoResize();
		} else if (property == CanvasProperty.VIEWPORT) {
			returnedProp = getViewport();
		} else if (property == CanvasProperty.AXESSIZE) {
			returnedProp = getAxesSize();
		} else if (property == FigureProperty.NAME) {
			returnedProp = getFigureName();
		} else if (property == FigureNameProperty.NAME) {
			returnedProp = getName();
		} else if (property == FigureNameProperty.ID) {
			returnedProp = getId();
		} else if (property == FigureProperty.INFOMESSAGE) {
			returnedProp = getInfoMessage();
		} else if (property == FigureProperty.COLORMAP) {
			returnedProp = getColorMap();
		} else if (property == FigureProperty.RENDERINGMODE) {
			returnedProp = getRenderingMode();
		} else if (property == RenderingModeProperty.PIXMAP) {
			returnedProp = getPixpmap();
		} else if (property == RenderingModeProperty.ANTIALIASING) {
			returnedProp = getAntialiasing();
		} else if (property == RenderingModeProperty.IMMEDIATEDRAWING) {
			returnedProp = getImmediateDrawing();
		} else if (property == FigureProperty.BACKGROUND) {
			returnedProp = getBackground();
		} else if (property == FigureProperty.EVENTHANDLER) {
			returnedProp = getEventHandler();
		} else if (property == EventHandlerProperty.EVENTHANDLER) {
			returnedProp = getEventHandlerString();
		} else if (property == FigureProperty.TAG) {
			returnedProp = getTag();
		} else if (property == FigureProperty.ROTATIONTYPE) {
			returnedProp = getRotation();
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
		if (property == FigureProperty.DIMENSIONS) {
			setDimensions((FigureDimensions) value);
		} else if (property == FigureDimensionsProperty.POSITION) {
			setPosition((int []) value);
		} else if (property == FigureDimensionsProperty.SIZE) {
			setSize((int []) value);
		} else if (property == FigureProperty.CANVAS) {
			setCanvas((Canvas) value);
		} else if (property == CanvasProperty.AUTORESIZE) {
			setAutoResize((Boolean) value);
		} else if (property == CanvasProperty.VIEWPORT) {
			setViewport((int []) value);
		} else if (property == CanvasProperty.AXESSIZE) {
			setAxesSize((int []) value);
		} else if (property == FigureProperty.NAME) {
			setFigureName((FigureName) value);
		} else if (property == FigureNameProperty.NAME) {
			setName((String) value);
		} else if (property == FigureNameProperty.ID) {
			setId((Integer) value);
		} else if (property == FigureProperty.INFOMESSAGE) {
			setInfoMessage((String) value);
		} else if (property == FigureProperty.COLORMAP) {
			setColorMap((int[][]) value);
		} else if (property == FigureProperty.RENDERINGMODE) {
			setRenderingMode((RenderingMode) value);
		} else if (property == RenderingModeProperty.PIXMAP) {
			setPixpmap((Boolean) value);
		} else if (property == RenderingModeProperty.ANTIALIASING) {
			setAntialiasing((Integer) value);
		} else if (property == RenderingModeProperty.IMMEDIATEDRAWING) {
			setImmediateDrawing((Boolean) value);
		} else if (property == FigureProperty.BACKGROUND) {
			setBackground((Integer) value);
		} else if (property == FigureProperty.EVENTHANDLER) {
			setEventHandler((EventHandler) value);
		} else if (property == EventHandlerProperty.EVENTHANDLER) {
			setEventHandlerString((String) value);
		} else if (property == FigureProperty.TAG) {
			setTag((Integer) value);
		} else if (property == FigureProperty.ROTATIONTYPE) {
			setRotation((RotationType) value);
		} else {
			super.setPropertyFast(property, value);
		}
	}

	/**
	 * @return the background
	 */
	public int getBackground() {
		return background;
	}

	/**
	 * @param background the background to set
	 */
	public void setBackground(Integer background) {
		this.background = background.intValue();
	}

	/**
	 * @return the canvas
	 */
	public Canvas getCanvas() {
		return canvas;
	}

	/**
	 * @param canvas the canvas to set
	 */
	public void setCanvas(Canvas canvas) {
		this.canvas = canvas;
	}

	/**
	 * @return the autoresize
	 */
	public boolean getAutoResize() {
		return canvas.autoResize;
	}

	/**
	 * @param autoResize the autoresize to set
	 */
	public void setAutoResize(Boolean autoResize) {
		canvas.autoResize = autoResize.booleanValue();
	}
	
	/**
	 * @return the viewport
	 */
	public int[] getViewport() {
		return canvas.viewPort;
	}
	
	/**
	 * @param viewport the viewport to set
	 */
	public void setViewport(int [] viewport) {
		canvas.viewPort = viewport;
	}

	/**
	 * @return the axes size
	 */
	public int[] getAxesSize() {
		return canvas.axesSize;
	}

	/**
	 * @param axesSize the axes size to set
	 */
	public void setAxesSize(int [] axesSize) {
		canvas.axesSize = axesSize;
	}
	
	/**
	 * @return the colorMap
	 */
	public int[][] getColorMap() {
		return colorMap;
	}

	/**
	 * @param colorMap the colorMap to set
	 */
	public void setColorMap(int[][] colorMap) {
		this.colorMap = colorMap;
	}

	/**
	 * @return the dimensions
	 */
	public FigureDimensions getDimensions() {
		return dimensions;
	}

	/**
	 * @param dimensions the dimensions to set
	 */
	public void setDimensions(FigureDimensions dimensions) {
		this.dimensions = dimensions;
	}
	
	/**
	 * @return the figure position
	 */
	public int[] getPosition() {
		return dimensions.position;
	}
	
	/**
	 * @param position the position to set
	 */
	public void setPosition(int [] position) {
		dimensions.position = position;
	}

	/**
	 * @return the figure size
	 */
	public int[] getSize() {
		return dimensions.size;
	}

	/**
	 * @param size the size to set
	 */
	public void setSize(int [] size) {
		this.dimensions.size = size;
	}

	/**
	 * @return the eventHandler
	 */
	public EventHandler getEventHandler() {
		return eventHandler;
	}

	/**
	 * @param eventHandler the eventHandler to set
	 */
	public void setEventHandler(EventHandler eventHandler) {
		this.eventHandler = eventHandler;
	}

	/**
	 * @return the eventHandler string
	 */
	public String getEventHandlerString() {
		return eventHandler.eventHandler;
	}

	/**
	 * @param eventHandlerString the eventHandler string to set
	 */
	public void setEventHandlerString(String eventHandlerString) {
		eventHandler.eventHandler = eventHandlerString;
	}

	/**
	 * @return the infoMessage
	 */
	public String getInfoMessage() {
		return infoMessage;
	}

	/**
	 * @param infoMessage the infoMessage to set
	 */
	public void setInfoMessage(String infoMessage) {
		this.infoMessage = infoMessage;
	}

	/**
	 * @return the name
	 */
	public FigureName getFigureName() {
		return figureName;
	}

	/**
	 * @param figureName the figure name to set
	 */
	public void setFigureName(FigureName figureName) {
		this.figureName = figureName;
	}

	/**
	 * @return the figure name
	 */
	public String getName() {
		return figureName.name;
	}

	/**
	 * @param name the name to set
	 */
	public void setName(String name) {
		figureName.name = name;
		
	}

	/**
	 * @return the id
	 */
	public int getId() {
		return figureName.id;
	}

	/**
	 * @param id the id to set
	 */
	public void setId(Integer id) {
		figureName.id = id.intValue();
		
	}

	
	/**
	 * @return the renderingMode
	 */
	public RenderingMode getRenderingMode() {
		return renderingMode;
	}

	/**
	 * @param renderingMode the renderingMode to set
	 */
	public void setRenderingMode(RenderingMode renderingMode) {
		this.renderingMode = renderingMode;
	}

	/**
	 * @return the pixmap
	 */
	public boolean getPixpmap() {
		return renderingMode.pixmap;
	}

	/**
	 * @param pixmap the pixmap to set
	 */
	public void setPixpmap(Boolean pixmap) {
		renderingMode.pixmap = pixmap.booleanValue();
	}

	/**
	 * @return the pixmap
	 */
	public int getAntialiasing() {
		return renderingMode.antialiasing;
	}

	/**
	 * @param antialiasing the antialiasing to set
	 */
	public void setAntialiasing(Integer antialiasing) {
		renderingMode.antialiasing = antialiasing.intValue();
	}

	/**
	 * @return the immediateDrawing
	 */
	public boolean getImmediateDrawing() {
		return renderingMode.immediateDrawing;
	}
	
	/**
	 * @param immediateDrawing the immediateDrawing to set
	 */
	public void setImmediateDrawing(Boolean immediateDrawing) {
		renderingMode.immediateDrawing = immediateDrawing.booleanValue();
	}

	/**
	 * @return the rotation
	 */
	public RotationType getRotation() {
		return rotation;
	}

	/**
	 * @param rotation the rotation to set
	 */
	public void setRotation(RotationType rotation) {
		this.rotation = rotation;
	}

	/**
	 * @return the tag
	 */
	public int getTag() {
		return tag;
	}

	/**
	 * @param tag the tag to set
	 */
	public void setTag(Integer tag) {
		this.tag = tag.intValue();
	} 
	
	//	 Methods: to be done
}
