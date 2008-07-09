/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.canvas;

import javax.media.opengl.GL;

import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Interface for SimpleCanvas the associated object to Scilab GUIs Canvas
 * @author Marouane BEN JELLOUL
 */
public interface SimpleCanvas {

	/**
	 * Gets the size of an Canvas (width and height)
	 * @return the size of the Canvas
	 */
	Size getDims();

	/**
	 * Sets the size of an Canvas (width and height)
	 * @param newSize the size we want to set to the Canvas
	 */
	void setDims(Size newSize);

	/**
	 * Gets the position of an Canvas (X-coordinate and Y-corrdinate)
	 * @return the position of the Canvas
	 */
	Position getPosition();

	/**
	 * Sets the position of an Canvas (X-coordinate and Y-corrdinate)
	 * @param newPosition the position we want to set to the Canvas
	 */
	void setPosition(Position newPosition);

	/**
	 * Gets the visibility status of an Canvas
	 * @return the visibility status of the Canvas (true if the Canvas is visible, false if not)
	 */
	boolean isVisible();

	/**
	 * Sets the visibility status of an Canvas
	 * @param newVisibleState the visibility status we want to set for the Canvas
	 * 			(true if the Canvas is visible, false if not)
	 */
	void setVisible(boolean newVisibleState);

	/**
	 * Draws an Canvas
	 */
	void draw();
	
	/**
	 * Force the canvas to render itself immediately.
	 */
	void display();
	 
	/**
	 * Get the GL pipeline used by the canvas
	 * @return GL pipeline
	 */
	GL getGL();
	 
	/**
	 * @param onOrOff set wether the swap buffer mode is on or off.
	 */
	void setAutoSwapBufferMode(boolean onOrOff);
	
	/**
	 * @return set wether the swap buffer mode is on or off.
	 */
	boolean getAutoSwapBufferMode();
	 
	/**
	 * Force the canvas to render itself with synchronisation with its OpenGL process.
	 */
	void repaint();
	
	/**
	 * Specify wether the canvas should fit the parent tab size
	 * (and consequently the scrollpane size) or not
	 * @param onOrOff true to enable autoresize mode
	 */
	void setAutoResizeMode(boolean onOrOff);
	
	/**
	 * @return wether the resize mode is on or off
	 */
	boolean getAutoResizeMode();
	
	/**
	 * Get the part of the canvas which is currently viewed
	 * @return [x,y,w,h] array
	 */
	int[] getViewingRegion();
	
	/**
	 * Specify a new viewport for the canvas
	 * For SwingScilabCanvas viewport can not be modified
	 * since it match the parent tab size
	 * @param posX X coordinate of upper left point of the viewport within the canvas
	 * @param posY Y coordinate of upper left point of the viewport within the canvas
	 * @param width width of the viewport
	 * @param height height of the viewport
	 */
	void setViewingRegion(int posX, int posY, int width, int height);
	
	/**
	 * Set the background of the Canvas.
	 * @param red red channel
	 * @param green green channel 
	 * @param blue blue channel
	 */
	void setBackgroundColor(double red, double green, double blue);
	
	 /**
	  * Set the event handler of the Canvas
	  * @param command the name of the Scilab function to call
	  */
	 void setEventHandler(String command);

	 /**
	  * Set the status of the event handler of the Canvas
	  * @param status is true to set the event handler active
	  */
	 void setEventHandlerEnabled(boolean status);
	 
	 /**
	  * Create an interactive selection rectangle and return its pixel coordinates
	  * @param isClick specify wether the rubber box is selected by one click for each one of the two edge
	  *                or a sequence of press-release
	  * @param isZoom specify if the rubber box is used for a zoom and then change the mouse cursor.
	  * @param initialRect if not null specify the initial rectangle to draw
	  * @param endRect array [x1,y1,x2,y2] containing the result of rubberbox
	  * @return Scilab code of the pressed button
	  */
	 int rubberBox(boolean isClick, boolean isZoom, int[] initialRect, int[] endRect);
	 
	 /**
	  * Get the displacement in pixel that should be used for rotating axes
	  * @param displacement out parameter, [x,y] array of displacement in pixels
	  * @return true if the diplacement recording continue, false otherwise
	  */
	 boolean getRotationDisplacement(int[] displacement);

	 /**
	  * Ansynchrnous stop of rotation tracking.
	  */
	 void stopRotationRecording();
	 
	 /**
	  * Disable the canvas befor closing
	  */
	void close();
}
