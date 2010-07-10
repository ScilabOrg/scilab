/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han DONG
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_gui.h"
/*--------------------------------------------------------------------------*/
#include "CreateUIControl.h"
#include "MALLOC.h" /* MALLOC */
#include "localization.h"
#include "stricmp.h"
#include "stack-c.h"
#include "sciprint.h"
#include "ObjectStructure.h"
#include "SetPropertyStatus.h"
#include "SetHashTable.h"
#include "UiTable.h" /* setCurentFigureAsUiTableParent */
#include "Scierror.h"
#include "WindowList.h" /* getFigureFromIndex */
#include "Widget.h" /* requestWidgetFocus */
#include "SetUicontrolPosition.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_uitable(char *fname, unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0, stkAdr = 0, k = 0;
  
  int setStatus = SET_PROPERTY_SUCCEED;

  int inputIndex = 0, beginIndex = 0;

  char *propertyName=NULL;

  sciPointObj *pParent=NULL;
  sciPointObj *pUicontrol=NULL;

  unsigned long GraphicHandle = 0;

  int lw = 0;

  CheckLhs(0,1);

  if(Rhs == 0)
  {
    GraphicHandle=sciGetHandle(CreateUIControl("uitable"));
    setCurentFigureAsUiTableParent(sciGetPointerFromHandle(GraphicHandle));
  }
  else if(Rhs == 1)
  {
    /* Create a uitable in figure given as parameter */
    /* Or give focus to the uicontrol given as parameter */
    if (VarType(1) != sci_handles)
    {
      lw = 1 + Top - Rhs;
      C2F(overload)(&lw,"uicontrol",9);
      return FALSE;
    }
    else
    {
      GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);

      if (nbRow*nbCol != 1)
      {
        Scierror(999,_("%s: Wrong size for input argument #%d: A graphic handle expected.\n"),fname, 1);
        return FALSE;
      }
      pParent=sciGetPointerFromHandle((long)*hstk(stkAdr));

      if ( (pParent != NULL) && (sciGetEntityType (pParent) == SCI_UICONTROL) ) /* Focus management */
      {
        GraphicHandle = (long)*hstk(stkAdr);
        if (pUICONTROL_FEATURE(pParent)->style == SCI_UIFRAME) /* Frame style uicontrol */
        {
          requestFrameFocus(pParent);
        }
        else
        {
          requestWidgetFocus(pParent);
        }
      }
      else if ( (pParent != NULL) && ((sciGetEntityType (pParent) == SCI_FIGURE) || (sciGetEntityType (pParent) == SCI_UIMENU)) )
      {
        /* Create a new uitable */
        GraphicHandle=sciGetHandle(CreateUIControl("uitable"));
              
        /* First parameter is the parent */
        setStatus = callSetProperty(sciGetPointerFromHandle(GraphicHandle), stkAdr, sci_handles, nbRow, nbCol, "parent");
        if (setStatus == SET_PROPERTY_ERROR)
        {
          Scierror(999, _("%s: Could not set property '%s'.\n"), fname, propertyName);
          return FALSE;
        }
      }
      else
      {
        Scierror(999,_("%s: Wrong type for input argument #%d: A '%s', '%s' or '%s' handle expected.\n"),fname,1,"Uicontrol","Figure","Uimenu");
        return FALSE;
      }
    }
  }
  else
  {
    if ((VarType(1) != sci_handles) && (VarType(1) != sci_strings))
    {
      lw = 1 + Top - Rhs;
      C2F(overload)(&lw,"uicontrol",9);
      return FALSE;
    }

    /**
       * Odd number of input arguments
       * First input is the parent ID
       * All event inputs are property names
       * All odd (except first) inputs are property values
       */
    if (Rhs%2==1)
    {
      if (VarType(1) != sci_handles)
      {
        if (VarType(1) == sci_matrix)
        {
          GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
          if (nbRow*nbCol == 1)
          {
            pParent = getFigureFromIndex((int)(*stk(stkAdr)));

            if ( (pParent == NULL) || (sciGetEntityType (pParent) != SCI_FIGURE) && !((sciGetEntityType (pParent) == SCI_UICONTROL) && (pUICONTROL_FEATURE(pParent)->style == SCI_UIFRAME)))
            {
              Scierror(999,_("%s: Wrong type for input argument #%d: A '%s' or a '%s' handle expected.\n"),fname,1,"Figure", "Frame uicontrol");
              return FALSE;
            }
          }
          else
          {
            Scierror(999,_("%s: Wrong size for input argument #%d: A graphic handle expected.\n"),fname, 1);
            return FALSE;
          }
        }
        else
        {
          Scierror(999,_("%s: Wrong type for input argument #%d: A '%s' or a '%s' handle expected.\n"),fname, 1,"Figure", "Frame uicontrol");
          return FALSE;
        }
      }
      else /* Get parent ID */
      {
        GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);

        if (nbRow*nbCol != 1)
        {
          Scierror(999,_("%s: Wrong size for input argument #%d: A '%s' or a '%s' handle expected.\n"),fname, 1, "Figure", "Frame uicontrol");
          return FALSE;
        }
        pParent=sciGetPointerFromHandle((long)*hstk(stkAdr));
        if ( (pParent == NULL) || (sciGetEntityType (pParent) != SCI_FIGURE) && !((sciGetEntityType (pParent) == SCI_UICONTROL) && (pUICONTROL_FEATURE(pParent)->style == SCI_UIFRAME)))
        {
          Scierror(999,_("%s: Wrong type for input argument #%d: A '%s' or a '%s' handle expected.\n"),fname, 1, "Figure", "Frame uicontrol");
          return FALSE;
        }
      }

      // First input parameter which is a property name
      beginIndex = 2;
    }
    /**
     * Even number of input arguments
     * All odd inputs are property names
     * All even inputs are property values
     */
    else
    {
      // First input parameter which is a property name
      beginIndex = 1;
    }

    /* Create a new uicontrol */
    pUicontrol = CreateUIControl("uitable");
    if (pUicontrol == NULL) /* Error in creation */
    {
      Scierror(999, _("%s: Could not create 'Uicontrol' handle.\n"), fname);
      return FALSE;
    }
    GraphicHandle=sciGetHandle(pUicontrol);
    setCurentFigureAsUiTableParent(sciGetPointerFromHandle(GraphicHandle));

    /* Loop through all user inputted property names and calls corresponding functinos */
    for(inputIndex = beginIndex; inputIndex<Rhs; inputIndex = inputIndex+2)
    {
      if (VarType(inputIndex) != sci_strings)
      {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, inputIndex);
        return FALSE;
      }
      else
      {
        GetRhsVar(inputIndex, STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);
        propertyName = cstk(stkAdr);

        if(strcmp(propertyName, "colnames")==0)
        {
          GetRhsVar((inputIndex+1), STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);
          setStatus = callSetProperty(sciGetPointerFromHandle(GraphicHandle), stkAdr, sci_strings, nbRow, nbCol, "colnames");
        }
        else if(strcmp(propertyName, "rownames")==0)
        {
          GetRhsVar((inputIndex+1), STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);
          setStatus = callSetProperty(sciGetPointerFromHandle(GraphicHandle), stkAdr, sci_strings, nbRow, nbCol, "rownames");
        }
        else if(strcmp(propertyName, "tabledata")==0)
        {
          GetRhsVar((inputIndex+1), STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);
          setStatus = callSetProperty(sciGetPointerFromHandle(GraphicHandle), stkAdr, sci_strings, nbRow, nbCol, "tabledata");
        }
        else if(strcmp(propertyName, "position")==0)
        {
          GetRhsVar((inputIndex+1),MATRIX_OF_DOUBLE_DATATYPE,&nbRow,&nbCol,&stkAdr);
          setStatus = callSetProperty(sciGetPointerFromHandle(GraphicHandle), stkAdr, sci_matrix, nbRow, nbCol, "position");
        }
        else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d\n"), fname, inputIndex);
        }
      }
    }
  }

  /* Create return variable */
  nbRow = 1;
  nbCol = 1;
  CreateVar(Rhs+1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
  *hstk(stkAdr) = GraphicHandle;

  LhsVar(1)=Rhs+1;

  C2F(putlhsvar)();

  return TRUE;
}
/*--------------------------------------------------------------------------*/
