#ifndef __SCO_WINDOWSCOPE_H__
#define __SCO_WINDOWSCOPE_H__

#include"scoBase.h"

/**
   \brief Initialization of the Window and its apparence
   \param pScopeMemory a pointer on a ScopeMemory
   \param dimension the dimension of the axes 2 or 3
   \param win_id the windows ID
   \param win_pos a table of 2 numbers which determines the position of the window on the screen
   \param win_dim a table of 2 numbers which determines the dimension of the window on the screen
   \param xmin a table of (number_of_subwin) size
   \param xmax a table of (number_of_subwin) size
   \param ymin a table of (number_of_subwin) size 
   \param ymax a table of (number_of_subwin) size
   \param zmin a table of (number_of_subwin) size
   \param zmax a table of (number_of_subwin) size
*/
void scoInitOfWindow(ScopeMemory * pScopeMemory, int dimension, int win_id, int * win_pos, int * win_dim, double * xmin, double * xmax, double * ymin, double * ymax, double * zmin, double * zmax);

/**
   \brief Create a polyline and return a pointer on it
*/
scoGraphicalObject scoCreatePolyline(scoGraphicalObject pAxes, scoInteger polyline_size,int color);

/**
   \brief Add a polyline for a shortdraw
*/
void scoAddPolylineForShortDraw(ScopeMemory * pScopeMemory, int i, int j, int color);

/**
   \brief Add a polyline for a longdraw
*/
void scoAddPolylineForLongDraw(ScopeMemory * pScopeMemory, int i, int j, int color);

/**
   \brief Add a couple of polylines one for shortdraw and one for longdraw
*/
void scoAddCoupleOfPolylines(ScopeMemory * pScopeMemory, int * colors);

/**
   \brief Del a couple of polyline one for the ShortDraw and one for the LongDraw they are lined
   \param pScopeMemory a pointer on a ScopeMemory
*/
void scoDelCoupleOfPolylines(ScopeMemory * pScopeMemory);

/**
   \brief Add a couple of polyline one for the ShortDraw and one for the LongDraw they are lined
   \param pScopeMemory a pointer on a ScopeMemory
   \param color a table of (number_of_curves_by_subwin[0]) size
*/
void scoAddCoupleOfSegments(ScopeMemory * pScopeMemory, int * color);

/**
   \brief Del a couple of polyline one for the ShortDraw and one for the LongDraw they are lined
   \param pScopeMemory a pointer on a ScopeMemory
*/
void scoDelCoupleOfSegments(ScopeMemory * pScopeMemory);

/**
   \brief Refresh the DataBounds of X if we have touched the end of the x-axes
   \param pScopeMemory a pointer on a ScopeMemory
   \param t the scicos time (get_scicos_time())
*/
void scoRefreshDataBoundsX(ScopeMemory * pScopeMemory,double t);
//CHANGE THE NAME OF THE FUNCTION TO PRECISE THAT WE ARE IN A AMPLITUDE TIME STYLE SCOPE

/**
   \brief Create a sphere by using the constructArc method of sgl and return a pointer on it
*/
scoGraphicalObject scoCreateSphere(scoGraphicalObject pAxes, double radius, int color);

/**
   \brief Add a sphere to the axis as a shortdraw
*/
void scoAddSphereForShortDraw(ScopeMemory * pScopeMemory, int i, int j, double radius, int color);

/**
   \brief Add a sphere to the axis as a longdraw
*/
void scoAddSphereForLongDraw(ScopeMemory * pScopeMemory, int i, int j, double radius, int color);

/**
   \brief Add a couple of spheres to the axis
*/
void scoAddCoupleOfSpheres(ScopeMemory * pScopeMemory, double * radius, int * colors);

/**
   \brief Create a Rectangle by using constructRectangle and return a pointer on it
*/
scoGraphicalObject scoCreateRectangle(scoGraphicalObject pAxes, double x, double y, double width, double height);

/**
   \brief Add a rectangle for a longdraw
*/
void scoAddRectangleForLongDraw(ScopeMemory * pScopeMemory, int i, int j, double x, double y, double width, double height);

scoGraphicalObject scoCreateGrayplot(scoGraphicalObject pAxes, int size_x, int size_y);

void scoAddGrayplotForShortDraw(ScopeMemory * pScopeMemory, int i, int j, int size_x, int size_y);

/* ----------------------DRAWING FONCTIONS --------------------------------*/

/**
   \brief Draw a Scope like CSCOPE : Amplitude + Time Based
   \param pScopeMemory a pointer on a ScopeMemory
   \param t the scicos time (get_scicos_time())
*/
void scoDrawScopeAmplitudeTimeStyle(ScopeMemory * pScopeMemory, double t);

/**
   \brief Draw a Scope like CSCOPXY or CSCOPXY3D
 */
void scoDrawScopeXYStyle(ScopeMemory * pScopeMemory);
/**
   \brief Add Titles and Backgound on the scope
 */
void scoAddTitlesScope(ScopeMemory * pScopeMemory,char * x, char * y, char * z);

/**
 */
void scoDrawScopeAnimXYStyle(ScopeMemory * pScopeMemory, double * u1, double * u2, double * u3);

#endif
