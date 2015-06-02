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

    extern int C2F(readdoublefile)(int* ID, double* dat, int* m, int* n, int* err);
    extern int C2F(readdoublefileform)(int* ID, char* form, double* dat, int* m, int* n, int* err, int);
    extern int C2F(readdoublelinefile)(int* ID, double* dat, int* n, int* err);
    extern int C2F(readdoublelinefileform)(int* ID, char* form, double* dat, int* n, int* err);

    extern int C2F(readintfileform)(int* ID, char* form, int* dat, int* m, int* n, int* err, int);
    extern int C2F(readintlinefileform)(int* ID, char* form, int* dat, int* n, int* err);

    extern int C2F(readstringfile)(int* ID, char* form, char* dat, int* siz, int* err, int);
    extern int C2F(readstring)(char* form, char* dat, int* siz, int* err, int);

}

using namespace types;

InternalType::ScilabType checkformatread(const char* format);

template<typename T>
bool is_of_type(const std::string & Str)
{
    std::istringstream iss(Str);

    T tmp;
    return (iss >> tmp) && (iss.eof());
}


/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_read(typed_list &in, int _iRetCount, typed_list &out)
{
    int iID = 0;
    int iAcces = 0;
    int iRhs = static_cast<int>(in.size());
    char* pstFormat = NULL;
    InternalType::ScilabType itTypeOfData = InternalType::ScilabDouble;

    if (iRhs < 3 || iRhs > 5)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "read", 2, 4);
        return Function::Error;
    }

    //file
    if (in[0]->isString())
    {
        String* pSPath = in[0]->getAs<String>();

        if (pSPath->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "read", 1);
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
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "read", 1);
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
        Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar or file descriptor expected.\n"), "read", 1);
        return Function::Error;
    }

    if (iRhs > 3)
    {
        int iPos = iRhs - 1;
        if (in[iPos]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "read", iRhs);
            return Function::Error;
        }

        String* pSFormat = in[iPos]->getAs<String>();
        if (pSFormat->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "read", iRhs);
            return Function::Error;
        }

        //checkformat
        pstFormat = wide_string_to_UTF8(pSFormat->get(0));

        itTypeOfData = checkformatread(pstFormat);
        if (itTypeOfData == InternalType::ScilabNull)
        {
            Scierror(999, _("Incorrect file or format.\n"));
            return Function::Error;
        }
    }

    int error = 0;

    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar integer value expected.\n"), "read", 2);
        return Function::Error;
    }

    Double* pIn1 = in[1]->getAs<Double>();
    if (pIn1->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar integer value expected.\n"), "read", 2);
        return Function::Error;
    }

    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar integer value expected.\n"), "read", 3);
        return Function::Error;
    }

    Double* pIn2 = in[2]->getAs<Double>();
    if (pIn2->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar integer value expected.\n"), "read", 3);
        return Function::Error;
    }

    int iRows = (int)pIn1->get(0);
    int iCols = (int) pIn2->get(0);

    //test dims
    if ( (iCols <= 0) || (iRows == 0))
    {
        if (pstFormat != NULL)
        {
            FREE(pstFormat);
        }
        out.push_back(Double::Empty());
        return Function::OK;
    }

    if (iRows < 0)
    {
        if (iID != 5 /*stdin*/)
        {
            switch (itTypeOfData)
            {
                case InternalType::ScilabDouble:
                {
                    iRows = 1;
                    Double* pD = new Double(iRows, iCols, false);

                    if (pstFormat == NULL)
                    {
                        while (error == 0)
                        {
                            double* pdData = new double[iCols];
                            C2F(readdoublelinefile)(&iID, pdData, &iCols, &error);
                            if (error == 0)
                            {
                                pD->resize(iRows, iCols);
                                for (int i = 0; i < iCols; ++i)
                                {
                                    pD->set((iRows - 1), i, pdData[i]);
                                }
                                ++iRows;
                            }
                            delete[] pdData;
                        }
                    }
                    else
                    {
                        while (error == 0)
                        {
                            double* pdData = new double[iCols];
                            C2F(readdoublelinefileform)(&iID, pstFormat, pdData, &iCols, &error);
                            if (error == 0)
                            {
                                pD->resize(iRows, iCols);
                                for (int i = 0; i < iCols; ++i)
                                {
                                    pD->set((iRows - 1), i, pdData[i]);
                                }
                                ++iRows;
                            }
                            delete[] pdData;
                        }
                    }

                    if (error != 2)
                    {
                        out.push_back(pD);
                    }
                    else
                    {
                        delete pD;
                    }
                }
                break;
                case InternalType::ScilabInt32:
                {
                    iRows = 1;
                    Int32* pI = new Int32(iRows, iCols);

                    while (error == 0)
                    {
                        int* piData = new int[iCols];
                        C2F(readintlinefileform)(&iID, pstFormat, piData, &iCols, &error);
                        if (error == 0)
                        {
                            pI->resize(iRows, iCols);
                            for (int i = 0; i < iCols; ++i)
                            {
                                pI->set((iRows - 1), i, piData[i]);
                            }
                            ++iRows;
                        }
                        delete[] piData;
                    }

                    if (error != 2)
                    {
                        out.push_back(pI);
                    }
                    else
                    {
                        delete pI;
                    }
                }
                break;
                case InternalType::ScilabString:
                {
                    if (iCols != 1)
                    {
                        Scierror(999, _("%s: Wrong input argument %d.\n"), "read", 3);
                        return Function::Error;
                    }
                    else
                    {
                        String* pS = new String(iRows, iCols);

                        while (error == 0)
                        {
                            char pCt[4096];
                            int siz = 0;
                            C2F(readstringfile)(&iID, pstFormat, pCt, &siz, &error, (int)strlen(pstFormat));
                            pCt[siz] = '\0';

                            if (error == 0)
                            {
                                pS->resize(iRows, iCols);
                                pS->set((iRows - 1), (iCols - 1), pCt);
                                ++iRows;
                            }
                        }

                        if (error != 2)
                        {
                            out.push_back(pS);
                        }
                        else
                        {
                            delete pS;
                        }
                    }
                }
                break;
                default:
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "read", 2);

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
        }
        else//read from the console
        {
            switch (itTypeOfData)
            {
                case InternalType::ScilabDouble:
                {
                    iRows = 1;
                    Double* pD = new Double(iRows, iCols, false);

                    char pstString[4] = "(a)";

                    while (error != 2)
                    {
                        char pCt[4096];
                        int siz = 0;
                        C2F(readstring)(pstString, pCt, &siz, &error, (int)strlen(pstString));
                        pCt[siz] = '\0';

                        if ((siz == 1) && (pCt[0] == ' '))
                        {
                            break;
                        }

                        char* pch;
                        int iColsTempo = 0;
                        pch = strtok(pCt, " ");
                        double* pdData = new double[iCols];
                        while ((pch != NULL) && (error == 0) && (iColsTempo < iCols))
                        {
                            if (is_of_type<double>(pch))
                            {
                                pdData[iColsTempo] = atof(pch);
                                iColsTempo++;
                            }
                            else
                            {
                                error = 2;
                            }
                            pch = strtok(NULL, " ");
                        }

                        if ((siz == 1) && (iColsTempo == 0))
                        {
                            delete[] pdData;
                            break;
                        }
                        if (iColsTempo != iCols)
                        {
                            delete[] pdData;
                            delete pD;

                            Scierror(999, _("End of file at line %d.\n"));
                            return Function::Error;
                        }
                        else
                        {
                            pD->resize(iRows, iCols);
                            for (int i = 0; i < iCols; i++)
                            {
                                pD->set((iRows - 1), i, pdData[i]);
                            }
                        }

                        //next line
                        ++iRows;
                    }

                    if (error == 0)
                    {
                        out.push_back(pD);
                    }
                }
                break;
                case InternalType::ScilabInt32:
                    Scierror(999, _("Incorrect file or format.\n"));
                    return Function::Error;
                case InternalType::ScilabString:
                {
                    if (iCols != 1)
                    {
                        Scierror(999, _("%s: Wrong input argument %d.\n"), "read", 3);
                        return Function::Error;
                    }
                    else
                    {
                        bool bEndWrite = false;
                        iRows = 1;
                        String* pS = new String(iRows, iCols);

                        for (; bEndWrite == false; iRows++)
                        {
                            char pCt[4096];
                            int siz = 0;
                            C2F(readstring)(pstFormat, pCt, &siz, &error, (int)strlen(pstFormat));
                            pCt[siz] = '\0';

                            if ((siz == 1) && (pCt[0] == ' '))
                            {
                                bEndWrite = true;
                            }
                            else
                            {
                                pS->resize(iRows, iCols);
                                pS->set((iRows - 1), (iCols - 1), pCt);
                            }
                            FREE(pCt);

                        }

                        if (error == 0)
                        {
                            out.push_back(pS);
                        }
                        else
                        {
                            delete pS;
                        }
                    }
                }
                break;
                default:
                    break;
            }
        }
    }
    else
    {
        if (iID != 5 /*stdin*/)
        {
            switch (itTypeOfData)
            {
                case InternalType::ScilabDouble:
                {
                    Double* pD = new Double(iRows, iCols, false);
                    double* pd = pD->get();

                    if (pstFormat == NULL)
                    {
                        C2F(readdoublefile)(&iID, pd, &iRows, &iCols, &error);
                    }
                    else
                    {
                        C2F(readdoublefileform)(&iID, pstFormat, pd, &iRows, &iCols, &error, (int)strlen(pstFormat));
                    }

                    if (error == 0)
                    {
                        out.push_back(pD);
                    }
                    else
                    {
                        delete pD;
                    }
                }
                break;
                case InternalType::ScilabInt32:
                {
                    Int32* pI = new Int32(iRows, iCols);
                    int* pi = pI->get();

                    C2F(readintfileform)(&iID, pstFormat, pi, &iRows, &iCols, &error, (int)strlen(pstFormat));

                    if (error == 0)
                    {
                        out.push_back(pI);
                    }
                    else
                    {
                        delete pI;
                    }
                }
                break;
                case InternalType::ScilabString:
                {
                    String* pS = new String(iRows, iCols);
                    for (int i = 0; i < iCols * iRows; ++i)
                    {
                        char pCt[4096];
                        int siz = 0;
                        C2F(readstringfile)(&iID, pstFormat, pCt, &siz, &error, (int)strlen(pstFormat));
                        pCt[siz] = '\0';
                        pS->set(i, pCt);
                    }

                    if (error == 0)
                    {
                        out.push_back(pS);
                    }
                    else
                    {
                        delete pS;
                    }
                }
                break;
                default:
                    Scierror(999, _("%s: Wrong type for input argument #%d : A string expected.\n"), "read", 2);

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

            if (error == 2)
            {
                Scierror(999, _("Incorrect file or format.\n"));
                return Function::Error;
            }
        }
        else//read from the console
        {
            switch (itTypeOfData)
            {
                case InternalType::ScilabDouble:
                {
                    Double* pD = new Double(iRows, iCols, false);

                    char pstString[4] = "(a)";

                    for (int i = 0; i < iRows && error == 0; ++i)
                    {
                        char pCt[4096];
                        int siz = 0;
                        C2F(readstring)(pstString, pCt, &siz, &error, (int)strlen(pstString));
                        pCt[siz] = '\0';

                        char* pch;
                        int iColsTempo = 0;
                        pch = strtok(pCt, " ");
                        while (pch != NULL && error == 0)
                        {
                            if (is_of_type<double>(pch))
                            {
                                pD->set(i, iColsTempo, atof(pch));
                                iColsTempo++;
                            }
                            else
                            {
                                error = 2;
                            }
                            pch = strtok(NULL, " ");
                        }

                        if (iColsTempo != iCols)
                        {
                            delete pD;
                            FREE(pstFormat);

                            Scierror(999, _("End of file at line %d.\n"));
                            return Function::Error;
                        }
                    }

                    FREE(pstFormat);

                    if (error == 0)
                    {
                        out.push_back(pD);
                    }
                    else
                    {
                        delete pD;
                    }
                }
                break;
                case InternalType::ScilabInt32:
                    Scierror(999, _("Incorrect file or format.\n"));
                    return Function::Error;
                case InternalType::ScilabString:
                {
                    if (iCols != 1)
                    {
                        Scierror(999, _("%s: Wrong input argument %d.\n"), "read", 3);
                        return Function::Error;
                    }
                    else
                    {
                        String* pS = new String(iRows, iCols);

                        for (int i = 0; i < (iRows); i++)
                        {
                            char pCt[4096];
                            int siz = 0;
                            C2F(readstring)(pstFormat, pCt, &siz, &error, (int)strlen(pstFormat));
                            pCt[siz] = '\0';
                            pS->set(i, pCt);
                        }

                        if (error == 0)
                        {
                            out.push_back(pS);
                        }
                        else
                        {
                            delete pS;
                        }
                    }
                }
                break;
                default:
                    break;
            }
        }

        if (error != 0)
        {
            Scierror(999, _("Incorrect file or format.\n"));
            return Function::Error;
        }

    }

    return Function::OK;
}

