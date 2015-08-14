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
#include "readwrite.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "Scierror.h"
#include "expandPathVariable.h"

    extern int C2F(clunit)(int*, char const*, int*, int);

    extern int C2F(writedouble)(char* form, double* dat, int* m, int* n, int* ierro, int);
    extern int C2F(writedoublefile)(int* ID, char* form, double* dat, int* m, int* n, int* ierro, int);
    extern int C2F(writedoubleszsc)(char* form, double* dat, int* szsr, int* m, int* n, int* ierro, int);
    extern int C2F(writedoubleszscfile)(int* ID, double* dat, int* szsr, int* m, int* n, int* ierro, int);

    extern int C2F(writeint)(char* form, int* dat, int* m, int* n, int* ierro, int);
    extern int C2F(writeintfile)(int* ID, char* form, int* dat, int* m, int* n, int* ierro, int);
    extern int C2F(writeintszsc)(char* form, int* dat, int* szsr, int* m, int* n, int* ierro, int);
    extern int C2F(writeintszscfile)(int* ID, char* form, int* dat, int* szsr, int* m, int* n, int* ierro, int);

    extern int C2F(writestring)(char* form, char* dat, int* ierro, int, int);
    extern int C2F(writestringfile)(int* ID, char* form, char* dat, int* ierro, int, int);

}

