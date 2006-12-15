#include "scoBase.h"
#include "scoWindowScope.h"
#include "scoMemoryScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"
#include "stdio.h"

void scoInitOfWindow(ScopeMemory * pScopeMemory, int dimension, int win_id, int * win_pos, int * win_dim, double * xmin, double * xmax, double * ymin, double * ymax, double * zmin, double * zmax)
{
  int i;
  scoGraphicalObject pTemp;
  scoGraphicalObject pTemp2;
  scoSetWindowID(pScopeMemory,win_id);
  DeleteObjs(win_id);
  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
  pTemp = sciGetCurrentFigure();
  scoSetHandleScopeWindow(pScopeMemory,sciGetHandle(pTemp));
  for (i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    {
      if (i == 0)
	{
	  //We are getting the Handle of the current axes but in the same time we are constructing it (see below)
	  //Here pTemp is the pointer on the ScopeWindow
	  scoSetHandleAxes(pScopeMemory,i,sciGetHandle(sciGetSelectedSubWin(pTemp)));
	}
      else
	{
	  //Here pTemp is the pointer on the ScopeWindow
	  scoSetHandleAxes(pScopeMemory,i,sciGetHandle(ConstructSubWin(pTemp, win_id)));
	}
      //Here pTemp2 is the pointer on the current Axes
      pTemp2 = scoGetPointerAxes(pScopeMemory,i);
      sciSetFontDeciWidth(pTemp2, 0);
      sciSetIsBoxed(pTemp2,TRUE);
      pSUBWIN_FEATURE(pTemp2)->tight_limits = TRUE;
      //Here PTemp2 is the pointer on the current Subwint
      pSUBWIN_FEATURE(pTemp2)->WRect[0] = 0;
      pSUBWIN_FEATURE(pTemp2)->WRect[1] = (double)i/scoGetNumberOfSubwin(pScopeMemory);
      pSUBWIN_FEATURE(pTemp2)->WRect[2] = 1;
      pSUBWIN_FEATURE(pTemp2)->WRect[3] = (double)1/scoGetNumberOfSubwin(pScopeMemory);
      switch(dimension)
	{
	case 3:
	  pSUBWIN_FEATURE(pTemp2)->is3d = TRUE;
	  pSUBWIN_FEATURE(pTemp2)->axes.axes_visible[2] = TRUE;
	  pSUBWIN_FEATURE(pTemp2)->SRect[4] = zmin[i];
	  pSUBWIN_FEATURE(pTemp2)->SRect[5] = zmax[i];
	case 2:
	  pSUBWIN_FEATURE(pTemp2)->axes.axes_visible[1] = TRUE;
	  pSUBWIN_FEATURE(pTemp2)->SRect[2] = ymin[i];
	  pSUBWIN_FEATURE(pTemp2)->SRect[3] = ymax[i];
	case 1:
	  pSUBWIN_FEATURE(pTemp2)->axes.axes_visible[0] = TRUE;
	  pSUBWIN_FEATURE(pTemp2)->SRect[0] = xmin[i];
	  pSUBWIN_FEATURE(pTemp2)->SRect[1] = xmax[i];
	  break;
	default:
	  sciprint("SCOPE ERROR : Error in dimension number");
	  fprintf(stderr,"SCOPE ERROR : Error in dimension number");
	  break;
	}

    }
  if(win_pos != NULL)
    {
      if (win_pos[0] >= 0)
	sciSetFigurePos(pTemp, win_pos[0], win_pos[1]);
    }
  if(win_dim != NULL)
    {
      if (win_dim[0] >= 0)
	sciSetDim(pTemp, &win_dim[0], &win_dim[1]);
    }

  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
  sciDrawObj(pTemp);
}

void scoRefreshDataBoundsX(ScopeMemory * pScopeMemory, double t)
{
  scoGraphicalObject pLongDraw;
  scoGraphicalObject pShortDraw;
  scoGraphicalObject pAxes;

  int NbrPts, current_period_counter;
  int i,j;
  int c__1 =   1 ;
  double period;
  int bool = 0;
  /*Go on the window*/
  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
  for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    {
      /*if we have to redraw the axis for x-axis*/
      if (scoGetNewDraw(pScopeMemory,i) < 0)
	{
	  bool = 1;
	  pAxes = scoGetPointerAxes(pScopeMemory,i);
	  period = scoGetPeriod(pScopeMemory,i);

	  /*Calculating the current period counter and update the value of x bounds*/
	  current_period_counter = (int)(t/period);
	  pSUBWIN_FEATURE(pAxes)->SRect[0] = period*(current_period_counter);
	  pSUBWIN_FEATURE(pAxes)->SRect[1] = period*(current_period_counter+1);
	  /*Don't forget to save the new value*/
	  scoSetPeriodCounter(pScopeMemory,i,current_period_counter);
	  
	  /*Instruction to reinitialize the ShortDraw and to copy the last points of the ShortDraw in the LongDraw to be sure to have continuity*/
	  pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,0);
	  switch(sciGetEntityType(pShortDraw))
	    {
	      /*Getting Number of Points in the ShortDraw
		Then for each polyline in the axis :
		-Reinitialize LongDraw
		-Copy last points of the ShortDraw into the LongDraw
		-Initialize LongDraw
		-Reinitialize the ShortDraw
		-copy last points of the last short into the new for continuity
		-initialize shortdraw
	      */
	    case SCI_POLYLINE:
	      {
		NbrPts = pPOLYLINE_FEATURE(pShortDraw)->n1;
		for (j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
		  {
		    pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);
		    pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,j);
		    pPOLYLINE_FEATURE(pShortDraw)->visible = FALSE;
		    //pPOLYLINE_FEATURE(pLongDraw)->visible = TRUE;

		    pPOLYLINE_FEATURE(pLongDraw)->n1 = 0;
		    C2F(dcopy)(&NbrPts,pPOLYLINE_FEATURE(pShortDraw)->pvx,&c__1,pPOLYLINE_FEATURE(pLongDraw)->pvx,&c__1);
		    C2F(dcopy)(&NbrPts,pPOLYLINE_FEATURE(pShortDraw)->pvy,&c__1,pPOLYLINE_FEATURE(pLongDraw)->pvy,&c__1);
		    pPOLYLINE_FEATURE(pLongDraw)->n1 = NbrPts;
		    pPOLYLINE_FEATURE(pShortDraw)->pvx[0] = pPOLYLINE_FEATURE(pLongDraw)->pvx[NbrPts-1];
		    pPOLYLINE_FEATURE(pShortDraw)->pvy[0] = pPOLYLINE_FEATURE(pLongDraw)->pvy[NbrPts-1];
		    pPOLYLINE_FEATURE(pShortDraw)->n1 = 1;
		  }
		break;
	      }
	    case SCI_SEGS:
	      {
		NbrPts = pSEGS_FEATURE(pShortDraw)->Nbr1;
		for (j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
		  {
		    pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);
		    pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,j);
		    pSEGS_FEATURE(pShortDraw)->visible = FALSE;
		    //		    pSEGS_FEATURE(pLongDraw)->visible = FALSE;

		    pSEGS_FEATURE(pLongDraw)->Nbr1 = 0;
		    pSEGS_FEATURE(pLongDraw)->Nbr2 = 0;
		    C2F(dcopy)(&NbrPts,pSEGS_FEATURE(pShortDraw)->vx ,&c__1,pSEGS_FEATURE(pLongDraw)->vx,&c__1);
		    C2F(dcopy)(&NbrPts,pSEGS_FEATURE(pShortDraw)->vy ,&c__1,pSEGS_FEATURE(pLongDraw)->vy,&c__1);
		    pSEGS_FEATURE(pLongDraw)->Nbr1 = NbrPts;
		    pSEGS_FEATURE(pLongDraw)->Nbr2 = NbrPts;
		  }
		break;
	      }
	    default:
	      sciprint("SCOPE ERROR : Cannot use scoRefreshDataBoundsX() with this type of object");
	      fprintf(stderr,"SCOPE ERROR : Cannot use scoRefreshDataBoundsX() with this type of object");
	      break;
	    }

	  scoSetNewDraw(pScopeMemory,i,0);
	}
    }
  if(bool == 1)
    {
      if(scoGetPointerScopeWindow(pScopeMemory) !=  NULL)
	{
	  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
	  sciDrawObj(scoGetPointerScopeWindow(pScopeMemory));
	}
      else
	{
	  scoScopeError(pScopeMemory,0);
	}
    }
  for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    {
      for (j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
	{
	  pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,0);
	  switch(sciGetEntityType(pShortDraw))
	    {
	    case SCI_POLYLINE:
	      {
		for (j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
		  {
		    pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);
		    pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,j);
		    pPOLYLINE_FEATURE(pShortDraw)->visible = TRUE;
		    //		    pPOLYLINE_FEATURE(pLongDraw)->visible = TRUE;
		  }
		break;
	      }
	    case SCI_SEGS:
	      {
		pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);
		pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,j);
		pSEGS_FEATURE(pShortDraw)->visible = TRUE;
		//		    pSEGS_FEATURE(pLongDraw)->visible = TRUE;

		break;
	      }
	    default:
	      {
		break;
	      }
	    }
	}
    }
}

