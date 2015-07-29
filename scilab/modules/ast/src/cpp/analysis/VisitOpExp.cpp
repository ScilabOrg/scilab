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

#include "AnalysisVisitor.hxx"
#include "analyzers/OperAnalyzer.hxx"
#include "tools.hxx"

namespace analysis
{



void AnalysisVisitor::visit(ast::OpExp & e)
{
    logger.log(L"OpExp", e.getLocation());
    TIType resT(getGVN());
    int tempId = -1;
    bool safe;

    e.getLeft().accept(*this);
    Result LR = getResult();
    e.getRight().accept(*this);
    Result & RR = getResult();
    if (LR.getType().isknown() && RR.getType().isknown())
    {
        TIType & LT = LR.getType();
        TIType & RT = RR.getType();

        if (!operSymbolicRange(e) && !operGVNValues(e))
        {
            switch (e.getOper())
            {
                case ast::OpExp::plus :
                {
                    resT = checkEWBinOp<_check_plus>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::minus:
                {
                    resT = checkEWBinOp<_check_minus>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::times:
                {
                    resT = Checkers::check_____times____(getGVN(), LT, RT);
                    if (resT.hasInvalidDims())
                    {
                        const bool ret = getCM().check(ConstraintManager::EQUAL, LT.cols.getValue(), RT.rows.getValue());
                        if (ret)
                        {
                            resT = Checkers::check_____times____(getGVN(), LT, RT);
                            safe = true;
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
                    else
                    {
                        safe = true;
                    }

                    tempId = dm.getTmpId(resT, false);
                    dm.releaseTmp(LR.getTempId());
                    dm.releaseTmp(RR.getTempId());

                    break;
                }
                case ast::OpExp::rdivide:
                {
                    resT = Checkers::check_____rdivide____(getGVN(), LT, RT);
                    if (resT.hasInvalidDims())
                    {
                        const bool ret = getCM().check(ConstraintManager::EQUAL, LT.cols.getValue(), RT.cols.getValue());
                        if (ret)
                        {
                            resT = Checkers::check_____rdivide____(getGVN(), LT, RT);
                            safe = true;
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
                    else
                    {
                        safe = true;
                    }

                    tempId = dm.getTmpId(resT, false);
                    dm.releaseTmp(LR.getTempId());
                    dm.releaseTmp(RR.getTempId());
                    break;
                }
                case ast::OpExp::ldivide:
                {
                    resT = Checkers::check_____ldivide____(getGVN(), LT, RT);
                    if (resT.hasInvalidDims())
                    {
                        const bool ret = getCM().check(ConstraintManager::EQUAL, LT.rows.getValue(), RT.rows.getValue());
                        if (ret)
                        {
                            resT = Checkers::check_____ldivide____(getGVN(), LT, RT);
                            safe = true;
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
                    else
                    {
                        safe = true;
                    }

                    tempId = dm.getTmpId(resT, false);
                    dm.releaseTmp(LR.getTempId());
                    dm.releaseTmp(RR.getTempId());
                    break;
                }
                case ast::OpExp::power:
                {
                    resT = Checkers::check_____power____(getGVN(), LT, RT);
                    if (resT.hasInvalidDims())
                    {
                        const bool ret = getCM().check(ConstraintManager::EQUAL, LT.rows.getValue(), LT.cols.getValue());
                        if (ret)
                        {
                            resT = Checkers::check_____power____(getGVN(), LT, RT);
                            safe = true;
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
                    else
                    {
                        safe = true;
                    }

                    tempId = dm.getTmpId(resT, false);
                    dm.releaseTmp(LR.getTempId());
                    dm.releaseTmp(RR.getTempId());
                    break;
                }
                case ast::OpExp::dottimes :
                {
                    resT = checkEWBinOp<_check_dottimes>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::dotrdivide:
                {
                    resT = checkEWBinOp<_check_dotrdiv>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::dotpower:
                {
                    resT = checkEWBinOp<_check_dotpower>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::unaryMinus :
                {
                    resT = Checkers::check_____unaryminus____(getGVN(), RT);
                    if (!resT.hasInvalidDims())
                    {
                        safe = true;
                    }
                    tempId = RR.getTempId();
                    break;
                }
                case ast::OpExp::krontimes :
                {
                    resT = Checkers::check_____krontimes____(getGVN(), LT, RT);
                    if (!resT.hasInvalidDims())
                    {
                        safe = true;
                    }
                    tempId = dm.getTmpId(resT, false);
                    dm.releaseTmp(LR.getTempId());
                    dm.releaseTmp(RR.getTempId());
                    break;
                }
                case ast::OpExp::eq:
                {
                    resT = checkEWBinOp<_check_eq>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::ne:
                {
                    resT = checkEWBinOp<_check_neq>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::lt:
                {
                    resT = checkEWBinOp<_check_lt>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::le:
                {
                    resT = checkEWBinOp<_check_le>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::gt:
                {
                    resT = checkEWBinOp<_check_gt>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::ge:
                {
                    resT = checkEWBinOp<_check_ge>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::logicalAnd:
                {
                    resT = checkEWBinOp<_check_and>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::logicalOr:
                {
                    resT = checkEWBinOp<_check_or>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::logicalShortCutAnd:
                {
                    resT = checkEWBinOp<_check_andand>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::logicalShortCutOr:
                {
                    resT = checkEWBinOp<_check_oror>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
            }
        }
        else
        {
            // SymbolicRange or GVNValue ops.
            return;
        }
    }

    e.getDecorator().safe = safe;
    e.getDecorator().res = Result(resT, tempId);
    setResult(e.getDecorator().res);

    OperAnalyzer opAn;
    opAn.analyze(*this, e);
}

void AnalysisVisitor::visit(ast::NotExp & e)
{
    logger.log(L"NotExp", e.getLocation());
    e.getExp().accept(*this);
    Result & LR = getResult();
    TIType & LT = LR.getType();
    const int tempId = LR.getTempId();
    if (LT.isknown())
    {
        TIType resT = Checkers::check_____not____(getGVN(), LT);
        e.getDecorator().res = Result(resT, tempId);
        e.getDecorator().safe = true;
    }
    else
    {
        e.getDecorator().res = Result(TIType(getGVN()), tempId);
    }
    setResult(e.getDecorator().res);
}

void AnalysisVisitor::visit(ast::TransposeExp & e)
{
    logger.log(L"TransposeExp", e.getLocation());
    e.getExp().accept(*this);
    Result & res = getResult();
    const TIType & type = res.getType();
    TIType resType(dm.getGVN(), type.type, type.cols, type.rows);
    e.getDecorator().res = Result(resType, dm.getTmpId(resType, false));
    e.getDecorator().safe = true;
    dm.releaseTmp(res.getTempId());

    setResult(e.getDecorator().res);
}
}
