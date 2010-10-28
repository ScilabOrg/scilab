/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) 2010 - DIGITEO - ELIAS Antoine
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "funcmanager.hxx"
#include "output_stream_gw.hxx"

extern "C"
{
#include "Scierror.h"
#include "MALLOC.h"
#include "do_xxprintf.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "os_wcsdup.h"
}

typedef struct
{
    int iArg;
    int iPos;
    InternalType::RealType type;
}ArgumentPosition;

wchar_t** scilab_sprintf(wchar_t* _pwstName, wchar_t* _pwstInput, typed_list &in, ArgumentPosition* _pArgs, int _iArgsCount, int* _piOutputRows);

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_msprintf(typed_list &in, int _piRetCount, typed_list &out)
{
    //Structure to store, link between % and input value
    ArgumentPosition* pArgs = NULL;

    if(in.size() < 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: at least %d expected.\n"), L"msprintf", 1);
        return Function::Error;
    }

    if(in[0]->isString() == false || in[0]->getAsString()->size_get() != 1)
    {
        ScierrorW(999, _W("l%s: Wrong type for input argument #%d: A string expected.\n"), L"msprintf" ,1);
        return Function::Error;
    }

    for(int i = 1 ; i < in.size() ; i++)
    {
        if(in[i]->isDouble() == false && in[i]->isString() == false)
        {
            //TODO: Overload
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Real matrix or matrix of strings expected.\n"), L"msprintf", i + 1);
            return Function::Error;
        }
    }

    wchar_t* pwstInput = in[0]->getAsString()->string_get()[0];
    int iNumberPercent = 0;
    for(int i = 0 ; i < wcslen(pwstInput) ; i++)
    {
        if(pwstInput[i] == L'%')
        {
			iNumberPercent++;
			if (pwstInput[i + 1] == L'%')
			{
                //it is a %%, not a %_ 
				iNumberPercent--;
                //force incremantation to bypass the second % of %%
                i++;
			}
        }
    }

    //Input values must be less or equal than excepted
    if((in.size() - 1) > iNumberPercent)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: at most %d expected.\n"), L"msprintf", iNumberPercent);
        return Function::Error;
    }

    //determine if imput values are ... multiple values
    int iNumberCols = 0;
    if( in.size() > 1 )
    {
        int iRefRows = in[1]->getAsGenericType()->rows_get();
        for(int i = 1 ; i < in.size() ; i++)
        {
            //all arguments must have the same numbers of rows !
            if(iRefRows != in[i]->getAsGenericType()->rows_get())
            {
                ScierrorW(999, _W("%ls: Wrong number of input arguments: data doesn't fit with format.\n"), L"msprintf");
                return Function::Error;
            }

            iNumberCols += in[i]->getAsGenericType()->cols_get();
        }
    }

    if(iNumberCols != iNumberPercent)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: data doesn't fit with format.\n"), L"msprintf");
        return Function::Error;
    }


    //fill ArgumentPosition structure
    pArgs = new ArgumentPosition[iNumberPercent];
    int idx = 0;
    for(int i = 1 ; i < in.size() ; i++)
    {
        for(int j = 0 ; j < in[i]->getAsGenericType()->cols_get() ; j++)
        {
            pArgs[idx].iArg = i;
            pArgs[idx].iPos = j;
            pArgs[idx].type = in[i]->getType();
            idx++;
        }
    }

    int iOutputRows = 0;
    wchar_t** pwstOutput = scilab_sprintf(L"msprintf", pwstInput, in, pArgs, iNumberPercent, &iOutputRows);

    String* pOut = new String(iOutputRows, 1);
    pOut->string_set(pwstOutput);
    out.push_back(pOut);

    for(int i = 0 ; i < iOutputRows ; i++)
    {
        FREE(pwstOutput[i]);
    }
    FREE(pwstOutput);
    return Function::OK;
}

typedef struct 
{
    wchar_t* pwstToken;
    InternalType::RealType outputType;
    bool bLengthFlag;
}TokenDef;

