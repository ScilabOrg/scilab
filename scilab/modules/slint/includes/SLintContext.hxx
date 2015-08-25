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

#ifndef __SLINT_CONTEXT_HXX__
#define __SLINT_CONTEXT_HXX__

#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

#include "allexp.hxx"
#include "allvar.hxx"
#include "alldec.hxx"

#include "SciFile.hxx"

namespace slint
{

class SLintContext
{

    SciFilePtr currentFile;
    const ast::Exp * LHSExp;
    std::stack<const ast::FunctionDec *> funStack;
    std::unordered_set<std::wstring> funIn;
    std::unordered_set<std::wstring> funOut;

public:

    SLintContext();
    ~SLintContext();

    void setSciFile(const SciFilePtr & file);
    const std::wstring & getFilename() const;
    const ast::Exp * getTree() const;
    unsigned int getCodeLength() const;
    const wchar_t * getCode() const;
    const ast::Exp * getLHSExp() const;
    void setLHSExp(const ast::Exp * _LHSExp);
    void pushFn(const ast::FunctionDec * e);
    const ast::FunctionDec * popFn();
    const ast::FunctionDec * topFn();
    bool isFirstLevelFn() const;
    const std::unordered_set<std::wstring> & getFunIn() const;
    const std::unordered_set<std::wstring> & getFunOut() const;
    bool isFunIn(const symbol::Symbol & sym) const;
    bool isFunOut(const symbol::Symbol & sym) const;
    bool isFunIn(const std::wstring & name) const;
    bool isFunOut(const std::wstring & name) const;
    const ast::AssignExp * getAssignExp() const;
    bool checkLineLength(const unsigned int max, std::vector<unsigned int> & out) const;
    unsigned int countLines() const;
    bool getPosition(const Location & loc, std::pair<unsigned int, unsigned int> & out) const;
    bool getFromPositionToEOL(const Location & loc, std::pair<unsigned int, unsigned int> & out) const;
    bool isAssignedVar(const ast::SimpleVar & var) const;
    std::wstring getRHSCallName() const;

private:

    void getInOut(const ast::FunctionDec * e);
};

} // namespace slint

#endif // __SLINT_CONTEXT_HXX__
