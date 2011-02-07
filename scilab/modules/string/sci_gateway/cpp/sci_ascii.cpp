/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
*  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* desc : This function convert Scilab string to a vector of ascii code  */
/*        or vector of ascii code to Scilab strings.                     */
/*        If  txt  is a matrix of string,  ascii(txt)  is equivalent to  */
/*          ascii(strcat(txt))                                           */
/*-----------------------------------------------------------------------*/

#include "string_gw.hxx"
#include "function.hxx"
#include "arrayof.hxx"

extern "C"
{
#include <ctype.h>
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
}

//#include "sci_warning.h"

using namespace types;

/*----------------------------------------------------------------------------*/
String* DoubleToString(Double* _pdbl);
String* IntToString(Int* _pi);
Double* StringToDouble(String* _pst);
/*----------------------------------------------------------------------------*/
Function::ReturnValue sci_ascii(typed_list &in, int _iRetCount, typed_list &out)
{
    if(in.size() != 1)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"ascii", 1);
        return Function::Error;
    }

    InternalType* pOut;
    switch(in[0]->getType())
    {
    case InternalType::RealDouble :
        {
            pOut = DoubleToString(in[0]->getAs<Double>());
            break;
        }
    case InternalType::RealString :
        {
            pOut = StringToDouble(in[0]->getAsString());
            break;
        }
    case InternalType::RealInt :
        {
            pOut = IntToString(in[0]->getAsInt());
            break;
        }
    default :
        ScierrorW(999, _W("%ls: Wrong type for argument #%d: Matrix of strings or Integer matrix expected.\n"), L"ascii", 1);
        return Function::Error;
    }

    out.push_back(pOut);
    return Function::OK;

