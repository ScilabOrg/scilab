/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef ___SLINT_CHECKER_HXX__
#define ___SLINT_CHECKER_HXX__

#include <string>
#include <unordered_set>
#include <vector>

#include "allexp.hxx"
#include "allvar.hxx"
#include "alldec.hxx"

#include "SLintContext.hxx"
#include "output/SLintResult.hxx"

extern "C"
{
#include "localization.h"
}

namespace slint
{

class SLintChecker
{

    void * data;

protected:

    static std::unordered_set<std::wstring> constants;

public:

    SLintChecker() : data(nullptr) { }

    virtual ~SLintChecker() { }

    virtual void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result) = 0;
    virtual void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result) = 0;
    virtual const std::string getName() const = 0;
    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const = 0;

    virtual void setData(void * _data);
    virtual void * getData() const;

    static bool isScilabConstant(const std::wstring & name);

private:

    static std::unordered_set<std::wstring> init();
};

} // namespace slint

#endif // __SLINT_CHECKER_HXX__
