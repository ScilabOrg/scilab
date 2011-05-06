/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2010-2010 - DIGITEO - Clément DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution. The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "Xcos.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "callxcos.h"
#include "charEncoding.h"
#include "getScilabJavaVM.h"
#include "charEncoding.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_xcos;
/*--------------------------------------------------------------------------*/
int callXcos(char **_filenames, int _nbfiles)
{
    try
    {
        if ( (_filenames) && (_nbfiles > 0) )
        {
            int i = 0;
            for (i = 0; i < _nbfiles; i++)
            {
                Xcos::xcos(getScilabJavaVM(), _filenames[i]);
            }
        }
        else
        {
            Xcos::xcos(getScilabJavaVM());
        }
    }
    catch (GiwsException::JniCallMethodException exception)
    {
        std::cerr << "[EXCEPTION JniCallMethodException] {" << exception.getJavaDescription().c_str() << "}"<< std::endl;
        std::cerr << "[EXCEPTION JniCallMethodException] {" << exception.what() << "}"<< std::endl;
        std::cerr << "[EXCEPTION JniCallMethodException] {" << exception.getJavaStackTrace() << "}"<< std::endl;
        std::cerr << "[EXCEPTION JniCallMethodException] {" << exception.getJavaExceptionName() << "}"<< std::endl;
        ScierrorW(999, L"%s: %s\n", "xcos", exception.getJavaDescription().c_str());
        return 0;
    }
    catch (GiwsException::JniException exception)
    {
        std::cerr << "[EXCEPTION JniException] " << exception.what() << std::endl;
        ScierrorW(999, L"%s: %s\n", "xcos", exception.what());
        return 0;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int callXcosW(wchar_t **_wcfilenames, int _nbfiles)
{
    try
    {
        if ( (_wcfilenames) && (_nbfiles > 0) )
        {
            int i = 0;
            char **filesname = (char**)MALLOC(sizeof(wchar_t*) * _nbfiles);
            if (filesname)
            {
                for (i = 0; i < _nbfiles; i++)
                {
                    filesname[i] = wide_string_to_UTF8(_wcfilenames[i]);
                }
                callXcos(filesname, _nbfiles);
                freeArrayOfString(filesname, _nbfiles);
            }
        }
    }
    catch (GiwsException::JniCallMethodException exception)
    {
        Scierror(999, "%s: %s\n", "xcos", exception.getJavaDescription().c_str());
        return 0;
    }
    catch (GiwsException::JniException exception)
    {
        Scierror(999, "%s: %s\n", "xcos", exception.what());
        return 0;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
