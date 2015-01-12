/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
* Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
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
#include "int.hxx"
#include "filemanager.hxx"


extern "C"
{
#include "sci_malloc.h"
#include "Scierror.h"
#include "expandPathVariable.h"

    extern int C2F(clunit)(int*, char const*, int*, int);

    extern int C2F(read_double_file)(int* ID, double* dat, int* m, int* n, int* err);
    extern int C2F(read_double_file_form)(int* ID, char* form, double* dat, int* m, int* n, int* err, int);

    extern int C2F(read_int_file_form)(int* ID, char* form, int* dat, int* m, int* n, int* err, int);

    extern int C2F(read_string_file)(int* ID, char* form, char* dat, int* siz, int* err, int);

}

using namespace types;

int checkformatread(char* format);

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_read(typed_list &in, int _iRetCount, typed_list &out)
{
    int iID = 0;
    int iAcces = 0;
    int iRhs = (int)in.size();
    char* pstFormat = NULL;

    types::InternalType::ScilabType itTypeOfData = types::InternalType::ScilabDouble;

    if (iRhs < 3 || iRhs > 5)
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

        int piMode[2] = { -1, 0};
        char* pstFilename = wide_string_to_UTF8(pSPath->get(0));
        int iErr = C2F(clunit)(&iID, pstFilename, piMode, (int)strlen(pstFilename));

        if (iErr == 240)
        {
            Scierror(999, _("File \"%s\" already exists or directory write access denied.\n"), pstFilename);
            FREE(pstFilename);
            return Function::Error;
        }

        if (iErr == 241)
        {
            Scierror(999, _("File \"%s\" does not exist or read access denied.\n"), pstFilename);
            FREE(pstFilename);
            return Function::Error;
        }

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

    if (iRhs > 3)
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

        char cTypeData = checkformatread(pstFormat);
        if (cTypeData == 0)
        {
            Scierror(999, _("Incorrect file or format.\n"));
            return Function::Error;
        }
        switch (cTypeData)
        {
            case '1':
                itTypeOfData = types::InternalType::ScilabDouble;
                break;
            case '2':
                itTypeOfData = types::InternalType::ScilabInt32;
                break;
            case '3':
                itTypeOfData = types::InternalType::ScilabBool;
                break;
            case '4':
                itTypeOfData = types::InternalType::ScilabString;
                break;
            default:
                break;
        }

    }

    int error = 0;
    //if (iID != 5 /*stdout*/)
    {
        int iRows = (int) in[1]->getAs<Double>()->get(0);
        int iCols = (int) in[2]->getAs<Double>()->get(0);

        switch (itTypeOfData)
        {
            case types::InternalType::ScilabDouble:
            {
                Double* pD = new types::Double(iRows, iCols, false);
                double* pd = pD->get();

                if (pstFormat == NULL)
                {
                    C2F(read_double_file)(&iID, pd, &iRows, &iCols, &error);
                }
                else
                {
                    C2F(read_double_file_form)(&iID, pstFormat, pd, &iRows, &iCols, &error, (int)strlen(pstFormat));
                }

                if (error == 0)
                {
                    out.push_back(pD);
                }
            }
            break;
            case types::InternalType::ScilabInt32:
            {
                types::Int32* pI = new types::Int32(iRows, iCols);
                int* pi = pI->get();

                C2F(read_int_file_form)(&iID, pstFormat, pi, &iRows, &iCols, &error, (int)strlen(pstFormat));

                if (error == 0)
                {
                    out.push_back(pI);
                }
            }
            break;
            case types::InternalType::ScilabString:
            {
                String* pS = new types::String(iRows, iCols);
                for (int i = 0; i < (iCols * iRows); i++)
                {
                    char* pCt = (char *)MALLOC(sizeof(char) * 4096);
                    int siz = 0;
                    C2F(read_string_file)(&iID, pstFormat, pCt, &siz, &error, (int)strlen(pstFormat));
                    char* pC = (char *)MALLOC(sizeof(char) * (siz + 1));
                    strncat(pC, pCt, siz);
                    pS->set(i, pC);
                    FREE(pCt);
                    FREE(pC);
                }

                if (error == 0)
                {
                    out.push_back(pS);
                }
            }
            break;
            default:
                Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "write", 2);

                //close file
                if (in[0]->isString())
                {
                    int piMode[2] = { 0, 0 };
                    String* pSPath = in[0]->getAs<String>();
                    char* pstFilename = wide_string_to_UTF8(pSPath->get(0));
                    int  close = -iID;
                    int iErr = C2F(clunit)(&close, pstFilename, piMode, (int)strlen(pstFilename));
                    FREE(pstFilename);
                }
                return Function::Error;
                break;
        }

        //close file
        if (in[0]->isString())
        {
            int piMode[2] = { 0, 0 };
            String* pSPath = in[0]->getAs<String>();
            char* pstFilename = wide_string_to_UTF8(pSPath->get(0));
            int  close = -iID;
            int iErr = C2F(clunit)(&close, pstFilename, piMode, (int)strlen(pstFilename));
            FREE(pstFilename);
        }

        if (error != 0)
        {
            Scierror(999, _("Incorrect file or format.\n"));
            return Function::Error;
        }
    }

    return Function::OK;
}

int checkformatread(char* format)
{
    char type1[] = { 'i', 'f', 'e', 'd', 'g', 'l', 'a' };
    char type2[] = { '2', '1', '1', '1', '1', '3', '4' };
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