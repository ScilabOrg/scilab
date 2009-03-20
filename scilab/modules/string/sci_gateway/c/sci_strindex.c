/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Cong WU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* desc : search position of a character string in an other string        */
/*        using regular expression .                                      */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "gw_string.h"
#include "pcre.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "pcre_private.h"
#include "BOOL.h"
#include "pcre_error.h"
#include "Scierror.h"
#include "charEncoding.h"
/*------------------------------------------------------------------------*/
#define CHAR_S "s"
#define CHAR_R "r"
/*------------------------------------------------------------------------*/
struct In
{
		int data;
		int position;
} ;
/*------------------------------------------------------------------------*/
int cmp( const void *a ,const void *b)
{
return (*(struct In *)a).data > (*(struct In *)b).data ? 1 : -1;
}
/*------------------------------------------------------------------------*/
int C2F(sci_strindex)(char *fname,unsigned long fname_len)
{
	BOOL bStrindex_with_pattern = FALSE;
	int outIndex = 0;
	int numRow = 1;
    int *next= NULL;
	int i = 0;

	CheckRhs(2,3);
	CheckLhs(1,2);

	if (Rhs == 3)
	{
		int m3 = 0;
		int n3 = 0;
		char **Strings_Input3 = NULL;
		int m3n3 = 0; /* m3 * n3 */

		if (VarType(3) != sci_strings)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Character expected.\n"),fname,3);
			return 0;
		}
		GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&m3,&n3,&Strings_Input3);
		m3n3 = m3*n3;

		if (m3n3 != 1)
		{
			freeArrayOfString(Strings_Input3,m3n3);
			Scierror(999,_("%s: Wrong type for input argument #%d: Character expected.\n"),fname,3);
			return 0;
		}

		if ( (strcmp(Strings_Input3[0],CHAR_R) == 0) || (strcmp(Strings_Input3[0],CHAR_S) == 0) )
		{
			if (strcmp(Strings_Input3[0],CHAR_R) == 0)
			{
				bStrindex_with_pattern = TRUE;
			}
			else
			{
				bStrindex_with_pattern = FALSE;
			}
			freeArrayOfString(Strings_Input3,m3n3);
		}
		else
		{
			freeArrayOfString(Strings_Input3,m3n3);
			Scierror(999,_("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"),fname,3,"s","r");
			return 0;
		}
	}

	if (VarType(1) == sci_matrix)
	{
		int m1 = 0;
		int n1 = 0;
		int l1 = 0;

		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
		if ((m1 == 0) && (n1 == 0))
		{
			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
			LhsVar(1) = Rhs+1 ;
			C2F(putlhsvar)();
			return 0;
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"),fname,3);
			return 0;
		}
	}

	if ( (VarType(1) == sci_strings) && (VarType(2) == sci_strings) )
	{
		int m1 = 0, n1 = 0;
		char **Strings_Input1 = NULL;
		int m1n1 = 0; /* m1 * n1 */

		int m2 = 0, n2 = 0;
		char **Strings_Input2 = NULL;
		int m2n2 = 0; /* m2 * n2 */

		struct In *values=NULL;

		int nbValues = 0;
		int nbposition = 0;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Strings_Input1);
		m1n1 = m1*n1;

		if (m1n1 != 1)
		{
			freeArrayOfString(Strings_Input1,m1n1);
			Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
			return 0;
		}

		GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Strings_Input2);
		m2n2 = m2*n2;

		if ( (m2 != 1) && (n2 != 1) )
		{
			freeArrayOfString(Strings_Input1,m1n1);
			freeArrayOfString(Strings_Input2,m2n2);
			Scierror(999,_("%s: Wrong type for input argument #%d: Row vector of strings or column vector of strings expected.\n"),fname,2);
			return 0;
		}

		if ( (int)strlen(Strings_Input1[0]) == 0 )
		{
			values= (struct In*)MALLOC(sizeof(struct In));
		}
		else
		{
			//max size of result array is size of the InputStrng * number of searched strings
			values = (struct In *)MALLOC( sizeof(struct In) * ( strlen(Strings_Input1[0]) ) * m2n2);
		}

		/*regexp*/
		if (bStrindex_with_pattern)
		{
			int x = 0;
			pcre_error_code w = PCRE_FINISHED_OK;

			int Output_Start = 0;
			int Output_End = 0;

			/* We use pcre library */
			for (x = 0; x < m2n2; ++x)
			{
				w = pcre_private(Strings_Input1[0],Strings_Input2[x],&Output_Start,&Output_End);
				if ( w == PCRE_FINISHED_OK)
				{
					values[nbValues++].data = Output_Start+1; /* adding the answer into the outputmatrix */
					values[nbposition++].position = x+1;        /* The number according to the str2 matrix */
				}
				else
				{
					if (w != NO_MATCH)
					{
						pcre_error(fname,w);
						return 0;
					}
					break;
				}
			}

			qsort(values,nbValues,sizeof(values[0]),cmp);

		}
		else
		{
			/* We don't use pcre library */
			int x = 0;
			int pos = 0;

			//convert original string to UTF-16 string
			wchar_t *wOrigStr = to_wide_string(Strings_Input1[0]);

			if(wOrigStr)
			{
				for (x=0; x < m2n2 ;++x)
				{
					int w = 0;

					//convert substring ti UTF-16 string

					if(strcmp(Strings_Input2[x], "/") == 0)
					{
						//printf("%s\n", Strings_Input1[0]);
					}

					wchar_t *wSubStr = to_wide_string(Strings_Input2[x]);
					if ( wcslen(wSubStr) == 0 )
					{
						freeArrayOfString(Strings_Input2,m2n2);
						freeArrayOfString(Strings_Input1,m1n1);
						if (next) {FREE(next); next = NULL;}
						if (values) {FREE(values); values = NULL;}
						Scierror(999, _("%s: Wrong size for input argument #%d: Non-empty string expected.\n"), fname,2);
						FREE(wSubStr);
						FREE(wOrigStr);
						return 0;
					}

					if (wSubStr)
					{
						wchar_t *wCurrent = wOrigStr;
						do
						{
							wCurrent = wcsstr(wCurrent, wSubStr);

							if(wCurrent != NULL)
							{
								//To not find the same "token" and base index 0
								wCurrent++;
								values[nbValues++].data = (int)(wCurrent - wOrigStr); //cast for pointer on 64bit OS
								values[nbposition++].position = x + 1;
							}
							else
							{
								if(strstr(Strings_Input1[0], Strings_Input2[0]) != NULL && nbValues == 0)
								{
//									printf("Strings_Input1 : !%s! (%s)\n", Strings_Input1[0],  Strings_Input2[x]);
//									printf("wCurrent : !%ls! (%ls)\n", wCurrent,  wSubStr);
									FILE *f = fopen("/home/elias/toto.txt", "w+");

									if(wSubStr == NULL)
									{
										fprintf(f, "---%s---\n", Strings_Input2[x]);
									}
									else
									{
										fwprintf(f, L"wSubStr : ---%ls---, ---%s---\n", wSubStr, Strings_Input2[x]);
									}
									fclose(f);
								}
							}
						}
						while(wCurrent != NULL);// wCurrent is the answer of wcsstr ( using the kmp algorithm )

						// values are sorted
						qsort(values,nbValues,sizeof(values[0]),cmp);
					}
					FREE(wSubStr);
				}
				FREE(wOrigStr);
			}
		}

