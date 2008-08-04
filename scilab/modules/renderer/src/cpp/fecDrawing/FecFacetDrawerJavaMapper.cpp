/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by FecFacetDrawer
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "FecFacetDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
FecFacetDrawerJavaMapper::FecFacetDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_fecDrawing::FecFacetDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
FecFacetDrawerJavaMapper::~FecFacetDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex); 
}
/*--------------------------------------------------------------------------*/
void FecFacetDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJavaMapper::setFacetParameters(double zMin, double zMax, int colMin,
                                                  int colMax, int colOutLow, int colOutUp)
{
  m_pJavaObject->setFacetParameters(zMin, zMax, colMin, colMax, colOutLow, colOutUp);
}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJavaMapper::drawFec(const double xCoords[], const double yCoords[],
                                       const double values[], int nbNodes,
                                       const int firstPoints[], const int secondPoints[],
                                       const int thirdPoints[], int nbTriangles)
{
  // convert to long
  long * javaFirstPoints = new long[nbTriangles];
  long * javaSecondPoints = new long[nbTriangles];
  long * javaThirdPoints = new long[nbTriangles];

  try
  {
    javaFirstPoints = new long[nbTriangles];
    javaSecondPoints = new long[nbTriangles];
    javaThirdPoints = new long[nbTriangles];
  }
  catch (const std::exception& e)
  {
    // allocation failed
    if (javaFirstPoints != NULL) { delete[] javaFirstPoints; }
    if (javaSecondPoints != NULL) { delete[] javaSecondPoints; }
    if( javaThirdPoints != NULL) { delete[] javaThirdPoints; }
    // propagate exception
    throw;
  }

  m_pJavaObject->drawFec((double *)xCoords, nbNodes,
                         (double *)yCoords, nbNodes,
                         (double *)values, nbNodes,
                         javaFirstPoints, nbTriangles,
                         javaSecondPoints, nbTriangles,
                         javaThirdPoints, nbTriangles);


  delete[] javaFirstPoints;
  delete[] javaSecondPoints;
  delete[] javaThirdPoints;
}
/*---------------------------------------------------------------------------------*/

}