InternalType::ScilabType checkformatread(const char* format)
{
    const char type1[] =
    {
        'i', 'f', 'e',
        'd', 'g', 'l',
        'a', 'I', 'F',
        'E', 'D', 'G',
        'L', 'A'
    };
    const InternalType::ScilabType type2[] =
    {
        InternalType::ScilabInt32, InternalType::ScilabDouble, InternalType::ScilabDouble,
        InternalType::ScilabDouble, InternalType::ScilabDouble, InternalType::ScilabBool,
        InternalType::ScilabString, InternalType::ScilabInt32, InternalType::ScilabDouble,
        InternalType::ScilabDouble, InternalType::ScilabDouble, InternalType::ScilabDouble,
        InternalType::ScilabBool, InternalType::ScilabString
    };

    int size = (int)strlen(format);
    bool isString = false;
    InternalType::ScilabType previousType = InternalType::ScilabNull;

    if (size < 2 || format[0] != '(' || format[size - 1] != ')')
    {
        return InternalType::ScilabNull;
    }

    for (int i = 1; i < size - 1; ++i)
    {
        char c = format[i];

        if (c == '\'')
        {
            isString = !isString;
        }

        //while we are in string continue
        if (isString)
        {
            continue;
        }

        for (int j = 0; j < sizeof(type1); j++)
        {
            if (c == type1[j])
            {
                if (previousType == InternalType::ScilabNull)
                {
                    previousType = type2[j];
                }

                //must have same format for all values
                if (type2[j] != previousType)
                {
                    return InternalType::ScilabNull;
                }

                break;
            }
        }
    }

    return previousType;
}
