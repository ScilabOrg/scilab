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

#include "SLintContext.hxx"

namespace slint
{

SLintContext::SLintContext() : LHSExp(nullptr) { }

SLintContext::~SLintContext()
{
}

void SLintContext::setSciFile(const SciFilePtr & file)
{
    currentFile = file;
}

const std::wstring & SLintContext::getFilename() const
{
    return currentFile->getFilename();
}

const ast::Exp * SLintContext::getTree() const
{
    return currentFile->getTree();
}

unsigned int SLintContext::getCodeLength() const
{
    return currentFile->getCodeLength();
}

const wchar_t * SLintContext::getCode() const
{
    return currentFile->getCode();
}

const ast::Exp * SLintContext::getLHSExp() const
{
    return LHSExp;
}

void SLintContext::setLHSExp(const ast::Exp * _LHSExp)
{
    LHSExp = _LHSExp;
}

void SLintContext::pushFn(const ast::FunctionDec * e)
{
    funStack.push(e);
    getInOut(e);
}

const ast::FunctionDec * SLintContext::popFn()
{
    if (funStack.empty())
    {
        return nullptr;
    }

    const ast::FunctionDec * e = funStack.top();
    funStack.pop();
    if (!funStack.empty())
    {
        getInOut(funStack.top());
    }
    else
    {
        funIn.clear();
        funOut.clear();
    }

    return e;
}

const ast::FunctionDec * SLintContext::topFn()
{
    if (funStack.empty())
    {
        return nullptr;
    }

    return funStack.top();
}

bool SLintContext::isFirstLevelFn() const
{
    return funStack.size() == 1;
}

const std::unordered_set<std::wstring> & SLintContext::getFunIn() const
{
    return funIn;
}

const std::unordered_set<std::wstring> & SLintContext::getFunOut() const
{
    return funOut;
}

bool SLintContext::isFunIn(const symbol::Symbol & sym) const
{
    return isFunIn(sym.getName());
}

bool SLintContext::isFunOut(const symbol::Symbol & sym) const
{
    return isFunOut(sym.getName());
}

bool SLintContext::isFunIn(const std::wstring & name) const
{
    return funIn.find(name) != funIn.end();
}

bool SLintContext::isFunOut(const std::wstring & name) const
{
    return funOut.find(name) != funOut.end();
}

const ast::AssignExp * SLintContext::getAssignExp() const
{
    if (LHSExp)
    {
        return static_cast<const ast::AssignExp *>(LHSExp->getParent());
    }
    return nullptr;
}

bool SLintContext::getPosition(const Location & loc, std::pair<unsigned int, unsigned int> & out) const
{
    return currentFile->getPosition(loc, out);
}

bool SLintContext::getFromPositionToEOL(const Location & loc, std::pair<unsigned int, unsigned int> & out) const
{
    return currentFile->getFromPositionToEOL(loc, out);
}

bool SLintContext::checkLineLength(const unsigned int max, std::vector<unsigned int> & out) const
{
    return currentFile->checkLineLength(max, out);
}

unsigned int SLintContext::countLines() const
{
    return currentFile->countLines();
}

bool SLintContext::isAssignedVar(const ast::SimpleVar & var) const
{
    if (getLHSExp())
    {
        if (&var == getLHSExp())
        {
            // a = ...
            return true;
        }
        else if (var.getParent() == getLHSExp())
        {
            if (var.getParent()->isCallExp())
            {
                ast::CallExp * ce = static_cast<ast::CallExp *>(var.getParent());
                if (&ce->getName() == &var)
                {
                    // a(...) = ...
                    return true;
                }
            }
            else if (var.getParent()->isCellCallExp())
            {
                ast::CellCallExp * cce = static_cast<ast::CellCallExp *>(var.getParent());
                if (&cce->getName() == &var)
                {
                    // a{...} = ...
                    return true;
                }
            }
            else if (var.getParent()->isFieldExp())
            {
                ast::FieldExp * fe = static_cast<ast::FieldExp *>(var.getParent());
                if (fe->getHead() == &var)
                {
                    // a.foo = ...
                    return true;
                }
            }
            else if (var.getParent()->isAssignListExp())
            {
                // [..., a, ...] = ...
                return true;
            }
        }
    }
    return false;
}

std::wstring SLintContext::getRHSCallName() const
{
    if (LHSExp)
    {
        const ast::AssignExp & ae = *getAssignExp();
        if (ae.getRightExp().isCallExp())
        {
            const ast::CallExp & ce = static_cast<const ast::CallExp &>(ae.getRightExp());
            if (ce.getName().isSimpleVar())
            {
                return static_cast<const ast::SimpleVar &>(ce.getName()).getSymbol().getName();
            }
        }
    }
    return L"";
}

void SLintContext::getInOut(const ast::FunctionDec * e)
{
    const ast::exps_t & args = e->getArgs().getVars();
    funIn.clear();
    for (const auto arg : args)
    {
        funIn.emplace(static_cast<const ast::SimpleVar *>(arg)->getSymbol().getName());
    }
    const ast::exps_t & rets = e->getReturns().getVars();
    funOut.clear();
    for (const auto ret : rets)
    {
        funOut.emplace(static_cast<const ast::SimpleVar *>(ret)->getSymbol().getName());
    }
}

}
