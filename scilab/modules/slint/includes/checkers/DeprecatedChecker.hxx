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

#ifndef __DEPRECATED_CHECKER_HXX__
#define __DEPRECATED_CHECKER_HXX__

#include <memory>
#include <unordered_map>

#include "SLintChecker.hxx"

namespace slint
{

class DeprecatedChecker : public SLintChecker
{

    class __Svd : public SLintChecker
    {
        void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
        void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result) { }
        const std::wstring getName() const
        {
            return L"svd";
        }
    };

    class __Mfprintf : public SLintChecker
    {
        void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
        void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result) { }
        const std::wstring getName() const
        {
            return L"mfprintf";
        }
    };

    static std::unordered_map<std::wstring, std::wstring> deprecated;
    static std::unordered_map<std::wstring, std::shared_ptr<SLintChecker>> partiallyDeprecated;

public:

    DeprecatedChecker() { }
    ~DeprecatedChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::wstring getName() const;

private:

    static std::unordered_map<std::wstring, std::wstring> initDep();
    static std::unordered_map<std::wstring, std::shared_ptr<SLintChecker>> initPartDep();
};

} // namespace slint

#endif // __DEPRECATED_CHECKER_HXX__
