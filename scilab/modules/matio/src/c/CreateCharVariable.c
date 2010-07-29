/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT 
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "api_scilab.h"
#include "CreateMatlabVariable.h"
#include "freeArrayOfString.h"
#include "os_strdup.h"

#define MATIO_ERROR if(_SciErr.iErr)	     \
    {					     \
      printError(&_SciErr, 0);		     \
      return 0;				     \
    }

int CreateCharVariable(void *pvApiCtx, int iVar, matvar_t *matVariable, int * parent, int item_position)
{
  int nbRow = 0, nbCol = 0;
  char **charData = NULL;
  int K = 0, L = 0;
  SciErr _SciErr;

  if(matVariable->rank==2) /* 2-D array */
    {
      nbRow = matVariable->dims[0];
      nbCol = nbRow==0 ? 0 : 1; /* In Scilab empty string has size 0x0 */
      
      if (nbRow != 0)
	{
	  charData =  (char**) MALLOC(sizeof(char*) * nbRow);
	  if (charData==NULL)
	    {
	      Scierror(999, _("%s: No more memory.\n"), "CreateCharVariable");
	      return FALSE;
	    }
	}
      
      for (K=0; K<nbRow; K++)
	{
	  charData[K] =  (char*) MALLOC(sizeof(char*) * (matVariable->dims[1] + 1));
	  if (charData[K]==NULL)
	    {
	      Scierror(999, _("%s: No more memory.\n"), "CreateCharVariable");
	      return FALSE;
	    }
	}
      
      /* Fill items: data in Matlab file is stored columnwise */
      for(K=0; K<matVariable->dims[0]; K++) /* Loop over items */
	{
	  for(L=0; L<matVariable->dims[1]; L++) /* Loop over chars */
	    {
	      if (matVariable->fp->version != MAT_FT_MAT4) /* MAT_FT_MAT4 format ==> data is a char* pointer */
		{
		  charData[K][L] = ((char *)matVariable->data)[L*matVariable->dims[0]+K];
		}
	      else /* MAT_FT_MAT4 format ==> data is a double* pointer */
		{
		  charData[K][L] = (char) ((double *)matVariable->data)[L*matVariable->dims[0]+K];
		}
	    }
	  charData[K][L] = '\0';
	}
      
      if (nbRow*nbCol != 0)
	{
	  if (parent==NULL)
	    {
	      _SciErr = createMatrixOfString(pvApiCtx, iVar, nbRow, nbCol, charData); MATIO_ERROR;
	    }
	  else
	    {
	      _SciErr = createMatrixOfStringInList(pvApiCtx, iVar, parent, item_position, nbRow, nbCol, charData); MATIO_ERROR;
	    }
	}
      else /* Empty character string */
	{
	  if (parent==NULL)
	    {
	      createSingleString(pvApiCtx, iVar, "\0");
	    }
	  else
	    {
	      char ** tmp_char = (char **)MALLOC(sizeof(char *));
	      tmp_char[0] = os_strdup("\0");
	      _SciErr = createMatrixOfStringInList(pvApiCtx, iVar, parent, item_position, 1, 1, tmp_char); MATIO_ERROR;
	      freeArrayOfString(tmp_char, 1);
	    }
	}
      
      freeArrayOfString(charData,nbRow*nbCol);
    }
  else /* Multi-dimension array -> Scilab HyperMatrix */
    {
      Scierror(999, _("%s: N-D arrays of chars not implemented.\n"), "CreateCharVariable");
      return FALSE;
    }
  
  return TRUE;
}
