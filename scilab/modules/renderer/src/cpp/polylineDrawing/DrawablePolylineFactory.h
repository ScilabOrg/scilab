/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Factory for polyline objects. Automatically create a polyline 
 * drawer with the right algorithms from the graphic handle
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_POLYLINE_FACTORY_H_
#define _DRAWABLE_POLYLINE_FACTORY_H_


#include "DrawableObjectFactory.h"
#include "ConcreteDrawablePolyline.hxx"

namespace sciGraphics
{

class DrawablePolylineFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  virtual DrawableObject * create( void ) ;

  /**
   * Update the drawer of the drawed object.
   * To be used when an object was deeply modified and to avoid to create it again completely.
   */
  virtual void update( void ) ;

protected:

  /**
   * Create the drawing strategies.
   * @param polyline Object on which to create the strategies
   */
  void setStrategies( ConcreteDrawablePolyline * polyline ) ;

} ;

}

#endif /* _DRAWABLE_POLYLINE_FACTORY_H_  */
