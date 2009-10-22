#ifndef _GFXRECTANGLE_H_
#define _GFXRECTANGLE_H_

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


extern "C"
{
#include "getScilabJavaVM.h"



#include "ObjectStructure.h"
#include "MALLOC.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "BuildObjects.h"
#include "InitObjects.h"

// TODO : pour strncpy a NETTOYER
#include <stdio.h>
#include <string.h>


  void
    BuildGFXRectangle(
        sciPointObj * pObj,
        double x, double y,
		    double height, double width,
        int *foreground, int *background,
		    int isfilled, int isline
        );

  void GFXDeleteFeatures(sciPointObj * pObj);
  void GFXSetHimselfAsDrawer(sciPointObj * pObj);

// class GraphicContext
// GET
  int     GFXGetForegroundColor(sciPointObj * pobj);
  int     GFXGetBackgroundColor(sciPointObj * pobj);
  int     GFXGetMarkForegroundColor(sciPointObj * pobj);
  int     GFXGetMarkBackgroundColor(sciPointObj * pobj);
  void    GFXGetLineWidth(sciPointObj * pobj, double * lw);
  bool    GFXGetIsFilled(sciPointObj * pobj);
  int     GFXGetFillColor(sciPointObj * pobj);
  bool    GFXGetIsLined(sciPointObj * pobj);
  int     GFXGetLineStyle(sciPointObj * pobj);
  bool    GFXGetIsMarked(sciPointObj * pobj);
  int     GFXGetMarkStyle(sciPointObj * pobj);
  int     GFXGetMarkSize(sciPointObj * pobj);
  int     GFXGetMarkSizeUnit(sciPointObj * pobj);
  int     GFXGetMarkBackgroundColor(sciPointObj * pobj);
  int     GFXGetMarkForegroundColor(sciPointObj * pobj);
// SET
  void GFXSetForegroundColor(sciPointObj * pobj, int color);
  void GFXSetBackgroundColor(sciPointObj * pobj, int color);
  void GFXSetMarkForegroundColor(sciPointObj * pobj, int color);
  void GFXSetMarkBackgroundColor(sciPointObj * pobj, int color);
  void GFXSetIsFilled(sciPointObj * pobj, bool filled);
  void GFXSetFillColor(sciPointObj * pobj, int color);
  void GFXSetIsLined(sciPointObj * pobj, bool lined);
  void GFXSetLineStyle(sciPointObj * pobj, int style);
  void GFXSetLineWidth(sciPointObj * pobj, double width);
  void GFXSetIsMarked(sciPointObj * pobj, bool marked);
  void GFXSetMarkStyle(sciPointObj * pobj, int style);
  void GFXSetMarkSize(sciPointObj * pobj, int size);
  void GFXSetMarkSizeUnit(sciPointObj * pobj, int sizeunit);
  void GFXSetMarkBackgroundColor(sciPointObj * pobj, int color);
  void GFXSetMarkForegroundColor(sciPointObj * pobj, int color);


// class UserData

  void GFXGetPointerToUserData(sciPointObj * pobj,int ***user_data_ptr, int **size_ptr);

// class GivePoint
  void GFXGetPoint(sciPointObj * pobj, double *tab, int *numrow, int *numcol);
  int GFXSetPoint(sciPointObj * pobj, double *tab, int *numrow, int *numcol);
  int GFXPointSize(sciPointObj * pobj);

// Deplacable
  int GFXmoveObj(sciPointObj * pobj, double displacement[], int displacementSize);
}


class Point
{
public:
  virtual int setPoint(double *tab, int *numrow, int *numcol)=0;
  virtual void getPoint(double *tab, int *numrow, int *numcol)=0;
  virtual int PointSize()=0;
};

class Deplacable
{
public:
  virtual int move(double displacement[], int displacementSize)=0;
};

/******************************************************************************/
// class CallBackable

/* -------------------------------------------------------------------------- */
// C function about CallBackable

