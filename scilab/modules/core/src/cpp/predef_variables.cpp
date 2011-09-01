/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <list>
#include <string>
#include <map>
#include <algorithm>
#include "predef_variables.h"
extern "C"
{
#include <stdio.h>
#include <string.h>
#include "api_scilab.h"
#include "stack-c.h"
#include "stack-def.h"
#include "stackinfo.h"
#include "MALLOC.h"
#include "core_math.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
};
/*--------------------------------------------------------------------------*/
using namespace std;
/*--------------------------------------------------------------------------*/
static list<string> listPredefinedVariableNames;
/*--------------------------------------------------------------------------*/
static void updateListPredefined(void);
static void removeAllInListPredefined(void);
/*--------------------------------------------------------------------------*/
int getNumberPredefVariablesProtected(void)
{
    return (C2F(vstk).isiz - C2F(vstk).bbot);
}
/*--------------------------------------------------------------------------*/
void setNumberPredefVariablesProtected(int nb)
{
    int new_value = Max( C2F(vstk).bot , Min( (C2F(vstk).isiz - nb), C2F(vstk).bot0 ) );
    if ( C2F(vstk).bbot != new_value ) C2F(vstk).bbot = new_value;
    updateListPredefined();
}
/*--------------------------------------------------------------------------*/
void clearPredef(void)
{
    if ( C2F(vstk).bbot != C2F(vstk).bot0 ) C2F(vstk).bbot = C2F(vstk).bot0;
    updateListPredefined();
}
/*--------------------------------------------------------------------------*/
void predefAll(void)
{
    if (C2F(vstk).bbot != C2F(vstk).bot)
    {
        /* bug 3773 */
        /* "ans" must not be protected */
        char *FirstNameVariableOnStack = getLocalNamefromId(1);

        if (FirstNameVariableOnStack)
        {
            if (strcmp(FirstNameVariableOnStack, "ans") == 0)
            {
                /* protect variable after "ans" */
                C2F(vstk).bbot = C2F(vstk).bot + 1;
            }
            else
            {
                C2F(vstk).bbot = C2F(vstk).bot;
            }
            FREE(FirstNameVariableOnStack);
        }
        else
        {
            C2F(vstk).bbot = C2F(vstk).bot;
        }
        updateListPredefined();
    }
}
/*--------------------------------------------------------------------------*/
BOOL isPredefinedVariable(void* _pvCtx, const char *variablename)
{
    int *piAddress = NULL;
    SciErr sciErr = getVarAddressFromName(_pvCtx, variablename, &piAddress);
    if (!sciErr.iErr)
    {
        int il = iadr(*Lstk(C2F(vstk).bbot));
        int *piAddressFirstPredef = istk(il);
        return (BOOL)(piAddress >= piAddressFirstPredef);
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
char **getPredefinedVariablesName(int *iReturnedSize)
{
    char **returnedVariablesName = NULL;
    *iReturnedSize = 0;

    if (listPredefinedVariableNames.size() > 0)
    {
        returnedVariablesName = (char**)MALLOC(sizeof(char *) * listPredefinedVariableNames.size());
        if (returnedVariablesName)
        {
            *iReturnedSize = listPredefinedVariableNames.size();
            int i = 0;
            for (list<string>::iterator pIterator = listPredefinedVariableNames.begin();
                pIterator != listPredefinedVariableNames.end(); ++ pIterator)
            {
                returnedVariablesName[i] = strdup(pIterator->c_str());
                i++;
            }
        }
    }
    return returnedVariablesName;
}
/*--------------------------------------------------------------------------*/
static void updateListPredefined(void)
{
    char **variablesName = NULL;
    int nbPredefined = 0;

     nbPredefined = getNumberPredefVariablesProtected() - 1; // remove ans
    
    if (nbPredefined > 0)
    {
        int i = 0;
        int Ltotal = 0;
        int Lused = 0;
        
        C2F(getvariablesinfo)(&Ltotal, &Lused);

        removeAllInListPredefined();

        for(i = 0; i < nbPredefined; i++)
        {
            char *variableName = getLocalNamefromId(Lused - i - 1);
            if (variableName)
            {
                if (!isPredefinedVariable(pvApiCtx, variableName))
                {
                    listPredefinedVariableNames.push_back(variableName);
                }
                FREE(variableName);
                variableName = NULL;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
static void removeAllInListPredefined(void)
{
    if (listPredefinedVariableNames.size() > 0)
    {
        listPredefinedVariableNames.erase(listPredefinedVariableNames.begin(), 
                                          listPredefinedVariableNames.end());
    }
}
/*--------------------------------------------------------------------------*/
