/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing functions to switch between user and window coordinates
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.utils;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.drawers.FillDrawerGL;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;
import org.scilab.modules.renderer.utils.geom3D.Matrix4D;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;


/**
 * Class containing functions to switch between user and window coordinates.
 * This is a singleton class.
 * @author Jean-Baptiste Silvy
 */
public class CoordinateTransformation {

	/** transformation matrices sizes */
	public static final int MATRIX_4X4_SIZE = 16;
	
	/** Size of the view port */
	public static final int VIEW_PORT_SIZE = 4;
	
	/** relative back depth is between this value and 1*/
	private static final double START_BACK_DEPTH = 0.95;
	/** relative front depth is between 0 and this value*/
	private static final double END_FRONT_DEPTH = 0.05;
	
	/**
	 * Offsets used for antialiasing using accumulation buffer.
	 * Values taken from OpenGL red book (see
	 * http://www.opengl.org/resources/code/samples/sig99/advanced99/notes/node124.html).
	 * the values must be recentered on 0 and multiplied by 2 (see example in the page).
	 * In the example, note that ortho[0] = ortho[5] = 1 in our case.
	 */
	private static final double[][][] ANTIALIASING_OFFSET =
		{null, /* 0 */
		 {{0.0, 0.0}}, /* 1 */
		 {{-0.5, 0.5},
		 {0.5, -0.5}}, /* 2 */
		 null, /* 3 */
		 {{-0.25, -0.5},
		 {0.25, 0.5},
		 {0.75, -0.5},
		 {0.25, 0.5}}, /* 4 */
		 null, /* 5 */
		 null, /* 6 */
		 null, /* 7 */
		 {{0.125, -0.125},
		  {-0.875, 0.875},
		  {-0.375, 0.375},
		  {0.375, 0.625},
		  {0.625, -0.625},
		  {0.875, 0.125},
		  {-0.125, -0.875},
		  {-0.625, -0.375}}, /* 8 */
		 null, /* 9 */
		 null, /* 10 */
		 null, /* 11 */
		 null, /* 12 */
		 null, /* 13 */
		 null, /* 14 */
		 null, /* 15 */
		 {{-0.25, -0.125},
		  {0.25, -0.875},
		  {0.75, -0.625},
		  {-0.75, -0.875},
		  {-0.25, 0.375},
		  {0.75, -0.125},
		  {0.25, 0.125},
		  {-0.25, 0.875},
		  {0.25, -0.375},
		  {-0.75, 0.125},
		  {-0.75, 0.625},
		  {-0.25, -0.625},
		  {0.75, 0.875},
		  {0.75, 0.375},
		  {-0.75, -0.375},
		  {0.25, 0.625}} /* 16 */
		};
	
	private Matrix4D projectionMatrix;
	private Matrix4D modelViewMatrix;
	
	private Matrix4D completeProjectMatrix;
	private Matrix4D unprojectMatrix;
	private double[] viewPort;
	
	private Vector3D additionalTranslation;
	private Vector3D additionalTranslationPix;
	
	private double zNear;
	private double zFar;
	
	private DrawableFigureGL displayedFigure;
	
	private boolean is2dMode;
	
	/** Current pass for accum buffer antialiasing (1,2,3 or 4) */
	private int antiAliasingPass;
	
	private int antiAliasingQuality;
	
	private int antiAliasingQualityBackup;
	
	/** switch between multisampling and accumuluation buffer */
	private boolean useFallBackAntialiasing;
	
	/**
	 * default constructor
	 * @param figure displayed figure
	 */
	public CoordinateTransformation(DrawableFigureGL figure) {
		completeProjectMatrix = null;
		unprojectMatrix = null;
		projectionMatrix = new Matrix4D();
		modelViewMatrix = new Matrix4D();
		viewPort = new double[VIEW_PORT_SIZE];
		additionalTranslation = null;
		additionalTranslationPix = null;
		zNear = 0.0;
		zFar = 1.0;
		is2dMode = false;
		antiAliasingPass = 0;
		antiAliasingQuality = 0;
		antiAliasingQualityBackup = 0;
		this.displayedFigure = figure;
		useFallBackAntialiasing = false;
	}
	
	
	/**
	 * @param is2d specify if the drawing is 2d or not
	 */
	public void set2dMode(boolean is2d) {
		this.is2dMode = is2d;
	}
	