using namespace types;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_write(typed_list &in, int _iRetCount, typed_list &out)
{
    int iID = 10000;
    int iAcces = 0;
    int iRhs = (int)in.size();
    char* pstFormat = NULL;

    types::InternalType::ScilabType itTypeOfData = types::InternalType::ScilabDouble;

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

        if (iErr == 240)
        {
            closeFile(in[0], iID);
            Scierror(999, _("File \"%s\" already exists or directory write access denied.\n"), pstFilename);
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

    if (iRhs == 4)
    {
        //iAccess is thrid parameter
    }

    int iRows = in[1]->getAs<types::GenericType>()->getRows();
    int iCols;
    if (in[1]->isString())
    {
        iCols = in[1]->getAs<types::GenericType>()->getSize();
    }
    else
    {
        iCols = in[1]->getAs<types::GenericType>()->getCols();
    }
    if (iCols == 0)
    {
        // the value is empty : do not display anything
        return Function::OK;
    }

    if (iRhs > 2)
    {
        int iPos = iRhs - 1;
        if (in[iPos]->isString() == false)
        {
            closeFile(in[0], iID);
            Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "write", iRhs);
            return Function::Error;
        }

        String* pSFormat = in[iPos]->getAs<String>();
        if (pSFormat->isScalar() == false)
        {
            closeFile(in[0], iID);
            Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "write", iRhs);
            return Function::Error;
        }

        //checkformat
        pstFormat = wide_string_to_UTF8(pSFormat->get(0));

        itTypeOfData = checkformat(pstFormat);
        if (itTypeOfData == InternalType::ScilabNull)
        {
            closeFile(in[0], iID);
            Scierror(999, _("Incorrect file or format.\n"));
            return Function::Error;
        }
    }
    else
    {
        switch (in[1]->getType())
        {
            case types::InternalType::ScilabInt32:
            {
                itTypeOfData = types::InternalType::ScilabInt32;
                pstFormat = (char*)MALLOC(sizeof(char) * (strlen("(%d(1x,I4))") + (int)log10(iCols) + 2));
                sprintf(pstFormat, "(%d(1x,I4))", iCols);
                break;
            }
            case types::InternalType::ScilabDouble:
            {
                if (iID == 6)
                {
                    pstFormat = (char*)MALLOC(sizeof(char) * (strlen("((1x,1pd17.10))") + (int)log10(iCols) + 2));
                    sprintf(pstFormat, "(%d(1x,1pd17.10))", iCols);
                }
                else
                {
                    pstFormat = (char*)MALLOC(sizeof(char) * (strlen("((1x,1pd17.10))") + (int)log10(iCols) + 2));
                    sprintf(pstFormat, "(%d(1x,1pd17.10))", iCols);
                }
                break;
            }
            case types::InternalType::ScilabString:
            {
                itTypeOfData = types::InternalType::ScilabString;
                pstFormat = (char*)MALLOC(sizeof(char) * (strlen("(a)") + 1));
                sprintf(pstFormat, "(a)");
                break;
            }
            default:
            {
                closeFile(in[0], iID);
                Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "write", 2);
                return Function::Error;
            }
        }
    }

    int error = 0;
    if (iAcces == 0)
    {
        if (iID != 6 /*stdout*/)
        {
            //sequential
            switch (in[1]->getType())
            {
                case types::InternalType::ScilabString:
                {
                    char* pd = NULL;
                    types::String* pD = in[1]->getAs<String>();

                    for (int i = 0; i < iCols; i++)
                    {
                        pd = wide_string_to_UTF8(pD->get(i));
                        C2F(writestringfile)(&iID, pstFormat, pd, &error, (int)strlen(pstFormat), (int)strlen(pd));
                        FREE(pd);
                    }
                }
                break;
                case types::InternalType::ScilabInt32:
                {
                    types::Int32* pI = in[1]->getAs<Int32>();
                    int* pi = pI->get();

                    if (itTypeOfData == in[1]->getType())
                    {
                        if (iRhs > 2)
                        {
                            C2F(writeintfile)(&iID, pstFormat, pi, &iRows, &iCols, &error, (int)strlen(pstFormat));
                        }
                        else
                        {
                            //we calculate the size for a set of data on a line
                            int iConsoleWidth = ConfigVariable::getConsoleWidth();
                            iConsoleWidth = ((int)(iConsoleWidth / 18)) * 18;
                            C2F(writeintszscfile)(&iID, pstFormat, pi, &iConsoleWidth, &iRows, &iCols, &error, (int)strlen(pstFormat));
                        }
                    }
                    else if (itTypeOfData == types::InternalType::ScilabDouble)
                    {
                        double* pd = new double [pI->getSize()];
                        for (int i = 0; i < pI->getSize(); i++)
                        {
                            pd[i] = (double)pi[i];
                        }

                        if (iRhs > 2)
                        {
                            C2F(writedoublefile)(&iID, pstFormat, pd, &iRows, &iCols, &error, (int)strlen(pstFormat));
                        }
                        else
                        {
                            //we calculate the size for a set of data on a line
                            int iConsoleWidth = ConfigVariable::getConsoleWidth();
                            iConsoleWidth = ((int)(iConsoleWidth / 18)) * 18;
                            C2F(writedoubleszscfile)(&iID, pd, &iConsoleWidth, &iRows, &iCols, &error, (int)strlen(pstFormat));
                        }

                        delete[] pd;
                    }
                }
                break;
                case types::InternalType::ScilabDouble:
                {
                    Double* pD = in[1]->getAs<Double>();
                    double* pd = pD->get();

                    if (itTypeOfData == in[1]->getType())
                    {
                        if (iRhs > 2)
                        {
                            C2F(writedoublefile)(&iID, pstFormat, pd, &iRows, &iCols, &error, (int)strlen(pstFormat));
                        }
                        else
                        {
                            //we calculate the size for a set of data on a line
                            int iConsoleWidth = ConfigVariable::getConsoleWidth();
                            iConsoleWidth = ((int)(iConsoleWidth / 18)) * 18;
                            C2F(writedoubleszscfile)(&iID, pd, &iConsoleWidth, &iRows, &iCols, &error, (int)strlen(pstFormat));
                        }
                    }
                    else if (itTypeOfData == types::InternalType::ScilabInt32)
                    {
                        int* pi = new int [pD->getSize()];
                        for (int i = 0; i < pD->getSize(); i++)
                        {
                            pi[i] = (int)pd[i];
                        }

                        if (iRhs > 2)
                        {
                            C2F(writeintfile)(&iID, pstFormat, pi, &iRows, &iCols, &error, (int)strlen(pstFormat));
                        }
                        else
                        {
                            //we calculate the size for a set of data on a line
                            int iConsoleWidth = ConfigVariable::getConsoleWidth();
                            iConsoleWidth = ((int)(iConsoleWidth / 18)) * 18;
                            C2F(writeintszscfile)(&iID, pstFormat, pi, &iConsoleWidth, &iRows, &iCols, &error, (int)strlen(pstFormat));
                        }

                        delete[] pi;
                    }
                }
                break;
                default:
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "write", 2);
                    closeFile(in[0], iID);
                    return Function::Error;
                }
            }

            //close file
            closeFile(in[0], iID);
        }
        else
        {
            //direct
            int iConsoleWidth = ConfigVariable::getConsoleWidth();
            switch (in[1]->getType())
            {
                case types::InternalType::ScilabString:
                {
                    char* pd = NULL;
                    types::String* pD = in[1]->getAs<String>();
                    for (int i = 0; i < iCols; i++)
                    {
                        pd = wide_string_to_UTF8(pD->get(i));
                        C2F(writestring)(pstFormat, pd, &error, (int)strlen(pstFormat), (int)strlen(pd));
                        FREE(pd);
                    }
                }
                break;
                case types::InternalType::ScilabInt32 :
                {
                    types::Int32* pI = in[1]->getAs<Int32>();
                    int* pi = pI->get();

                    if (itTypeOfData == in[1]->getType())
                    {
                        if (iRhs > 2)
                        {
                            C2F(writeint)(pstFormat, pi, &iRows, &iCols, &error, (int)strlen(pstFormat));
                        }
                        else
                        {
                            //we calculate the size for a set of data on a line
                            int iConsoleWidth = ConfigVariable::getConsoleWidth();
                            iConsoleWidth = ((int)(iConsoleWidth / 18)) * 18;
                            C2F(writeintszsc)(pstFormat, pi, &iConsoleWidth, &iRows, &iCols, &error, (int)strlen(pstFormat));
                        }
                    }
                    else if (itTypeOfData == types::GenericType::ScilabDouble)
                    {
                        double* pd = new double[pI->getSize()];
                        for (int i = 0; i < pI->getSize(); i++)
                        {
                            pd[i] = (double)pi[i];
                        }

                        if (iRhs > 2)
                        {
                            C2F(writedouble)(pstFormat, pd, &iRows, &iCols, &error, (int)strlen(pstFormat));
                        }
                        else
                        {
                            //we calculate the size for a set of data on a line
                            int iConsoleWidth = ConfigVariable::getConsoleWidth();
                            iConsoleWidth = ((int)(iConsoleWidth / 18)) * 18;
                            C2F(writedoubleszsc)(pstFormat, pd, &iConsoleWidth, &iRows, &iCols, &error, (int)strlen(pstFormat));
                        }

                        delete[] pd;
                    }
                }
                break;
                case types::InternalType::ScilabDouble:
                {
                    Double* pD = in[1]->getAs<Double>();
                    double* pd = pD->get();

                    if (itTypeOfData == in[1]->getType())
                    {
                        if (iRhs > 2)
                        {
                            C2F(writedouble)(pstFormat, pd, &iRows, &iCols, &error, (int)strlen(pstFormat));
                        }
                        else
                        {
                            //we calculate the size for a set of data on a line
                            int iConsoleWidth = ConfigVariable::getConsoleWidth();
                            iConsoleWidth = ((int)(iConsoleWidth / 18)) * 18;
                            C2F(writedoubleszsc)(pstFormat, pd, &iConsoleWidth, &iRows, &iCols, &error, (int)strlen(pstFormat));
                        }
                    }
                    else if (itTypeOfData == types::GenericType::ScilabInt32)
                    {
                        int* pi = new int[pD->getSize()];
                        for (int i = 0; i < pD->getSize(); i++)
                        {
                            pi[i] = (int)pd[i];
                        }

                        if (iRhs > 2)
                        {
                            C2F(writeint)(pstFormat, pi, &iRows, &iCols, &error, (int)strlen(pstFormat));
                        }
                        else
                        {
                            //we calculate the size for a set of data on a line
                            int iConsoleWidth = ConfigVariable::getConsoleWidth();
                            iConsoleWidth = ((int)(iConsoleWidth / 18)) * 18;
                            C2F(writeintszsc)(pstFormat, pi, &iConsoleWidth, &iRows, &iCols, &error, (int)strlen(pstFormat));
                        }

                        delete[] pi;
                    }
                }
                break;
                default:
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "write", 2);
                    return Function::Error;
                }
            }
        }
    }

    if (pstFormat)
    {
        FREE(pstFormat);
    }

    if (error != 0)
    {
        Scierror(999, _("Incorrect file or format.\n"));
        return Function::Error;
    }

    return Function::OK;
}