/*
		if(nbValues == 0)
		{
			if(strstr(Strings_Input1[0], Strings_Input2[0]) != NULL)
			{
				int i = 0;
				printf("Strings_Input1 : !%s! (", Strings_Input1[0]);
				for(i = 0 ; i < m2n2 ; i++)
				{
					if(i != 0)
					{
						printf(", ");
					}

					printf("%s", Strings_Input2[i]);
				}
				printf(")\n");
			}
		}
*/
		freeArrayOfString(Strings_Input1,m1n1);
		freeArrayOfString(Strings_Input2,m2n2);

		numRow   = 1;
		outIndex = 0;
		CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbValues,&outIndex);
		for ( i = 0 ; i < nbValues ; i++ )
		{
			stk(outIndex)[i] = (double)values[i].data ;
		}
		LhsVar(1) = Rhs+1 ;

		if (Lhs == 2)
		{
			numRow   = 1;
			outIndex = 0;
			CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbposition,&outIndex);
			for ( i = 0 ; i < nbposition ; i++ )
			{
				stk(outIndex)[i] = (double)values[i].position ;
			}
			LhsVar(2) = Rhs+2;
		}

		C2F(putlhsvar)();


		if (values) {FREE(values); values = NULL;}
	}
	else
	{
		if(VarType(1) != sci_strings)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Row vector of strings or column vector of strings expected.\n"),fname,2);
		}
		return 0;
	}
	return 0;
}
/*------------------------------------------------------------------------*/
