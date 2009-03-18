/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * desc : Class containing the driver dependant routines to draw a 
 * figure object with JoGL 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_FIGURE_JOGL_H_
#define _DRAWABLE_FIGURE_JOGL_H_

#include "DrawableFigureBridge.h"
#include "DrawableFigure.h"
#include "DrawableFigureJavaMapper.hxx"
#include "DrawableObjectJoGL.h"

namespace sciGraphics
{

class DrawableFigureJoGL : public virtual DrawableFigureBridge, public DrawableObjectJoGL
{

public:

  DrawableFigureJoGL( DrawableFigure * drawer ) ;

  virtual ~DrawableFigureJoGL( void ) ;

  /**
  * Tell the canvas to draw itself.
  */
  virtual void drawCanvas( void ) ;

  /**
   * Create the rendering canvas for the figure if none exists.
   */
  virtual void createVisualFigure( int figureIndex ) ;

  /**
   * Close the rendering canvas
   */
  virtual void closeVisualFigure( void ) ;

  /**
   * Set the background color of the figure
   */
  virtual void setFigureParameters(void) ;

  /**
   * Draw the background of the figure
   */
  virtual void drawBackground(void);

  /**
   * Set a new colormap to this figure
   * @param nbColor x 3 matrix contains the three channels RGB of each color
   * @param nbColor number of color in the colormap
   */
  virtual void setColorMap( const double rgbMat[], int nbColor ) ;

  /**
   * Fill the rgbMat with the colors in the colormap
   * @param rgbMat should be a nbColor x 3 matrix with nbColor the number of color in the colormap
   */
  virtual void getColorMap( double rgbMat[] ) ;

  /**
   * Get the size (width, height) of the figure in parameter size.
   */
  virtual void getSize( int size[2] ) ;

  /**
   * Set the size (width, height) of the figure.
   * @return indicates if the size could be successfully modified
   */
  virtual int setSize( const int size[2] ) ;

  /**
   * Get the size of the window enclosing the figure.
   */
  virtual void getWindowSize( int size[2] ) ;

  /**
   * Set the size of the window enclosing the figure.
   */
  virtual void setWindowSize( const int size[2] ) ;

  /**
   * Get the size of the window enclosing the figure.
   */
  virtual void getWindowPosition( int pos[2] ) ;

  /**
   * Set the size of the window enclosing the figure.
   */
  virtual void setWindowPosition( const int pos[2] ) ;

  /**
   * Set the message displayed by the figure.
   */
  virtual void setInfoMessage( const char * message ) ;

	/**
	 * Get the info message of the figure.
	 * @param[out] message string filled with the message. Should have a minimum size
	 *                     given by getInfoMessageLength.
	 */
	virtual void getInfoMessage(char * message);

	/**
	 * @return size of the info message to be used in combination with getInfoMessage.
	 */
	virtual int getInfoMessageLength(void);

  /**
   * Set the auto_resize status (on or off).
   */
  virtual void setAutoResizeMode(bool onOrOff);

  /**
   * Get tje auto_resize status (on or off).
   */
  virtual bool getAutoResizeMode(void);

  /**
   * Get the viewport on the canvas
   */
  virtual void getViewport(int viewport[4]);

  /**
   * Set the viewport on the canvas
   */
  virtual void setViewport(const int viewport[4]);

  /**
   * Set the background of the figure
   */
  virtual void setBackgroundColor(int backColor);

  /**
   * Call rubber box on the figure
   * @param isClick specify whether the rubber box is selected by mouse click
   *        or a sequence of press/release.
   * @param intialRect if not null, specify the initial rectangle to use
   * @param[out] endRect array containing the coordinates of two opposite corners of
   *                     the rubber box
   * @param[out] usedButton Scilab code of the button used to terminate the rubber box
   */
  virtual void rubberBox(bool isClick, const int initialRect[4], int endRect[4], int * usedButton);

	/**
	 * Perfomr an interactive zoom of the figure using a rectangular selection.
	 */
	virtual void interactiveZoom(void);

  /**
   * Set the title of the figure.
   */
  virtual void setTitle( const char * title ) ;

	/**
	 * Perform an interactive rotation of a subwin contained in the figure.
	 */
	virtual void interactiveRotation(void);

  /**
   * Put the figure in top of other windows.
   */
  virtual void showWindow(void);

	/**
	 * Create a graphic canvas for the figure.
	 * Figure is then able to draw 3d plots.
	 */
	virtual void openGraphicCanvas(void);

	/**
	 * Remove the graphic canvas for the figure.
	 * Figure is no longer able to display 3D plots.
	 */
	virtual void closeGraphicCanvas(void);

	/**
	 * Force the window to be be single buffered
	 */
	virtual void setUseSingleBuffer(bool useSingleBuffer);

	/**
	 * @return the number of pass used for antialiasing or 0 if antialiasing is disable.
	 */
	int getAntialiasingQuality(void);
	
	/**
	 * Modify the quality of antialiasing or disable it.
	 * If quality if 0, the antialiasing is disables,
	 * otherwise it might be either 1, 2, 4, 8 or 16 and then
	 * specify the number of pass for antialiasing.
	 * @param quality positive integer.
	 */
	void setAntialiasingQuality(int quality);

  /**
   * Check wether we the environemebnt allows us to create a new window.
   */
  static bool isAbleToCreateFigure(void);

  /**
   * Return the drawn object
   */
  DrawableFigure * getFigureDrawer( void ) ;

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  DrawableFigureJavaMapper * getFigureJavaMapper(void);

  /**
   * Free the Java resources used by this object.
   * Override it with an empty function othwerwise canavs will be closed twice.
   */
  virtual void destroy( void ) ;

  /**
   * Interface with the java object generated by giws
   */
  //org_scilab_modules_gui_graphicWindow::ScilabGraphicWindow * m_pJavaWindow;

} ;

}

#endif /* _DRAWABLE_FIGURE_JOGL_H_  */

