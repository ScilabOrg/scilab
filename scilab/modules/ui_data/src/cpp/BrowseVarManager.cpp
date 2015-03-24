/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <set>

#include "BrowseVar.hxx"
#ifdef _MSC_VER
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
#endif
#include "context.hxx"

extern "C"
{
#include "BrowseVarManager.h"
#include "localization.h"
#include "sci_malloc.h"
#include "BOOL.h"
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "os_string.h"
#include "api_scilab.h"
}

#define N_A "N/A"

using namespace org_scilab_modules_ui_data;
using std::string;

static std::set<string> createScilabDefaultVariablesSet();
static char * getListName(char * variableName);
static std::string formatMatrix(int nbRows, int nbCols, double *pdblReal, double *pdblImg);
static char * valueToDisplay(types::InternalType* pIT, int variableType, int nbRows, int nbCols);
void OpenBrowseVar()
{
    BrowseVar::openVariableBrowser(getScilabJavaVM());
    SetBrowseVarData();
}

void UpdateBrowseVar()
{
    if (BrowseVar::isVariableBrowserOpened(getScilabJavaVM()))
    {
        SetBrowseVarData();
    }
}

void SetBrowseVarData()
{
    SciErr err;
    int iGlobalVariablesUsed = 0;
    int iGlobalVariablesTotal = 0;
    int iLocalVariablesUsed = 0;
    int iLocalVariablesTotal = 0;
    int i = 0;

    //// First get how many global / local variable we have.
    //C2F(getvariablesinfo) (&iLocalVariablesTotal, &iLocalVariablesUsed);
    //C2F(getgvariablesinfo) (&iGlobalVariablesTotal, &iGlobalVariablesUsed);

    symbol::Context* ctx = symbol::Context::getInstance();
    std::list<std::wstring>* lstVars = ctx->getVarsName(ctx->getScopeLevel(), false);
    std::list<std::wstring>* lstGlobals = ctx->getGlobalNameForWho(false);
    iLocalVariablesUsed = static_cast<int>(lstVars->size());
    iGlobalVariablesUsed = static_cast<int>(lstGlobals->size());

    char **pstAllVariableNames = (char **)MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(char *));
    char **pstAllVariableVisibility = (char **)MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(char *));
    char **pstAllVariableListTypes = (char **)MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(char *));
    int *piAllVariableBytes = (int *)MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(int));
    char **pstAllVariableSizes = (char **)MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(char *));
    int *piAllVariableTypes = (int *)MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(int));
    int *piAllVariableIntegerTypes = (int *)MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(int));
    bool *piAllVariableFromUser = (bool *) MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(bool));
    /* Necessary for the plots in the var browser */
    int *piAllVariableNbRows = (int *)MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(int));
    int *piAllVariableNbCols = (int *)MALLOC((iLocalVariablesUsed + iGlobalVariablesUsed) * sizeof(int));

    int nbRows, nbCols;
    char *sizeStr = NULL;

    std::set<string> scilabDefaultVariablesSet = createScilabDefaultVariablesSet();


    // for each local variable get information
    for (auto var : *lstVars)
    {
        //get var from context
        types::InternalType* pIT = ctx->get(symbol::Symbol(var));
        // name
        pstAllVariableNames[i] = wide_string_to_UTF8(var.data());

        // global / local ??
        pstAllVariableVisibility[i] = os_strdup("local");

        // type
        err = getVarType(NULL, (int*)pIT, &piAllVariableTypes[i]);
        if (!err.iErr)
        {
            piAllVariableBytes[i] = 0;
            err = getVarDimension(NULL, (int*)pIT, &nbRows, &nbCols);
        }

        if (err.iErr || nbRows * nbCols == 0)
        {
            pstAllVariableSizes[i] = os_strdup(N_A);
            //pstAllVariableSizes[i] = (char *)MALLOC((sizeof(N_A) + 1) * sizeof(char));
            //strcpy(pstAllVariableSizes[i], N_A);
        }
        else
        {
            pstAllVariableSizes[i] = valueToDisplay(pIT, piAllVariableTypes[i], nbRows, nbCols);
            piAllVariableNbRows[i] = nbRows;
            piAllVariableNbCols[i] = nbCols;
        }

        if (piAllVariableTypes[i] == sci_ints)
        {
            // Integer case
            int iPrec       = 0;
            err = getMatrixOfIntegerPrecision(NULL, (int*)pIT, &iPrec);
            switch (iPrec)
            {
                case SCI_INT8:
                    piAllVariableIntegerTypes[i] = 8;
                    break;
                case SCI_INT16:
                    piAllVariableIntegerTypes[i] = 16;
                    break;
                case SCI_INT32:
                    piAllVariableIntegerTypes[i] = 32;
                    break;
#ifdef __SCILAB_INT64__
                case SCI_INT64:
                    piAllVariableIntegerTypes[i] = 64;
                    break;
#endif
                default:
                    piAllVariableIntegerTypes[i] = 0; // Should never occurs
                    break;
            }
        }
        else
        {
            piAllVariableIntegerTypes[i] = -1;
        }

        if (pIT->isTList() || pIT->isMList())
        {
            pstAllVariableListTypes[i] = getListName(pstAllVariableNames[i]);
        }
        else if (pIT->isStruct())
        {
            pstAllVariableListTypes[i] = os_strdup("st");
        }
        else
        {
            pstAllVariableListTypes[i] = os_strdup("");
        }

        if (scilabDefaultVariablesSet.find(string(pstAllVariableNames[i])) == scilabDefaultVariablesSet.end() && piAllVariableTypes[i] != sci_lib)
        {
            piAllVariableFromUser[i] = TRUE;
        }
        else
        {
            piAllVariableFromUser[i] = FALSE;
        }

        ++i;
    }

    // for each global variable get information
    for (auto var : *lstGlobals)
    {
        types::InternalType* pIT = ctx->getGlobalValue(symbol::Symbol(var));

        // name
        pstAllVariableNames[i] = wide_string_to_UTF8(var.data());

        // global / local ??
        pstAllVariableVisibility[i] = os_strdup("global");

        // Bytes used - 8 is the number of bytes in a word
        piAllVariableBytes[i] = 0;

        // type
        // Calling "API Scilab": not yet implemented for global variable
        //getNamedVarType(pvApiCtx, pstAllVariableNames[i], &piAllVariableTypes[i]);
        // Using old stack operations...

        // type
        err = getVarType(NULL, (int*)pIT, &piAllVariableTypes[i]);
        if (!err.iErr)
        {
            piAllVariableBytes[i] = 0;
            err = getVarDimension(NULL, (int*)pIT, &nbRows, &nbCols);
        }

        if (err.iErr || nbRows * nbCols == 0)
        {
            pstAllVariableSizes[i] = os_strdup(N_A);
            //pstAllVariableSizes[i] = (char *)MALLOC((sizeof(N_A) + 1) * sizeof(char));
            //strcpy(pstAllVariableSizes[i], N_A);
        }
        else
        {
            pstAllVariableSizes[i] = valueToDisplay(pIT, piAllVariableTypes[i], nbRows, nbCols);
            piAllVariableNbRows[i] = nbRows;
            piAllVariableNbCols[i] = nbCols;
        }

        if (piAllVariableTypes[i] == sci_tlist || piAllVariableTypes[i] == sci_mlist)
        {
            pstAllVariableListTypes[i] = getListName(pstAllVariableNames[i]);
        }
        else
        {
            pstAllVariableListTypes[i] = os_strdup("");
        }


        if (scilabDefaultVariablesSet.find(string(pstAllVariableNames[i])) == scilabDefaultVariablesSet.end()
                && piAllVariableTypes[i] != sci_c_function && piAllVariableTypes[i] != sci_lib)
        {
            piAllVariableFromUser[i] = TRUE;
        }
        else
        {
            piAllVariableFromUser[i] = FALSE;
        }

        ++i;
    }

    // Launch Java Variable Browser through JNI
    BrowseVar::setVariableBrowserData(getScilabJavaVM(),
                                      pstAllVariableNames, iLocalVariablesUsed + iGlobalVariablesUsed,
                                      piAllVariableBytes, iLocalVariablesUsed + iGlobalVariablesUsed,
                                      piAllVariableTypes, iLocalVariablesUsed + iGlobalVariablesUsed,
                                      piAllVariableIntegerTypes, iLocalVariablesUsed + iGlobalVariablesUsed,
                                      pstAllVariableListTypes, iLocalVariablesUsed + iGlobalVariablesUsed,
                                      pstAllVariableSizes, iLocalVariablesUsed + iGlobalVariablesUsed,
                                      piAllVariableNbRows, iLocalVariablesUsed + iGlobalVariablesUsed,
                                      piAllVariableNbCols, iLocalVariablesUsed + iGlobalVariablesUsed,
                                      pstAllVariableVisibility, iLocalVariablesUsed + iGlobalVariablesUsed,
                                      piAllVariableFromUser, iLocalVariablesUsed + iGlobalVariablesUsed);

    freeArrayOfString(pstAllVariableNames, iLocalVariablesUsed + iGlobalVariablesUsed);
    freeArrayOfString(pstAllVariableVisibility, iLocalVariablesUsed + iGlobalVariablesUsed);
    freeArrayOfString(pstAllVariableSizes, iLocalVariablesUsed + iGlobalVariablesUsed);
    freeArrayOfString(pstAllVariableListTypes, iLocalVariablesUsed + iGlobalVariablesUsed);

    FREE(piAllVariableFromUser);
    FREE(piAllVariableBytes);
    FREE(piAllVariableTypes);
    FREE(piAllVariableIntegerTypes);
    FREE(piAllVariableNbRows);
    FREE(piAllVariableNbCols);

    delete lstVars;
    delete lstGlobals;
}