void scoDrawScopeAmplitudeTimeStyle(ScopeMemory * pScopeMemory, double t)
{
  int c__1 = 1;
  int i,j;
  int NbrPtsShort,NbrPtsLong,inc;
  int current_period_counter;
  scoGraphicalObject pLongDraw;
  scoGraphicalObject pShortDraw;
  for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    {
      pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,0);
      switch(sciGetEntityType(pShortDraw))
	{
	case SCI_POLYLINE:
	  NbrPtsShort = pPOLYLINE_FEATURE(pShortDraw)->n1;
	  break;
	case SCI_SEGS:
	  NbrPtsShort = pSEGS_FEATURE(pShortDraw)->Nbr1;
	  break;
	default:
	  sciprint("SCOPE ERROR : Error in scoDrawScopeAmplitudeTimeStyle()");
	  fprintf(stderr,"SCOPE ERROR : Error in scoDrawScopeAmplitudeTimeStyle()");
	  break;
	}
      /*If this scope needs a redraw*/
      if (NbrPtsShort >= scoGetShortDrawSize(pScopeMemory,i))
	{
	  /*Block for Realloc*/
	  pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,0);
	  switch(sciGetEntityType(pLongDraw))
	    {
	    case SCI_POLYLINE:
	      NbrPtsLong = pPOLYLINE_FEATURE(pLongDraw)->n1;
	      break;
	    case SCI_SEGS:
	      NbrPtsLong = pSEGS_FEATURE(pLongDraw)->Nbr1;
	      break;
	    default:
	      sciprint("SCOPE ERROR : Error in scoDrawScopeAmplitudeTimeStyle()");
	      fprintf(stderr,"SCOPE ERROR : Error in scoDrawScopeAmplitudeTimeStyle()");
	      break;
	    }
	  if ((NbrPtsLong + scoGetShortDrawSize(pScopeMemory,i)) >= scoGetLongDrawSize(pScopeMemory,i))
	    {
	      for (j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
		{
		  pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);
		  scoReallocLongDraw(pLongDraw, NbrPtsLong, scoGetShortDrawSize(pScopeMemory,i),1000);
		}
	      scoSetLongDrawSize(pScopeMemory,i,NbrPtsLong + scoGetShortDrawSize(pScopeMemory,i)+1000);
	    }
	  pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,0);
	  /*End od block for Realloc*/

	  /*Block for Draw*/
	  switch(sciGetEntityType(pLongDraw))
	    {
	    case SCI_POLYLINE:
	      NbrPtsShort = pPOLYLINE_FEATURE(pShortDraw)->n1-1;
	      NbrPtsLong = pPOLYLINE_FEATURE(pLongDraw)->n1;
	      break;
	    case SCI_SEGS:
	      NbrPtsShort = pSEGS_FEATURE(pShortDraw)->Nbr1;
	      NbrPtsLong = pSEGS_FEATURE(pLongDraw)->Nbr1;
	      break;
	    default:
	      sciprint("SCOPE ERROR : Error in scoDrawScopeAmplitudeTimeStyle()");
	      fprintf(stderr,"SCOPE ERROR : Error in scoDrawScopeAmplitudeTimeStyle()");
	      break;
	    }

	  for (j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
	    {
	      pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,j);
	      pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);
	      if(pShortDraw != NULL)
		{
		  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
		  sciSetSelectedSubWin(scoGetPointerAxes(pScopeMemory,i));
		  pPOLYLINE_FEATURE(pShortDraw)->visible = TRUE;
		  sciDrawObj(pShortDraw);
		  pPOLYLINE_FEATURE(pShortDraw)->visible = FALSE;
		}
	      else
		{
		  scoScopeError(pScopeMemory,0);
		}
	      /*End of Block for Draw*/

	      /*Block for Memory*/
	      inc = NbrPtsLong!=0; //A trick to handle the trace empty case
	      switch(sciGetEntityType(pShortDraw))
		{
		case SCI_POLYLINE:
		  C2F(dcopy)(&NbrPtsShort,pPOLYLINE_FEATURE(pShortDraw)->pvx+inc,&c__1,pPOLYLINE_FEATURE(pLongDraw)->pvx+NbrPtsLong,&c__1);
		  C2F(dcopy)(&NbrPtsShort,pPOLYLINE_FEATURE(pShortDraw)->pvy+inc,&c__1,pPOLYLINE_FEATURE(pLongDraw)->pvy+NbrPtsLong,&c__1);
		  pPOLYLINE_FEATURE(pLongDraw)->n1 = NbrPtsLong+NbrPtsShort;
		  break;
		case SCI_SEGS:
		  C2F(dcopy)(&NbrPtsShort,pSEGS_FEATURE(pShortDraw)->vx,&c__1,pSEGS_FEATURE(pLongDraw)->vx+NbrPtsLong,&c__1);
		  C2F(dcopy)(&NbrPtsShort,pSEGS_FEATURE(pShortDraw)->vy,&c__1,pSEGS_FEATURE(pLongDraw)->vy+NbrPtsLong,&c__1);
		  pSEGS_FEATURE(pLongDraw)->Nbr1 = NbrPtsLong+NbrPtsShort;
		  pSEGS_FEATURE(pLongDraw)->Nbr2 = NbrPtsLong+NbrPtsShort;
		  break;
		default:
		  sciprint("SCOPE ERROR : Error in scoDrawScopeAmplitudeTimeStyle()");
		  fprintf(stderr,"SCOPE ERROR : Error in scoDrawScopeAmplitudeTimeStyle()");
		  break;
		}
	      /*End of Block for Memory*/
	    }

	  /*Block for Detecting redrawing of x-axis values and others activities*/
	  current_period_counter = (int)(t/scoGetPeriod(pScopeMemory,i));
	  if (current_period_counter != scoGetPeriodCounter(pScopeMemory,i))
	    {
	      scoSetNewDraw(pScopeMemory,i,-1);
	      scoSetPeriodCounter(pScopeMemory,i,current_period_counter);
	    }
	  else
	    {
	      for (j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
		{
		  pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,j);
		  switch(sciGetEntityType(pShortDraw))
		    {
		    case SCI_POLYLINE:
		      /*Last point is next first point*/
		      pPOLYLINE_FEATURE(pShortDraw)->pvx[0] = pPOLYLINE_FEATURE(pShortDraw)->pvx[NbrPtsShort];
		      pPOLYLINE_FEATURE(pShortDraw)->pvy[0] = pPOLYLINE_FEATURE(pShortDraw)->pvy[NbrPtsShort];
		      pPOLYLINE_FEATURE(pShortDraw)->n1 = 1;
		      break;
		    case SCI_SEGS:
		      /*Do Nothing*/
		      break;
		    default:
		      sciprint("SCOPE ERROR : Error in scoDrawScopeAmplitudeTimeStyle()");
		      fprintf(stderr,"SCOPE ERROR : Error in scoDrawScopeAmplitudeTimeStyle()");
		      break;
		    }
		}
	    }
	}
    }
}

