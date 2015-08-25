/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "function.hxx"
#include "string.hxx"
#include "funcmanager.hxx"
#include "filemanager.hxx"
#include "fileio_gw.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "splitpath.h"
}

enum PartialPart
{
    PathPart,
    NamePart,
    ExtensionPart,
    AllPart = -1
};
/*--------------------------------------------------------------------------*/
#define FILEPARTS_PATH_SELECTOR L"path"
#define FILEPARTS_FNAME_SELECTOR L"fname"
#define FILEPARTS_EXTENSION_SELECTOR L"extension"

using namespace types;
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_fileparts(typed_list &in, int _iRetCount, typed_list &out)
{
    PartialPart iPartialPart = AllPart;

    types::String* pStrPath = NULL;
    types::String* pStrOut  = NULL;
    types::String* pStrOut2 = NULL;
    types::String* pStrOut3 = NULL;

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "fileparts", 1, 2);
        return Function::Error;
    }

    if (in.size() == 2 && _iRetCount != 1 && _iRetCount != -1)
    {
        Scierror(78, _("%s: Wrong number of output arguments: %d expected.\n"), "fileparts", 1);
        return Function::Error;
    }

    if (in.size() == 1 && _iRetCount > 3)
    {
        Scierror(78, _("%s: Wrong number of output arguments: %d to %d expected.\n"), "fileparts", 1, 3);
        return Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "fileparts", 1);
        return Function::Error;
    }

    pStrPath = in[0]->getAs<types::String>();

    if (in.size() == 2)
    {
        if (in[1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "fileparts", 2);
            return Function::Error;
        }

        if (in[1]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), "fileparts", 2);
            return Function::Error;
        }

        wchar_t* pParts = in[1]->getAs<types::String>()->get(0);
        if (wcscmp(pParts, FILEPARTS_PATH_SELECTOR) == 0)
        {
            iPartialPart = PathPart;
        }
        else if (wcscmp(pParts, FILEPARTS_FNAME_SELECTOR) == 0)
        {
            iPartialPart = NamePart;
        }
        else if (wcscmp(pParts, FILEPARTS_EXTENSION_SELECTOR) == 0)
        {
            iPartialPart = ExtensionPart;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d.\n"), "fileparts", 2);
            return Function::Error;
        }

        pStrOut = new types::String(pStrPath->getDims(), pStrPath->getDimsArray());
        for (int i = 0; i < pStrPath->getSize(); i++)
        {
            wchar_t* pPath = pStrPath->get(i);

            wchar_t* pwstDrive      = new wchar_t[wcslen(pPath) + 1];
            wchar_t* pwstDirectory  = new wchar_t[wcslen(pPath) + 1];
            wchar_t* pwstName       = new wchar_t[wcslen(pPath) + 1];
            wchar_t* pwstExtension  = new wchar_t[wcslen(pPath) + 1];

            splitpathW(pPath, FALSE, pwstDrive, pwstDirectory, pwstName, pwstExtension);
            wcscat(pwstDrive, pwstDirectory);

            switch (iPartialPart)
            {
                case PathPart :
                {
                    pStrOut->set(i, pwstDrive);
                    break;
                }
                case NamePart :
                {
                    pStrOut->set(i, pwstName);
                    break;
                }
                case ExtensionPart :
                {
                    pStrOut->set(i, pwstExtension);
                    break;
                }
                default :
                {
                    //Never occur
                }
            }

            delete[] pwstDirectory;
            delete[] pwstDrive;
            delete[] pwstExtension;
            delete[] pwstName;
        }

        out.push_back(pStrOut);
    }
    else
    {
        pStrOut = new types::String(pStrPath->getDims(), pStrPath->getDimsArray());
        pStrOut2 = new types::String(pStrPath->getDims(), pStrPath->getDimsArray());
        pStrOut3 = new types::String(pStrPath->getDims(), pStrPath->getDimsArray());

        for (int i = 0; i < pStrPath->getSize(); i++)
        {
            wchar_t* pPath = pStrPath->get(i);

            wchar_t* pwstDrive      = new wchar_t[wcslen(pPath) + 1];
            wchar_t* pwstDirectory  = new wchar_t[wcslen(pPath) + 1];
            wchar_t* pwstName       = new wchar_t[wcslen(pPath) + 1];
            wchar_t* pwstExtension  = new wchar_t[wcslen(pPath) + 1];

            splitpathW(pPath, FALSE, pwstDrive, pwstDirectory, pwstName, pwstExtension);
            wcscat(pwstDrive, pwstDirectory);

            //standard case, 3 outputs
            pStrOut->set(i, pwstDrive);
            pStrOut2->set(i, pwstName);
            pStrOut3->set(i, pwstExtension);

            delete[] pwstDirectory;
            delete[] pwstDrive;
            delete[] pwstExtension;
            delete[] pwstName;
        }

        out.push_back(pStrOut);
        if (_iRetCount > 1)
        {
            out.push_back(pStrOut2);
            if (_iRetCount == 3)
            {
                out.push_back(pStrOut3);
            }
            else
            {
                delete pStrOut3;

            }
        }
        else
        {
            delete pStrOut2;
            delete pStrOut3;
        }
    }

    return Function::OK;
}
/*--------------------------------------------------------------------------*/
