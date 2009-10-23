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
/* file: scigfx_GraphicContext.h                                              */
/* desc : declare the class Clipped                                           */
/*  //TODO                                                                    */
/*----------------------------------------------------------------------------*/

#ifndef _SCI_GRAPHICCONTEXT_H_
#define _SCI_GRAPHICCONTEXT_H_

  // Include nécessaire pour sciPointObj
/*----------------------------------------------------------------------------*/
#include "ObjectStructure.h"

  // Include nécessaire pour l'héritage
/* -------------------------------------------------------------------------- */
#include "scigfx_PropertyContainer.h"
#include "scigfx_ColorMapUser.h"


// C function about GraphicContext
/* -------------------------------------------------------------------------- */
extern "C"
{
  int  GetForegroundColor(sciPointObj * pObj);
  int  GetBackgroundColor(sciPointObj * pObj);
  int  GetMarkForegroundColor(sciPointObj * pObj);
  int  GetMarkBackgroundColor(sciPointObj * pObj);
  void GetLineWidth(sciPointObj * pObj, double * lw);
  BOOL GetIsFilled(sciPointObj * pObj);
  int  GetFillColor(sciPointObj * pObj);
  BOOL GetIsLined(sciPointObj * pObj);
  int  GetLineStyle(sciPointObj * pObj);
  BOOL GetIsMarked(sciPointObj * pObj);
  int  GetMarkStyle(sciPointObj * pObj);
  int  GetMarkSize(sciPointObj * pObj);
  int  GetMarkSizeUnit(sciPointObj * pObj);
  int  GetMarkBackgroundColor(sciPointObj * pObj);
  int  GetMarkForegroundColor(sciPointObj * pObj);

  void SetForegroundColor(sciPointObj * pObj, int color);
  void SetBackgroundColor(sciPointObj * pObj, int color);
  void SetMarkForegroundColor(sciPointObj * pObj, int color);
  void SetMarkBackgroundColor(sciPointObj * pObj, int color);
  void SetIsFilled(sciPointObj * pObj, BOOL filled);
  void SetFillColor(sciPointObj * pObj, int color);
  void SetIsLined(sciPointObj * pObj, BOOL lined);
  void SetLineStyle(sciPointObj * pObj, int style);
  void SetLineWidth(sciPointObj * pObj, double width);
  void SetIsMarked(sciPointObj * pObj, BOOL marked);
  void SetMarkStyle(sciPointObj * pObj, int style);
  void SetMarkSize(sciPointObj * pObj, int size);
  void SetMarkSizeUnit(sciPointObj * pObj, int sizeunit);
  void SetMarkBackgroundColor(sciPointObj * pObj, int color);
  void SetMarkForegroundColor(sciPointObj * pObj, int color);
}
/* -------------------------------------------------------------------------- */





class GraphicContext:
  public ColorMapUser,
  virtual public PropertyContainer
{
// TODO diviser cette classe ??
private:
  int backgroundColor;      
  int foregroundColor;

  bool isFilled;
  int fillColor;

  bool isLined;
  int lineStyle;
  double lineWidth;

  bool isMarked;
  int markStyle;
  int markSize;

  int markSizeUnit;


  int markBackgroundColor;
  int markForegroundColor;

public:
  GraphicContext(sciPointObj * pObj);

  void setGraphicContext(sciGraphicContext * sgc);

  void setDefaultValues();

  void    setBackgroundColor(int bgc);
  void    setBackgroundSciColor(int bgc);
  int     getBackgroundColor();
  void    setForegroundColor(int fgc);
  void    setForegroundSciColor(int fgc);
  int     getForegroundColor();

  void    setIsFilled(bool filled);
  bool    getIsFilled();
  void    setFillColor(int fc);
  int     getFillColor();

  void    setIsLined(bool lined);
  bool    getIsLined();
  void    setLineStyle(int ls);
  int     getLineStyle();
  void    setLineWidth(double lw);
  double  getLineWidth();

  void    setIsMarked(bool marked);
  bool    getIsMarked();
  void    setMarkStyle(int ms);
  int     getMarkStyle();
  void    setMarkSize(int ms);
  int     getMarkSize();
  void    setMarkSizeUnit(int msu);
  int     getMarkSizeUnit();
  char*   getMarkSizeUnitCString();
  void    setMarkBackgroundColor(int mb);
  int     getMarkBackgroundColor();
  void    setMarkForegroundColor(int mf);
  int     getMarkForegroundColor();

protected:
  // Implement PropertyContainer::getPropertyByName
  GetPropertyStatus getPropertyByName(const std::string& propertyName);
};


#endif //_SCI_GRAPHICCONTEXT_H_
