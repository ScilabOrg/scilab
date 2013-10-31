/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - DIGITEO - Cedric DELAMARRE
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __OPTIMIZATIONS_HXX__
#define __OPTIMIZATIONS_HXX__

#include <map>
#include "dynlib_optimization.h"
#include "string.hxx"
#include "callable.hxx"

extern "C"
{
#include "Thread_Wrapper.h"
}

extern "C"
{
    // optim
    void C2F(genros)(int *ind, int *n, double *x, double *f, double *g, int *ti, float *tr, double *td);
    void C2F(topt2)(int *ind, int *n, double *x, double *f, double *g, int *ti, float *tr, double *td);
    void C2F(icsemc)(int *ind, int *n, double *x, double *f, double *g, int *ti, float *tr, double *td);
    void C2F(mcsec)(int *ind, int *n, double *x, double *f, double *g, int *ti, float *tr, double *td);

}

class OPTIMIZATION_IMPEXP OptimizationFunctions
{

public :

    OptimizationFunctions(std::wstring callerName);
    ~OptimizationFunctions();

    // optim
    void setXRows(int);
    void setXCols(int);
    int getXRows();
    int getXCols();
    void setOptimCostfFunction(types::Callable*);
    bool setOptimCostfFunction(types::String*);
    void setCostfArgs(types::InternalType*);

    void execCostf(int *ind, int *n, double *x, double *f, double *g, int *ti, float *tr, double *td);

private :

    std::map<std::wstring, void*> m_staticFunctionMap;
    std::wstring m_wstrCaller;

    // optim
    int m_iXRows;
    int m_iXCols;
    types::Callable* m_pCallOptimCostfFunction;
    types::String* m_pStringOptimCostfFunctionDyn;
    types::String* m_pStringOptimCostfFunctionStatic;

    std::vector<types::InternalType*> m_OptimArgs;

    void callCostfMacro(int *ind, int *n, double *x, double *f, double *g, int *ti, float *tr, double *td);

};

class OPTIMIZATION_IMPEXP Optimization
{
    // differential equation functions
private :
    static std::map<__threadId, OptimizationFunctions*> m_mapOptimizationFunctions;

public :
    static void addOptimizationFunctions(OptimizationFunctions* _opFunction);
    static void removeOptimizationFunctions();
    static OptimizationFunctions* getOptimizationFunctions();

};
#endif /* !__OPTIMIZATIONS_HXX__ */

