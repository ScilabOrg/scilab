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

#include "checkers/SpacesAroundOpChecker.hxx"

namespace slint
{

void SpacesAroundOpChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::Exp * left = nullptr;
    unsigned int opSize;
    bool isPower = false;
    bool isAssign = false;
    if (e.isOpExp())
    {
        const ast::OpExp & oe = static_cast<const ast::OpExp &>(e);
        if (oe.getOper() != ast::OpExp::unaryMinus)
        {
            isPower = oe.getOper() == ast::OpExp::power;
            left = &oe.getLeft();
            opSize = SpacesAroundOpChecker::getOpSize(oe.getOper());
        }
    }
    else if (e.isAssignExp())
    {
        const ast::AssignExp & ae = static_cast<const ast::AssignExp &>(e);
        isAssign = true;
        left = &ae.getLeftExp();
        opSize = 1;
    }

    if (left)
    {
        std::pair<unsigned int, unsigned int> pos;
        if (context.getPosition(left->getLocation(), pos))
        {
            if (pos.first < pos.second)
            {
                const wchar_t * op = context.getCode() + pos.second;
                while (*op == L' ' || *op == L'\t' || *op == L')')
                {
                    ++op;
                }
                if (isPower && *op == L'*')
                {
                    ++opSize;
                }

                if (*(op - 1) != L' ' || (!isAssign && *(op - 2) == L' ') || *(op + opSize) != L' ' || (!isAssign && *(op + opSize + 1) == L' '))
                {
                    result.report(context, e.getLocation(), *this, _("Operator %s should be surrounded by single spaces."), std::wstring(op, op + opSize));
                }
            }
        }
    }
}

void SpacesAroundOpChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string SpacesAroundOpChecker::getName() const
{
    return "SpacesAroundOpChecker";
}

const unsigned int SpacesAroundOpChecker::getOpSize(const ast::OpExp::Oper oper)
{
    switch (oper)
    {
        case ast::OpExp::plus:
        case ast::OpExp::minus:
        case ast::OpExp::times:
        case ast::OpExp::rdivide:
        case ast::OpExp::ldivide:
        case ast::OpExp::power:
        case ast::OpExp::lt:
        case ast::OpExp::gt:
        case ast::OpExp::logicalAnd:
        case ast::OpExp::logicalOr:
            return 1;
        case ast::OpExp::dottimes:
        case ast::OpExp::dotrdivide:
        case ast::OpExp::dotldivide:
        case ast::OpExp::dotpower:
        case ast::OpExp::controltimes:
        case ast::OpExp::controlrdivide:
        case ast::OpExp::controlldivide:
        case ast::OpExp::eq:
        case ast::OpExp::ne:
        case ast::OpExp::le:
        case ast::OpExp::ge:
        case ast::OpExp::logicalShortCutAnd:
        case ast::OpExp::logicalShortCutOr:
            return 2;
        case ast::OpExp::krontimes:
        case ast::OpExp::kronrdivide:
        case ast::OpExp::kronldivide:
            return 3;
        default:
            return 0;
    }
}
}
