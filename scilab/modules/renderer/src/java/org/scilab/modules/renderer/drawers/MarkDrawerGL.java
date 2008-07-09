/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Parent class for mark drawing algorithms 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.drawers;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.DrawableObjectGL;

import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.MarkDrawing.MarkDrawer;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Parent class for mark drawing algorithms
 * @author Jean-Baptiste Silvy
 */
public abstract class MarkDrawerGL extends DrawableObjectGL {
	
	/** index of background color */
	private MarkDrawer drawer;
	
	private Vector3D[] markPos;
	
	/**
	 * Default Constructor
	 */
	public MarkDrawerGL() {
		super();
		drawer = new MarkDrawer();
		markPos = null;
	}
	
	
	/**
	 * @return The drawing object.
	 */
	public MarkDrawer getDrawer() {
		return drawer;
	}
	
	/**
	 * Function called before beginning to use OpenGL methods.
	 * @param parentFigureIndex index of the parent figure.
	 *                          Needed to get the GL context to draw in.
	 */
	public void initializeDrawing(int parentFigureIndex) {
		super.initializeDrawing(parentFigureIndex);
		drawer.initializeDrawing(parentFigureIndex);
	}
	
	/**
	 * Function called at the end of the OpenGL use.
	 */
	@Override
	public void endDrawing() {
		drawer.endDrawing();
		super.endDrawing();
	}
	
	/**
	 * Set rectangle background color
	 * @param color index of background color
	 */
	public void setBackground(int color) {
		drawer.setBackground(color);
	}
	
	/**
	 * Set rectangle foreground color
	 * @param color index of foreground color
	 */
	public void setForeground(int color) {
		drawer.setForeground(color);
	}
	
	/**
	 * Set the marksize unit to use, ie poitn or tabulated
	 * @param markSizeUnit index of the kind of mark
	 */
	public void setMarkSizeUnit(int markSizeUnit) {
		if (markSizeUnit == 2) {
			drawer.setTabulatedUnit();
		} else {
			drawer.setPointUnit();
		}
	}
	
	/**
	 * Specify a new size for marks
	 * @param markSize new size
	 */
	public void setMarkSize(int markSize) {
		drawer.setMarkSize(markSize);
	}
	
	
	/**
	 * Specify the kind of mark to draw
	 * @param markStyleIndex index of the markStyle
	 */
	public void setMarkStyle(int markStyleIndex) {
		drawer.setMarkStyle(markStyleIndex);
	}
	
	
	/**
	 * A single mothod to set every parameters
	 * @param background index of background color
	 * @param foreground index of foreground color
	 * @param markSizeUnit index of markSize unit
	 * @param markSize markSize in pixels or tabulated
	 * @param markStyleIndex index of mark style
	 */
	public void setMarkParameters(int background, int foreground, int markSizeUnit, int markSize, int markStyleIndex) {
		setBackground(background);
		setForeground(foreground);
		setMarkSizeUnit(markSizeUnit);
		setMarkSize(markSize);
		setMarkStyle(markStyleIndex);
	}

	/**
	 * Draw marks from already precomputed data
	 */
	public void redrawMarks() {
		GL gl = getGL();
		
		CoordinateTransformation transform = getCoordinateTransformation();
		
		// need to perform this befaore swithching to pixel coordinates
		Vector3D[] pixCoords = transform.getCanvasCoordinates(gl, markPos);
		
		// switch to pixel coordinates
		GLTools.usePixelCoordinates(gl, getParentFigureGL());
		
		// create the display list using pixel coordinates
		startRecordDL();
		// mark are drawn with a line width of 1.
		gl.glLineWidth(1.0f);
		for (int i = 0; i < markPos.length; i++) {
			// switch back to the new frame
			getDrawer().drawMark(pixCoords[i].getX(), pixCoords[i].getY(), pixCoords[i].getZ());
		}
		endRecordDL();
		// we recreate the dl each time
		//getDrawer().clearDisplayList();
		
		GLTools.endPixelCoordinates(gl, getParentFigureGL());
	}
	
	/**
	 * Show the compute ddisplay list
	 */
	public void showMarks() {
		GL gl = getGL();
		
		// switch to pixel coordinates
		GLTools.usePixelCoordinates(gl, getParentFigureGL());
		displayDL();
		
		GLTools.endPixelCoordinates(gl, getParentFigureGL());
	}
	
	/**
	 * Display the marks at the positions specified by marksPosition
	 * @param marksPosition positions in 3D of marks
	 */
	public void drawMarks(Vector3D[] marksPosition) {
		this.markPos = marksPosition;
		// mark object has chnaged, we mey need to recreate the display list
		getDrawer().createDisplayList();
		redrawMarks();
	}
	
	/**
	 * To be called when the cpp object is destroyed
	 * @param parentFigureIndex index of parent figure
	 */
	@Override
	public void destroy(int parentFigureIndex) {
		drawer.destroy(parentFigureIndex);
		super.destroy(parentFigureIndex);
	}
}
