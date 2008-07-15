/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the drawing routine for a subwin object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_SUBWIN_H_
#define _DRAWABLE_SUBWIN_H_

#include <list>

#include "../DrawableObject.h"
#include "Camera.h"
#include "DrawableSubwinBridge.h"

namespace sciGraphics
{

class DrawableSubwin : public DrawableObject
{

public:

  DrawableSubwin( sciPointObj * pObj ) ;

  virtual ~DrawableSubwin( void ) ;

  /**
   * Set/Get the current camera
   */
  Camera * getCamera( void ) { return m_pCamera; }

  /**
   * Set/Get the current camera
   */
  void setCamera( Camera * cam ) ;

  /**
   * Redefine has changed so children
   * so that needed children will also be redrawn
   */
  virtual void hasChanged( void ) ;

  /**
   * Display the subwin by just setting the camera
   * and its children pObj
   */
  void displaySingleObjs(std::list<sciPointObj *>& singleObjs);

  /**
   * Compute real data bounds from the user ones.
   * ie fills Frect.
   */
  virtual void computeRealDataBounds(void) = 0;

  /**
   * Just update scale and size if needed
   */
  virtual void updateScale(void) = 0;

  /**
   * Get the initial number of ticks (with no reduction) that will be drawn.
   */
  virtual int getNbXTicks(void) = 0;

  /**
   * Get the initial ticks positions and labels (with no reduction) that will be drawn.
   * To be used to know the positions and labels from outside the rendering process
   */
  virtual void getXTicksPos(double ticksPositions[], char ** ticksLabels) = 0;

  /**
   * Get the initial number of ticks (with no reduction) that will be drawn.
   */
  virtual int getNbYTicks(void) = 0;

  /**
   * Get the initial ticks positions and labels (with no reduction) that will be drawn.
   * To be used to know the positions and labels from outside the rendering process
   */
  virtual void getYTicksPos(double ticksPositions[], char ** ticksLabels) = 0;

  /**
   * Get the initial number of ticks (with no reduction) that will be drawn.
   */
  virtual int getNbZTicks(void) = 0;

  /**
   * Get the initial ticks positions and labels (with no reduction) that will be drawn.
   * To be used to know the positions and labels from outside the rendering process
   */
  virtual void getZTicksPos(double ticksPositions[], char ** ticksLabels) = 0;

  /**
   * Compute bounds of X axis and direction of ticks
   */
  virtual bool getXAxisPosition(double axisStart[3], double axisEnd[3], double ticksDirection[3]) = 0;

  /**
   * Compute bounds of Y axis and direction of ticks
   */
  virtual bool getYAxisPosition(double axisStart[3], double axisEnd[3], double ticksDirection[3]) = 0;

  /**
   * Compute bounds of Z axis and direction of ticks
   */
  virtual bool getZAxisPosition(double axisStart[3], double axisEnd[3], double ticksDirection[3]) = 0;


protected:

  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  virtual EDisplayStatus draw( void ) ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual EDisplayStatus show( void ) ;

  /**
   * Redraw the subwindow using precomputed data
   */
  virtual EDisplayStatus redraw(void) ;

  /**
   * Draw the bounding box and ticks of the subwin object.
   */
  void drawAxesBox(void);

  /**
   * Draw the bounding box and ticks of a subwin object with alredy computed data.
   */
  void showAxesBox(void);

  /**
   * Draw the surrounding cube of the subwin object.
   */
  virtual void drawBox(void) = 0;

  /**
   * Draw the ticks of the subwin object.
   */
  virtual void drawTicks(void) = 0;

  /**
   * Draw the surrounding cube of the subwin object from alraedy computed data.
   */
  virtual void showBox(void) = 0;

  /**
   * Draw the ticks of the subwin from alraedy computed data.
   */
  virtual void showTicks(void) = 0;

  /**
   * draw the subwin by just setting the camera
   * and its children pObj
   */
  virtual void drawSingleObjs(std::list<sciPointObj *>& singleObjs);

  /**
   * show the subwin by just setting the camera
   * and its children pObj
   */
  virtual void showSingleObjs(std::list<sciPointObj *>& singleObjs);

  /**
   * Find wether a list of objects contains a subwindow
   */
  bool containsSubwin(std::list<sciPointObj *>& pObjs);

  /**
   * Call display on the single objs
   */
  void printSingleObjs(std::list<sciPointObj *>& pObjs);

  /**
   * Return the real type of implementation object
   */
  DrawableSubwinBridge * getSubwinImp( void ) ;


  /*-----------------------------------------------------------------------------*/
  /** Camera used to visualize this axes */
  Camera * m_pCamera;
  /*-----------------------------------------------------------------------------*/


} ;

}

#endif /* _DRAWABLE_SUBWIN_H_  */