//	CheckRhs(1,1);
//	CheckLhs(0,1);
//
//	switch ( GetType(1))
//	{
//		case sci_strings:
//			asciiStrings(fname);
//		break;
//
//		case sci_matrix :
//			asciiMatrix(fname);
//		break;
//
//		case sci_ints :
//			asciiIntMatrix(fname);
//		break;
//
//		default:
//			asciiOthers(fname);
//		break;
//  }
//  return 0;
//}
///*--------------------------------------------------------------------------*/
//static int asciiStrings(char *fname)
//{
//	/* interface written with stack3 */
//	/* 3 phases :
//		 1] get data from stack
//			conversion scilab code to ascii and strings (char **)
//		 2] algo. conversion ascii to values
//		 3] put results on stack
//	*/
//
//	/* interface written with stack1*/
//	/* it works immediately on stack (read and write)
//	conversion scilab code to ascii values
//	*/
//
//	/* Benchmark
//	str_test_mat =  ["abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
//	"abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz"; ..
//	"abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
//	"abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz"];
//
//	tic();
//	for i=1:10000000
//	ascii(["abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz"]);
//	end
//	duree = toc();
//
//	printf("\nDUREE 1 = %d seconds\n\n",duree);
//	*/
//
//	/* on windows C2D 6600 2400 mhz */
//	/* scilab 4.1.2 : 40 s */
//	/* scilab with stack3 interface : 75 s*/
//	/* scilab with stack1 interface : 41 s */
//
///*
//	char **Input_StringMatrix = NULL;
//	int x = 0,y = 0,Row_Num = 0,Col_Num = 0;
//
//	int Length_Output_Matrix = 0;
//	double *Output_IntMatrix = NULL;
//	int nbOutput_IntMatrix = 0;
//	int numRow = 1;
//
//	GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_Num,&Col_Num,&Input_StringMatrix);
//
//	Length_Output_Matrix = 0;
//	for (x = 0;x < Row_Num*Col_Num;x++) Length_Output_Matrix = Length_Output_Matrix + (int)strlen(Input_StringMatrix[x]);
//
//	if (Length_Output_Matrix !=0)
//	{
//		Output_IntMatrix = (double*)MALLOC(sizeof(double)*(Length_Output_Matrix));
//	}
//	else Output_IntMatrix = (double*)MALLOC(sizeof(double));
//
//	if (Output_IntMatrix == NULL)
//	{
//		freeArrayOfString(Input_StringMatrix,Row_Num*Col_Num);
//		Scierror(999,_("%s: Memory allocation error.\n"),fname);
//		return 0;
//	}
//
//	for (x = 0; x < Row_Num*Col_Num; x++)
//	{
//		for (y = 0;y < (int)strlen(Input_StringMatrix[x]); y++)
//		{
//			Output_IntMatrix[nbOutput_IntMatrix]=Input_StringMatrix[x][y];
//			nbOutput_IntMatrix++;
//		}
//	}
//
//	freeArrayOfString(Input_StringMatrix,Row_Num*Col_Num);
//
//
//	CreateVarFromPtr(Rhs + 1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbOutput_IntMatrix,&Output_IntMatrix);
//
//	LhsVar(1) = Rhs + 1 ;
//	C2F(putlhsvar)();
//	if (Output_IntMatrix) { FREE(Output_IntMatrix); Output_IntMatrix=NULL;}
//	return 0;
//*/
//
//	BOOL is_a_reference_on_stack = FALSE; /* variable is a reference on stack */
//	int i = 0, j = 0;
//	int nbr_characters  = 0;
//	int  l = 0, il = 0, lr = 0;
//	int ilr = 0;
//
//	il = iadr( C2F(vstk).lstk[Top - 1] );
//
//	ilr = il;
//	if (*istk(il) < 0)
//	{
//		il = iadr(*istk(il+1));
//	}
//	/* check if parameter is a reference */
//	is_a_reference_on_stack = (il != ilr);
//
//	/* find number of characters */
//	nbr_characters = *istk(il + 4 + *istk(il + 1) * *istk(il + 2)) - 1;
//
//	l = il + 5 + *istk(il+1) * *istk(il+2);
//
//	if (is_a_reference_on_stack)
//	{
//		/* it is a reference on stack : txt = 'Scilab' ; ascii(txt) */
//		j = ilr + 4;
//		Err = j / 2 + 1 + nbr_characters - C2F(vstk).lstk[Bot - 1];
//		if (Err > 0)
//		{
//			/* stacksize exceeded */
//			SciError(17);
//			return 0;
//		}
//	}
//	else
//	{
//		/* it is not a reference on stack  ascii('Scilab') */
//		int one = 1;
//		int lw = l + nbr_characters;
//
//		j = lw + nbr_characters;
//		Err = j / 2 + 1 - C2F(vstk).lstk[Bot - 1];
//		if (Err > 0)
//		{
//			/* stacksize exceeded */
//			SciError(17);
//			return 0;
//		}
//		C2F(icopy)(&nbr_characters, istk(l), &one, istk(l + nbr_characters), &one);
//		l = lw;
//	}
//
//	/* create output int matrix , see C2F(crematvar) stack1.c */
//	*istk(ilr) = sci_matrix;
//	*istk(ilr+1) = 1;
//	*istk(ilr+2) = nbr_characters;
//	*istk(ilr+3) = 0;
//
//	j = ilr + 4;
//	lr = j / 2 + 1;
//
//	/* put each value on stack */
//	for (i = 0; i < nbr_characters; i++)
//	{
//		int scilab_code = *istk(l + i);
//		*stk(lr + i) = convertScilabCodeToAsciiCode(scilab_code);
//	}
//	/* update Top of stack */
//	C2F(vstk).lstk[Top] = lr + nbr_characters;
//
//	return 0;
//
//}
///*--------------------------------------------------------------------------*/
//static int asciiMatrix(char *fname)
//{
//	int Row_Num = 0,Col_Num = 0,Stack_Pos = 0;
//	int outIndex = 0 ;
//	int len = 0;
//	BOOL FirstWarning = TRUE;
//	/*When input vector of int ascii codes  */
//	GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&Row_Num,&Col_Num,&Stack_Pos);
//	len = Row_Num * Col_Num ;
//
//	if (len != 0)
//	{
//		int x = 0;
//		int one = 1;
//		int *Input_IntMatrix = NULL;
//		char *Output_StringMatrix = NULL;
//
//		Input_IntMatrix = istk(Stack_Pos);
//
//		outIndex = 0 ;
//		CreateVar(Rhs+1,STRING_DATATYPE,&len,&one,&outIndex);
//		Output_StringMatrix = cstk(outIndex);
//		for (x = 0; x < len; x++)
//		{
//			if ( FirstWarning )
//			{
//				if ( (Input_IntMatrix[x] < ASCII_MIN) || (Input_IntMatrix[x] > ASCII_MAX) )
//				{
//					if (getWarningMode())
//					{
//						sciprint(_("WARNING : \n"));
//						sciprint(_("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"),fname,1,ASCII_MIN,ASCII_MAX);
//					}
//					FirstWarning = FALSE;
//				}
//			}
//
//			if (isascii(Input_IntMatrix[x]))
//			{
//				Output_StringMatrix[x] = (char)toascii(Input_IntMatrix[x]);
//			}
//			else
//			{
//				Output_StringMatrix[x] = (char)Input_IntMatrix[x];
//			}
//		}
//		Output_StringMatrix[len] = '\0';
//	}
//	else
//	{
//		#define EMPTY_STR ""
//		int one    = 1 ;
//		int lenStr   = (int)strlen(EMPTY_STR);
//		outIndex = 0 ;
//
//		CreateVar(Rhs+1,STRING_DATATYPE,&lenStr,&one,&outIndex);
//		strcpy(cstk(outIndex),EMPTY_STR);
//	}
//
//	LhsVar(1) = Rhs+1 ;
//	C2F(putlhsvar)();
//	return 0;
//}
//
///*--------------------------------------------------------------------------*/
//static int asciiIntMatrix(char *fname)
//{
//	int Row_Num = 0,Col_Num = 0;
//	int outIndex = 0 ;
//	int len = 0;
//
//	SciIntMat M;
//
//	GetRhsVar(1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&Row_Num,&Col_Num,&M);
//	len = Row_Num * Col_Num ;
//
//	if (len != 0)
//	{
//		int one        = 1;
//		int ichar      = I_UCHAR;
//		static int inc = 1;
//
//		char *Output_StringMatrix = NULL;
//
//		CreateVar(Rhs+1,STRING_DATATYPE,&len,&one,&outIndex);
//		Output_StringMatrix = cstk(outIndex);
//
//		/* from intxx to char */
//		C2F(tpconv)(&M.it,&ichar,&len, M.D, &inc, Output_StringMatrix, &inc);
//		Output_StringMatrix[len] = '\0';
//	}
//	else
//	{
//		#define EMPTY_STR ""
//		int one    = 1 ;
//		int lenStr = (int)strlen(EMPTY_STR);
//
//		CreateVar(Rhs+1,STRING_DATATYPE,&lenStr,&one,&outIndex);
//		strcpy(cstk(outIndex),EMPTY_STR);
//	}
//
//	LhsVar(1) = Rhs+1 ;
//	C2F(putlhsvar)();
//	return 0;
//}
//
///*--------------------------------------------------------------------------*/
//static int asciiOthers(char *fname)
//{
//	Scierror(999,_("%s: Wrong type for argument #%d: Matrix of strings or Integer matrix expected.\n"),fname,1);
//	return 0;
}
/*--------------------------------------------------------------------------*/
String* DoubleToString(Double* _pdbl)
{
    String* pOut = NULL;
    char* pst = (char*)MALLOC(sizeof(char) * (_pdbl->size_get() + 1));
    memset(pst, 0x00, _pdbl->size_get() + 1);
    double* pdbl = _pdbl->get_real();

    bool bWarning = false;
    for(int i = 0 ; i < _pdbl->size_get() ; i++)
    {
        if(bWarning == false && pdbl[i] > 255)
        {
            sciprintW(_W("WARNING : \n"));
            sciprintW(_W("%ls: Wrong value for input argument #%d: Must be between %d and %d.\n"), L"ascii", 1, 0, 255);
            bWarning = true;
        }
        pst[i] = (char)toascii((int)pdbl[i]);
    }

    wchar_t* pwst = to_wide_string(pst);
    pOut = new String(1, 1);
    pOut->string_set(0, 0, pwst);
    return pOut;
}
/*--------------------------------------------------------------------------*/
String* IntToString(Int* _pi)
{
    String* pOut = NULL;

    return pOut;
}
/*--------------------------------------------------------------------------*/
Double* StringToDouble(String* _pst)
{
    Double* pOut = NULL;
    /*compute total length*/
    for(int i = 0 ; i < _pst->size_get() ; i++)
    {
        char* pst = wide_string_to_UTF8(_pst->string_get(i));
        if(pOut == NULL)
        {
            pOut = new Double(1, (int)strlen(pst));
            int iLen = (int)strlen(pst);
            double* pD = pOut->get_real();
            for(int j = 0 ; j < iLen ; j++)
            {
                pD[j] = pst[j];
            }
        }
        else
        {
            int iLen = (int)strlen(pst);
            Double *pIn = new Double(1, iLen);
            double* pD = pIn->get_real();
            for(int j = 0 ; j < iLen ; j++)
            {
                pD[j] = pst[j];
            }

            int iOldCols = pOut->cols_get();
            pOut->resize(1, iOldCols + iLen);
            pOut->append(0, iOldCols, pIn);
            delete pIn;
        }
    }
    return pOut;
}
/*--------------------------------------------------------------------------*/
