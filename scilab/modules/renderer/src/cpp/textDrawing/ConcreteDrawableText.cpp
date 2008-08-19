/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing implementation of driver independent
 * algorithms
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "ConcreteDrawableText.hxx"
#include "DrawingBridge.h"

extern "C"
{
#include "GetProperty.h"
#include "SetProperty.h"
}

namespace sciGraphics
{

using namespace std;

/*---------------------------------------------------------------------------------*/
ConcreteDrawableText::ConcreteDrawableText(sciPointObj * pObj) : DrawableText(pObj)
{
  m_oDrawingBoxStrategies.clear();
  m_pDrawingTextStrategy = NULL;
}
/*---------------------------------------------------------------------------------*/
ConcreteDrawableText::~ConcreteDrawableText(void)
{
  removeBoxDrawingStrategies();
  setTextDrawingStrategy(NULL);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableText::getBoundingRectangle(double corner1[3], double corner2[3], double corner3[3], double corner4[3])
{
  m_pDrawingTextStrategy->getBoundingRectangle(corner1, corner2, corner3, corner4);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableText::getScreenBoundingBox(int corner1[2], int corner2[2], int corner3[2], int corner4[2])
{
  m_pDrawingTextStrategy->getScreenBoundingBox(corner1, corner2, corner3, corner4);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableText::addBoxDrawingStrategy(DrawTextBoxStrategy * strategy)
{
  m_oDrawingBoxStrategies.push_back(strategy);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableText::removeBoxDrawingStrategies(void)
{
  list<DrawTextBoxStrategy *>::iterator it = m_oDrawingBoxStrategies.begin();
  for( ; it != m_oDrawingBoxStrategies.end(); it++)
  {
    delete *it;
    *it = NULL;
  }
  m_oDrawingBoxStrategies.clear();
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableText::setTextDrawingStrategy(DrawTextContentStrategy * strategy)
{
  if (m_pDrawingTextStrategy != NULL)
  {
    delete m_pDrawingTextStrategy;
  }
  m_pDrawingTextStrategy = strategy;
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableText::drawBox(void)
{
  list<DrawTextBoxStrategy *>::iterator it = m_oDrawingBoxStrategies.begin();
  
  // get bounding rectangle corners
  double corners[4][3];
  sciGetTextBoundingBox(m_pDrawed, corners[0], corners[1], corners[2], corners[3]);
  //m_pDrawingTextStrategy->getBoundingRectangle(corners[0], corners[1], corners[2], corners[3]);

  for( ; it != m_oDrawingBoxStrategies.end(); it++)
  {
    (*it)->setBoxCorners(corners[0], corners[1], corners[2], corners[3]);
    (*it)->drawBox();
  }
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableText::drawTextContent(void)
{
  // draw text content and update bounding box
  sciText * ppText = pTEXT_FEATURE(m_pDrawed);
  m_pDrawingTextStrategy->drawTextContent(ppText->corners[0], ppText->corners[1],
                                          ppText->corners[2], ppText->corners[3]);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableText::redrawTextContent(void)
{
  // draw text content and update bounding box
  sciText * ppText = pTEXT_FEATURE(m_pDrawed);
  m_pDrawingTextStrategy->redrawTextContent(ppText->corners[0], ppText->corners[1],
                                            ppText->corners[2], ppText->corners[3]);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableText::showBox(void)
{
  list<DrawTextBoxStrategy *>::iterator it = m_oDrawingBoxStrategies.begin();

  // get bounding rectangle corners
  double corners[4][3];
  sciGetTextBoundingBox(m_pDrawed, corners[0], corners[1], corners[2], corners[3]);
  //m_pDrawingTextStrategy->getBoundingRectangle(corners[0], corners[1], corners[2], corners[3]);

  for( ; it != m_oDrawingBoxStrategies.end(); it++)
  {
    (*it)->setBoxCorners(corners[0], corners[1], corners[2], corners[3]);
    (*it)->showBox();
  }
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableText::showTextContent(void)
{
  m_pDrawingTextStrategy->showTextContent();
}
/*---------------------------------------------------------------------------------*/
bool ConcreteDrawableText::isTextEmpty(void)
{
  return sciisTextEmpty(m_pDrawed);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableText::updateTextBox(void)
{
  if (!m_bNeedRedraw && !m_bNeedDraw)
  {
    // text already up to date
    return;
  }
  // just update, no need to draw
  sciText * ppText = pTEXT_FEATURE(m_pDrawed);
  getBoundingRectangle(ppText->corners[0], ppText->corners[1],
                       ppText->corners[2], ppText->corners[3]);
  
}
/*---------------------------------------------------------------------------------*/
}
