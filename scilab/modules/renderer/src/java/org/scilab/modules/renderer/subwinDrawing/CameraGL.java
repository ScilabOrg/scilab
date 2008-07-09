/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines position the camera
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.subwinDrawing;

import org.scilab.modules.renderer.ObjectGL;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.Matrix4D;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.UnitaryCubeGL;

import javax.media.opengl.GL;

/**
 * Class containing the driver dependant routines position the camera.
 * @author Jean-Baptiste Silvy
 */
public abstract class CameraGL extends ObjectGL {

	/** Distance of the far clipping plane, should be greater than 1.0 and not to high */
	public static final double FAR_PLANE_DISTANCE = 10.0;
	
	/** Default rotation angles, 2D view */
	private static final double DEFAULT_ALPHA = 0.0;
	private static final double DEFAULT_THETA = 270.0;
	
	/** Move viewPort to respect margins */
	private Vector3D viewPortTranslation = new Vector3D(); 
	
	/** Rescale viewport to respect margins */
	private Vector3D viewPortScale = new Vector3D();
	
	/** Scale to put all datas between 0 and 1 */
	private Vector3D normalizeScale = new Vector3D();
	
	/** Translation to put all datas between 0 and 1. */
	private Vector3D normalizeTranslation = new Vector3D();
	
	/** center of the axes box */
	private Vector3D rotationCenter = new Vector3D();
	
	/** Keep back camera parameters in oder to switch back to 2D mode if needed */
	private double alpha;
	private double theta;
	
	/** scale used to get the best fitting in the canvas for axes */
	private Vector3D fittingScale = new Vector3D();
	
	/** size of area enclosed by margins. */
	private double[] marginSize;
	
	/** To know which axis to revert */
	private boolean xAxisRevert;
	private boolean yAxisRevert;
	private boolean zAxisRevert;
	
	private Matrix4D projectionMatrix;
	private Matrix4D unprojectMatrix;
	private Matrix4D projectionMatrix2D;
	private Matrix4D unprojectMatrix2D;
	
	/**
	 * Default constructor
	 */
	public CameraGL() {
		super();
	}
	
	
	/**
	 * @return Width and height of the margins in pixels.
	 */
	public double[] getMarginSize() {
		return marginSize;
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		showCamera();
	}
	
	/**
	 * Place the camera using alredy computed matrices
	 */
	public void showCamera() {
		GL gl = getGL();
		gl.glMatrixMode(GL.GL_PROJECTION);
		gl.glLoadIdentity();
		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glLoadMatrixd(projectionMatrix.getOpenGLRepresentation(), 0);
		gl.glPushMatrix();
		getCoordinateTransformation().update(gl);
	}
	
	/**
	 * Place the camera using precomputed data
	 */
	public void redrawCamera() {
		placeCamera();
	}
	
	/**
	 * Override endrawing to update coordinate transforation.
	 */
	public void endDrawing() {
		super.endDrawing();
		// Camera is placed, update transformation
		//GL gl = getGL();
		//CoordinateTransformation.getTransformation(gl).update(gl);
	}
	
	/**
	 * Set the viewPort of the object.
	 */
	protected abstract void setViewPort();
	
	/**
	 * @return view port width.
	 */
	protected abstract double getViewPortWidth();
	
	/**
	 * @return view port height.
	 */
	protected abstract double getViewPortHeight();
	
	
	/**
	 * Set the parameters for positionning viewing area (specified by axes margins)
	 * @param transX X translation of the viewing area
	 * @param transY Y translation of the viewing area
	 * @param scaleX X scale of the viewing area
	 * @param scaleY Y scale of the viewing area
	 */
	public void setViewingArea(double transX, double transY, double scaleX, double scaleY) {
		// we move it in 2D.
		viewPortTranslation.setValues(transX, transY, 0.0);
		viewPortScale.setValues(scaleX, scaleY, 1.0);
	}
	
	/**
	 * Position the viewing area in order to respect Subwin position.
	 */
	protected void moveViewingArea() {
		GL gl = getGL();
		
		setViewPort();
		
	    gl.glMatrixMode(GL.GL_MODELVIEW);
	    gl.glLoadIdentity();

	    gl.glScaled(getViewPortWidth(), getViewPortHeight(), 1.0);
		gl.glTranslated(viewPortTranslation.getX(), viewPortTranslation.getY(), viewPortTranslation.getZ());
		gl.glScaled(viewPortScale.getX(), viewPortScale.getY(), viewPortScale.getZ());
		
		// save pixel coordinates of the margin box, our axes box needs to fit inside.
		computeMarginsSize();
		
	}
	
