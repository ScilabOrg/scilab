/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

#include "function.hxx"
#include "gatewaystruct.hxx"
#include "alltypes.hxx"

extern "C"
{
#include <string.h>
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "call_scilab.h"
#include "sciprint.h"
#include "sci_malloc.h"
}


using namespace types;

static int findOptional(void* _pvCtx, char *fname, rhs_opts opts[]);
static void printOptionalNames(void* _pvCtx, rhs_opts opts[]);

/**************************/
/*   optional functions   */
/**************************/
int getOptionals(void* _pvCtx, char* pstFuncName, rhs_opts opts[])
{
    GatewayStruct* pStr = (GatewayStruct*)_pvCtx;
    types::optional_list opt = *pStr->m_pOpt;
    for (int i = 0 ; i < opt.size() ; i++)
    {
        int typeOfOpt = -1;
        char* pstOpts = wide_string_to_UTF8(opt[i].first.c_str());
        int index = findOptional(_pvCtx, pstOpts, opts);
        FREE(pstOpts);

        if (index < 0)
        {
            sciprint(_("%ls: Unrecognized optional arguments %ls.\n"), pStr->m_pstName, opt[i].first.c_str());
            printOptionalNames(_pvCtx, opts);
            return 0;
        }

        opts[index].iPos = i + 1;
        GenericType* pGT = (GenericType*)opt[i].second;
        typeOfOpt = pGT->getType();
        opts[index].iType = typeOfOpt;

        if (typeOfOpt == GenericType::ScilabImplicitList)
        {
            InternalType* pIT = NULL;
            ImplicitList* pIL = pGT->getAs<ImplicitList>();
            pIT = pIL->extractFullMatrix();
            Double* impResult = (Double*)pIT;
            opts[index].iRows = impResult->getRows();
            opts[index].iCols = impResult->getCols();
            opts[index].piAddr = (int*)impResult;
        }
        else
        {
            opts[index].iRows = pGT->getRows();
            opts[index].iCols = pGT->getCols();
            opts[index].piAddr = (int*)pGT;
        }
    }
    //   int index = -1;
    //GatewayStruct* pStr = (GatewayStruct*)_pvCtx;

    //   wchar_t* pwstProperty = to_wide_string(pstProperty);

    //   for(int i = 0 ; i < pStr->m_pOpt->size() ; i++)
    //   {
    //       std::pair<std::wstring, InternalType*> current = (*pStr->m_pOpt)[i];
    //       if(wcscmp(current.first.c_str(), pwstProperty) == 0)
    //       {
    //           index = i;
    //           break;
    //       }
    //   }

    //   FREE(pwstProperty);

    return 1;
}

int FirstOpt(void* _pvCtx)
{
    GatewayStruct* pStr = (GatewayStruct*)_pvCtx;
    return (int)pStr->m_pIn->size() + 1;
}

int NumOpt(void* _pvCtx)
{
    GatewayStruct* pStr = (GatewayStruct*)_pvCtx;
    return (int)pStr->m_pOpt->size();
}

int FindOpt(void* _pvCtx, char* pstProperty, rhs_opts opts[])
{
    int i = findOptional(_pvCtx, pstProperty, opts);
    if (i > 0 && opts[i].iPos > 0)
    {
        return i;
    }

    return 0;
}

static int findOptional(void* _pvCtx, char *pstProperty, rhs_opts opts[])
{
    int rep = -1, i = 0;

    while (opts[i].pstName != NULL)
    {
        int cmp;

        /* name is terminated by white space and we want to ignore them */
        if ((cmp = strcmp(pstProperty, opts[i].pstName)) == 0)
        {
            rep = i;
            break;
        }
        else if (cmp < 0)
        {
            break;
        }
        else
        {
            i++;
        }
    }
    return rep;
}

void printOptionalNames(void* _pvCtx, rhs_opts opts[])
/* array of optinal names (in alphabetical order)
* the array is null terminated */
{
    int i = 0;

    if (opts[i].pstName == NULL)
    {
        sciprint(_("Optional argument list is empty.\n"));
        return;
    }

    sciprint(_("Optional arguments list: \n"));
    while (opts[i + 1].pstName != NULL)
    {
        sciprint("%s, ", opts[i].pstName);
        i++;
    }
    sciprint(_("and %s.\n"), opts[i].pstName);
}
