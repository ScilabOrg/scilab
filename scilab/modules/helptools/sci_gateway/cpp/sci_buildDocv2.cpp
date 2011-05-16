/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "SciDocMain.hxx"
#include "GiwsException.hxx"

extern "C"
{
    /*--------------------------------------------------------------------------*/
#include <string.h>
#include "api_scilab.h"
#include "stack-c.h"
#include "api_oldstack.h"
#include "charEncoding.h"
#include "gw_helptools.h"
#include "Scierror.h"
#include "sci_path.h"
#include "localization.h"
#include "setgetlanguage.h"
#include "getScilabJavaVM.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "ConvertSlash.h"
#endif
    /*--------------------------------------------------------------------------*/
#define PATHTOCSS "/modules/helptools/css/javahelp.css"
#define PATHTOBUILDDOC "/modules/helptools/build/doc/scilab_%s_help/"
#define PATHTOMASTERXML "/modules/helptools/master_%s_help.xml"
#define DEFAULTEXPORT "JH"
#ifdef _MSC_VER
    static void __slashToAntislash(std::string *in)
    {
        size_t found = in->rfind("/");

        while (found != std::string::npos)
        {
            in->replace (found, 1, "\\");
            found = in->find("/");
        }
    }
#endif
    /*--------------------------------------------------------------------------*/
    int sci_buildDocv2(char *fname, int *_piKey)
    {
        static int l1 = 0,n1 = 0,m1 = 0;
        static int l2 = 0,n2 = 0,m2 = 0;
        static int l3 = 0,n3 = 0,m3 = 0;
        static int l4 = 0,n4 = 0,m4 = 0;
        //     static int Row_Output = 0, Col_Output = 0;
        std::string exportFormat;
        std::string SciPath = getSCI(); /* Scilab path */
        std::string masterXML; /* Which file contains all the doc stuff */
        std::string masterXMLTMP;
        std::string outputDirectory; /* Working directory */
        std::string outputDirectoryTMP;
        std::string language;
        std::string styleSheet; /* the CSS */
        //     std::string pathToGenerated;
        org_scilab_modules_helptools::SciDocMain *doc = NULL;
        SciErr sciErr;
        int* piAddr = NULL;
        int iRet    = 0;

        CheckRhs(0,4);
        CheckLhs(1,1);

        styleSheet = SciPath+PATHTOCSS;

        if (Rhs < 1)
        {
            exportFormat = DEFAULTEXPORT;
        }
        else
        {
            char* pstData = NULL;
            sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            if(!isStringType(_piKey, piAddr))
            {
                Scierror(999,_("%s: Wrong type for input argument #%d: Single string expected.\n"),fname,1);
                return 0;
                // Wrong type string
            }

            iRet = getAllocatedSingleString(_piKey, piAddr, &pstData);
            if(iRet)
            {
                freeAllocatedSingleString(pstData);
                return iRet;
            }
            exportFormat = std::string(pstData);
            freeAllocatedSingleString(pstData);


        }

        if ( Rhs < 3) /* Language not provided */
        {
            language = wide_string_to_UTF8(getlanguage());
        }
        else
        {
            char* pstData = NULL;
            sciErr = getVarAddressFromPosition(_piKey, 3, &piAddr);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            if(!isStringType(_piKey, piAddr))
            {
                Scierror(999,_("%s: Wrong type for input argument #%d: Single string expected.\n"),fname,3);
                return 0;
                // Wrong type string
            }

            if(!isScalar(_piKey, piAddr))
            {
                language = wide_string_to_UTF8(getlanguage());
            }
            else
            {
                iRet = getAllocatedSingleString(_piKey, piAddr, &pstData);
                if(iRet)
                {
                    freeAllocatedSingleString(pstData);
                    return iRet;
                }
                language = std::string(pstData);
                freeAllocatedSingleString(pstData);

            }

        }

        if (Rhs < 2)
        {
            /* Update the path with the localization */
            masterXMLTMP = std::string("/modules/helptools/master_")+language+std::string("_help.xml");
            masterXML = SciPath + masterXMLTMP;
        }
        else
        {
            char* pstData = NULL;
            sciErr = getVarAddressFromPosition(_piKey, 2, &piAddr);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            if(!isStringType(_piKey, piAddr))
            {
                Scierror(999,_("%s: Wrong type for input argument #%d: Single string expected.\n"),fname,2);
                return 0;
                // Wrong type string
            }


            iRet = getAllocatedSingleString(_piKey, piAddr, &pstData);
            if(iRet)
            {
                freeAllocatedSingleString(pstData);
                return iRet;
            }
            masterXML = std::string(pstData);
            freeAllocatedSingleString(pstData);
        }

        if (Rhs == 4)
        {
            char* pstData = NULL;
            sciErr = getVarAddressFromPosition(_piKey, 4, &piAddr);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            if(!isStringType(_piKey, piAddr))
            {
                Scierror(999,_("%s: Wrong type for input argument #%d: Single string expected.\n"),fname,4);
                return 0;
                // Wrong type string
            }

            iRet = getAllocatedSingleString(_piKey, piAddr, &pstData);
            if(iRet)
            {
                freeAllocatedSingleString(pstData);
                return iRet;
            }
            outputDirectory = std::string(pstData)+std::string("/scilab_")+language+std::string("_help/");
            freeAllocatedSingleString(pstData);

        }
        else /* Scilab help */
        {
            /* Update the path with the localization */
            outputDirectoryTMP = std::string("/modules/helptools/")+std::string(exportFormat)+std::string("/scilab_")+language+std::string("_help/");

            outputDirectory = SciPath+outputDirectoryTMP;

        }

        try
        {
            doc = new org_scilab_modules_helptools::SciDocMain(getScilabJavaVM());

#ifdef _MSC_VER
            __slashToAntislash(&outputDirectory);
            __slashToAntislash(&styleSheet);
            __slashToAntislash(&masterXML);
#endif

            //            doc->buildDocumentation((char*) exportFormat.c_str());

            if (doc->setOutputDirectory((char *) outputDirectory.c_str()))
            {
                doc->setWorkingLanguage((char *) language.c_str());
                doc->setExportFormat((char *) exportFormat.c_str());
                doc->setIsToolbox(Rhs == 4);
                doc->process((char *) masterXML.c_str(), (char *) styleSheet.c_str());
            }
            else
            {
                Scierror(999,_("%s: Could find or create the working directory %s.\n"), fname, outputDirectory.c_str());
                return FALSE;
            }
        }
        catch(GiwsException::JniException ex)
        {
            ScierrorW(999,_W("%s: Error while building documentation: %s.\n"), fname, ex.getJavaDescription().c_str());
            ScierrorW(999,_W("%s: Execution Java stack: %s.\n"), fname, ex.getJavaStackTrace().c_str());
            ScierrorW(999,_W("If Scilab is started in a chroot, you might want to try to set the two environment variables: SCI_DISABLE_TK=1 SCI_JAVA_ENABLE_HEADLESS=1\n"));
            return FALSE;
        }

        if (doc != NULL)
        {
            delete doc;
        }
        LhsVar(1) = 0 ;
        PutLhsVar();
        return 0;
    }
    /*--------------------------------------------------------------------------*/
}