	/**
	 * @return true is we are displaying in 2D.
	 */
	public boolean is2dMode() {
		return is2dMode;
	}
	
	/**
	 * @return current projection matrix
	 */
	public Matrix4D getCompleteProjectionMatrix() {
		return completeProjectMatrix;
	}
	
	/**
	 * @return current model view matrix
	 */
	public Matrix4D getModelViewMatrix() {
		return modelViewMatrix;
	}
	
	/**
	 * @return current inverse of projection matrix
	 */
	public Matrix4D getUnprojectMatrix() {
		return unprojectMatrix;
	}
	
	/**
	 * @return current viewPort
	 */
	public double[] getViewPort() {
		return viewPort;
	}
	
	/**
	 * @param trans translation to apply
	 */
	public void setAdditionalTranslation(Vector3D trans) {
		this.additionalTranslation = trans;
		this.additionalTranslationPix = null; // not up to date
		
	}
	
	/**
	 * @return additional translation
	 */
	public Vector3D getAdditionalTranslation() {
		return additionalTranslation;
	}
	
	/**
	 * @param gl current OpenGL pipeline
	 * @return additional translation
	 */
	public Vector3D getAdditionalTranslationPix(GL gl) {
		if (additionalTranslation == null) {
			return null;
		} else if (additionalTranslationPix == null) {
			additionalTranslationPix = getCanvasDirection(gl, additionalTranslation);
			return additionalTranslationPix;
		} else {
			return additionalTranslationPix;
		}
	}
	
	/**
	 * Update the projection data of the coordinates.
	 * @param gl current Gl pipeline
	 */
	public void update(GL gl) {
		// get OpenGL transformation matrices
		double[] oglModelViewMatrix = new double[MATRIX_4X4_SIZE];
		double[] oglProjectionMatrix = new double[MATRIX_4X4_SIZE];
		gl.glGetDoublev(GL.GL_MODELVIEW_MATRIX, oglModelViewMatrix, 0);
		gl.glGetDoublev(GL.GL_PROJECTION_MATRIX, oglProjectionMatrix, 0);
		//gl.glGetDoublev(GL.GL_VIEWPORT, viewPort, 0);
		// force values to 0 to be compatible when
		// OpenGL pipeline is enable. The viewport position
		// is modified. See http://www.javagaming.org/forums/index.php?topic=16414.0.
		viewPort[0] = 0;
		viewPort[1] = 0;
		viewPort[2] = displayedFigure.getCanvasWidth();
		viewPort[VIEW_PORT_SIZE - 1] = displayedFigure.getCanvasHeight();
		
		// projection (without viewport is done by v' = P.M.v
		// where v' is the canvas coordinates and v scene coordinates
		projectionMatrix.setFromOpenGLRepresentation(oglProjectionMatrix);
		modelViewMatrix.setFromOpenGLRepresentation(oglModelViewMatrix);
		completeProjectMatrix = projectionMatrix.mult(modelViewMatrix);
		
		// unproject is done by v = (P.M)^-1.v'
		unprojectMatrix = completeProjectMatrix.getInverse();
	}
	
	/**
	 * Get the coordinate of a 3D position in a canvas frame from its user coordinates.
	 * @param gl current OpenGL pipeline
	 * @param pos coordinates of the position.
	 * @return Vector the X, Y and Z positions in the canvas frame.
	 */
	public Vector3D getCanvasCoordinates(GL gl, Vector3D pos) {
		// I first used gluProject, but it is slower since it will always perform matrices multiplications and inverse.
		//return projectMatrix.mult(pos);
		return project(gl, pos);
	}
	
	/**
	 * Perform the same operation as gluProject.
	 * @param gl unused
	 * @param pos scene position
	 * @return pixel coordinate of the point.
	 */
	public Vector3D project(GL gl, Vector3D pos) {
		Vector3D canvasCoord = completeProjectMatrix.mult(pos);
		canvasCoord.setX(viewPort[0] + viewPort[2] * (canvasCoord.getX() + 1.0) / 2.0);
		canvasCoord.setY(viewPort[1] + viewPort[VIEW_PORT_SIZE - 1] * (canvasCoord.getY() + 1.0) / 2.0);
		canvasCoord.setZ((canvasCoord.getZ() + 1.0) / 2.0);
		return canvasCoord;
	}
	
