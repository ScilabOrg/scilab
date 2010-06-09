/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "sci_home.h"

#include "configvariable.hxx"

#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "charEncoding.h"
#include "string.hxx"
#include "context.hxx"
#include "PATH_MAX.h"
#include "setenvvar.hxx"
#include "machine.h"

char *getSCIHOME(void)
{
    return strdup(ConfigVariable::getSCIHOME().c_str());
}

/*--------------------------------------------------------------------------*/ 
wchar_t *getSCIHOMEW(void)
{
    return to_wide_string(const_cast<char*>(ConfigVariable::getSCIHOME().c_str()));
}

/*--------------------------------------------------------------------------*/ 
void setSCIHOME(const char* _sci_home)
{
    //add SCI value in context as variable
    types::String *pS = new types::String(_sci_home);
    symbol::Context::getInstance()->put("SCIHOME", *pS);

    std::string sci_home(_sci_home);
    ConfigVariable::setSCIHOME(sci_home);
}

/*--------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
char* computeSCIHOME(void)
{
#define BASEDIR "Scilab"
    int ierr = 0;
    int buflen = PATH_MAX;
    int iflag = 0;

    char USERPATHSCILAB[PATH_MAX];
    char SCIHOMEPATH[PATH_MAX * 2];

    char USERHOMESYSTEM[PATH_MAX];
    char *SHORTUSERHOMESYSTEM = NULL;

    bool bConverted = false;

    getenvc(&ierr, "APPDATA", USERHOMESYSTEM, &buflen, &iflag);

    /* if APPDATA not found we try with USERPROFILE */
    if(ierr)
    {
        getenvc(&ierr, "USERPROFILE", USERHOMESYSTEM, &buflen, &iflag);
    }

    /* convert long path to short path format : remove some special characters */
    SHORTUSERHOMESYSTEM = getshortpathname(USERHOMESYSTEM, &bConverted);
    if(SHORTUSERHOMESYSTEM)
    {
        if(!isdir(SHORTUSERHOMESYSTEM)) 
        {
            /* last chance, we try to get default all users profile */
            getenvc(&ierr, "ALLUSERSPROFILE", USERHOMESYSTEM, &buflen, &iflag);
            if(ierr) 
            {
                delete []SHORTUSERHOMESYSTEM; 
                return NULL;
            }

            /* convert long path to short path format : remove some special characters */
            SHORTUSERHOMESYSTEM = getshortpathname(USERHOMESYSTEM, &bConverted);

            if((!SHORTUSERHOMESYSTEM) || !isdir(SHORTUSERHOMESYSTEM))
            {
                if(SHORTUSERHOMESYSTEM)
                { 
                    delete []SHORTUSERHOMESYSTEM; 
                }
                return NULL;
            }
        }
    }
    else
    {
        if(SHORTUSERHOMESYSTEM) 
        { 
            delete []SHORTUSERHOMESYSTEM; 
        }
        return NULL;
    }

    /* checks that directory exists */
    strcpy(USERHOMESYSTEM, SHORTUSERHOMESYSTEM);
    if(SHORTUSERHOMESYSTEM)
    { 
        delete []SHORTUSERHOMESYSTEM; 
    }

    /* Set SCIHOME environment variable */
    sprintf(USERPATHSCILAB, "%s%s%s", USERHOMESYSTEM, DIR_SEPARATOR, BASEDIR);
    sprintf(SCIHOMEPATH, "%s%s%s", USERPATHSCILAB, DIR_SEPARATOR, SCI_VERSION_STRING);

    /* creates directory if it does not exists */
    if(!isdir(SCIHOMEPATH))
    {
        if(!isdir(USERPATHSCILAB))
        {
            createdirectory(USERPATHSCILAB);
        }

        if(createdirectory(SCIHOMEPATH))
        {
            return strdup(SCIHOMEPATH);
        }
    }
    else
    {
        return strdup(SCIHOMEPATH);
    }

    return NULL;
}
#else
char* computeSCIHOME(void)
{
#define BASEDIR ".Scilab"
    int ierr   = 0;
    int buflen = PATH_MAX;
    int iflag  = 0;
    char USERPATHSCILAB[PATH_MAX];
    char USERHOMESYSTEM[PATH_MAX];
    char SCIHOMEPATH[PATH_MAX * 2];
    char HOME[] = "HOME";

    getenvc(&ierr, HOME, USERHOMESYSTEM, &buflen, &iflag);
    if(ierr)
    {
        return NULL; 
    }

    /* Set SCIHOME environment variable */
    sprintf(USERPATHSCILAB, "%s%s%s", USERHOMESYSTEM, DIR_SEPARATOR, BASEDIR);
    sprintf(SCIHOMEPATH, "%s%s%s", USERPATHSCILAB, DIR_SEPARATOR, SCI_VERSION_STRING);

    /* creates directory if it does not exists */
    if(!isdir(SCIHOMEPATH))
    {
        if(!isdir(USERPATHSCILAB))
        {
            createdirectory(USERPATHSCILAB);
        }

        if(createdirectory(SCIHOMEPATH))
        {
            return strdup(SCIHOMEPATH);
        }
    }
    else 
    {
        return strdup(SCIHOMEPATH);
    }

    return NULL;
}
#endif

/*--------------------------------------------------------------------------*/ 
char* getenvSCIHOME(void)
{
    return NULL;
}

/*--------------------------------------------------------------------------*/ 
void putenvSCIHOME(const char* _sci_path)
{
}

/*--------------------------------------------------------------------------*/ 
void defineSCIHOME()
{
    char* sci_home = computeSCIHOME();
    setSCIHOME(sci_home);
    putenvSCIHOME(sci_home);
    FREE(sci_home);
}