	/**
	 * Compute width and height of the margins in pixels.
	 */
	protected void computeMarginsSize() {
          marginSize = UnitaryCubeGL.getCubeScreenExtent(getGL(), getCoordinateTransformation());
	}
	
	/**
	 * Set parameters so the axes box is map onto a the [0,1]x[0,1]x[0,1] cube.
	 * @param normalizeScaleX X scale to fit bounds within [0,1].
	 * @param normalizeScaleY Y scale to fit bounds within [0,1].
	 * @param normalizeScaleZ Z scale to fit bounds within [0,1].
	 * @param transX X translation to put the axes in view.
	 * @param transY Y translation to put the axes in view.
	 * @param transZ Z translation to put the axes in view.
	 */
	public void setNormalizationParameters(double normalizeScaleX, double normalizeScaleY, double normalizeScaleZ,
                                               double transX, double transY, double transZ) {
		normalizeScale.setValues(normalizeScaleX, normalizeScaleY, normalizeScaleZ);
		normalizeTranslation.setValues(transX, transY, transZ);
	}
	
	/**
	 * Specify which axies to revert
	 * @param xAxisRevert set wether X axis is revert or not.
	 * @param yAxisRevert set wether Y axis is revert or not.
	 * @param zAxisRevert set wether Z axis is revert or not.
	 */
	public void setAxesReverse(boolean xAxisRevert, boolean yAxisRevert, boolean zAxisRevert) {
		this.xAxisRevert = xAxisRevert;
		this.yAxisRevert = yAxisRevert;
		this.zAxisRevert = zAxisRevert;
	}
	
	/**
	 * Move the axes box so it map the onto a the [0,1]x[0,1]x[0,1] cube.
	 */
	protected void moveAxesBox() {
		GL gl = getGL();
		gl.glScaled(normalizeScale.getX(), normalizeScale.getY(), normalizeScale.getZ());
		
		gl.glTranslated(normalizeTranslation.getX(), normalizeTranslation.getY(), normalizeTranslation.getZ());
		
	}
	
	/**
	 * Set the parameters for rotating the axes box.
	 * @param centerX X coordiantes of the rotation center 
	 * @param centerY Y coordinates of the rotation center
	 * @param centerZ Z coordinates of the rotation center
	 * @param alpha rotation angle around axe X
	 * @param theta rotation angle around axe Z
	 */
	public void setAxesRotationParameters(double centerX, double centerY, double centerZ,
									      double alpha, double theta) {
		rotationCenter.setValues(centerX, centerY, centerZ);
		this.alpha = alpha;
		this.theta = theta;
	}
	
	/**
	 * Set the scale used to best fit the margins.
	 * @param scaleX X coordinate of the scale
	 * @param scaleY Y coordinate of the scale
	 * @param scaleZ Z cordinate of the scale
	 */
	public void setFittingScale(double scaleX, double scaleY, double scaleZ) {
		fittingScale.setValues(scaleX, scaleY, scaleZ);
	}
	
	/**
	 * Move the box to the center of the screen.
	 */
	protected void centerAxesBox() {
		GL gl = getGL();
		
		// rotate around the center of the box axes
		gl.glTranslated(rotationCenter.getX(),  rotationCenter.getY(),  rotationCenter.getZ());
		
		// reduction need to be performed on the center of the screen
		gl.glScaled(1.0 / (normalizeScale.getX() * getViewPortWidth()),
					1.0 / (normalizeScale.getY() * getViewPortHeight()),
					1.0 / normalizeScale.getZ());
	}
	
	/**
	 * Compute the bounding rectangle of the axes box projection on the screen. 
	 * @param gl current OpenGL pipeline.
	 * @return array of size 2: width and height in pixels.
	 */
	protected double[] getBoxScreenExtent(GL gl) {
		// apply transformation a first time to 
		gl.glPushMatrix();
		applyRotation(gl, alpha, theta);
		
		gl.glScaled(fittingScale.getX() / normalizeScale.getX(),
					fittingScale.getY() / normalizeScale.getY(),
					fittingScale.getZ() / normalizeScale.getZ());
		
		// get the size of the viewing area on the screen.
		double[] screenExtent = UnitaryCubeGL.getCubeScreenExtent(gl, getCoordinateTransformation());
		gl.glPopMatrix();
		return screenExtent;
	}
	
	/**
	 * Compute the scale wich will best fit the window in accordance with viewing angles.
	 */
	protected abstract void computeFittingScale();
	
	
	/**
	 * Rotate the axes box using angles and scale.
	 */
	protected void rotateAxesBox() {
		GL gl = getGL();
		applyRotation(gl, alpha, theta);
		gl.glScaled(fittingScale.getX(), fittingScale.getY(), fittingScale.getZ());
		gl.glTranslated(-rotationCenter.getX(), -rotationCenter.getY(), -rotationCenter.getZ()); // translate origin back
		
	}
	
