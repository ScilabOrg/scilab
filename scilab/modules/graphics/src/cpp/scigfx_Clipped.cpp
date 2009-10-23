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
/* file: scigfx_Clipped.cpp                                                   */
/* desc : implement methodes of the class Clipped                             */
/*  //TODO                                                                    */
/*----------------------------------------------------------------------------*/


#include "scigfx_Clipped.h"

  // Include nécessaire au setClippingRegion(sciGetClipping(sciGetParentSubwin(pObj)))
  // TODO a enlever ?
/*----------------------------------------------------------------------------*/
#include "HandleManagement.h"
#include "GetProperty.h"

  // Include nécessaire pour sciPointObj
/*----------------------------------------------------------------------------*/
#include "ObjectStructure.h"

  // Include nécessaire au CastObject
/*----------------------------------------------------------------------------*/
#include "scigfx_CastObject.h"


extern "C"
{
  // Include nécessaire accés a la jvm
/*----------------------------------------------------------------------------*/
  #include "getScilabJavaVM.h"
}


    // C function about Clipped
/* -------------------------------------------------------------------------- */
extern "C"
{
  void SetClippingRegion(sciPointObj * pobj, double pclip[4])
  {
      CastObject<Clipped*>(pobj)->setClippingRegion(pclip);
  }
  void UnsetClippingRegion(sciPointObj * pobj)
  {
      CastObject<Clipped*>(pobj)->unsetClippingRegion();
  }
  bool GetClippingRegionSet(sciPointObj * pobj)
  {
      return  CastObject<Clipped*>(pobj)->getClippingRegionSet();
  }
  void GetClippingRegion(sciPointObj * pobj, double pclip[])
  {
      CastObject<Clipped*>(pobj)->getClippingRegionFromC(pclip);
  }
  void SetClippingMethode(sciPointObj * pobj, int cm)
  {
      CastObject<Clipped*>(pobj)->setClippingMethodeFromC(cm);
  }
  int  GetClippingMethode(sciPointObj * pobj)
  {
      return  CastObject<Clipped*>(pobj)->getClippingMethodeFromC();
  }
}
/* -------------------------------------------------------------------------- */

    // "Clipped" methods implementation
/* -------------------------------------------------------------------------- */

  // Implement PropertyContainer::getPropertyByName
PropertyContainer::GetPropertyStatus Clipped::getPropertyByName(const std::string& propertyName)
{
    if(propertyName.compare("clip_state")==0) return aCStringForScilab(getClippingMethodeCString());

// TODO
//    if(propertyName.compare("clip_box")==0) return aMatrixOfDoubleForScilab(clippingRegion, 2, 2);

    return PROPERTY_NOT_FOUND;
}


void Clipped::setScaledClippingRegion()
{
  switch(clippingMethode)
  {
  case AXES_BOUNDS:
    sciGetRealDataBounds(sciGetParentSubwin(m_pDrawed), scaledClippingRegion);
  break;
  case OWN_BOUNDS:
    sciGraphics::DrawableObject::pointScale(clippingRegion[0], clippingRegion[1] - clippingRegion[3], 0.0,
                                            &(scaledClippingRegion[0]), &(scaledClippingRegion[2]), &(scaledClippingRegion[4]));
    sciGraphics::DrawableObject::pointScale(clippingRegion[0] + clippingRegion[2], clippingRegion[1], 0.0,
                                            &(scaledClippingRegion[1]), &(scaledClippingRegion[3]), &(scaledClippingRegion[5]));
  break;
  case NO_BOUNDS:
  break;
  }
}

void Clipped::clip()
{
  setScaledClippingRegion();
  switch(clippingMethode)
  {
  case AXES_BOUNDS:
    clipper->initializeDrawing(sciGetNum(sciGetParentFigure(m_pDrawed)));
    clipper->clipX(scaledClippingRegion[0],scaledClippingRegion[1]);
    clipper->clipY(scaledClippingRegion[2],scaledClippingRegion[3]);
    clipper->clipZ(scaledClippingRegion[4],scaledClippingRegion[5]);
  break;
  case OWN_BOUNDS:
    clipper->initializeDrawing(sciGetNum(sciGetParentFigure(m_pDrawed)));
    clipper->clipX(scaledClippingRegion[0],scaledClippingRegion[1]);
    clipper->clipY(scaledClippingRegion[2],scaledClippingRegion[3]);
  break;
  case NO_BOUNDS:
  break;
  }
}


void Clipped::unClip()
{
  if(clippingMethode != NO_BOUNDS)
  {
    clipper->endDrawing();
    clipper->unClip();
  }
}

void Clipped::initializeDrawing()
{
  std::cout << "Clipped : initializeDrawing" << std::endl;
  clipper->initializeDrawing(sciGetNum(sciGetParentFigure(m_pDrawed)));
}

void Clipped::endDrawing()
{
  std::cout << "Clipped : endDrawing" << std::endl;
  clipper->endDrawing();
}


Clipped::Clipped(sciPointObj *pObj):
  sciGraphics::DrawableObject(pObj),
  clippingMethode(AXES_BOUNDS),
  isClippingRegionSet(false)
{
  //TODO : clipper en static
  clipper = new org_scilab_modules_renderer_rectangleDrawing::DrawableRectangleGL(getScilabJavaVM());
  //TODO : use c++ call when hierarchy was in cpp
  setClippingMethodeFromC(sciGetIsClipping(sciGetParentSubwin(pObj)));
  setClippingRegion(sciGetClipping(sciGetParentSubwin(pObj)));
}

Clipped::~Clipped()
{
  //TODO : clipper en static
  delete(clipper);
}


void Clipped::setClippingRegion(double pclip[4])
{
  clippingRegion[0]=pclip[0];
  clippingRegion[1]=pclip[1];
  clippingRegion[2]=pclip[2];
  clippingRegion[3]=pclip[3];
  isClippingRegionSet = true;
}

void Clipped::unsetClippingRegion()
{
  isClippingRegionSet = false;
}

bool Clipped::getClippingRegionSet()
{
  return isClippingRegionSet;
}

void Clipped::getClippingRegionFromC(double pclip[]) // for compabiliti// TODO : faire disparaitre !
{
  pclip[0]=clippingRegion[0];
  pclip[1]=clippingRegion[1];
  pclip[2]=clippingRegion[2];
  pclip[3]=clippingRegion[3];
}

void Clipped::setClippingMethode(ClippingMethode cm)
{
  clippingMethode = cm;
}

void Clipped::setClippingMethodeFromC(int cm)// for compabiliti// TODO : faire disparaitre !
{
  if(cm==0) clippingMethode=AXES_BOUNDS;
  else if(cm==1) clippingMethode=OWN_BOUNDS;
  else clippingMethode=NO_BOUNDS;
}

Clipped::ClippingMethode Clipped::getClippingMethode()
{
  return clippingMethode;
}

// TODO essayer la surcharge
char * Clipped::getClippingMethodeCString()
{
  switch(clippingMethode)
  {
    case AXES_BOUNDS:
      return "clipgrf";
      break;
    case OWN_BOUNDS:
      return "on";
      break;
    case NO_BOUNDS:
      return "off";
      break;
  }
  return "off";
}

int Clipped::getClippingMethodeFromC()// for compabiliti// TODO : faire disparaitre !
{
  if(clippingMethode==AXES_BOUNDS) return 0;
  else if(clippingMethode==OWN_BOUNDS) return 1;
  return -1;
}
