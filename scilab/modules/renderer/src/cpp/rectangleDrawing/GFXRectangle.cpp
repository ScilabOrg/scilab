#include "GFXRectangle.h"

template<typename T>
T MyCast(sciPointObj* pobj)
{
  switch (sciGetEntityType (pobj))
  {
    case SCI_RECTANGLE:
      return reinterpret_cast<Rectangle*>(pobj->pfeatures);
      break;
    case SCI_FIGURE:
    case SCI_SUBWIN:
    case SCI_ARC:
    case SCI_SEGS: 
    case SCI_FEC:
    case SCI_GRAYPLOT: 
    case SCI_POLYLINE:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_LEGEND:
    case SCI_TEXT:
    case SCI_LABEL:
    case SCI_AGREG:
    case SCI_UIMENU:
    default:
      return 0;
      break;
  }
  return 0;
}

extern "C"
{
  void BuildGFXRectangle(
        sciPointObj * pObj,
        double x, double y,
		    double height, double width,
        int *foreground, int *background,
		    int isfilled, int isline)
  {
/*
    if ( height < 0.0 || width < 0.0 )
    {
      Scierror(999,_("Width and height must be positive.\n"));
      return NULL ;
    }
*/

    Rectangle* geom = new Rectangle(pObj);
    // Set GraphicContext from the parent one.
    geom->setGraphicContext(sciGetGraphicContext(sciGetParent(pObj)));
    geom->setVisibility(sciGetVisibility(sciGetParentSubwin(pObj)));

    geom->setPosition(x,y);
    geom->setSize(height, width);

    if(foreground!=NULL) geom->setForegroundSciColor(*foreground);
    if(background!=NULL) geom->setBackgroundSciColor(*background);
    geom->setIsFilled(isfilled != 0);
    geom->setIsLined(isline != 0);

    pObj->pfeatures = reinterpret_cast<void*>(geom);

    GFXSetHimselfAsDrawer(pObj);
  }

  void GFXDeleteFeatures(sciPointObj * pObj)
  {
    delete(pObj->pfeatures);
    pObj->pfeatures=NULL;
  }

  void GFXSetHimselfAsDrawer(sciPointObj * pObj)
  {
    sciGraphics::DrawableObjectWrapper* wrapper = new sciGraphics::DrawableObjectWrapper();
    wrapper->drawer = MyCast<sciGraphics::DrawableObject*>(pObj);
    pObj->pDrawer = wrapper;
  }

/******************************************************************************/
// class GraphicContext
/*----------------------------------------------------------------------------*/
// GET
  int GFXGetForegroundColor(sciPointObj * pobj)
  {
    return MyCast<GraphicContext*>(pobj)->getForegroundColor();
  }
  int GFXGetBackgroundColor(sciPointObj * pobj)
  {
    return MyCast<GraphicContext*>(pobj)->getBackgroundColor();
  }
  void GFXGetLineWidth(sciPointObj * pobj, double * lw)
  {
    *lw=MyCast<GraphicContext*>(pobj)->getLineWidth();
  }
  BOOL GFXGetIsFilled(sciPointObj * pobj)
  {
    return MyCast<GraphicContext*>(pobj)->getIsFilled()?TRUE:FALSE;
  }
  int GFXGetFillColor(sciPointObj * pobj)
  {
    return MyCast<GraphicContext*>(pobj)->getFillColor();
  }
  BOOL GFXGetIsLined(sciPointObj * pobj)
  {
    return MyCast<GraphicContext*>(pobj)->getIsLined()?TRUE:FALSE;
  }
  int GFXGetLineStyle(sciPointObj * pobj)
  {
    return MyCast<GraphicContext*>(pobj)->getLineStyle();
  }
  BOOL GFXGetIsMarked(sciPointObj * pobj)
  {
    return MyCast<GraphicContext*>(pobj)->getIsMarked()?TRUE:FALSE;
  }
  int GFXGetMarkStyle(sciPointObj * pobj)
  {
    return MyCast<GraphicContext*>(pobj)->getMarkStyle();
  }
  int GFXGetMarkSize(sciPointObj * pobj)
  {
    return MyCast<GraphicContext*>(pobj)->getMarkSize();
  }
  int GFXGetMarkSizeUnit(sciPointObj * pobj)
  {
    return MyCast<GraphicContext*>(pobj)->getMarkSizeUnit();
  }
  int GFXGetMarkBackgroundColor(sciPointObj * pobj)
  {
    return MyCast<GraphicContext*>(pobj)->getMarkBackgroundColor();
  }
  int GFXGetMarkForegroundColor(sciPointObj * pobj)
  {
    return MyCast<GraphicContext*>(pobj)->getMarkForegroundColor();
  }
/*----------------------------------------------------------------------------*/
// SET
  void GFXSetForegroundColor(sciPointObj * pobj, int color)
  {
    MyCast<GraphicContext*>(pobj)->setForegroundColor(color);
  }
  void GFXSetBackgroundColor(sciPointObj * pobj, int color)
  {
    MyCast<GraphicContext*>(pobj)->setBackgroundColor(color);
  }
  void GFXSetLineWidth(sciPointObj * pobj, double width)
  {
    MyCast<GraphicContext*>(pobj)->setLineWidth(width);
  }
  void GFXSetIsFilled(sciPointObj * pobj, BOOL filled)
  {
    MyCast<GraphicContext*>(pobj)->setIsFilled(filled!=0);
  }
  void GFXSetFillColor(sciPointObj * pobj, int color)
  {
    MyCast<GraphicContext*>(pobj)->setFillColor(color);
  }
  void GFXSetIsLined(sciPointObj * pobj, BOOL lined)
  {
    MyCast<GraphicContext*>(pobj)->setIsLined(lined!=0);
  }
  void GFXSetLineStyle(sciPointObj * pobj, int style)
  {
    MyCast<GraphicContext*>(pobj)->setLineStyle(style);
  }
  void GFXSetIsMarked(sciPointObj * pobj, BOOL marked)
  {
    MyCast<GraphicContext*>(pobj)->setIsMarked(marked!=0);
  }
  void GFXSetMarkStyle(sciPointObj * pobj, int style)
  {
    MyCast<GraphicContext*>(pobj)->setMarkStyle(style);
  }
  void GFXSetMarkSize(sciPointObj * pobj, int size)
  {
    MyCast<GraphicContext*>(pobj)->setMarkSize(size);
  }
  void GFXSetMarkSizeUnit(sciPointObj * pobj, int sizeunit)
  {
    MyCast<GraphicContext*>(pobj)->setMarkSizeUnit(sizeunit);
  }
  void GFXSetMarkBackgroundColor(sciPointObj * pobj, int color)
  {
    MyCast<GraphicContext*>(pobj)->setMarkBackgroundColor(color);
  }
  void GFXSetMarkForegroundColor(sciPointObj * pobj, int color)
  {
    MyCast<GraphicContext*>(pobj)->setMarkForegroundColor(color);
  }
/******************************************************************************/
// class Deplacable
  int GFXmoveObj(sciPointObj * pobj, double displacement[], int displacementSize)
  {
    return MyCast<Deplacable*>(pobj)->move(displacement, displacementSize);
  }
/******************************************************************************/
// class CallBackable

  void GFXSetCallback(sciPointObj * pobj, char *code, int len, int mevent)
  {
    MyCast<CallBackable*>(pobj)->setCallback(code, len, mevent);
  }
  void GFXGetCallback(sciPointObj * pobj, char *code)
  {
    MyCast<CallBackable*>(pobj)->getCallback(code);
  }
  void GFXSetCallbackMouseEvent(sciPointObj * pobj, int mevent)
  {
    MyCast<CallBackable*>(pobj)->setCallbackMouseEvent(mevent);
  }
  void GFXDelCallback(sciPointObj * pobj)
  {
    MyCast<CallBackable*>(pobj)->delCallback();
  }
  int GFXGetCallbackMouseEvent(sciPointObj * pobj)
  {
    return MyCast<CallBackable*>(pobj)->getCallbackMouseEvent();
  }
  int GFXGetCallbackSize(sciPointObj * pobj)
  {
    return MyCast<CallBackable*>(pobj)->getCallbackSize();
  }
/******************************************************************************/
// class Point
  int GFXSetPoint(sciPointObj * pobj, double *tab, int *numrow, int *numcol)
  {
    return MyCast<Point*>(pobj)->setPoint(tab, numrow, numcol);
  }
  void GFXGetPoint(sciPointObj * pobj, double *tab, int *numrow, int *numcol)
  {
    MyCast<Point*>(pobj)->getPoint(tab, numrow, numcol);
  }
  int GFXPointSize(sciPointObj * pobj)
  {
    return MyCast<Point*>(pobj)->PointSize();
  }
/******************************************************************************/
// class  UserData
  void GFXGetPointerToUserData(sciPointObj * pobj,int ***user_data_ptr, int **size_ptr)
  {
      *user_data_ptr = &(MyCast<UserData*>(pobj)->user_data);
      *size_ptr =  &(MyCast<UserData*>(pobj)->size_of_user_data);
  }
/******************************************************************************/
// class  ClippedObject
  void GFXSetClippingRegion(sciPointObj * pobj, double pclip[4])
  {
      MyCast<ClippedObject*>(pobj)->setClippingRegion(pclip);
  }
  void GFXUnsetClippingRegion(sciPointObj * pobj)
  {
      MyCast<ClippedObject*>(pobj)->unsetClippingRegion();
  }
  bool GFXGetClippingRegionSet(sciPointObj * pobj)
  {
      return  MyCast<ClippedObject*>(pobj)->getClippingRegionSet();
  }
  void GFXGetClippingRegion(sciPointObj * pobj, double pclip[])
  {
      MyCast<ClippedObject*>(pobj)->getClippingRegionFromC(pclip);
  }
  void GFXSetClippingMethode(sciPointObj * pobj, int cm)
  {
      MyCast<ClippedObject*>(pobj)->setClippingMethodeFromC(cm);
  }
  int  GFXGetClippingMethode(sciPointObj * pobj)
  {
      return  MyCast<ClippedObject*>(pobj)->getClippingMethodeFromC();
  }
/******************************************************************************/
// class  Visible
  void GFXSetVisibility(sciPointObj * pobj, BOOL value)
  {
    MyCast<Visible*>(pobj)->setVisibility(value!=0);
  }
  BOOL GFXGetVisibility(sciPointObj * pobj)
  {
    return MyCast<Visible*>(pobj)->getVisibility()?TRUE:FALSE;
  }
}
///     end extern C
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
// class  Rectangle


