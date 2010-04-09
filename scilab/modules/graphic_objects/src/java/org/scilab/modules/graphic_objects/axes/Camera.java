package org.scilab.modules.graphic_objects.axes;

/**
 * Camera class
 * @author juliachs
 */
public class Camera {
	/** Camera properties names */
	public enum CameraProperty { VIEW, ISOVIEW, CUBESCALING, ROTATIONANGLES };
	
	/** View type */
	public enum ViewType { VIEW_2D, VIEW_3D };
	
	/** Specifies whether the projection must be performed for a 2D or a 3D scene */  
	private ViewType view;
	
	/** Specifies whether an isoview transformation must be applied or not */
	private boolean isoview;
	
	/** Specifies whether a unit cube transformation must be performed */
	private boolean cubeScaling;
	
	/** 2-element array (alpha and theta rotation angles) */
	private double[] rotationAngles;

	/** Constructor */
	public Camera() {
		view = ViewType.VIEW_2D;
		isoview = false;
		cubeScaling = false;
		rotationAngles = new double[2];
	}

	/**
	 * @return the cubeScaling
	 */
	public Boolean getCubeScaling() {
		return new Boolean(cubeScaling);
	}

	/**
	 * @param cubeScaling the cubeScaling to set
	 */
	public void setCubeScaling(Boolean cubeScaling) {
		this.cubeScaling = cubeScaling.booleanValue();
	}

	/**
	 * @return the isoview
	 */
	public Boolean getIsoview() {
		return new Boolean(isoview);
	}

	/**
	 * @param isoview the isoview to set
	 */
	public void setIsoview(Boolean isoview) {
		this.isoview = isoview.booleanValue();
	}

	/**
	 * @return the rotationAngles
	 */
	public Double[] getRotationAngles() {
		Double [] retRotationAngles = new Double[2];

		retRotationAngles[0] = new Double(rotationAngles[0]);
		retRotationAngles[1] = new Double(rotationAngles[1]);

		return retRotationAngles;
	}

	/**
	 * @param rotationAngles the rotationAngles to set
	 */
	public void setRotationAngles(Double[] rotationAngles) {
		this.rotationAngles[0] = rotationAngles[0].doubleValue();
		this.rotationAngles[1] = rotationAngles[0].doubleValue();
	}

	/**
	 * @return the view
	 */
	public ViewType getView() {
		return view;
	}

	/**
	 * @param view the view to set
	 */
	public void setView(ViewType view) {
		this.view = view;
	}
	
	// Methods: to be done
}
