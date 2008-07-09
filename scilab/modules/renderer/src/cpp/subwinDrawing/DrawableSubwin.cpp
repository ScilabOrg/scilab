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

#include "DrawableSubwin.h"
#include "DrawableSubwinBridge.h"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableSubwin::DrawableSubwin(sciPointObj * pObj) : DrawableObject(pObj)
{
  m_pCamera = NULL ;
}
/*---------------------------------------------------------------------------------*/
DrawableSubwin::~DrawableSubwin( void )
{
  if ( m_pCamera != NULL )
  {
    delete m_pCamera ;
    m_pCamera = NULL ;
  }
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::setCamera( Camera * cam )
{
  if (m_pCamera != NULL)
  {
    delete m_pCamera;
    m_pCamera = NULL;
  }
  m_pCamera = cam;
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::hasChanged( void )
{
  DrawableObject::hasChanged();
  parentSubwinChanged();
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::displaySingleObj(sciPointObj * pObj)
{

  if (sciGetEntityType(pObj) == SCI_SUBWIN)
  {
    if (m_bNeedRedraw)
    {
      draw();
    }
    else
    {
      show();
    }
  }
  // it is a child
  else if (m_bNeedRedraw)
  {
    drawSingleObj(pObj);
  }
  else
  {
    showSingleObj(pObj);
  }
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableSubwin::draw( void )
{
  initializeDrawing() ;

  // fill Frect with real data bounds
  computeRealDataBounds();

  // set up camera
  // so update coordinates transformations
  m_pCamera->draw();

  if ( !checkVisibility() )
  {
    // needed
    m_pCamera->replaceCamera();

    endDrawing();
    return UNCHANGED;
  }

  drawAxesBox();

  //drawBox();

  displayChildren() ;

  // draw ticks after otherwise there are some transparency issues
  //drawTicks();

  // needed
  m_pCamera->replaceCamera();

  endDrawing();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableSubwin::show( void )
{
  
  initializeDrawing() ;

  // set up camera
  // so update coordinates transformations
  m_pCamera->show();

  if ( !checkVisibility() )
  {
    // needed
    m_pCamera->replaceCamera();

    endDrawing();
    return UNCHANGED;
  }

  showAxesBox();
  //showBox();

  displayChildren() ;

  // draw ticks after otherwise there are some transparency issues
  //showTicks();

  // needed
  m_pCamera->replaceCamera();

  endDrawing();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus DrawableSubwin::redraw(void)
{
  initializeDrawing() ;

  // set up camera
  // so update coordinates transformations
  m_pCamera->redraw();

  if ( !checkVisibility() )
  {
    // needed
    m_pCamera->replaceCamera();

    endDrawing();
    return UNCHANGED;
  }

  drawAxesBox();
  //drawBox();

  displayChildren() ;

  // draw ticks after otherwise there are some transparency issues
  //drawTicks();

  // needed
  m_pCamera->replaceCamera();

  endDrawing();
  return SUCCESS;
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::drawAxesBox(void)
{
  drawBox();
  drawTicks();
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::showAxesBox(void)
{
  showBox();
  showTicks();
}
/*------------------------------------------------------------------------------------------*/
void DrawableSubwin::drawSingleObj(sciPointObj * pObj)
{

  initializeDrawing() ;

  // fill Frect with real data bounds
  computeRealDataBounds();

  // set up camera
  m_pCamera->draw();

  if ( !checkVisibility() )
  {
    // needed
    m_pCamera->replaceCamera();

    endDrawing();
    return;
  }

  // camera has been set
  // display only the children
  getHandleDrawer(pObj)->display();

  // needed
  m_pCamera->replaceCamera();

  endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void DrawableSubwin::showSingleObj(sciPointObj * pObj)
{
  initializeDrawing() ;

  // fill Frect with real data bounds
  computeRealDataBounds();

  // set up camera
  m_pCamera->show();

  if ( !checkVisibility() )
  {
    // needed
    m_pCamera->replaceCamera();

    endDrawing();
    return;
  }

  // camera has been set
  // display only the children
  getHandleDrawer(pObj)->display();

  // needed
  m_pCamera->replaceCamera();

  endDrawing();
}
/*---------------------------------------------------------------------------------*/
DrawableSubwinBridge * DrawableSubwin::getSubwinImp( void )
{
  return dynamic_cast<DrawableSubwinBridge *>(m_pImp) ;
}
/*---------------------------------------------------------------------------------*/

}