Rectangle::Rectangle(sciPointObj *pObj):
        x(0), y(0), z(0), width(0), height(0),
        sciGraphics::DrawableObject(pObj),
        ColorMapUser(pObj),
        CallBackable(),
        UserData(),
        ClippedObject(pObj),
        Visible(),
        Point(),
        Deplacable(),
        GraphicContext(pObj)
{
}


void Rectangle::getPoint(double *tab, int *numrow, int *numcol)
{
//TODO : nettoyer  
  *numrow = 1;
  *numcol= 5;//pSUBWIN_FEATURE(GetParentSubwin())->is3d ? 5: 4;
  tab[0] = x;
  tab[1] = y;
  if (true)//pSUBWIN_FEATURE(GetParentSubwin())->is3d)
  {
    tab[2] = z;
    tab[3] = width;
    tab[4] = height;
	}
  else
	{
    tab[2] = width;
    tab[3] = height; 
	}
}

int Rectangle::PointSize()
{
  return 5;//pSUBWIN_FEATURE(GetParentSubwin())->is3d ? 5: 4;
}

int Rectangle::setPoint(double *tab, int *numrow, int *numcol)
    {
      int widthIndex = 2 ;
      int size = *numrow * *numcol ;
      if ( size != 5 && size != 4 )
      {
  //TODO: remetre le '_'
      //  Scierror(999, ("Number of elements must be %d (%d if %s coordinate).\n"),4,5,"z");
        return -1;
      }

      x = tab[0] ;
      y = tab[1] ;

      if ( size == 5 )
      {
        z = tab[2] ;
        widthIndex = 3 ;
      }

      /* check that the height and width are positive */
      if ( tab[widthIndex] < 0.0 || tab[widthIndex + 1] < 0.0 )
      {
  //TODO: METRE le '_'
      //  Scierror(999,"Width and height must be positive.\n") ;
        return -1 ;
      }
      width  = tab[widthIndex    ] ;
      height = tab[widthIndex + 1] ;

      return 0;
    }

int Rectangle::move(double displacement[], int displacementSize)
{
// TODO : factoriser dans un 'simple deplacable'
    if(displacementSize >= 1)
      x += displacement[0];
    if(displacementSize >= 2)
      y += displacement[1];
    if(displacementSize >= 3)
      z += displacement[2];
}
