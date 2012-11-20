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
#include "string.hxx"
#include "double.hxx"
#include "int.hxx"

extern "C"
{
#include <ctype.h>
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "sci_warning.h"
}

/* Benchmark
str_test_mat =  ["abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
"abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz"; ..
"abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
"abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz"];

tic();
for i=1:10000000
ascii(["abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz"]);
end
duree = toc();

printf("\nDUREE 1 = %d seconds\n\n",duree);
*/

/* on windows 17 3770 @ 3.4 GHz */
/* scilab 4.1.2 : 17 s */
/* scilab 5.4.0 : 32 s */
/* scilab 6 dev : 16 s */




using namespace types;

/*----------------------------------------------------------------------------*/
String* DoubleToString(Double* _pdbl);
String* IntToString(InternalType* _pIT);
Double* StringToDouble(String* _pst);
/*----------------------------------------------------------------------------*/
Function::ReturnValue sci_ascii(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "ascii", 1);
        return Function::Error;
    }

    InternalType* pOut;
    switch (in[0]->getType())
    {
        case InternalType::RealDouble :
        {
            pOut = DoubleToString(in[0]->getAs<Double>());
            break;
        }
        case InternalType::RealString :
        {
            pOut = StringToDouble(in[0]->getAs<types::String>());
            break;
        }
        case InternalType::RealInt8 :
        case InternalType::RealUInt8 :
        case InternalType::RealInt16 :
        case InternalType::RealUInt16 :
        case InternalType::RealInt32 :
        case InternalType::RealUInt32 :
        case InternalType::RealInt64 :
        case InternalType::RealUInt64 :
        {
            pOut = IntToString(in[0]);
            break;
        }
        default :
            Scierror(999, _("%s: Wrong type for argument #%d: Matrix of strings or Integer matrix expected.\n"), "ascii", 1);
            return Function::Error;
    }

    out.push_back(pOut);
    return Function::OK;

}
/*--------------------------------------------------------------------------*/
String* DoubleToString(Double* _pdbl)
{
    String* pOut = NULL;
    char* pst = (char*)MALLOC(sizeof(char) * (_pdbl->getSize() + 1));
    memset(pst, 0x00, _pdbl->getSize() + 1);
    double* pdbl = _pdbl->getReal();

    bool bWarning = getWarningMode() == 0;
    for (int i = 0 ; i < _pdbl->getSize() ; i++)
    {
        if (bWarning == false && pdbl[i] > 255)
        {
            sciprint(_("WARNING : \n"));
            sciprint(_("%ls: Wrong value for input argument #%d: Must be between %d and %d.\n"), L"ascii", 1, 0, 255);
            bWarning = true;
        }

        if (pdbl[i] == 0)
        {
            pst[i] = ' ';
        }
        else
        {
            pst[i] = (char)pdbl[i];
        }
    }

    wchar_t* pwst = to_wide_string(pst);
    pOut = new String(1, 1);
    pOut->set(0, 0, pwst);
    return pOut;
}
/*--------------------------------------------------------------------------*/
String* IntToString(InternalType* _pIT)
{
    String* pOut = NULL;
    char* pst = NULL;
    switch (_pIT->getType())
    {
        case InternalType::RealInt8 :
        {
            Int8* pI = _pIT->getAs<Int8>();
            char* p = pI->get();
            pst = (char*)MALLOC(sizeof(char) * pI->getSize() + 1);

            for (int i = 0 ; i < pI->getSize() ; i++)
            {
                if (p[i] == 0)
                {
                    pst[i] = ' ';
                }
                else
                {
                    pst[i] = p[i];
                }
            }
            break;
        }
        case InternalType::RealUInt8 :
        {
            UInt8* pI = _pIT->getAs<UInt8>();
            unsigned char* p = pI->get();
            pst = (char*)MALLOC(sizeof(char) * pI->getSize() + 1);

            for (int i = 0 ; i < pI->getSize() ; i++)
            {
                if (p[i] == 0)
                {
                    pst[i] = ' ';
                }
                else
                {
                    pst[i] = p[i];
                }
            }
            break;
        }
        case InternalType::RealInt16 :
        {
            Int16* pI = _pIT->getAs<Int16>();
            short* p = pI->get();
            pst = (char*)MALLOC(sizeof(char) * pI->getSize() + 1);

            for (int i = 0 ; i < pI->getSize() ; i++)
            {
                if (p[i] == 0)
                {
                    pst[i] = ' ';
                }
                else
                {
                    pst[i] = p[i];
                }
            }
            break;
        }
        case InternalType::RealUInt16 :
        {
            UInt16* pI = _pIT->getAs<UInt16>();
            unsigned short* p = pI->get();
            pst = (char*)MALLOC(sizeof(char) * pI->getSize() + 1);

            for (int i = 0 ; i < pI->getSize() ; i++)
            {
                if (p[i] == 0)
                {
                    pst[i] = ' ';
                }
                else
                {
                    pst[i] = p[i];
                }
            }
            break;
        }
        case InternalType::RealInt32 :
        {
            Int32* pI = _pIT->getAs<Int32>();
            int* p = pI->get();
            pst = (char*)MALLOC(sizeof(char) * pI->getSize() + 1);

            for (int i = 0 ; i < pI->getSize() ; i++)
            {
                if (p[i] == 0)
                {
                    pst[i] = ' ';
                }
                else
                {
                    pst[i] = p[i];
                }
            }
            break;
        }
        case InternalType::RealUInt32 :
        {
            UInt32* pI = _pIT->getAs<UInt32>();
            unsigned int* p = pI->get();
            pst = (char*)MALLOC(sizeof(char) * pI->getSize() + 1);

            for (int i = 0 ; i < pI->getSize() ; i++)
            {
                if (p[i] == 0)
                {
                    pst[i] = ' ';
                }
                else
                {
                    pst[i] = p[i];
                }
            }
            break;
        }
        case InternalType::RealInt64 :
        {
            Int64* pI = _pIT->getAs<Int64>();
            long long* p = pI->get();
            pst = (char*)MALLOC(sizeof(char) * pI->getSize() + 1);

            for (int i = 0 ; i < pI->getSize() ; i++)
            {
                if (p[i] == 0)
                {
                    pst[i] = ' ';
                }
                else
                {
                    pst[i] = p[i];
                }
            }
            break;
        }
        case InternalType::RealUInt64 :
        {
            UInt64* pI = _pIT->getAs<UInt64>();
            unsigned long long* p = pI->get();
            pst = (char*)MALLOC(sizeof(char) * pI->getSize() + 1);

            for (int i = 0 ; i < pI->getSize() ; i++)
            {
                if (p[i] == 0)
                {
                    pst[i] = ' ';
                }
                else
                {
                    pst[i] = p[i];
                }
            }
            break;
        }
    }

    wchar_t* pwst = to_wide_string(pst);
    pOut = new String(1, 1);
    pOut->set(0, 0, pwst);
    return pOut;
}
/*--------------------------------------------------------------------------*/
Double* StringToDouble(String* _pst)
{
    Double* pOut = NULL;
    /*compute total length*/
    int iTotalLen = 0;
    int iSize = _pst->getSize();
    char** pst = new char*[iSize];
    int* pstLen = new int[iSize];
    for (int i = 0 ; i < iSize ; i++)
    {
        pst[i] = wide_string_to_UTF8(_pst->get(i));
        pstLen[i] = (int)strlen(pst[i]);
        iTotalLen += pstLen[i];
    }

    if (iTotalLen == 0)
    {
        return Double::Empty();
    }

    pOut = new Double(1, iTotalLen);
    double* pdbl = pOut->get();
    int index = 0;

    //for each element of input string matrix
    for (int i = 0 ; i < iSize ; i++)
    {
        //for each character of input string matrix
        for (int j = 0 ; j < pstLen[i] ; j++)
        {
            //transform character value as double.
            pdbl[index++] = pst[i][j];
        }
    }

    delete[] pstLen;
    for (int i = 0 ; i < iSize ; i++)
    {
        delete pst[i];
    }

    delete[] pst;
    return pOut;
}
/*--------------------------------------------------------------------------*/
