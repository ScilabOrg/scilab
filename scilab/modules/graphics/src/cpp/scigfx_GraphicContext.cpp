/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*----------------------------------------------------------------------------*/
/* file: scigfx_GraphicContext.cpp                                            */
/* desc : implement the methods of "GraphicContext"                           */
/*  //TODO                                                                    */
/*----------------------------------------------------------------------------*/

#include "scigfx_GraphicContext.h"

  // Include nécessaire au CastObject
/*----------------------------------------------------------------------------*/
#include "scigfx_CastObject.h"

// C function about GraphicContext
/* -------------------------------------------------------------------------- */
extern "C"
{
  int GetForegroundColor(sciPointObj * pobj)
  {
    return CastObject<GraphicContext*>(pobj)->getForegroundColor();
  }
  int GetBackgroundColor(sciPointObj * pobj)
  {
    return CastObject<GraphicContext*>(pobj)->getBackgroundColor();
  }
  void GetLineWidth(sciPointObj * pobj, double * lw)
  {
    *lw=CastObject<GraphicContext*>(pobj)->getLineWidth();
  }
  BOOL GetIsFilled(sciPointObj * pobj)
  {
    return CastObject<GraphicContext*>(pobj)->getIsFilled()?TRUE:FALSE;
  }
  int GetFillColor(sciPointObj * pobj)
  {
    return CastObject<GraphicContext*>(pobj)->getFillColor();
  }
  BOOL GetIsLined(sciPointObj * pobj)
  {
    return CastObject<GraphicContext*>(pobj)->getIsLined()?TRUE:FALSE;
  }
  int GetLineStyle(sciPointObj * pobj)
  {
    return CastObject<GraphicContext*>(pobj)->getLineStyle();
  }
  BOOL GetIsMarked(sciPointObj * pobj)
  {
    return CastObject<GraphicContext*>(pobj)->getIsMarked()?TRUE:FALSE;
  }
  int GetMarkStyle(sciPointObj * pobj)
  {
    return CastObject<GraphicContext*>(pobj)->getMarkStyle();
  }
  int GetMarkSize(sciPointObj * pobj)
  {
    return CastObject<GraphicContext*>(pobj)->getMarkSize();
  }
  int GetMarkSizeUnit(sciPointObj * pobj)
  {
    return CastObject<GraphicContext*>(pobj)->getMarkSizeUnit();
  }
  int GetMarkBackgroundColor(sciPointObj * pobj)
  {
    return CastObject<GraphicContext*>(pobj)->getMarkBackgroundColor();
  }
  int GetMarkForegroundColor(sciPointObj * pobj)
  {
    return CastObject<GraphicContext*>(pobj)->getMarkForegroundColor();
  }

  void SetForegroundColor(sciPointObj * pobj, int color)
  {
    CastObject<GraphicContext*>(pobj)->setForegroundColor(color);
  }
  void SetBackgroundColor(sciPointObj * pobj, int color)
  {
    CastObject<GraphicContext*>(pobj)->setBackgroundColor(color);
  }
  void SetLineWidth(sciPointObj * pobj, double width)
  {
    CastObject<GraphicContext*>(pobj)->setLineWidth(width);
  }
  void SetIsFilled(sciPointObj * pobj, BOOL filled)
  {
    CastObject<GraphicContext*>(pobj)->setIsFilled(filled!=0);
  }
  void SetFillColor(sciPointObj * pobj, int color)
  {
    CastObject<GraphicContext*>(pobj)->setFillColor(color);
  }
  void SetIsLined(sciPointObj * pobj, BOOL lined)
  {
    CastObject<GraphicContext*>(pobj)->setIsLined(lined!=0);
  }
  void SetLineStyle(sciPointObj * pobj, int style)
  {
    CastObject<GraphicContext*>(pobj)->setLineStyle(style);
  }
  void SetIsMarked(sciPointObj * pobj, BOOL marked)
  {
    CastObject<GraphicContext*>(pobj)->setIsMarked(marked!=0);
  }
  void SetMarkStyle(sciPointObj * pobj, int style)
  {
    CastObject<GraphicContext*>(pobj)->setMarkStyle(style);
  }
  void SetMarkSize(sciPointObj * pobj, int size)
  {
    CastObject<GraphicContext*>(pobj)->setMarkSize(size);
  }
  void SetMarkSizeUnit(sciPointObj * pobj, int sizeunit)
  {
    CastObject<GraphicContext*>(pobj)->setMarkSizeUnit(sizeunit);
  }
  void SetMarkBackgroundColor(sciPointObj * pobj, int color)
  {
    CastObject<GraphicContext*>(pobj)->setMarkBackgroundColor(color);
  }
  void SetMarkForegroundColor(sciPointObj * pobj, int color)
  {
    CastObject<GraphicContext*>(pobj)->setMarkForegroundColor(color);
  }
}
/* -------------------------------------------------------------------------- */



    // "GraphicContext" methods implementation