scoGraphicalObject scoCreatePolyline(scoGraphicalObject pAxes, scoInteger polyline_size,int color)
{
  scoGraphicalObject pPolyline;
  double * vx, * vy, *vz;
  vx = (double*)scicos_malloc(polyline_size*sizeof(double));
  vy = (double*)scicos_malloc(polyline_size*sizeof(double));
  if (pSUBWIN_FEATURE(pAxes)->axes.axes_visible[2] == TRUE) //3D
    {
      vz = (double*)scicos_malloc(polyline_size*sizeof(double));
      pPolyline=ConstructPolyline(pAxes,vx,vy,vz,0,polyline_size,1,1, NULL,NULL,NULL,NULL,NULL,FALSE,FALSE,TRUE,FALSE);
    }
  else //2D
    {
      pPolyline=ConstructPolyline(pAxes,vx,vy,NULL,0,polyline_size,1,1, NULL,NULL,NULL,NULL,NULL,FALSE,FALSE,TRUE,FALSE);
    }
  pPOLYLINE_FEATURE(pPolyline)->n1 = 0;
  if (color >= 0)
    {
      sciSetForeground(pPolyline, color);
      sciSetIsLine(pPolyline, 1);
      sciSetLineStyle(pPolyline, 1);
      sciSetMarkStyle(pPolyline, 0);
      sciSetIsMark(pPolyline, 0);
    }
  else if (color < 0)
    {
      sciSetMarkForeground(pPolyline, -1);
      sciSetIsLine(pPolyline, 0);
      sciSetLineStyle(pPolyline, 0);
      sciSetIsMark(pPolyline, 1);
      sciSetMarkStyle(pPolyline, -color);
    }
  sciSetIsClipping(pPolyline, 0);
  return pPolyline;
}