/*--------------------------------------------------------------------------*/
static std::set<string> createScilabDefaultVariablesSet()
{
    string arr[] = { "home",
                     "PWD",
                     "%tk",
                     "%pvm",
                     "MSDOS",
                     "%F",
                     "%T",
                     "%f",
                     "%t",
                     "%e",
                     "%pi",
                     "%modalWarning",
                     "%nan",
                     "%inf",
                     "SCI",
                     "WSCI",
                     "SCIHOME",
                     "TMPDIR",
                     "%gui",
                     "%fftw",
                     "%helps",
                     "%eps",
                     "%io",
                     "%i",
                     "demolist",
                     "%z",
                     "%s",
                     "$",
                     "%toolboxes",
                     "%toolboxes_dir",
                     "TICTOC",
                     "%helps_modules",
                     "%_atoms_cache",
                     "evoid", // Constant for external object
                     "jvoid", // Constant for external object Java (jims)
                     "jnull", // Constant for external object Java (jims)
                     "enull"  // Constant for external object
                   };
    int i = 0;

#define NBELEMENT 37
    std::set<string> ScilabDefaultVariables;

    for (i = 0; i < NBELEMENT; i++)
    {
        ScilabDefaultVariables.insert(arr[i]);
    }

    return ScilabDefaultVariables;
}