	/**
	 * 
	 *
	 */
	protected void revertAxes() {
		// to be applyed on the center of the box
		GL gl = getGL();
		gl.glTranslated(rotationCenter.getX(), rotationCenter.getY(), rotationCenter.getZ()); // translate origin back

		if (xAxisRevert) {
			gl.glScaled(-1.0, 1.0, 1.0);
		}
		if (yAxisRevert) {
			gl.glScaled(1.0, -1.0, 1.0);
		}
		if (zAxisRevert) {
			gl.glScaled(1.0, 1.0, -1.0);
		}
		gl.glTranslated(-rotationCenter.getX(), -rotationCenter.getY(), -rotationCenter.getZ()); // translate origin back
	}
	
	/**
	 * Rotate the axes in accordance with viewing angles.
	 */
	public void placeCamera() {
		
		GL gl = getGL();
		
		// set viewport
		moveViewingArea();

		// set transformation such has coordinates are mapped on the unitary cube.
		moveAxesBox();

		// put box on its center to perform rotation.
		centerAxesBox();
		
		// find the best scale to fit the view port.
		computeFittingScale();
		
		
		gl.glPushMatrix();
		rotateAxesBox();
		revertAxes();
		
		
		// compute the matrix for project and unproject.
		CoordinateTransformation transform = getCoordinateTransformation();
		transform.update(gl);
		
		// update coordinate chnage matrices
		this.projectionMatrix = new Matrix4D(transform.getProjectionMatrix());
		this.unprojectMatrix  = new Matrix4D(transform.getUnprojectMatrix());
		switchTo2DCoordinates();
		this.projectionMatrix2D = new Matrix4D(transform.getProjectionMatrix());
		this.unprojectMatrix2D = new Matrix4D(transform.getUnprojectMatrix());
		backTo3DCoordinates();
		
	}
	
	/**
	 * Calls to turn the 
	 * @param gl current GL pipeline.
	 * @param alpha rotation angle around axe X in degree
	 * @param theta rotation angle around axe Z in degree
	 */
	protected void applyRotation(GL gl, double alpha, double theta) {
		gl.glRotated(DEFAULT_ALPHA - alpha, 1.0 , 0.0, 0.0); /* Seems we need to rotate counterclok-wise */
		gl.glRotated(DEFAULT_THETA - theta, 0.0 , 0.0, 1.0);
	}
	
	/**
	 * To be called at the end of camera use.
	 */
	public void replaceCamera() {
		GL gl = getGL();
		gl.glPopMatrix();
		//CoordinateTransformation.getTransformation(gl).update(gl);
	}
	
	/**
	 * @return array of size 16 containing the current projection matrix
	 */
	public double[] getProjectionMatrix() {
		return projectionMatrix.getRowWiseRepresentation();
	}
	
	/**
	 * @return array of size 16 containing the inverse of projection matrix.
	 */
	public double[] getUnprojectMatrix() {
		return unprojectMatrix.getRowWiseRepresentation();
	}
	
	/**
	 * @return array of size 16 containing the projection matrix for 2d view.
	 */
	public double[] get2dViewProjectionMatrix() {
		return projectionMatrix2D.getRowWiseRepresentation();
	}
	
	/**
	 * @return array of size 16 containing the inverse of projection matrix for 2d view.
	 */
	public double[] get2dViewUnprojectMatrix() {
		return unprojectMatrix2D.getRowWiseRepresentation();
	}
	
	/**
	 * @return Array of size 4 containing the viewPort.
	 */
	public double[] getViewPort() {
		return getCoordinateTransformation().getViewPort();
	}
	
	/**
	 * Move the camera to the default 2D coordinates.
	 * Try to avoid to use this method, but it sometime needed
	 * for backward compatibility with old renderer.
	 */
	protected void switchTo2DCoordinates() {
		GL gl = getGL();
		gl.glPopMatrix();
		gl.glPushMatrix();
		gl.glScaled(fittingScale.getX(), fittingScale.getY(), fittingScale.getZ());
		gl.glTranslated(-rotationCenter.getX(), -rotationCenter.getY(), -rotationCenter.getZ()); // translate origin back
		revertAxes();
		
		// update transformation
		getCoordinateTransformation().update(gl);
		
	}
	
	/**
	 * Return to the normal view after a call to switchTo2DCoordinates.
	 */
	protected void backTo3DCoordinates() {
		GL gl = getGL();
		gl.glPopMatrix();
		gl.glPushMatrix();
		rotateAxesBox();
		revertAxes();
		
		// update transformation
		getCoordinateTransformation().update(gl);
	}
	
	
	
	
}