void scoAddPolylineForShortDraw(ScopeMemory * pScopeMemory, int i, int j, int color)
{
  scoGraphicalObject pShortDraw;
  pShortDraw = scoCreatePolyline(scoGetPointerAxes(pScopeMemory,i),scoGetShortDrawSize(pScopeMemory,i),color);
  scoSetHandleFromPointerShortDraw(pScopeMemory,i,j,pShortDraw);
}

void scoAddPolylineForLongDraw(ScopeMemory * pScopeMemory, int i, int j, int color)
{

  scoGraphicalObject pLongDraw;
  pLongDraw = scoCreatePolyline(scoGetPointerAxes(pScopeMemory,i),scoGetLongDrawSize(pScopeMemory,i),color);
  scoSetHandleFromPointerLongDraw(pScopeMemory,i,j,pLongDraw);

}

void scoAddCoupleOfPolylines(ScopeMemory * pScopeMemory, int * colors)
{
  int i,j;
  int inc = -1;
  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
  for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    {
      for(j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
	{
	  if(colors != NULL)
	    {
	      scoAddPolylineForShortDraw(pScopeMemory,i,j,colors[j+1+inc]);
	      scoAddPolylineForLongDraw(pScopeMemory,i,j,colors[j+1+inc]);
	    }
	  else
	    {
	      scoAddPolylineForShortDraw(pScopeMemory,i,j,-1);
	      scoAddPolylineForLongDraw(pScopeMemory,i,j,-1);
	    }
	}
      inc = j+inc; //not +1 because of we have exited the loop
    }
}

void scoDelCoupleOfPolylines(ScopeMemory * pScopeMemory)
{
  int i,j;
  int c__1 = 1;
  scoGraphicalObject pLongDraw;
  scoGraphicalObject pShortDraw;
  int NbrPtsLong, NbrPtsShort;
  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
  if(scoGetPointerScopeWindow(pScopeMemory) != NULL)
    {
      for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
	{
	  pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,0);
	  NbrPtsLong = pPOLYLINE_FEATURE(pLongDraw)->n1;
	  if(NbrPtsLong + scoGetShortDrawSize(pScopeMemory,i) > scoGetLongDrawSize(pScopeMemory,i))
	    {
	      for(j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
		{
		  pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);
		  scoReallocLongDraw(pLongDraw, NbrPtsLong, scoGetShortDrawSize(pScopeMemory,i), 1000);
		}
	      scoSetLongDrawSize(pScopeMemory,i,NbrPtsLong + scoGetShortDrawSize(pScopeMemory,i) + 1000);
	    }
      
	  pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,0);
	  NbrPtsShort = pPOLYLINE_FEATURE(pShortDraw)->n1;
	  pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,0);
	  NbrPtsLong = pPOLYLINE_FEATURE(pLongDraw)->n1;
      
	  for(j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
	    {
	      pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,j);
	      pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);

	      C2F(dcopy)(&NbrPtsShort,pPOLYLINE_FEATURE(pShortDraw)->pvx,&c__1,pPOLYLINE_FEATURE(pLongDraw)->pvx+NbrPtsLong,&c__1);
	      C2F(dcopy)(&NbrPtsShort,pPOLYLINE_FEATURE(pShortDraw)->pvy,&c__1,pPOLYLINE_FEATURE(pLongDraw)->pvy+NbrPtsLong,&c__1);

	      pPOLYLINE_FEATURE(pLongDraw)->n1 = NbrPtsLong + NbrPtsShort;
	      pPOLYLINE_FEATURE(pShortDraw)->n1 = 0;
	      DestroyPolyline(pShortDraw);
	    }
	}
      sciSetUsedWindow(scoGetWindowID(pScopeMemory));
      sciDrawObj(scoGetPointerScopeWindow(pScopeMemory));
    }
}

