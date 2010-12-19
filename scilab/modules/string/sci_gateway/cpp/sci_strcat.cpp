/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET , Cong WU
* Copyright (C) DIGITEO - 2010 - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/* desc : concatenate character strings                                   */
/* Examples: strcat(string(1:10),',')                                     */
/*                                                                        */
/*------------------------------------------------------------------------*/

#include "string_gw.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C"
{
#include <string.h>
#include <stdio.h>
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
}
/*-------------------------------------------------------------------------------------*/
#define STAR '*'
#define COL 'c'
#define ROW 'r'
#define ONE_CHAR 1
#define EMPTY_CHAR ""
/*-------------------------------------------------------------------------------------*/
static int sci_strcat_three_rhs(char *fname);
static int sci_strcat_two_rhs(char *fname);
static int sci_strcat_one_rhs(char *fname);
static int sci_strcat_rhs_one_is_a_matrix(char *fname);
static int sumlengthstring(int rhspos);
static int *lengthEachString(int rhspos, int *sizeArrayReturned);
/*-------------------------------------------------------------------------------------*/

using namespace types;

Function::ReturnValue sci_strcat(typed_list &in, int _iRetCount, typed_list &out)
{
    int iMode               = 0;
    wchar_t* pwstToInsert   = NULL;

    //check input paramters
    if(in.size() < 1 || in.size() > 3)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: %d or %d expected.\n"), L"strcat", 1, 3);
        return Function::Error;
    }

    for(int i = 1 ; i < in.size() ; i++)
    {
        if(in[i]->isString() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Matrix of strings expected.\n"), L"strcat", i + 1);
            return Function::Error;
        }
    }

    if(in[0]->isDouble() && in[0]->getAsDouble()->size_get() == 0)
    {
        String *pOut = new String(1,1);
        pOut->string_set(0, L"");
        out.push_back(pOut);
        return Function::OK;
    }
    else if(in[0]->isString() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: String expected.\n"), L"strcat", 1);
        return Function::Error;
    }

    if(in.size() == 3)
    {
        wchar_t wcMode = in[2]->getAsString()->string_get(0)[0];
        switch(wcMode)
        {
        case L'r' :
            iMode = 1;
            break;
        case L'c' :
            iMode = 2;
            break;
        default :
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: ''%s'' or ''%s'' expected.\n"), L"strcat", 3, L"c", L"r");
            return Function::Error;
        }
    }

    if(in.size() > 1)
    {
        if(in[1]->getAsString()->size_get() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: String expected.\n"), L"strcat", 2);
            return Function::Error;
        }

        pwstToInsert = in[1]->getAsString()->string_get(0);
    }

    String* pS = in[0]->getAsString();

    String* pOut = NULL;
    switch(iMode)
    {
    case 0 : //"*"
        {
            pOut = new String(1, 1);
            /*compute final size*/
            int iLen = 1; //L'\0'
            for(int i = 0 ; i < pS->size_get() ; i++)
            {
                iLen += (int)wcslen(pS->string_get(i));
            }

            if(pwstToInsert != NULL)
            {
                iLen += (int)wcslen(pwstToInsert) * (pS->size_get() - 1);
            }

            wchar_t* pwstOut = (wchar_t*)MALLOC(sizeof(wchar_t) * iLen);
            pwstOut[0] = L'\0';
            for(int i = 0 ; i < pS->size_get() ; i++)
            {
                size_t iOffset = wcslen(pwstOut);
                if(iOffset != 0 && pwstToInsert != NULL)
                {
                    wcscat(pwstOut + iOffset, pwstToInsert);
                }
                wcscat(pwstOut + iOffset, pS->string_get(i));
            }

            pOut->string_set(0, pwstOut);
            FREE(pwstOut);
        }
        break;
    case 1 : //"r"
        {
            pOut = new String(1, pS->cols_get());
            /*compute final size*/
            for(int i = 0 ; i < pS->cols_get() ; i++)
            {
                int iLen = 1; //L'\0'
                for(int j = 0 ; j < pS->rows_get() ; j++)
                {
                    iLen += (int)wcslen(pS->string_get(j, i));
                }

                if(pwstToInsert != NULL)
                {
                    iLen += (int)wcslen(pwstToInsert) * (pS->rows_get() - 1);
                }

                wchar_t* pwstOut = (wchar_t*)MALLOC(sizeof(wchar_t) * iLen);
                pwstOut[0] = L'\0';

                for(int j = 0 ; j < pS->rows_get() ; j++)
                {
                    size_t iOffset = wcslen(pwstOut);
                    if(iOffset != 0 && pwstToInsert != NULL)
                    {
                        wcscat(pwstOut + iOffset, pwstToInsert);
                    }
                    wcscat(pwstOut + iOffset, pS->string_get(j, i));
                }
                pOut->string_set(0, i, pwstOut);
            }
            break;
        }
    case 2 : //"c"
        {
            pOut = new String(pS->rows_get(), 1);
            /*compute final size*/
            for(int i = 0 ; i < pS->rows_get() ; i++)
            {
                int iLen = 1; //L'\0'
                for(int j = 0 ; j < pS->cols_get() ; j++)
                {
                    iLen += (int)wcslen(pS->string_get(i, j));
                }

                if(pwstToInsert != NULL)
                {
                    iLen += (int)wcslen(pwstToInsert) * (pS->cols_get() - 1);
                }

                wchar_t* pwstOut = (wchar_t*)MALLOC(sizeof(wchar_t) * iLen);
                pwstOut[0] = L'\0';

                for(int j = 0 ; j < pS->cols_get() ; j++)
                {
                    size_t iOffset = wcslen(pwstOut);
                    if(iOffset != 0 && pwstToInsert != NULL)
                    {
                        wcscat(pwstOut + iOffset, pwstToInsert);
                    }
                    wcscat(pwstOut + iOffset, pS->string_get(i, j));
                }
                pOut->string_set(i, 0, pwstOut);
            }
            break;
        }
    }


    out.push_back(pOut);
    return Function::OK;
