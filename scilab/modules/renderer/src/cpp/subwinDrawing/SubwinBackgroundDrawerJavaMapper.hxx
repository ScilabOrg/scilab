/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _SUBWIN_BACKGROUND_DRAWER_JAVA_MAPPER_HXX_
#define _SUBWIN_BACKGROUND_DRAWER_JAVA_MAPPER_HXX_

#include "../DrawableObjectJavaMapper.hxx"
#include "SubwinBackgroundDrawerGL.hxx"


namespace sciGraphics
{

/**
 * Contains mapping of java method used by BackTrihedronDrawerJoGL 
 */
class SubwinBackgroundDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  SubwinBackgroundDrawerJavaMapper(void);

  virtual ~SubwinBackgroundDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for back trihedron
  virtual void setBoxParameters(int backgroundColor);
  virtual void drawBox(double xMin, double xMax, double yMin,
                       double yMax, double zMin, double zMax,
                       int concealedCornerIndex);
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper
   */
  org_scilab_modules_renderer_subwinDrawing::SubwinBackgroundDrawerGL * m_pJavaObject;

};

}

#endif /* _SUBWIN_BACKGROUND_DRAWER_JAVA_MAPPER_HXX_ */