wchar_t** scilab_sprintf(wchar_t* _pwstName, wchar_t* _pwstInput, typed_list &in, ArgumentPosition* _pArgs, int _iArgsCount, int* _piOutputRows)
{
    wchar_t** pwstOutput        = NULL;
    wchar_t* pwstFirstOutput    = NULL;
    
    if(_iArgsCount == 0)
    {//no arg, just return _pwstInput value
        pwstFirstOutput = os_wcsdup(_pwstInput);
    }
    else
    {
        //store all sub parts of the input string
        //for the input string "bla1 %s bla2 %d bla3"
        //store  :
        //pwstToken[0] : "bla1 "
        //pwstToken[0] : "%s bla2 "
        //pwstToken[2] : "%d bla3"

        size_t iStart       = 0;
        size_t iEnd         = 0;
        int iToken          = 0;
        TokenDef* pToken = new TokenDef[_iArgsCount + 1];
        wchar_t* pwstStart  = _pwstInput;

        bool bFinish = false;
        while(!bFinish)
        {
            wchar_t* pwstEnd = wcsstr(pwstStart + (iToken == 0 ? 0 : 1), L"%");
            if(pwstEnd != NULL)
            {
                if(pwstEnd[1] == L'%')
                {//manage "%%"
                    pwstEnd = wcsstr(pwstEnd + 2, L"%");
                }

                iStart  = pwstStart - _pwstInput;
                iEnd    = pwstEnd - _pwstInput;
            }
            else
            {//end of string
                iStart  = pwstStart - _pwstInput;
                iEnd = wcslen(_pwstInput);
                bFinish = true;
            }

            pToken[iToken].pwstToken = new wchar_t[iEnd - iStart + 1];

            wcsncpy(pToken[iToken].pwstToken, _pwstInput + iStart, iEnd - iStart);
            pToken[iToken].pwstToken[iEnd - iStart] = L'\0';

            //identify destination type
            //format : %[flags][width][.precision][length]specifier
            //pToken[iToken].pwstToken

            //find %
            wchar_t* pwstPercent = wcsstr(pToken[iToken].pwstToken, L"%");

            if(pwstPercent != NULL)
            {
                //looking for flags
                if(*(pwstPercent + 1) == L'-' || 
                    *(pwstPercent + 1) == L'+' || 
                    *(pwstPercent + 1) == L' ' || 
                    *(pwstPercent + 1) == L'#' || 
                    *(pwstPercent + 1) == L'0')
                {
                    pwstPercent++;
                }

                //looking for width
                if(*(pwstPercent + 1) == L'*')
                {
                    pwstPercent++;
                }
                else
                {//number
                    while(iswdigit(*(pwstPercent + 1)))
                    {
                        pwstPercent++;
                    }
                }

                //looking for precision
                if(*(pwstPercent + 1) == L'.')
                {
                    pwstPercent++;
                    while(iswdigit(*(pwstPercent + 1)))
                    {
                        pwstPercent++;
                    }
                }

                //looking for length
                if(*(pwstPercent + 1) == L'h' || 
                    *(pwstPercent + 1) == L'l' || 
                    *(pwstPercent + 1) == L'L')
                {
                    pToken[iToken].bLengthFlag = true;
                    pwstPercent++;
                }
                else
                {
                    pToken[iToken].bLengthFlag = false;
                }

                wchar_t wcType = *(pwstPercent + 1);
                switch(wcType)
                {
                case L'i' : //integer
                case L'd' : //integer
                case L'o' : //octal
                case L'u' : //unsigned
                case L'x' : //hex
                case L'X' : //HEX
                    if(_pArgs[iToken - 1].type != InternalType::RealDouble)
                    {
                        ScierrorW(999, _W("%ls: Wrong number of input arguments: data doesn't fit with format.\n"), _pwstName);
                        return NULL;
                    }
                    pToken[iToken].outputType = InternalType::RealInt;
                    break;
                case L'f' : //float
                case L'e' : //exp
                case L'E' : //EXP
                case L'g' : //shorter between float or exp
                case L'G' : //shorter between float or EXP
                    if(_pArgs[iToken - 1].type != InternalType::RealDouble)
                    {
                        ScierrorW(999, _W("%ls: Wrong number of input arguments: data doesn't fit with format.\n"), _pwstName);
                        return NULL;
                    }
                    pToken[iToken].outputType = InternalType::RealDouble;
                    break;
                case L's' :
                case L'c' :
                    if(_pArgs[iToken - 1].type != InternalType::RealString)
                    {
                        ScierrorW(999, _W("%ls: Wrong number of input arguments: data doesn't fit with format.\n"), _pwstName);
                        return NULL;
                    }
                    pToken[iToken].outputType = InternalType::RealString;
                    break;
                default : 
                    //houston ...
                    break;
                }

            }

            pwstStart = pwstEnd;
            iToken++;
        }

        int iLoop = in[1]->getAsGenericType()->rows_get();
        pwstFirstOutput = (wchar_t*)MALLOC(sizeof(wchar_t*) * iLoop * bsiz);
        memset(pwstFirstOutput, 0x00, sizeof(wchar_t*) * iLoop * bsiz);
        for(int j = 0 ; j < iLoop ; j++)
        {
            //copy the 0th token
            wcscat(pwstFirstOutput, pToken[0].pwstToken);

            //start at 1, the 0th is always without %
            for(int i = 1 ; i < _iArgsCount + 1 ; i++)
            {
                wchar_t pwstTemp[bsiz];
                void* pvVal = NULL;
                if(_pArgs[i - 1].type == InternalType::RealDouble)
                {
                    double dblVal = in[_pArgs[i - 1].iArg]->getAsDouble()->real_get(j, _pArgs[i - 1].iPos);
                    if(pToken[i].outputType == InternalType::RealDouble)
                    {
                        swprintf(pwstTemp, bsiz, pToken[i].pwstToken, dblVal);
                    }
                    else if(pToken[i].outputType == InternalType::RealInt)
                    {
                        swprintf(pwstTemp, bsiz, pToken[i].pwstToken, (int)dblVal);
                    }
                }
                else if(_pArgs[i - 1].type == InternalType::RealString)
                {
                    wchar_t* pwstStr = in[_pArgs[i - 1].iArg]->getAsString()->string_get(j, _pArgs[i - 1].iPos);

                    if(pToken[i].bLengthFlag == false)
                    {
                        //replace %s by %ls to wide char compatibility
                        wchar_t* pwstToken = (wchar_t*)MALLOC(sizeof(wchar_t) * (wcslen(pToken[i].pwstToken) + 2));
                        swprintf(pwstToken, wcslen(pToken[i].pwstToken) + 2, pToken[i].pwstToken, "%ls");
                        swprintf(pwstTemp, bsiz, pwstToken, pwstStr);
                        FREE(pwstToken);
                    }
                    else
                    {
                        swprintf(pwstTemp, bsiz, pToken[i].pwstToken, pwstStr);
                    }
                }
                else
                {//impossible but maybe in the futur
                }

                wcscat(pwstFirstOutput, pwstTemp);
            }
        }
    }

    //\n \n\r \r to string
    //find number of lines
    *_piOutputRows = 1;
    for(int i = 0 ; i < wcslen(pwstFirstOutput) - 2 ; i++)
    {
        if(pwstFirstOutput[i] == L'\\' && pwstFirstOutput[i + 1] == L'r' && pwstFirstOutput[i + 2] != L'\0')
        {
            (*_piOutputRows)++;
            i += 2;
            if(pwstFirstOutput[i] == L'\\' && pwstFirstOutput[i + 1] == L'n' && pwstFirstOutput[i + 2] != L'\0')
            {
                i += 2;
            }
        }
        else if(pwstFirstOutput[i] == L'\\' && pwstFirstOutput[i + 1] == L'n' && pwstFirstOutput[i + 2] != L'\0')
        {
            (*_piOutputRows)++;
            i += 2;
        }
    }

    //alloc output data
    pwstOutput = (wchar_t**)MALLOC(sizeof(wchar_t*) * *_piOutputRows);
    wchar_t* pwstPtr = pwstFirstOutput;
    int iRows = 0;

    //split in multiple strings
    for(int i = 0 ; i < wcslen(pwstPtr) ; i++)
    {
        int idx = 0;
        bool bNewLine = false;
        if(pwstPtr[i] == L'\\' && pwstPtr[i + 1] == L'r')
        {
            idx = i;
            bNewLine = true;
            i += 2;
            if(pwstPtr[i] == L'\\' && pwstPtr[i + 1] == L'n')
            {
                i += 2;
            }
        }
        else if(pwstPtr[i] == L'\\' && pwstPtr[i + 1] == L'n')
        {
            idx = i;
            bNewLine = true;
            i += 2;
        }

        if(bNewLine || pwstPtr[i + 1] == L'\0')
        {
            if(pwstPtr[i + 1] == L'\0')
            {//to copy end of data in a new lines
                idx = wcslen(pwstPtr);
            }

            pwstOutput[iRows] = (wchar_t*)MALLOC(sizeof(wchar_t) * (idx + 1));
            wcsncpy(pwstOutput[iRows], pwstPtr, idx);
            pwstOutput[iRows][idx] = L'\0';
            pwstPtr += i;
            iRows++;
            i = 0;
        }
    }

    return pwstOutput;
}

	//char **lstr=NULL;
	//static int l1, m1, n1,n2,lcount,rval,blk=200;
	//static int k;
	//char ** strs;
	//char *str,*str1;
	//int n,nmax,cat_to_last,ll;
	//
	//char *ptrFormat   = NULL;
	//int i             = 0;
	//int NumberPercent = 0;
	//int NumberCols    = 0;

	//Nbvars = 0;
	//CheckRhs(1,1000);
	//CheckLhs(0,1);
	//
	//if ( Rhs < 1 )
	//{
	//	Scierror(999,_("%s: Wrong number of input arguments: at least %d expected.\n"),fname,1);
	//	return 0;
	//}
	//
	//for (k=2;k<=Rhs;k++)
	//{
	//	if ( (VarType(k) != sci_matrix) && (VarType(k) != sci_strings) )
	//	{
	//		OverLoad(k);
	//		return 0;
	//	}
	//}
	//
	//GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
	//ptrFormat=cstk(l1);
	//
	//for( i=0; i<(int)strlen(ptrFormat); i++)
	//{
	//	if (ptrFormat[i]=='%')
	//	{
	//		NumberPercent++;
	//		if (ptrFormat[i+1]=='%')
	//		{
	//			NumberPercent--;i++;
	//		}
	//	}
	//}
	//
	//if ( (Rhs - 1) > NumberPercent )
	//{
	//	Scierror(999,_("%s: Wrong number of input arguments: at most %d expected.\n"),fname,NumberPercent);
	//	return 0;
	//}
	//
	//if( Rhs > 1 )
	//{
	//	for( i = 2 ; i <= Rhs ; i++ )
	//	{
	//		int mk = 0;
	//		int nk = 0;
	//		GetMatrixdims(i,&mk,&nk);
	//		NumberCols += nk;
	//	}
	//}
	//
	//if ( NumberCols != NumberPercent )
	//{
	//	Scierror(999,_("%s: Wrong number of input arguments: data doesn't fit with format.\n"),fname);
	//	return 0;
	//}
	//
	//n           = 0; /* output line counter */
	//nmax        = 0;
	//strs        = NULL;
	//lcount      = 1;
	//cat_to_last = 0;

	//while (1)
	//{
	//	if ((rval=do_xxprintf("msprintf",(FILE *) 0,cstk(l1),Rhs,1,lcount,(char **) &lstr)) < 0) break;
	//	
	//	lcount++;
	//	str =(char *) lstr;
	//	if ( str == NULL )
	//	{
	//		Scierror(999,_("%s: Wrong value of input argument %d: data doesn't fit with format.\n"),fname,1);
	//		return 0;
	//	}
	//	str1 = str;
	//	while (*str != '\0')  {
	//		if (strncmp(str,"\\n",2) ==0) {
	//			k=(int)(str-str1);
	//			if (! cat_to_last) 
	//			{ 
	//				/*add a new line */
	//				if (n==nmax) 
	//				{
	//					nmax+=blk;
	//					if (strs) {
	//						strs = (char **) REALLOC(strs,nmax*sizeof(char **));
	//					} else {
	//						strs = (char **) MALLOC(nmax*sizeof(char **));
	//					}
	//					if ( strs == NULL) {
	//						Scierror(999,_("%s: No more memory.\n"),fname);
	//						return 0;
	//					}
	//				}
	//				if ((strs[n]=MALLOC((k+1))) == NULL) {
	//					Scierror(999,_("%s: No more memory.\n"),fname);
	//					return 0;
	//				}
	//				strncpy(strs[n],str1, k);
	//				strs[n][k]='\0';
	//				n++;
	//			}
	//			else { /* cat to previous line */
	//				ll=(int)strlen(strs[n-1]);
	//				if ((strs[n-1]=REALLOC(strs[n-1],(k+1+ll))) == NULL) {
	//					Scierror(999,_("%s: No more memory.\n"),fname);
	//					return 0;
	//				}
	//				strncpy(&(strs[n-1][ll]),str1, k);
	//				strs[n-1][k+ll]='\0';
	//			}
	//			k=0;
	//			str+=2;
	//			str1=str;
	//			cat_to_last=0;
	//		}
	//		else
	//			{
	//				str++;
	//			}
	//	}
	//	k=(int)(str-str1); /* @TODO add comment */
	//	if (k>0) {
	//		if ((! cat_to_last) || (n == 0)) { /*add a new line */
	//			if (n==nmax) {
	//				nmax+=blk;
	//				if (strs)
	//				{
	//					if ((strs = (char **) REALLOC(strs,nmax*sizeof(char **))) == NULL) {
	//						Scierror(999,_("%s: No more memory.\n"),fname);
	//						return 0;
	//					}
	//				}
	//				else
	//				{
	//					if ( (strs = (char **) MALLOC(nmax*sizeof(char **))) == NULL) {
	//						Scierror(999,_("%s: No more memory.\n"),fname);
	//						return 0;
	//					}
	//				}

	//			}
	//			if ((strs[n]=MALLOC((k+1))) == NULL) {
	//				Scierror(999,_("%s: No more memory.\n"),fname);
	//				return 0;
	//			}
	//			strncpy(strs[n],str1, k);
	//			strs[n][k]='\0';
	//			n++;

	//		}
	//		else { /* cat to previous line */
	//			ll=(int)strlen(strs[n-1]);
	//			if ((strs[n-1]=REALLOC(strs[n-1],(k+1+ll))) == NULL) {
	//				Scierror(999,_("%s: No more memory.\n"),fname);
	//				return 0;
	//			}
	//			strncpy(&(strs[n-1][ll]),str1, k);
	//			strs[n-1][k+ll]='\0';
	//		}
	//	}
	//	if (strncmp(str-2,"\\n",2) !=0) cat_to_last=1;
	//	if (Rhs == 1) break;

	//}
	//if (rval == RET_BUG) return 0;
	///** Create a Scilab String : lstr must not be freed **/
	//n2=1;
	//CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &n, &n2, strs);

	//freeArrayOfString(strs, n);
	//
	//LhsVar(1)=Rhs+1;
	//PutLhsVar();
	//return 0;
//}
/*--------------------------------------------------------------------------*/ 