/* -------------------------------------------------------------------------- */
GraphicContext::GraphicContext(sciPointObj * pObj):
  sciGraphics::DrawableObject(pObj),
  ColorMapUser(pObj),
  backgroundColor(0),   foregroundColor(0),
  isFilled(false),      fillColor(0),
  isLined(false),  lineStyle(0),
  lineWidth(0),    isMarked(false),
  markStyle(0),    markSize(0),
  markSizeUnit(0),    markBackgroundColor(0),
  markForegroundColor(0)
{
}

void GraphicContext::setGraphicContext(sciGraphicContext * sgc)
{
  if(sgc!=NULL)
  {
    backgroundColor     = sgc->backgroundcolor;
    foregroundColor     = sgc->foregroundcolor;
    isFilled            = sgc->isfilled!=0;
    fillColor           = sgc->fillcolor;
    isLined             = sgc->isline!=0;
    lineStyle           = sgc->linestyle;
    lineWidth           = sgc->linewidth;
    isMarked            = sgc->ismark!=0;
    markStyle           = sgc->markstyle;
    markSize            = sgc->marksize;
    markSizeUnit        = sgc->marksizeunit;
    markBackgroundColor = sgc->markbackground;
    markForegroundColor = sgc->markforeground;
  }
}

void GraphicContext::setDefaultValues()
{
  backgroundColor=0;
  foregroundColor=0;
  isFilled=false;
  fillColor=0;
  isLined=false;
  lineStyle=0;
  lineWidth=0;
  isMarked=false;
  markStyle=0;
  markSize=0;
  markSizeUnit=0;
  markBackgroundColor=0;
  markForegroundColor=0;
}

void GraphicContext::setBackgroundColor(int bgc){backgroundColor=bgc;}
void GraphicContext::setBackgroundSciColor(int bgc){backgroundColor=sci2cppColorIndex(bgc);}
int  GraphicContext::getBackgroundColor(){return backgroundColor;}
void GraphicContext::setForegroundColor(int fgc){foregroundColor=fgc;}
void GraphicContext::setForegroundSciColor(int fgc){foregroundColor=sci2cppColorIndex(fgc);}
int  GraphicContext::getForegroundColor(){return foregroundColor;}

void GraphicContext::setIsFilled(bool filled){isFilled=filled;}
bool GraphicContext::getIsFilled(){return isFilled;}
void GraphicContext::setFillColor(int fc){fillColor=fc;}
int  GraphicContext::getFillColor(){return fillColor;}

void GraphicContext::setIsLined(bool lined){isLined=lined;}
bool GraphicContext::getIsLined(){return isLined;}
void GraphicContext::setLineStyle(int ls){lineStyle=ls;}
int  GraphicContext::getLineStyle(){return lineStyle;}
void GraphicContext::setLineWidth(double lw){lineWidth=lw;}
double GraphicContext::getLineWidth(){return lineWidth;}

void GraphicContext::setIsMarked(bool marked){isMarked=marked;}
bool GraphicContext::getIsMarked(){return isMarked;}
void GraphicContext::setMarkStyle(int ms){markStyle=ms;}
int  GraphicContext::getMarkStyle(){return markStyle;}
void GraphicContext::setMarkSize(int ms){markSize=ms;}
int  GraphicContext::getMarkSize(){return markSize;}
void GraphicContext::setMarkSizeUnit(int msu){markSizeUnit=msu;}
int  GraphicContext::getMarkSizeUnit(){return markSizeUnit;}
char* GraphicContext::getMarkSizeUnitCString()
{// TODO an EnumString
  if(markSizeUnit == 2) return "tabulated";
  return "point";
}
void GraphicContext::setMarkBackgroundColor(int mb){markBackgroundColor=mb;}
int  GraphicContext::getMarkBackgroundColor(){return markBackgroundColor;}
void GraphicContext::setMarkForegroundColor(int mf){markForegroundColor=mf;}
int  GraphicContext::getMarkForegroundColor(){return markForegroundColor;}


  // Implement PropertyContainer::getPropertyByName
PropertyContainer::GetPropertyStatus GraphicContext::getPropertyByName(const std::string& propertyName)
{
    if(propertyName.compare("background")==0) return aDoubleForScilab((double)backgroundColor);
    if(propertyName.compare("foreground")==0) return aDoubleForScilab((double)foregroundColor);

    if(propertyName.compare("mark_mode")==0) return aBooleanStringForScilab(isMarked);
    if(propertyName.compare("mark_style")==0) return aDoubleForScilab((double)markStyle);
    if(propertyName.compare("mark_size_unit")==0) return aCStringForScilab(getMarkSizeUnitCString()); // TODO an EnumString
    if(propertyName.compare("mark_size")==0) return aDoubleForScilab((double)markSize);
    if(propertyName.compare("mark_background")==0) return aDoubleForScilab((double)markBackgroundColor);
    if(propertyName.compare("mark_foreground")==0) return aDoubleForScilab((double)markForegroundColor);

    if(propertyName.compare("line_mode")==0) return aBooleanStringForScilab(isLined);
    if(propertyName.compare("line_style")==0) return aDoubleForScilab((double)lineStyle);
    if(propertyName.compare("thickness")==0) return aDoubleForScilab(lineWidth);

    if(propertyName.compare("fill_mode")==0) return aBooleanStringForScilab(isFilled);

// TODO other

    return PROPERTY_NOT_FOUND;
}

