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

#include "HandleManagement.h" // sciGetParent


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
  int     GFXGetForegroundColor(sciPointObj * pObj);
  int     GFXGetBackgroundColor(sciPointObj * pObj);
  int     GFXGetMarkForegroundColor(sciPointObj * pObj);
  int     GFXGetMarkBackgroundColor(sciPointObj * pObj);
  void    GFXGetLineWidth(sciPointObj * pObj, double * lw);
  BOOL    GFXGetIsFilled(sciPointObj * pObj);
  int     GFXGetFillColor(sciPointObj * pObj);
  BOOL    GFXGetIsLined(sciPointObj * pObj);
  int     GFXGetLineStyle(sciPointObj * pObj);
  BOOL    GFXGetIsMarked(sciPointObj * pObj);
  int     GFXGetMarkStyle(sciPointObj * pObj);
  int     GFXGetMarkSize(sciPointObj * pObj);
  int     GFXGetMarkSizeUnit(sciPointObj * pObj);
  int     GFXGetMarkBackgroundColor(sciPointObj * pObj);
  int     GFXGetMarkForegroundColor(sciPointObj * pObj);
// SET
  void GFXSetForegroundColor(sciPointObj * pObj, int color);
  void GFXSetBackgroundColor(sciPointObj * pObj, int color);
  void GFXSetMarkForegroundColor(sciPointObj * pObj, int color);
  void GFXSetMarkBackgroundColor(sciPointObj * pObj, int color);
  void GFXSetIsFilled(sciPointObj * pObj, BOOL filled);
  void GFXSetFillColor(sciPointObj * pObj, int color);
  void GFXSetIsLined(sciPointObj * pObj, BOOL lined);
  void GFXSetLineStyle(sciPointObj * pObj, int style);
  void GFXSetLineWidth(sciPointObj * pObj, double width);
  void GFXSetIsMarked(sciPointObj * pObj, BOOL marked);
  void GFXSetMarkStyle(sciPointObj * pObj, int style);
  void GFXSetMarkSize(sciPointObj * pObj, int size);
  void GFXSetMarkSizeUnit(sciPointObj * pObj, int sizeunit);
  void GFXSetMarkBackgroundColor(sciPointObj * pObj, int color);
  void GFXSetMarkForegroundColor(sciPointObj * pObj, int color);


// class UserData

  void GFXGetPointerToUserData(sciPointObj * pObj,int ***user_data_ptr, int **size_ptr);

// class GivePoint
  void GFXGetPoint(sciPointObj * pObj, double *tab, int *numrow, int *numcol);
  int GFXSetPoint(sciPointObj * pObj, double *tab, int *numrow, int *numcol);
  int GFXPointSize(sciPointObj * pObj);

// Deplacable
  int GFXmoveObj(sciPointObj * pObj, double displacement[], int displacementSize);
}

class ColorMapUser:
  virtual public sciGraphics::DrawableObject
{
public:
  ColorMapUser(sciPointObj * pObj):
    sciGraphics::DrawableObject(pObj)
  {
  }

  int sci2cppColorIndex(int colorindex)
  {
    int m = getColorMapSize();

    if(colorindex<-2) colorindex = -2;
    if(colorindex>m+2) colorindex = m+2;

    //TODO allow index = -1
    if( colorindex ==  0 ) colorindex = 1;
    if( colorindex == -1 ) colorindex = m+1;
    if( colorindex == -2 ) colorindex = m+2;    

    return colorindex-1;
  }

  int getColorMapSize()
  {
    return sciGetNumColors(m_pDrawed);
  }
};


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
  void GFXSetCallback(sciPointObj * pObj, char *code, int len, int mevent);
  void GFXGetCallback(sciPointObj * pObj, char *code);
  void GFXSetCallbackMouseEvent(sciPointObj * pObj, int mevent);
  void GFXDelCallback(sciPointObj * pObj);
  int GFXGetCallbackMouseEvent(sciPointObj * pObj);
  int GFXGetCallbackSize(sciPointObj * pObj);
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

class GraphicContext:
  virtual public ColorMapUser
{
// TODO diviser cette classe
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
  GraphicContext(sciPointObj * pObj):
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

  void setGraphicContext(sciGraphicContext * sgc)
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

  void setDefaultValues()
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

  void    setBackgroundColor(int bgc){backgroundColor=bgc;}
  void    setBackgroundSciColor(int bgc){backgroundColor=sci2cppColorIndex(bgc);}
  int     getBackgroundColor(){return backgroundColor;}
  void    setForegroundColor(int fgc){foregroundColor=fgc;}
  void    setForegroundSciColor(int fgc){foregroundColor=sci2cppColorIndex(fgc);}
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
  BOOL GFXGetVisibility(sciPointObj * pObj);
  void GFXSetVisibility(sciPointObj * pObj, BOOL value);
}

/* -------------------------------------------------------------------------- */
// class Visible
class Visible
{
private:
  bool visible;
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
  bool getVisibility(){return visible;}
};

/******************************************************************************/
// class ClipedObject

/* -------------------------------------------------------------------------- */
// C function about ClipedObject
extern "C"
{
  void GFXSetClippingRegion(sciPointObj * pObj, double pclip[4]);
  void GFXUnsetClippingRegion(sciPointObj * pObj);
  bool GFXGetClippingRegionSet(sciPointObj * pObj);
  void GFXGetClippingRegion(sciPointObj * pObj, double pclip[]);

  void GFXSetClippingMethode(sciPointObj * pObj, int cm);
  int  GFXGetClippingMethode(sciPointObj * pObj);
}
/* -------------------------------------------------------------------------- */
// class ClipedObject
class ClippedObject:
  virtual public sciGraphics::DrawableObject
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
protected:
/*
  org_scilab_modules_renderer_rectangleDrawing::RectangleLineDrawerGL* lineDrawer;
  org_scilab_modules_renderer_rectangleDrawing::RectangleMarkDrawerGL* markDrawer;
  org_scilab_modules_renderer_rectangleDrawing::RectangleFillDrawerGL* fillDrawer;
*/
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