	/**
	 * Perform the same operation as gluUnproject.
	 * @param gl unused
	 * @param canvasPos canvas position
	 * @return scene coordinate of the point
	 */
	public Vector3D unProject(GL gl, Vector3D canvasPos) {
		Vector3D sceneCoord 
			= new Vector3D(2.0 * (canvasPos.getX() - viewPort[0]) / viewPort[2] - 1.0,
						   2.0 * (canvasPos.getY() - viewPort[1]) / viewPort[VIEW_PORT_SIZE - 1] - 1.0,
						   2.0 * canvasPos.getZ() - 1.0);
		return unprojectMatrix.mult(sceneCoord);
		
	}
	
	/**
	 * Convert several user coordinates to the canvas one
	 * @param gl current OpenGL pipeline
	 * @param positions coordinates of the different position (size n x 3).
	 * @return array of size n x 3 containing the X, Y and Z positions in the canvas frame
	 * 					   corresponding to each 
	 */
	public Vector3D[] getCanvasCoordinates(GL gl, Vector3D[] positions) {
		int nbCoords = positions.length;
		Vector3D[] canvasCoords = new Vector3D[nbCoords];
		
		for (int i = 0; i < nbCoords; i++) {
			canvasCoords[i] = getCanvasCoordinates(gl, positions[i]);
		}
		return canvasCoords;
	}
	
	/**
	 * Convert a direction in pixels
	 * @param gl current GL pipeline
	 * @param direction direction to transform
	 * @return direcyion in pixels
	 */
	public Vector3D getCanvasDirection(GL gl, Vector3D direction) {
		Vector3D originPix = getCanvasCoordinates(gl, Vector3D.ORIGIN);
		Vector3D res = getCanvasCoordinates(gl, direction);
		return res.substract(originPix);
	}
	
	/**
	 * Opposite transformation od getCanvasCoordinates.
	 * Compute back scene coordinates from window coordinates.
	 * @param gl current OpengL pipeline
	 * @param canvasPos coordinates of the point in the canvas frame
	 * @return coodinates in object frame
	 */
	public Vector3D retrieveSceneCoordinates(GL gl, Vector3D canvasPos) {
		// I first used gluUnproject, but it is slower since it will always perform matrices multiplications and inverse.
		
		//return unprojectMatrix.mult(canvasPos);
		return unProject(gl, canvasPos);
	}
	
	/**
	 * Transform a direction vector from current coordinates system to canvas position.
	 * In this mode, translation are discared.
	 * @param gl current GL pipeline unused.
	 * @param dir direction in the current coordinates system.
	 * @return direction in the canvas coordinate system.
	 */
	public Vector3D getCanvasCoordinates3D(GL gl, Vector3D dir) {
		return completeProjectMatrix.mult3D(dir);
	}
	
	/**
	 * Opposite transformation of getCanvasCoordinates3D.
	 * @param gl current GL pipeline unused.
	 * @param canvasDir direction in canvas coordinates
	 * @return direction in the current coordinates system
	 */
	public Vector3D retrieveSceneCoordinates3D(GL gl, Vector3D canvasDir) {
		return unprojectMatrix.mult3D(canvasDir);
	}
	
	/**
	 * @return String representation of the transformationException.
	 */
	@Override
	public String toString() {
		return completeProjectMatrix.toString();
	}
	
	/**
	 * Set the depth range to use to draw the following primitives
	 * @param zNear Specifies the mapping of the near clipping plane t window coordinates.
	 *              The initial value is 0.
	 * @param zFar Specifies the mapping of the far clipping plane to
     *             window coordinates. The initial value is 1.
	 */
	public void setDepthRange(double zNear, double zFar) {
		this.zNear = zNear;
		this.zFar = zFar;
	}
	
	/**
	 * Draw the following primitives in front of any others
	 * by setting the depth range to something close to 0.
	 * @param gl current gl pipeline
	 */
	public void drawFront(GL gl) {
		gl.glDepthRange(zNear, getFrontEndDepth());
	}
	
