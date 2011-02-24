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

package org.scilab.modules.graphic_objects.axes;

/**
 * Camera class
 * @author Manuel JULIACHS
 */
public class Camera {
	/** Default rotation angles */
	public static final double[] DEFAULT_ROTATION_ANGLES = {0.0, 270.0};

	/** Camera properties names */
	public enum CameraProperty { VIEW, ISOVIEW, CUBESCALING, ROTATIONANGLES, ROTATIONANGLES3D };

	/** View type */
	public enum ViewType { VIEW_2D, VIEW_3D;

		/**
		 * Converts an integer to the corresponding enum
		 * @param intValue the integer value
		 * @return the view type type enum
		 */
		public static ViewType intToEnum(Integer intValue) {
			switch (intValue) {
				case 0:
					return ViewType.VIEW_2D;
				case 1:
					return ViewType.VIEW_3D;
				default:
					return null;
			}
		}
	}

	/** Specifies whether the projection must be performed for a 2D or a 3D scene */  
	private ViewType view;

	/** Specifies whether an isoview transformation must be applied or not */
	private boolean isoview;

	/** Specifies whether a unit cube transformation must be performed */
	private boolean cubeScaling;

	/** 2-element array (alpha and theta rotation angles) */
	private double[] rotationAngles;

	/**
	 * Last know values of the rotation angles when View was equal to VIEW_3D
	 * 2-element array (alpha and theta)
	 */
	private double[] rotationAngles3d;

	/** Constructor */
	public Camera() {
		view = ViewType.VIEW_2D;
		isoview = false;
		cubeScaling = false;
		rotationAngles = new double[2];
		rotationAngles3d = new double[2];
	}

	/**
	 * Copy constructor
	 * @param camera the Camera to copy
	 */
	public Camera(Camera camera) {
		view = camera.view;
		isoview = camera.isoview;
		cubeScaling = camera.cubeScaling;

		rotationAngles = new double[2];

		rotationAngles[0] = camera.rotationAngles[0];
		rotationAngles[1] = camera.rotationAngles[1];

		rotationAngles3d = new double[2];

		rotationAngles3d[0] = camera.rotationAngles3d[0];
		rotationAngles3d[1] = camera.rotationAngles3d[1];
	}

	/**
	 * @return the cubeScaling
	 */
	public Boolean getCubeScaling() {
		return cubeScaling;
	}

	/**
	 * @param cubeScaling the cubeScaling to set
	 */
	public void setCubeScaling(Boolean cubeScaling) {
		this.cubeScaling = cubeScaling;
	}

	/**
	 * @return the isoview
	 */
	public Boolean getIsoview() {
		return isoview;
	}

	/**
	 * @param isoview the isoview to set
	 */
	public void setIsoview(Boolean isoview) {
		this.isoview = isoview;
	}

	/**
	 * @return the rotationAngles
	 */
	public Double[] getRotationAngles() {
		Double [] retRotationAngles = new Double[2];

		retRotationAngles[0] = rotationAngles[0];
		retRotationAngles[1] = rotationAngles[1];

		return retRotationAngles;
	}

	/**
	 * @param rotationAngles the rotationAngles to set
	 */
	public void setRotationAngles(Double[] rotationAngles) {
		if (rotationAngles[0] == DEFAULT_ROTATION_ANGLES[0] && rotationAngles[1] == DEFAULT_ROTATION_ANGLES[1]) {
			view = ViewType.VIEW_2D;
		} else if (view == ViewType.VIEW_2D) {
			view = ViewType.VIEW_3D;
		}

		this.rotationAngles[0] = rotationAngles[0];
		this.rotationAngles[1] = rotationAngles[1];
	}

	/**
	 * @return the 3d rotationAngles
	 */
	public Double[] getRotationAngles3d() {
		Double [] retRotationAngles3d = new Double[2];

		retRotationAngles3d[0] = rotationAngles3d[0];
		retRotationAngles3d[1] = rotationAngles3d[1];

		return retRotationAngles3d;
	}

	/**
	 * @param rotationAngles3d the 3d rotationAngles to set
	 */
	public void setRotationAngles3d(Double[] rotationAngles3d) {
		this.rotationAngles3d[0] = rotationAngles3d[0];
		this.rotationAngles3d[1] = rotationAngles3d[1];
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
		if (view == ViewType.VIEW_2D) {
			rotationAngles3d[0] = rotationAngles[0];
			rotationAngles3d[1] = rotationAngles[1];

			rotationAngles[0] = DEFAULT_ROTATION_ANGLES[0];
			rotationAngles[1] = DEFAULT_ROTATION_ANGLES[1];
		} else if (view == ViewType.VIEW_3D && this.view == ViewType.VIEW_2D) {
			rotationAngles[0] = rotationAngles3d[0];
			rotationAngles[1] = rotationAngles3d[1];
		}

		this.view = view;
	}

}
