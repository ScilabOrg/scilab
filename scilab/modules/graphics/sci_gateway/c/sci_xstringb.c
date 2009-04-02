
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xstringb.c                                                   */
/* desc : interface for xstringb routine                                  */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_xstringb.h"
#include "stack-c.h"
#include "sci_demo.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "Scierror.h"
#include "GetProperty.h"
#include "MALLOC.h"
#include "sciCall.h"
#include "freeArrayOfString.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_xstringb(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,m2,n2,l2,m3,n3,m4,n4,l4,m5,n5,l5,m6,n6,l6;
  BOOL autoSize = TRUE ;
  double x,y,w,hx;
  char **Str;
  double rect[4],angle=0;
  long hdlstr;
  double userSize[2] ;

  if ( Rhs <= 0 )
  {
    /* demo */
    char demo[] = "scf(); axes = gca() ; axes.axes_visible = 'on' ; str = ['Scilab','is';'not','Esilab']; xstringb(0.1,0.1,str,0.5,0.5,'fill') ; txt = gce() ; txt.box = 'on' ;" ;
    sci_demo( fname, demo, FALSE ) ;
    return 0 ;
  }

  CheckRhs(5,6);

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1); CheckScalar(1,m1,n1);  x = *stk(l1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2); CheckScalar(2,m2,n2);  y = *stk(l2);
  GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&m3,&n3,&Str);
  if ( m3*n3 == 0 )
	{
		LhsVar(1)=0;
		C2F(putlhsvar)();
		return 0;
	} 

  GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE,&m4,&n4,&l4); CheckScalar(4,m4,n4);  w = *stk(l4);
  GetRhsVar(5,MATRIX_OF_DOUBLE_DATATYPE,&m5,&n5,&l5); CheckScalar(5,m5,n5);  hx = *stk(l5);

  if (Rhs == 6)
  {
    GetRhsVar(6,STRING_DATATYPE,&m6,&n6,&l6);
    if ( m6*n6 !=0 && strcmp(cstk(l6),"fill") == 0 )
    {
      autoSize = FALSE ;
    } 
    else
    {
      Scierror(999,_("%s: Wrong value for input argument #%d: '%s' expected.\n"), fname, 6, "fill");
      return 0;
    }
  }

  userSize[0] = w ;
  userSize[1] = hx ;
  Objstring (Str,m3,n3,x,y,&angle,rect,autoSize,userSize,&hdlstr,TRUE,NULL,NULL,FALSE,TRUE,FALSE,ALIGN_CENTER);

	freeArrayOfString(Str,m3*n3);

  LhsVar(1)=0;
	C2F(putlhsvar)();

  return 0;

}
/*--------------------------------------------------------------------------*/