extern "C"
{
  void GFXSetCallback(sciPointObj * pobj, char *code, int len, int mevent);
  void GFXGetCallback(sciPointObj * pobj, char *code);
  void GFXSetCallbackMouseEvent(sciPointObj * pobj, int mevent);
  void GFXDelCallback(sciPointObj * pobj);
  int GFXGetCallbackMouseEvent(sciPointObj * pobj);
  int GFXGetCallbackSize(sciPointObj * pobj);
}

/* -------------------------------------------------------------------------- */
// class CallBackable
class CallBackable
{
public:
  CallBackable():
    mouseEvent(100)
  {
  }
  void setCallback(char *code, int len, int mevent)
  {
    callback = code;
    mouseEvent = mevent;
  }
  void getCallback(char *code)
  {
    strcpy(code,callback.c_str());
  }
  int getCallbackSize()
  {
    return callback.size();
  }
  void setCallbackMouseEvent(int mevent){mouseEvent = mevent;}
  void delCallback()
  {
    mouseEvent=100;
  }
  int getCallbackMouseEvent(){return mouseEvent;}
protected:
  std::string callback;
  int mouseEvent;
};

class UserData
{
public:
  UserData():
    user_data(0), size_of_user_data(0)
  {
  }
  int * user_data;
  int size_of_user_data;
};

class GraphicContext
{
// TODO diviser cette classe
private:
  int backgroundColor;      
  int foregroundColor;

  BOOL isFilled;
  int fillColor;

  BOOL isLined;
  int lineStyle;
  double lineWidth;

  BOOL isMarked;
  int markStyle;
  int markSize;
  int markSizeUnit;
  int markBackgroundColor;
  int markForegroundColor;

public:
  GraphicContext():
    backgroundColor(1), foregroundColor(1), isFilled(false), fillColor(1),
    isLined(false), lineStyle(1), lineWidth(1),
    isMarked(false), markStyle(1), markSize(1), markSizeUnit(1), markBackgroundColor(1), markForegroundColor(1)
  {
  }
  void    setBackgroundColor(int bgc){backgroundColor=bgc;}
  int     getBackgroundColor(){return backgroundColor;}
  void    setForegroundColor(int fgc){foregroundColor=fgc;}
  int     getForegroundColor(){return foregroundColor;}

  void    setIsFilled(bool filled){isFilled=filled;}
  bool    getIsFilled(){return isFilled;}
  void    setFillColor(int fc){fillColor=fc;}
  int     getFillColor(){return fillColor;}

  void    setIsLined(bool lined){isLined=lined;}
  bool    getIsLined(){return isLined;}
  void    setLineStyle(int ls){lineStyle=ls;}
  int     getLineStyle(){return lineStyle;}
  void    setLineWidth(double lw){lineWidth=lw;}
  double  getLineWidth(){return lineWidth;}
  void    getLineWidthFromC(double * lw){*lw = lineWidth;}

  void    setIsMarked(bool marked){isMarked=marked;}
  bool    getIsMarked(){return isMarked;}
  void    setMarkStyle(int ms){markStyle=ms;}
  int     getMarkStyle(){return markStyle;}
  void    setMarkSize(int ms){markSize=ms;}
  int     getMarkSize(){return markSize;}
  void    setMarkSizeUnit(int msu){markSizeUnit=msu;}
  int     getMarkSizeUnit(){return markSizeUnit;}
  void    setMarkBackgroundColor(int mb){markBackgroundColor=mb;}
  int     getMarkBackgroundColor(){return markBackgroundColor;}
  void    setMarkForegroundColor(int mf){markForegroundColor=mf;}
  int     getMarkForegroundColor(){return markForegroundColor;}

};

/******************************************************************************/
// class Visible

/* -------------------------------------------------------------------------- */
// C function about Visible
extern "C"
{
  bool GFXGetVisibility(sciPointObj * pobj);
  void GFXSetVisibility(sciPointObj * pobj, BOOL value);
}

/* -------------------------------------------------------------------------- */
// class Visible
class Visible
{
private:
  BOOL visible;
public:
  Visible():
    visible(true)
  {
  }
  Visible(bool v):
    visible(v)
  {
  }
  void setVisibility(bool v){visible=v;}
  BOOL getVisibility(){return visible;}
};