static char * getListName(char * variableName)
{
    SciErr sciErr;
    int *piAddr = NULL;
    int* piAddr1 = NULL;
    int iRows = 0;
    int iCols = 0;
    char **pstType;
    char *tmpChar;
    sciErr = getVarAddressFromName(NULL, variableName, &piAddr);
    if (sciErr.iErr)
    {
        return os_strdup("");
    }

    sciErr = getListItemAddress(NULL, piAddr, 1, &piAddr1);
    if (sciErr.iErr)
    {
        return os_strdup("");
    }

    if (getAllocatedMatrixOfString(NULL, piAddr1, &iRows, &iCols, &pstType))
    {

        return os_strdup("");
    }
    tmpChar = os_strdup(pstType[0]);
    freeAllocatedMatrixOfString(iRows, iCols, pstType);
    return tmpChar;
}

static char * valueToDisplay(types::InternalType* pIT, int variableType, int nbRows, int nbCols)
{
    // 4 is the dimension max to which display the content
    if (nbRows * nbCols <= 4 && variableType == sci_matrix)
    {
        types::Double* pD = pIT->getAs<types::Double>();
        // Small double value, display it
        double* pdblReal = nullptr;
        double* pdblImg = nullptr;
        pdblReal = pD->get();

        if (pD->isComplex())
        {
            pdblImg = pD->getImg();
        }


        return os_strdup(formatMatrix(nbRows, nbCols, pdblReal, pdblImg).c_str());
    }
    else
    {
        char *sizeStr = NULL;
        // 11 =strlen("2147483647")+1 (1 for security)
        sizeStr = (char *)MALLOC((11 + 11 + 1 + 1) * sizeof(char));
        sprintf(sizeStr, "%dx%d", nbRows, nbCols);
        return sizeStr;
    }
}

std::string formatMatrix(int nbRows, int nbCols, double *pdblReal, double *pdblImg)
{
    int i, j ;
#define PRECISION_DISPLAY 3
    if (nbRows * nbCols == 1)
    {
        std::ostringstream os;
        os.precision(PRECISION_DISPLAY);
        os << pdblReal[0]; // Convert the double to string
        if (pdblImg)
        {
            os << " + " << pdblImg[0] << "i";
        }
        return os.str();
    }

    std::string formated = "[";
    for (j = 0 ; j < nbRows ; j++)
    {
        for (i = 0 ; i < nbCols ; i++)
        {
            /* Display the formated matrix ... the way the user
             * expect */
            std::ostringstream os;
            os.precision(PRECISION_DISPLAY);
            os << pdblReal[i * nbRows + j]; // Convert the double to string
            formated += os.str();
            if (pdblImg)
            {
                std::ostringstream osComplex;
                osComplex.precision(PRECISION_DISPLAY);
                osComplex << pdblImg[i * nbRows + j];
                formated += " + " + osComplex.str() + "i";
            }


            if (i + 1 != nbCols) // Not the last element of the matrix
            {
                formated += ", ";
            }
        }
        if (j + 1 != nbRows) // Not the last line of the matrix
        {
            formated += "; ";
        }
    }
    return formated + "]";
}