void scoAddCoupleOfSegments(ScopeMemory * pScopeMemory, int * color)
{
  int i,j;
  int longdraw_size;
  double vx1[2], vy1[2];
  double * vx2, * vy2;
  int style[1];
  scoGraphicalObject pShortDraw;
  scoGraphicalObject pLongDraw;
  scoGraphicalObject pAxes;

  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
  for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    {
      longdraw_size = scoGetLongDrawSize(pScopeMemory,i);
 
      vx1[0] = 0.0;
      vx1[1] = 0.0;
      vy1[0] = 0.0;
      vy1[1] = 0.0;
      
      vx2 = (double*)scicos_malloc(longdraw_size*sizeof(double));
      vy2 = (double*)scicos_malloc(longdraw_size*sizeof(double));

      for(j = 0 ; j < longdraw_size ; j++)
	{
	  vx2[j] = 0.0;
	  vy2[j] = 0.0;
	}
      
      style[0] = 0;

      pAxes = scoGetPointerAxes(pScopeMemory,i);

      for(j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
	{

	  pLongDraw = ConstructSegs(pAxes, 0, vx2, vy2, longdraw_size, longdraw_size, NULL, NULL,0,style, 0, TRUE, 0, 0);
	  pSEGS_FEATURE(pLongDraw)->Nbr1 = 0;
	  pSEGS_FEATURE(pLongDraw)->Nbr2 = 0;
	  if (color == NULL)
	    {
	      sciSetForeground(pLongDraw, -1);
	    }
	  else
	    {
	      sciSetForeground(pLongDraw, color[j]);
	    }
	  sciSetIsLine(pLongDraw, 1);
	  sciSetLineStyle(pLongDraw, 1);
	  sciSetMarkStyle(pLongDraw, 0);
	  sciSetIsMark(pLongDraw, 0);
	  sciSetIsClipping(pLongDraw, 0);
	  scoSetHandleFromPointerLongDraw(pScopeMemory,i,j,pLongDraw);

	  pShortDraw = ConstructSegs(pAxes, 0, vx1, vy1, 2, 2, NULL, NULL,0,style, 0, TRUE, 0, 0);
	  pSEGS_FEATURE(pShortDraw)->Nbr1 = 0;
	  pSEGS_FEATURE(pShortDraw)->Nbr2 = 0;
	  if (color == NULL)
	    {
	      sciSetForeground(pShortDraw, -1);
	    }
	  else
	    {
	      sciSetForeground(pShortDraw, color[j]);
	    }
	  sciSetIsLine(pShortDraw, 1);
	  sciSetLineStyle(pShortDraw, 1);
	  sciSetMarkStyle(pShortDraw, 0);
	  sciSetIsMark(pShortDraw, 0);
	  sciSetIsClipping(pShortDraw, 0);
	  scoSetHandleFromPointerShortDraw(pScopeMemory,i,j,pShortDraw);
	}
      scicos_free(vx2);
      scicos_free(vy2);
    }
}

void scoDelCoupleOfSegments(ScopeMemory * pScopeMemory)
{


}


scoGraphicalObject scoCreateSphere(scoGraphicalObject pAxes, int radius, int color)
{
  scoGraphicalObject pSphere;
  pSphere=ConstructArc(pAxes, 0, 0, radius, radius, 0, 23040, &color, &color, TRUE, FALSE);
  return pSphere;
}

void scoAddSphereForShortDraw(ScopeMemory * pScopeMemory, int i, int j, int radius, int color)
{
  scoGraphicalObject pShortDraw;
  pShortDraw = scoCreateSphere(scoGetPointerAxes(pScopeMemory,i),radius,color);
  scoSetHandleFromPointerShortDraw(pScopeMemory,i,j,pShortDraw);
}

void scoAddSphereForLongDraw(ScopeMemory * pScopeMemory, int i, int j, int radius, int color)
{
  scoGraphicalObject pLongDraw;
  pLongDraw = scoCreateSphere(scoGetPointerAxes(pScopeMemory,i),radius,color);
  scoSetHandleFromPointerLongDraw(pScopeMemory,i,j,pLongDraw);
}

void scoAddCoupleOfSpheres(ScopeMemory * pScopeMemory, int * radius, int * colors)
{
  int i,j;
  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
  for (i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    { 
      for(j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
	{
	  if(colors != NULL)
	    {
	      scoAddSphereForShortDraw(pScopeMemory,i,j,radius[i+j], colors[i+j]);
	      scoAddSphereForLongDraw(pScopeMemory,i,j,radius[i+j], colors[i+j]);
	    }
	  else
	    {
	      scoAddSphereForShortDraw(pScopeMemory,i,j,1,1);
	      scoAddSphereForLongDraw(pScopeMemory,i,j,1,1);
	    }
	}
    }
}

scoGraphicalObject scoCreateRectangle(scoGraphicalObject pAxes, double x, double y, double width, double height)
{
  scoGraphicalObject pRectangle;
  pRectangle = ConstructRectangle(pAxes,x,y,height,width,0,0,NULL,NULL,0,1,0,FALSE);
  return pRectangle;
}

void scoAddRectangleForLongDraw(ScopeMemory * pScopeMemory, int i, int j, double x, double y, double width, double height)
{
  scoGraphicalObject pLongDraw;
  pLongDraw = scoCreateRectangle(scoGetPointerAxes(pScopeMemory,i),x,y,width,height);
  scoSetHandleFromPointerLongDraw(pScopeMemory,i,j,pLongDraw);
}

void scoAddTitlesScope(ScopeMemory * pScopeMemory, char * x, char * y, char * z)
{
  int i;
  int nxname;
  int kfun;
  char buf[40];
  int i__1;
  char ** title=NULL;
  char * x_title=NULL;
  char * y_title;

  title = (char**)scicos_malloc(scoGetNumberOfSubwin(pScopeMemory)*sizeof(char*));
  for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    {
      title[i] = (char*)malloc(500*sizeof(char));
      sprintf(title[i],"Graphic %d",i+1);
    }

  
  x_title = x;
  
  y_title = y;

  for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    {
      sciSetText(pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,i))->mon_title,title[i],500);
      sciSetText(pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,i))->mon_x_label,x_title,500);
      sciSetText(pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,i))->mon_y_label,y_title,500);

      
      sciSetFontDeciWidth(pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,i))->mon_x_label, 0);
      sciSetFontDeciWidth(pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,i))->mon_y_label, 0);
      sciSetFontDeciWidth(pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,i))->mon_title, 0);
    }

  if(z!= NULL)
    {
      char * z_title;
      z_title = z;

      for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
	{
	  sciSetFontDeciWidth(pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,i))->mon_z_label, 0);
	  sciSetText(pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,i))->mon_z_label,z_title,500);
	}
    }

  scicos_free(title);

  /* Code for naming the window, for the moment I don't know if I put this in the standard constructor or not*/
  nxname = 40;
  kfun = get_block_number();
  C2F(getlabel)(&kfun, buf, &nxname);
  if(nxname > 39)
    {
      nxname = 39;
    }
  i__1 = nxname;
  *(buf+i__1) = *"\000";
  if((nxname == 1 && *(unsigned char *)buf == ' ') || (nxname ==0))
    {
      /*do nothing*/
    }
  else
    {
      sciSetName(scoGetPointerScopeWindow(pScopeMemory), buf ,nxname);
    }
  /*End of code for naming window */
}