//    CheckRhs(1,3);
//    CheckLhs(1,1);
//
//    switch (Rhs)
//    {
//    case 3:
//        {
//            sci_strcat_three_rhs(fname);
//        }
//        break;
//    case 2:
//        {
//            sci_strcat_two_rhs(fname);
//        }
//        break;
//    case 1:
//        {
//            sci_strcat_one_rhs(fname);
//        }
//        break;
//    default:
//        /* nothing */
//        break;
//    }
//    return 0;
//}
///*-------------------------------------------------------------------------------------*/
//static int sci_strcat_three_rhs(char *fname)
//{
//    int Row_One = 0,Col_One = 0;
//    char **Input_String_One = NULL;
//    int mn = 0;
//    static char def_sep[] = "";
//    char *Input_String_Two = def_sep;
//    char typ = 0;
//    int i = 0;
//
//    if (VarType(1) != sci_strings)
//    {
//        Scierror(999,_("%s: Wrong type for input argument #%d: a string vector expected.\n"),fname,1);
//        return 0;
//    }
//
//    if (VarType(2) != sci_strings)
//    {
//        Scierror(999,"%s : Wrong size for input argument #%d: Single string expected.\n",fname,2);
//        return 0;
//    }
//
//    if (VarType(3) != sci_strings)
//    {
//        Scierror(999,"%s : Wrong size for input argument #%d: Single string expected.\n",fname,3);
//        return 0;
//    }
//
//    GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_One,&Col_One,&Input_String_One);
//    mn = Row_One * Col_One;
//
//    if (Rhs >= 2)
//    {
//        /* second argument always a string and not a matrix of string */
//        int l2 = 0;
//        int Row_Two = 0,Col_Two = 0;
//        GetRhsVar(2, STRING_DATATYPE, &Row_Two, &Col_Two, &l2);
//        Input_String_Two = cstk(l2);
//    }
//
//    if (Rhs >= 3)
//    {
//        int Row_Three = 0, Col_Three = 0;
//        int l3 = 0;
//        GetRhsVar(3, STRING_DATATYPE, &Row_Three, &Col_Three, &l3);
//        if (Row_Three * Col_Three != 0) typ = cstk(l3)[0];
//        if (typ != COL && typ != ROW )
//        {
//            freeArrayOfString(Input_String_One,mn);
//            Scierror(999,_("%s: Wrong type for input argument #%d: ''%s'' or ''%s'' expected.\n"),fname,3,"c","r");
//            return 0;
//        }
//    }
//
//    switch ( typ )
//    {
//    case STAR :
//        {
//            int nchars = 0;
//            int one = 1;
//            int l3 = 0;
//            int k = 0;
//
//            /* just return one string */
//            for ( i = 0 ; i < mn ; i++ ) nchars += (int)strlen(Input_String_One[i]);
//            nchars += (mn-1)*(int)strlen(Input_String_Two);
//
//            CreateVar(Rhs+1,STRING_DATATYPE,&one,&nchars,&l3);
//
//            for ( i = 0 ; i < mn ; i++ )
//            {
//                int j = 0;
//                for ( j =0 ; j < (int)strlen(Input_String_One[i]) ; j++ ) *cstk(l3+ k++) = Input_String_One[i][j];
//                if ( i != mn-1) for ( j =0 ; j < (int)strlen(Input_String_Two) ; j++ ) *cstk(l3+ k++) = Input_String_Two[j];
//            }
//            freeArrayOfString(Input_String_One,mn);
//            LhsVar(1) = Rhs+1  ;
//        }
//        break;
//    case COL:
//        {
//            char **Output_String = NULL;
//            int nchars = 0;
//            int one = 1;
//            /* return a column matrix */
//            if ( (Output_String = (char**)MALLOC((Row_One+1)*sizeof(char *)))==NULL)
//            {
//                freeArrayOfString(Input_String_One,mn);
//                Scierror(999,_("%s: No more memory.\n"),fname);
//                return 0;
//            }
//            Output_String[Row_One]=NULL;
//            for (i= 0 ; i < Row_One ; i++)
//            {
//                int j = 0;
//                /* length of row i */
//                nchars = 0;
//                for ( j = 0 ; j < Col_One ; j++ ) nchars += (int)strlen(Input_String_One[i+ Row_One*j]);
//                nchars += (Col_One-1)*(int)strlen(Input_String_Two);
//
//                Output_String[i]=(char*)MALLOC((nchars+1)*sizeof(char));
//                if ( Output_String[i] == NULL)
//                {
//                    freeArrayOfString(Output_String,i);
//                    freeArrayOfString(Input_String_One,mn);
//                    Scierror(999,_("%s: No more memory.\n"),fname);
//                    return 0;
//                }
//                /* fill the string */
//                strcpy(Output_String[i],Input_String_One[i]);
//
//                for ( j = 1 ; j < Col_One ; j++ )
//                {
//                    strcat(Output_String[i],Input_String_Two);
//                    strcat(Output_String[i],Input_String_One[i+ Row_One*j]);
//                }
//            }
//
//            CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &Row_One, &one, Output_String);
//            freeArrayOfString(Input_String_One,mn);
//            freeArrayOfString(Output_String,Row_One+1);
//            LhsVar(1) = Rhs+1  ;
//        }
//        break;
//
//    case ROW:
//        {
//            int nchars = 0;
//            int j = 0;
//            int one = 1;
//            char **Output_String = (char **)CALLOC(Col_One, sizeof(char*));
//
//            /* return a row matrix */
//            if (Output_String == NULL)
//            {
//                freeArrayOfString(Input_String_One, mn);
//                Scierror(999,_("%s: No more memory.\n"), fname);
//                return 0;
//            }
//
//            for (j = 0 ; j < Col_One ; j++)
//            {
//                Output_String[j] = os_strdup(Input_String_One[j*Row_One]);
//
//                if ( Output_String[j] == NULL)
//                {
//                    freeArrayOfString(Output_String,j);
//                    freeArrayOfString(Input_String_One, mn);
//                    Scierror(999,_("%s: No more memory.\n"), fname);
//                    return 0;
//                }
//
//                for ( i = 1 ; i < Row_One ; i++ )
//                {
//                    int lenOutput = 0;
//                    int lenInput = 0;
//                    int lenNew = 0;
//
//                    if (Input_String_Two)
//                    {
//                        lenOutput = (int)strlen(Output_String[j]);
//                        lenInput = (int)strlen(Input_String_Two);
//                        lenNew = lenOutput + lenInput + 1;
//
//                        Output_String[j] = (char*)REALLOC(Output_String[j], sizeof(char*) * lenNew);
//                        if (Output_String[j] == NULL)
//                        {
//                            freeArrayOfString(Output_String,j);
//                            freeArrayOfString(Input_String_One, mn);
//                            Scierror(999,_("%s: No more memory.\n"), fname);
//                            return 0;
//                        }
//                        strcat(Output_String[j], Input_String_Two);
//                    }
//
//                    if (Input_String_One[i+ Row_One*j])
//                    {
//                        lenOutput = (int)strlen(Output_String[j]);
//                        lenInput = (int)strlen(Input_String_One[i+ Row_One*j]);
//                        lenNew = lenOutput + lenInput + 1;
//
//                        Output_String[j] = (char*)REALLOC(Output_String[j], sizeof(char*) * lenNew);
//                        if (Output_String[j] == NULL)
//                        {
//                            freeArrayOfString(Output_String,j);
//                            freeArrayOfString(Input_String_One, mn);
//                            Scierror(999,_("%s: No more memory.\n"), fname);
//                            return 0;
//                        }
//                        strcat(Output_String[j], Input_String_One[i+ Row_One*j]);
//                    }
//                }
//            }
//
//            CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &one, &Col_One, Output_String);
//            freeArrayOfString(Input_String_One, mn);
//            if (Col_One == 1)
//            {
//                FREE(Output_String);
//                Output_String = NULL;
//            }
//            else
//            {
//                freeArrayOfString(Output_String, Col_One);
//            }
//            LhsVar(1) = Rhs+1  ;
//        }
//        break;
//
//    default:
//        {
//            freeArrayOfString(Input_String_One,mn);
//            Scierror(999,_("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"),fname,3,"c","r");
//            return 0;
//        }
//        break;
//
//    }
//    C2F(putlhsvar)();
//    return 0;
//}
///*-------------------------------------------------------------------------------------*/
//static int sci_strcat_two_rhs(char *fname)
//{
//    int Type_One = VarType(1);
//    int Type_Two = VarType(2);
//
//    int Number_Inputs_Two = 0;
//    char **Input_String_Two = NULL;
//
//    if (Type_Two != sci_strings)
//    {
//        Scierror(246,_("%s: Wrong type for input argument #%d: Single string expected.\n"),fname,2);
//        return 0;
//    }
//    else /* sci_strings */
//    {
//        int Row_Two = 0,Col_Two = 0;
//
//        GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&Row_Two,&Col_Two,&Input_String_Two);
//        Number_Inputs_Two = Row_Two * Col_Two;
//        /* check we have only a string as second parameter */
//        if (Number_Inputs_Two != 1)
//        {
//            freeArrayOfString(Input_String_Two,Number_Inputs_Two);
//            Scierror(36,"%s : Wrong size for input argument #%d: Single string expected.\n",fname,2);
//            return 0;
//        }
//    }
//
//    if ( (Type_One != sci_strings) && (Type_One != sci_matrix) )
//    {
//        freeArrayOfString(Input_String_Two,Number_Inputs_Two);
//        Scierror(246,"%s: Wrong type for input argument #%d: Matrix of strings expected.\n",fname,1);
//        return 0;
//    }
//    else
//    {
//        if (Type_One == sci_matrix)
//        {
//            freeArrayOfString(Input_String_Two,Number_Inputs_Two);
//            sci_strcat_rhs_one_is_a_matrix(fname);
//        }
//        else /* sci_strings */
//        {
//            char **Input_String_One = NULL;
//            int Row_One = 0,Col_One = 0;
//            int Number_Inputs_One = 0;
//            int length_output = 0;
//
//            GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_One,&Col_One,&Input_String_One);
//            Number_Inputs_One = Row_One * Col_One;
//
//            if (Input_String_One)
//            {
//                if ( (Number_Inputs_One == 1) && (strcmp(Input_String_One[0],"")==0) )
//                {
//                    /* With no input argument returns a zero length character string */
//                    int m1 = 0, n1 = 0, l1 = 0;
//
//                    CreateVar(Rhs+1,STRING_DATATYPE,  &m1, &n1, &l1);
//                    LhsVar(1)=Rhs+1;
//                    C2F(putlhsvar)();
//
//                    freeArrayOfString(Input_String_Two,Number_Inputs_Two);
//                    freeArrayOfString(Input_String_One,Number_Inputs_One);
//                    return 0;
//                }
//                else
//                {
//                    if (Number_Inputs_One == 1)
//                    {
//                        length_output = (int)strlen(Input_String_One[0]);
//                    }
//                    else
//                    {
//                        int lengthInput_String_Two = (int)strlen(Input_String_Two[0]);
//                        length_output = sumlengthstring(1) + (int)(Number_Inputs_One)*lengthInput_String_Two - lengthInput_String_Two;
//                    }
//                }
//            }
//
//            if (length_output > 0)
//            {
//                static int n1 = 0, m1 = 0;
//                int outIndex = 0 ;
//                char *Output_String = NULL;
//                int i = 0;
//                int Number_Inputs_OneLessOne = Number_Inputs_One - 1;
//
//                m1 = length_output;
//                n1 = 1;
//
//                CreateVar( Rhs+1,STRING_DATATYPE,&m1,&n1,&outIndex);
//                Output_String = cstk(outIndex);
//
//                /* strcpy + strcat faster than sprintf */
//                strcpy(Output_String,Input_String_One[0]);
//                ( 0 < Number_Inputs_OneLessOne )? strcat(Output_String,Input_String_Two[0]):0;
//
//                for (i = 1; i < Number_Inputs_One; i++)
//                {
//                    strcat(Output_String,Input_String_One[i]);
//                    ( i < Number_Inputs_OneLessOne )? strcat(Output_String,Input_String_Two[0]):0;
//                }
//
//                LhsVar(1) = Rhs+1;
//                C2F(putlhsvar)();
//                freeArrayOfString(Input_String_One,Row_One*Col_One);
//                freeArrayOfString(Input_String_Two,Number_Inputs_Two);
//            }
//            else
//            {
//                if (length_output == 0)
//                {
//                    int one    = 1 ;
//                    int len   = (int)strlen(EMPTY_CHAR);
//                    int outIndex = 0 ;
//
//                    CreateVar(Rhs+1,STRING_DATATYPE,&len,&one,&outIndex);
//                    strcpy(cstk(outIndex),EMPTY_CHAR);
//                    LhsVar(1) = Rhs+1 ;
//                    C2F(putlhsvar)();
//
//                    freeArrayOfString(Input_String_Two,Number_Inputs_Two);
//                    freeArrayOfString(Input_String_One,Number_Inputs_One);
//                }
//                else
//                {
//                    freeArrayOfString(Input_String_Two,Number_Inputs_Two);
//                    freeArrayOfString(Input_String_One,Number_Inputs_One);
//                    Scierror(999,_("%s : Wrong size for input argument(s).\n"),fname);
//                }
//            }
//        }
//    }
//    return 0;
//}
///*-------------------------------------------------------------------------------------*/
//static int sci_strcat_one_rhs(char *fname)
//{
//    int Type_One = VarType(1);
//    if ( (Type_One != sci_strings) && (Type_One != sci_matrix) )
//    {
//        Scierror(246,"%s: Wrong type for input argument #%d: Matrix of strings expected.\n",fname,1);
//        return 0;
//    }
//    else
//    {
//        if (Type_One == sci_strings)
//        {
//            int lenstrcat =  sumlengthstring(1);
//
//            if (lenstrcat >= 0)
//            {
//                char **Input_String_One = NULL;
//                int m = 0, n = 0; /* matrix size */
//                int mn = 0; /* m*n */
//                int n1 = 1, m1 = lenstrcat;
//                int outIndex = 0 ;
//
//                char *Output_String = NULL;
//                int i = 0;
//
//                int sizeLengths = 0;
//                int *lengths = lengthEachString(1, &sizeLengths);
//
//                int l = 0;
//
//                if (lengths == NULL)
//                {
//                    Scierror(999,_("%s: error.\n"),fname);
//                    return 0;
//                }
//
//                GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m,&n,&Input_String_One);
//                mn = m * n;
//
//                CreateVar( Rhs+1,STRING_DATATYPE,&m1,&n1,&outIndex);
//                Output_String = cstk(outIndex);
//
//                l = 0;
//                for (i = 0; i < mn; i++)
//                {
//                    /* bug 4728 Compatibility with Scilab 4.1.2 */
//                    /* replaces strcpy & strcat by memcpy */
//                    /* copy of char array and not string */
//                    if ( i == 0)
//                    {
//                        memcpy(Output_String,Input_String_One[i], lengths[i]);
//                    }
//                    else
//                    {
//                        memcpy(Output_String + l, Input_String_One[i],  lengths[i]);
//                    }
//                    l = l + lengths[i];
//                }
//
//                FREE(lengths); lengths = NULL;
//
//                LhsVar(1) = Rhs+1;
//                C2F(putlhsvar)();
//                if (Input_String_One) freeArrayOfString(Input_String_One,mn);
//            }
//            else
//            {
//                Scierror(999,_("%s: Wrong size for input argument(s).\n"),fname);
//            }
//        }
//        else /* sci_matrix*/
//        {
//            sci_strcat_rhs_one_is_a_matrix(fname);
//        }
//    }
//    return 0;
//}
///*-------------------------------------------------------------------------------------*/
//static int sci_strcat_rhs_one_is_a_matrix(char *fname)
//{
//    /* strcat([],'A') returns a empty string matrix */
//    double *Input_String_One = NULL;
//    int Row_One = 0,Col_One = 0;
//
//    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&Row_One,&Col_One,&Input_String_One);
//
//    /* check that we have [] */
//    if ((Row_One == 0) && (Col_One == 0))
//    {
//        int one    = 1 ;
//        int len   = (int)strlen(EMPTY_CHAR);
//        int outIndex = 0 ;
//
//        CreateVar(Rhs+1,STRING_DATATYPE,&len,&one,&outIndex);
//        strcpy(cstk(outIndex),EMPTY_CHAR);
//        LhsVar(1) = Rhs+1 ;
//        C2F(putlhsvar)();
//    }
//    else
//    {
//        Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of strings or empty real matrix expected.\n"),fname,1);
//    }
//    return 0;
//}
///*-------------------------------------------------------------------------------------*/
//static int sumlengthstring(int rhspos)
//{
//    int sizelengths = 0;
//    int sumlength = -1; /* error */
//    int *lengths = lengthEachString(rhspos, &sizelengths);
//    int i = 0;
//    if (lengths)
//    {
//        sumlength = 0;
//        for(i = 0;i < sizelengths; i++)
//        {
//            sumlength = sumlength + lengths[i];
//        }
//        FREE(lengths); lengths = NULL;
//    }
//
//    return sumlength;
//}
///*-------------------------------------------------------------------------------------*/
//static int *lengthEachString(int rhspos, int *sizeArrayReturned)
//{
//    int *StringsLength = NULL;
//
//    if (VarType(rhspos) == sci_strings)
//    {
//        int m = 0, n = 0; /* matrix size */
//        int mn = 0; /* m*n */
//
//        int il = 0; int ilrd = 0;
//        int l1 = 0;
//
//        int x = 0;
//
//        int lw = rhspos + Top - Rhs;
//        int lenstrcat = 0;
//
//        l1 = *Lstk(lw);
//        il = iadr(l1);
//
//        if (*istk(il ) < 0) il = iadr(*istk(il + 1));
//
//        /* get dimensions */
//        m = getNumberOfLines(il); /* row */
//        n = getNumberOfColumns(il); /* col */
//        mn = m * n ;
//        ilrd = il + 4;
//
//        StringsLength = (int*)MALLOC(sizeof(int) * mn);
//        if (StringsLength == NULL) return NULL;
//
//        *sizeArrayReturned = mn;
//
//        for  ( x = 0; x < mn; x++ )
//        {
//            StringsLength[x]=  (int) (*istk(ilrd + x + 1) - *istk(ilrd + x));
//        }
//    }
//    return StringsLength;
}
/*-------------------------------------------------------------------------------------*/
