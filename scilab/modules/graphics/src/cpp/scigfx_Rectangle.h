#ifndef _SCI_RECTANGLE_H_
#define _SCI_RECTANGLE_H_

// TODO : ajouter destructeur virtuel au classes heritable.

#include <iostream>
#include <string>
#include <cstring>
#include "getHandleDrawer.h"    //pour DrawableObjectWrapper
#include "DrawableObject.h"
#include "RectangleLineDrawerGL.hxx"
#include "RectangleMarkDrawerGL.hxx"
#include "RectangleFillDrawerGL.hxx"
#include "DrawableClippedObjectJavaMapper.hxx"
#include "DrawableRectangleGL.hxx"

#include "GetProperty.h"


  // Include nécessaire a GetParent
  // Todo enlever et créé une classe hierarchi
/*----------------------------------------------------------------------------*/
#include "HandleManagement.h"

  // Include nécessaire a la création
/*----------------------------------------------------------------------------*/
#include "scigfx_API.h"

  // Include nécessaire a l'héritage
/*----------------------------------------------------------------------------*/
#include "scigfx_CallBackable.h"
#include "scigfx_UserDataContainer.h"
#include "scigfx_GraphicContext.h"
#include "scigfx_Clipped.h"
#include "scigfx_Visible.h"
#include "scigfx_DataContainer.h"
#include "scigfx_Movable.h"
#include "scigfx_PropertyContainer.h"

extern "C"
{
  void  BuildRectangle(
        sciPointObj * pObj,
        double x, double y,
		    double height, double width,
        int *foreground, int *background,
		    int isfilled, int isline
        );
}

/******************************************************************************/
// class Rectangle

class Rectangle:
  public ColorMapUser,
  public CallBackable,
  public UserDataContainer,
  public GraphicContext,
  public Clipped,
  public Visible,
  public DataContainer,
  public Movable,
  virtual public PropertyContainer
{
protected:
/*
  org_scilab_modules_renderer_rectangleDrawing::RectangleLineDrawerGL* lineDrawer;
  org_scilab_modules_renderer_rectangleDrawing::RectangleMarkDrawerGL* markDrawer;
  org_scilab_modules_renderer_rectangleDrawing::RectangleFillDrawerGL* fillDrawer;
*/
  PropertyContainer::GetPropertyStatus getPropertyByName(const std::string& propertyName)
  {
//    CallBackable::getPropertyByName(propertyName);
    if(PROPERTY_RETURNED == UserDataContainer::getPropertyByName(propertyName)) return PropertyContainer::PROPERTY_RETURNED;
    if(PROPERTY_RETURNED == GraphicContext::getPropertyByName(propertyName)) return PropertyContainer::PROPERTY_RETURNED;
    if(PROPERTY_RETURNED == Clipped::getPropertyByName(propertyName)) return PropertyContainer::PROPERTY_RETURNED;
    if(PROPERTY_RETURNED == Visible::getPropertyByName(propertyName)) return PropertyContainer::PROPERTY_RETURNED;
//    DataContainer::getPropertyByName(propertyName);

    if(propertyName.compare("type")==0) return aCStringForScilab("Rectangle");
    return aCStringForScilab("N/C");
  }


public:
  double x;
  double y;
  double z;
  double width;
  double height;


  Rectangle(sciPointObj *pObj);

  void setPosition(double _x, double _y){x=_x; y=_y;}
  void setPosition(double _x, double _y, double _z){x=_x; y=_y; z=_z;}

  void setSize(double _width, double _height){width=_width; height=_height;}



  // Implement "DataContainer"
/*----------------------------------------------------------------------------*/
  int  setData(double *tab, int *numrow, int *numcol);
  void getData(double *tab, int *numrow, int *numcol);
  int  getDataSize();
/*----------------------------------------------------------------------------*/

// Class Deplacable
  int move(double displacement[], int displacementSize);

// Class sciGraphics::DrawableObject
  void display();


  void hasChanged(){std::cout << "Rectangle : hasChanged" << std::endl;}
  void displayChildren(){std::cout << "Rectangle : displayChildren" << std::endl;}
  void inversePointScale(double xScaled, double yScaled, double zScaled, double * xCoord, double * yCoord, double * zCoord)
  {std::cout << "Rectangle : inversePointScale" << std::endl;}
  void pointScale(double vectorX[], double vectorY[], double vectorZ[], int vectorLength)
  {std::cout << "Rectangle : pointScale" << std::endl;}
  void directionScale(  double xCoord, double yCoord, double zCoord,
                                double startingPointX, double startingPointY, double startingPointZ,
                                double * xScaled, double * yScaled, double * zScaled)
  {std::cout << "Rectangle : directionScale" << std::endl;}
  void directionScale(  double vectorX[], double vectorY[], double vectorZ[],
                                double startingPointsX[], double startingPointsY[],
                                double startingPointsZ[], double vectorLength)
  {std::cout << "Rectangle : directionScale" << std::endl;}

protected:
  EDisplayStatus draw(){std::cout << "Rectangle : draw" << std::endl;return SUCCESS;}
  EDisplayStatus show(){std::cout << "Rectangle : show" << std::endl;return SUCCESS;}
  EDisplayStatus redraw(){std::cout << "Rectangle : redraw" << std::endl;return SUCCESS;}
  void translate(){std::cout << "Rectangle : translate" << std::endl;}
  void endTranslate(){std::cout << "Rectangle : endTranslate" << std::endl;}
  void move(const double translation[3])
  {std::cout << "Rectangle : move" << std::endl;}
  void initializeDrawing(){std::cout << "Rectangle : initializeDrawing" << std::endl;}
  void endDrawing(){std::cout << "Rectangle : endDrawing" << std::endl;}
  void update(){std::cout << "Rectangle : update" << std::endl;}

};

/* -------------------------------------------------------------------------- */

#endif /* _SCI_RECTANGLE_H_ */
