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
/* file: scigfx_Clipped.h                                                     */
/* desc : declare the class Clipped                                           */
/*  //TODO                                                                    */
/*----------------------------------------------------------------------------*/

#ifndef _SCIGFX_CLIPPED_H_
#define _SCIGFX_CLIPPED_H_

  // Include nécessaire pour PropertyContainer
/*----------------------------------------------------------------------------*/
#include "scigfx_PropertyContainer.h"

  // Include nécessaire pour sciGraphics::DrawableObject
/*----------------------------------------------------------------------------*/
#include "DrawableObject.h"

  // Include nécessaire pour org_scilab_modules_renderer_rectangleDrawing
  // TODO : a changer
/*----------------------------------------------------------------------------*/
#include "DrawableRectangleGL.hxx"

  // C function about Clipped
/* -------------------------------------------------------------------------- */
extern "C"
{
  void SetClippingRegion(sciPointObj * pObj, double pclip[4]);
  void UnsetClippingRegion(sciPointObj * pObj);
  bool GetClippingRegionSet(sciPointObj * pObj);
  void GetClippingRegion(sciPointObj * pObj, double pclip[]);

  void SetClippingMethode(sciPointObj * pObj, int cm);
  int  GetClippingMethode(sciPointObj * pObj);
}
/* -------------------------------------------------------------------------- */
// class Clipped
class Clipped:
  virtual public sciGraphics::DrawableObject,
  virtual public PropertyContainer
{
public:
  enum ClippingMethode {AXES_BOUNDS, OWN_BOUNDS, NO_BOUNDS};
private:


  //TODO passer en static. et utiliser un vrai clipper
  org_scilab_modules_renderer_rectangleDrawing::DrawableRectangleGL* clipper;
  ClippingMethode clippingMethode;
  double clippingRegion[4];         //TODO : passer à 3*2
  double scaledClippingRegion[6];
  bool isClippingRegionSet;

  void setScaledClippingRegion();
protected:
  // Implement PropertyContainer::getPropertyByName
  GetPropertyStatus getPropertyByName(const std::string& propertyName);

  void clip();
  void unClip();

  void initializeDrawing();
  void endDrawing();

public:
  Clipped(sciPointObj *pObj);
  ~Clipped();

  void setClippingRegion(double pclip[4]);
  void unsetClippingRegion();
  bool getClippingRegionSet();
  void getClippingRegionFromC(double pclip[]); // for compabiliti// TODO : faire disparaitre !

  void setClippingMethode(ClippingMethode cm);
  void setClippingMethodeFromC(int cm); // for compabiliti// TODO : faire disparaitre !

  ClippingMethode getClippingMethode();
  char * getClippingMethodeCString();

  int getClippingMethodeFromC(); // for compabiliti// TODO : faire disparaitre !

};

#endif //_SCIGFX_CLIPPED_H_