/******************************************************************************/
// class ClipedObject

/* -------------------------------------------------------------------------- */
// C function about ClipedObject
extern "C"
{
  void GFXSetClippingRegion(sciPointObj * pobj, double pclip[4]);
  void GFXUnsetClippingRegion(sciPointObj * pobj);
  bool GFXGetClippingRegionSet(sciPointObj * pobj);
  void GFXGetClippingRegion(sciPointObj * pobj, double pclip[]);

  void GFXSetClippingMethode(sciPointObj * pobj, int cm);
  int  GFXGetClippingMethode(sciPointObj * pobj);
}
/* -------------------------------------------------------------------------- */
// class ClipedObject
class ClippedObject:
  public sciGraphics::DrawableObject
{
public:
  enum ClippingMethode
  {
    AXES_BOUNDS,
    OWN_BOUNDS,
    NO_BOUNDS
  };
private:
  org_scilab_modules_renderer_rectangleDrawing::DrawableRectangleGL* clipper;
  ClippingMethode clippingMethode;
  double clippingRegion[4];         //TODO : passer à 3*2
  double scaledClippingRegion[6];
  bool isClippingRegionSet;

  void setScaledClippingRegion()
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
protected:
  void clip()
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
  void unClip()
  {
    if(clippingMethode != NO_BOUNDS)
    {
      clipper->endDrawing();
      clipper->unClip();
    }
  }
  void initializeDrawing()
  {
    std::cout << "ClippedObject : initializeDrawing" << std::endl;
    clipper->initializeDrawing(sciGetNum(sciGetParentFigure(m_pDrawed)));
  }
  void endDrawing()
  {
    std::cout << "ClippedObject : endDrawing" << std::endl;
    clipper->endDrawing();
  }
public:
  ClippedObject(sciPointObj *pObj):
    isClippingRegionSet(false), clippingMethode(AXES_BOUNDS),
    sciGraphics::DrawableObject(pObj)
  {
    clipper = new org_scilab_modules_renderer_rectangleDrawing::DrawableRectangleGL(getScilabJavaVM());
//TODO : use c++ call when hierarchy was in cpp
    setClippingMethodeFromC(sciGetIsClipping(sciGetParentSubwin(pObj)));
    setClippingRegion(sciGetClipping(sciGetParentSubwin(pObj)));
  }

  ~ClippedObject()
  {
    delete(clipper);
  }


  void setClippingRegion(double pclip[4])
  {
    clippingRegion[0]=pclip[0];
    clippingRegion[1]=pclip[1];
    clippingRegion[2]=pclip[2];
    clippingRegion[3]=pclip[3];
    isClippingRegionSet = true;
  }
  void unsetClippingRegion()
  {
    isClippingRegionSet = false;
  }
  bool getClippingRegionSet(){return isClippingRegionSet;}
  void getClippingRegionFromC(double pclip[]) // for compabiliti// TODO : faire disparaitre !
  {
    pclip[0]=clippingRegion[0];
    pclip[1]=clippingRegion[1];
    pclip[2]=clippingRegion[2];
    pclip[3]=clippingRegion[3];
  }

  void setClippingMethode(ClippingMethode cm){clippingMethode = cm;}
  void setClippingMethodeFromC(int cm)// for compabiliti// TODO : faire disparaitre !
  {
    if(cm==0) clippingMethode=AXES_BOUNDS;
    else if(cm==1) clippingMethode=OWN_BOUNDS;
    else clippingMethode=NO_BOUNDS;
  }
  ClippingMethode getClippingMethode(){return clippingMethode;}
  int getClippingMethodeFromC()// for compabiliti// TODO : faire disparaitre !
  {
    if(clippingMethode==AXES_BOUNDS) return 0;
    else if(clippingMethode==OWN_BOUNDS) return 1;
    return -1;
  }

};

/* -------------------------------------------------------------------------- */

