#include "scigfx_Rectangle.h"

extern "C"
{
  #include "getScilabJavaVM.h"
}

extern "C"
{
  void BuildRectangle(
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

    SetHimselfAsDrawer(pObj);
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
        UserDataContainer(),
        Clipped(pObj),
        Visible(),
        DataContainer(),
        Movable(),
        GraphicContext(pObj)
{
}


void Rectangle::getData(double *tab, int *numrow, int *numcol)
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

int Rectangle::getDataSize()
{
  return 5;//pSUBWIN_FEATURE(GetParentSubwin())->is3d ? 5: 4;
}

int Rectangle::setData(double *tab, int *numrow, int *numcol)
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

void Rectangle::display()
 {
  std::cout << "Rectangle : display" << std::endl;
  if(getVisibility()==false) return;

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
}