	/**
	 * Draw the following primitives behind of any others
	 * by setting the depth range to something close to 0.
	 * @param gl current gl pipeline
	 */
	public void drawBack(GL gl) {
		gl.glDepthRange(getBackStartDepth(), zFar);
	}
	
	/**
	 * Standard mode. Draw primitives in the middle behind the one
	 * drawn in front and the above drawn in back.
	 * @param gl current gl pipeline
	 */
	public void drawMiddle(GL gl) {
		gl.glDepthRange(getFrontEndDepth(), getBackStartDepth());
	}
	
	/**
	 * @return end depth of the front depth range
	 */
	private double getFrontEndDepth() {
		return (1 - END_FRONT_DEPTH) * zNear + END_FRONT_DEPTH * zFar;
	}
	
	/**
	 * @return startdepth of the back depth range
	 */
	private double getBackStartDepth() {
		return (1 - START_BACK_DEPTH) * zNear + START_BACK_DEPTH * zFar;
	}
	
	/**
	 * Enable polygon offset
	 * @param gl current gl pipeline
	 * @param drawer object drawing polygons
	 */
	public void pushPolygonsBack(GL gl, FillDrawerGL drawer) {
		if (!is2dMode || !drawer.isZConstant()) {
			GLTools.pushPolygonsBack(gl);
		}
	}
	
	/**
	 * Disable polygon offset
	 * @param gl current gl pipeline
	 * @param drawer object drawing polygons
	 */
	public void endPushPolygonsBack(GL gl, FillDrawerGL drawer) {
		if (!is2dMode || !drawer.isZConstant()) {
			GLTools.endPushPolygonsBack(gl);
		}
	}
	
	/**
	 * Enable polygon offset if not in 2D mode.
	 * @param gl current gl pipeline
	 */
	public void pushPolygonsBack(GL gl) {
		if (!is2dMode) {
			GLTools.pushPolygonsBack(gl);
		}
	}
	
	
	/**
	 * Disable polygon offset if not in 2D mode.
	 * @param gl current gl pipeline
	 */
	public void endPushPolygonsBack(GL gl) {
		if (!is2dMode) {
			GLTools.pushPolygonsBack(gl);
		}
	}
	
	/**
	 * Get the number of pass used for antialiasing with accum buffer.
	 * @return number of pass that must be down
	 */
	protected int getNbAntialisingPass() {
		return getAntialiasingFactors(antiAliasingQuality).length;
	}
	
	/**
	 * Go to the next pass of antialiasing with accumulation buffer. 
	 * @param gl curren GL pointer
	 * @return true if some rendering of the scene is needed, false otherwise
	 */
	public boolean nextAntialiasingPass(GL gl) {
		if (isAccumBufferAntialiasingEnable()) {
			// perform anti-aliasing using accumulation buffers
			if (antiAliasingPass == 0) {
				// before the first pass
				gl.glClear(GL.GL_ACCUM_BUFFER_BIT);
			} else  {
				// just after the end of a pass
				gl.glAccum(GL.GL_ACCUM, 1.0f / ((float) getNbAntialisingPass()));
			}
			
			if (antiAliasingPass == getNbAntialisingPass()) {
				// just after the last pass
				gl.glAccum(GL.GL_RETURN, 1.0f);
				
				// reset before next call
				antiAliasingPass = 0;
				
				// the end
				return false;
			}
			
			antiAliasingPass++;
			
			// some more accumulation is needed
			return true;
		} else if (antiAliasingPass == 0) {
			
			// render as usual
			antiAliasingPass++;
			return true;
		} else {
			
			// normal render already done
			antiAliasingPass = 0;
			return false;
		}
	}
	
	/**
	 * @return true is antialiasing is enable, false otherwise
	 */
	public boolean isAntiAliasingEnable() {
		return (antiAliasingQuality > 0);
	}
	
	/**
	 * @return true if accum buffer antialiasing should be used
	 */
	private boolean isAccumBufferAntialiasingEnable() {
		return isAntiAliasingEnable() && useFallBackAntialiasing;
	}
	