class Rectangle:
  public CallBackable,
  public UserData,
  public GraphicContext,
  public ClippedObject,
  public Visible,
  public Point,
  public Deplacable
{
private:
/*
  org_scilab_modules_renderer_rectangleDrawing::RectangleLineDrawerGL* lineDrawer;
  org_scilab_modules_renderer_rectangleDrawing::RectangleMarkDrawerGL* markDrawer;
  org_scilab_modules_renderer_rectangleDrawing::RectangleFillDrawerGL* fillDrawer;
*/
public:
  Rectangle(
        sciPointObj *pObj,
        double x, double y,
		    double height, double width
        );

public:
  
  double x;
  double y;
  double z;
  double width;
  double height;

// Class Point
  void getPoint(double *tab, int *numrow, int *numcol);
  int setPoint(double *tab, int *numrow, int *numcol);
  int PointSize();

// Class Deplacable
  int move(double displacement[], int displacementSize);

// Class sciGraphics::DrawableObject
  void display()
  {
  std::cout << "Rectangle : display" << std::endl;
  if(getVisibility()==false) return;
/******************************************************************************/
  double coord[12];
  sciGraphics::DrawableObject::pointScale(x,       y,        z,&coord[0],&coord[ 1],&coord[ 2]);
  sciGraphics::DrawableObject::pointScale(x+width, y,        z,&coord[3],&coord[ 4],&coord[ 5]);
  sciGraphics::DrawableObject::pointScale(x+width, y-height, z,&coord[6],&coord[ 7],&coord[ 8]);
  sciGraphics::DrawableObject::pointScale(x,       y-height, z,&coord[9],&coord[10],&coord[11]);
  clip();
  if(getIsLined())
  {
    org_scilab_modules_renderer_rectangleDrawing::RectangleLineDrawerGL* lineDrawer = new org_scilab_modules_renderer_rectangleDrawing::RectangleLineDrawerGL(getScilabJavaVM());
    lineDrawer->initializeDrawing(sciGetNum(sciGetParentFigure(m_pDrawed)));
    lineDrawer->setLineParameters(getForegroundColor(), getLineWidth(), getLineStyle());
    lineDrawer->drawRectangle(coord[0],coord[1],coord[2],coord[3],coord[4],coord[5],coord[6],coord[7],coord[8],coord[9],coord[10],coord[11]);
    lineDrawer->endDrawing();
    delete(lineDrawer);
  }
  if(getIsMarked())
  {
    org_scilab_modules_renderer_rectangleDrawing::RectangleMarkDrawerGL* markDrawer = new org_scilab_modules_renderer_rectangleDrawing::RectangleMarkDrawerGL(getScilabJavaVM());
    markDrawer->initializeDrawing(sciGetNum(sciGetParentFigure(m_pDrawed)));
    markDrawer->setMarkParameters(getMarkBackgroundColor(), getMarkForegroundColor(), getMarkSizeUnit(), getMarkSize(), getMarkStyle());
    markDrawer->drawRectangle(coord[0],coord[1],coord[2],coord[3],coord[4],coord[5],coord[6],coord[7],coord[8],coord[9],coord[10],coord[11]);
    markDrawer->endDrawing();
    delete(markDrawer);
  }
  if(getIsFilled())
  {
    org_scilab_modules_renderer_rectangleDrawing::RectangleFillDrawerGL* fillDrawer = new org_scilab_modules_renderer_rectangleDrawing::RectangleFillDrawerGL(getScilabJavaVM());
    fillDrawer->initializeDrawing(sciGetNum(sciGetParentFigure(m_pDrawed)));
    fillDrawer->setBackColor(getBackgroundColor());
    fillDrawer->drawRectangle(coord[0],coord[1],coord[2],coord[3],coord[4],coord[5],coord[6],coord[7],coord[8],coord[9],coord[10],coord[11]);
    fillDrawer->endDrawing();
    delete(fillDrawer);
  }
  unClip();
/******************************************************************************/
  }
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

#endif /* _GFXRECTANGLE_H_ */
