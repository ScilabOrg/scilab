/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "funcmanager.hxx"

extern "C"
{
#include "functions_manager.h"
}

void LoadModules(int _iNoStart)
{
    FuncManager* pFM = FuncManager::getInstance();
    if (pFM)
    {
        pFM->LoadModules(_iNoStart != 0);
    }
}

void UnloadModules(int _iNoStart)
{
    FuncManager* pFM = FuncManager::getInstance();
    if (pFM)
    {
        pFM->UnloadModules(_iNoStart != 0);
    }
}

void destroyfunctionManagerInstance(void)
{
    FuncManager::destroyInstance();
}