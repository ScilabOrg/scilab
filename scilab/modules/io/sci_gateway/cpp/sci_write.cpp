/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string.h>
#include "function.hxx"
#include "io_gw.hxx"
#include "string.hxx"
#include "double.hxx"
#include "filemanager.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "Scierror.h"
#include "expandPathVariable.h"

    extern int C2F(clunit)(int*, char const*, int*, int);
    extern int C2F(write_double)(char* form, double* dat, int* m, int* n, int);
}

int checkformat(char* format);


using namespace types;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_write(typed_list &in, int _iRetCount, typed_list &out)
{
    int iID = -1;
    int iAcces = 0;
    int iRhs = (int)in.size();
    char* pstFormat = NULL;
    if (iRhs < 2 || iRhs > 4)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "write", 2, 4);
        return Function::Error;
    }

    //file
    if (in[0]->isString())
    {
        String* pSPath = in[0]->getAs<String>();

        if (pSPath->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "write", 1);
            return Function::Error;
        }

        int piMode[2] = {0, 0};
        char* pstFilename = wide_string_to_UTF8(pSPath->get(0));
        int iErr = C2F(clunit)(&iID, pstFilename, piMode, (int)strlen(pstFilename));
        FREE(pstFilename);
    }
    else if (in[0]->isDouble())
    {
        Double* pDId = in[0]->getAs<Double>();
        if (pDId->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "write", 1);
            return Function::Error;
        }

        iID = (int)pDId->get(0);
        if (iID == -1)
        {
            iID = FileManager::getCurrentFile();
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar or file descriptor expected.\n"), "write", 1);
        return Function::Error;
    }

    if (iRhs == 4)
    {
        //iAccess is thrid parameter
    }

    if (iRhs > 2)
    {
        int iPos = iRhs - 1;
        if (in[iPos]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "write", iRhs);
            return Function::Error;
        }

        String* pSFormat = in[iPos]->getAs<String>();
        if (pSFormat->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "write", iRhs);
            return Function::Error;
        }

        //checkformat
        pstFormat = wide_string_to_UTF8(pSFormat->get(0));
        if (checkformat(pstFormat) == 0)
        {
            Scierror(999, _("Incorrect file or format.\n"));
            return Function::Error;
        }
    }
    else
    {
        pstFormat = (char*)MALLOC(sizeof(char) * 2);
        pstFormat[0] = ' ';
        pstFormat[1] = '\0';
    }

    if (iAcces == 0)
    {
        if (iID != 6 /*stdout*/)
        {
            //sequential
        }
        else
        {
            //direct
            Double* pD = in[1]->getAs<Double>();
            double* pd = pD->get();
            int iRows = pD->getRows();
            int iCols = pD->getCols();

            C2F(write_double)(pstFormat, pd, &iRows, &iCols, strlen(pstFormat));
        }
    }
    else
    {

    }

    return Function::OK;
}

int checkformat(char* format)
{
    char type1[] = {'i', 'f', 'e', 'd', 'g', 'l', 'a'};
    char type2[] = {'2', '1', '1', '1', '1', '3', '4'};
    int ret = 0;
    int size = (int)strlen(format);
    int count = 0;
    if (size < 2 || format[0] != '(' || format[size - 1] != ')')
    {
        return 0;
    }

    for (int i = 1; i < size - 1; i++)
    {
        char c = format[i];
        if (c == '\'')
        {
            if (count == 0)
            {
                count++;
            }
            else
            {
                count--;
            }
            continue;
        }

        for (int j = 0; j < sizeof(type1); j++)
        {
            if (c == type1[j])
            {
                if (ret == 0)
                {
                    ret = type2[j];
                }

                if (type2[j] != ret)
                {
                    return 0;
                }

                break;
            }
        }
    }

    return ret;
}