void scoDrawScopeXYStyle(ScopeMemory * pScopeMemory)
{
  scoGraphicalObject Pinceau;
  scoGraphicalObject Trait;
  int NbrPtsShort, NbrPtsLong;
  int c__1 = 1;

  Pinceau = scoGetPointerShortDraw(pScopeMemory,0,0);
  Trait = scoGetPointerLongDraw(pScopeMemory,0,0);

  NbrPtsShort = pPOLYLINE_FEATURE(Pinceau)->n1;
  if(NbrPtsShort >= scoGetShortDrawSize(pScopeMemory,0))
    {
      sciSetUsedWindow(scoGetWindowID(pScopeMemory));
      sciDrawObj(Pinceau);
      NbrPtsLong = pPOLYLINE_FEATURE(Trait)->n1;
      if(NbrPtsLong + NbrPtsShort >= scoGetLongDrawSize(pScopeMemory,0))
	{
	  scoReallocLongDraw(Trait, NbrPtsLong, NbrPtsShort, 5000);
	  scoSetLongDrawSize(pScopeMemory, 0, NbrPtsLong + NbrPtsShort + 5000);
	}
      NbrPtsLong = pPOLYLINE_FEATURE(Trait)->n1;
      C2F(dcopy)(&NbrPtsShort,pPOLYLINE_FEATURE(Pinceau)->pvx,&c__1,pPOLYLINE_FEATURE(Trait)->pvx+NbrPtsLong,&c__1);
      C2F(dcopy)(&NbrPtsShort,pPOLYLINE_FEATURE(Pinceau)->pvy,&c__1,pPOLYLINE_FEATURE(Trait)->pvy+NbrPtsLong,&c__1);
      if(pPOLYLINE_FEATURE(Pinceau)->pvz != NULL)
	{
	  C2F(dcopy)(&NbrPtsShort,pPOLYLINE_FEATURE(Pinceau)->pvz,&c__1,pPOLYLINE_FEATURE(Trait)->pvz+NbrPtsLong,&c__1);
	}      
      pPOLYLINE_FEATURE(Trait)->n1 = NbrPtsLong + NbrPtsShort;
      pPOLYLINE_FEATURE(Pinceau)->pvx[0] = pPOLYLINE_FEATURE(Pinceau)->pvx[NbrPtsShort-1];
      pPOLYLINE_FEATURE(Pinceau)->pvy[0] = pPOLYLINE_FEATURE(Pinceau)->pvy[NbrPtsShort-1];
      if(pPOLYLINE_FEATURE(Pinceau)->pvz != NULL)
	{
	  pPOLYLINE_FEATURE(Pinceau)->pvz[0] = pPOLYLINE_FEATURE(Pinceau)->pvz[NbrPtsShort-1];
	}
      pPOLYLINE_FEATURE(Pinceau)->n1 = 1;
    }
}