	/**
	 * Get the aliasing factors to use for a given quality.
	 * @param quality Quality to use (2,4,8 or 16).
	 * @return matrix of index.
	 */
	private double[][] getAntialiasingFactors(int quality) {
		if (quality < ANTIALIASING_OFFSET.length && ANTIALIASING_OFFSET[quality] != null) {
			// should be the case normally
			return ANTIALIASING_OFFSET[quality];
		} else {
			// return the quality just below
			return getAntialiasingFactors(quality - 1);
		}
	}
	
	/**
	 * Move the viewport in order to perform antizliased display.
	 * @param gl current GL pointer
	 */
	public void loadIdentityWithAntialiasing(GL gl) {
		
		gl.glLoadIdentity();
		if (isAccumBufferAntialiasingEnable() && antiAliasingPass > 0) {
		    // move viewpoint a little to perform anti-aliasing
			double[][] antialiasingFactors = getAntialiasingFactors(antiAliasingQuality);
			gl.glTranslated(antialiasingFactors[antiAliasingPass - 1][0] / displayedFigure.getCanvasWidth(),
					        antialiasingFactors[antiAliasingPass - 1][1] / displayedFigure.getCanvasHeight(),
							0.0);
		}
	}
	
	/**
	 * @return the number of pass used for antialiasing or 0 if antialiasing is disable.
	 */
	public int getAntialiasingQuality() {
		return antiAliasingQuality;
	}
	
	/**
	 * Modify the quality of antialiasing or disable it.
	 * If quality if 0, the antialiasing is disables,
	 * otherwise it might be either 1, 2, 4, 8 or 16 and then
	 * specify the number of pass for antialiasing.
	 * @param quality positive integer.
	 */
	public void setAntialiasingQuality(int quality) {
		antiAliasingQuality = quality;
	}
	
	
	/**
	 * Initialize anti-aliasing factors for a figure.
	 * @param gl current GL pointer.
	 */
	public void initalizeAntialiasing(GL gl) {
		if (isAntiAliasingEnable()) {
			// anti-aliasing is enable, check if we can use
			// multiSampling
			if (gl.isExtensionAvailable("GL_VERSION_1_3")) {
				// multisampling can be enabled
				// but check if we really have the requested number of buffers
				int[] nbSampleBuffers = {0};
				gl.glGetIntegerv(GL.GL_SAMPLES, nbSampleBuffers, 0);
				if (nbSampleBuffers[0] == antiAliasingQuality) {
					// every thing ok, we have as many samples as we need
					useFallBackAntialiasing = false;
					gl.glEnable(GL.GL_MULTISAMPLE);
				} else {
					// we don't have the same, number we need to use accumulation buffers
					useFallBackAntialiasing = true;
					gl.glDisable(GL.GL_MULTISAMPLE);
				}
			} else {
				// gl version is lower than 1.3, multi sampling is not available
				useFallBackAntialiasing = true;
			}
			
			// if fallback mode is on, check that accum buffer is OK
			if (useFallBackAntialiasing) {
				int[] nbAccumBits = {0, 0, 0};
				gl.glGetIntegerv(GL.GL_ACCUM_RED_BITS, nbAccumBits, 0);
				gl.glGetIntegerv(GL.GL_ACCUM_GREEN_BITS, nbAccumBits, 1);
				gl.glGetIntegerv(GL.GL_ACCUM_BLUE_BITS, nbAccumBits, 2);
				
				if (nbAccumBits[0] == 0 || nbAccumBits[1] == 0 || nbAccumBits[2] == 0) {
					// accum buffers not there, so disable antiAliasing
					antiAliasingQuality = 0;
				}
			}
		}
		
		
	}
	
	/**
	 * Disable antiAliasing until enableAntiAliasing is called.
	 */
	public void disableAntialiasing() {
		if (antiAliasingQuality > 0) {
			antiAliasingQualityBackup = antiAliasingQuality;
		}
		antiAliasingQuality = 0;
	}
	
	/**
	 * Enable antialiasing again after a call to disableAntiAliasing.
	 */
	public void enableAntialiasing() {
		if (antiAliasingQuality == 0) {
			antiAliasingQuality = antiAliasingQualityBackup;
		}
		antiAliasingQualityBackup = 0;
	}
	
}
