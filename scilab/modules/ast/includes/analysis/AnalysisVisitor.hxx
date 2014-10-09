/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __ANALYSIS_VISITOR_HXX__
#define __ANALYSIS_VISITOR_HXX__

#include <algorithm>
#include <chrono>
#include <limits>
#include <map>
#include <memory>

#include "visitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "Checkers.hxx"
#include "ForList.hxx"
#include "Result.hxx"
#include "SymInfo.hxx"
#include "execvisitor.hxx"

namespace analysis
{

class AnalysisVisitor : public ast::Visitor
{

public:

    typedef std::map<symbol::Symbol, SymInfo> MapSymInfo;

private:

    MapSymInfo symsinfo;
    Result _result;
    unsigned int scalars_tmp[TIType::COUNT][2];
    unsigned int arrays_tmp[TIType::COUNT][2];

    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;

public:

    AnalysisVisitor()
    {
        start_chrono();
        std::fill(&scalars_tmp[0][0], &scalars_tmp[0][0] + TIType::COUNT * 2, 0);
        std::fill(&arrays_tmp[0][0], &arrays_tmp[0][0] + TIType::COUNT * 2, 0);
    }

    const MapSymInfo & get_infos() const
    {
        return symsinfo;
    }

    // Only for debug use
    inline void print_info()
    {
        stop_chrono();
        std::wcout << L"Analysis duration: " << get_duration() << L" s" << std::endl;

        std::wcout << L"Temporary scalars:" << std::endl;
        for (unsigned int i = 0; i < TIType::COUNT; ++i)
        {
            if (scalars_tmp[i][0] || scalars_tmp[i][1])
            {
                std::wcout << TIType((TIType::Type)i) << ": " << scalars_tmp[i][0] << L" and " << scalars_tmp[i][1] << std::endl;
            }
        }

        std::wcout << std::endl;

        std::wcout << L"Temporary arrays:" << std::endl;
        for (unsigned int i = 0; i < TIType::COUNT; ++i)
        {
            if (arrays_tmp[i][0] || arrays_tmp[i][1])
            {
                std::wcout << TIType((TIType::Type)i) << ": " << arrays_tmp[i][0] << L" and " << arrays_tmp[i][1] << std::endl;
            }
        }

        std::wcout << std::endl;

        for (MapSymInfo::const_iterator i = symsinfo.begin(), end = symsinfo.end(); i != end; ++i)
        {
            std::wcout << i->first.getName() << L" -> " << i->second << std::endl;
        }

        std::wcout << std::endl;
    }

    void start_chrono()
    {
        start = std::chrono::steady_clock::now();
    }

    void stop_chrono()
    {
        end = std::chrono::steady_clock::now();
    }

    double get_duration() const
    {
        return (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-9;
    }

private:

    inline void add_tmp(const TIType & t, const int n = 1, const bool scalar = false)
    {
        if (scalar)
        {
            scalars_tmp[t.type][0] += n;
            if (n > 0 && scalars_tmp[t.type][0] > scalars_tmp[t.type][1])
            {
                scalars_tmp[t.type][1] = scalars_tmp[t.type][0];
            }
        }
        else
        {
            arrays_tmp[t.type][0] += n;
            if (n > 0 && arrays_tmp[t.type][0] > arrays_tmp[t.type][1])
            {
                arrays_tmp[t.type][1] = arrays_tmp[t.type][0];
            }
        }
    }

    inline TIType get_ti(const symbol::Symbol & sym)
    {
        MapSymInfo::const_iterator i = symsinfo.find(sym);
        if (i != symsinfo.end())
        {
            return i->second.current_type;
        }

        types::InternalType * pIT = symbol::Context::getInstance()->get(sym);
        TIType t;
        if (pIT && pIT->isGenericType())
        {
            TIType::Type type;
            types::GenericType * pGT = static_cast<types::GenericType *>(pIT);
            switch (pIT->getType())
            {
                case types::InternalType::ScilabInt8 :
                    type = TIType::Type::INT8;
                    break;
                case types::InternalType::ScilabUInt8 :
                    type = TIType::Type::UINT8;
                    break;
                case types::InternalType::ScilabInt16 :
                    type = TIType::Type::INT16;
                    break;
                case types::InternalType::ScilabUInt16 :
                    type = TIType::Type::UINT16;
                    break;
                case types::InternalType::ScilabInt32 :
                    type = TIType::Type::INT32;
                    break;
                case types::InternalType::ScilabUInt32 :
                    type = TIType::Type::UINT32;
                    break;
                case types::InternalType::ScilabInt64 :
                    type = TIType::Type::INT64;
                    break;
                case types::InternalType::ScilabUInt64 :
                    type = TIType::Type::UINT64;
                    break;
                case types::InternalType::ScilabString :
                    type = TIType::Type::STRING;
                    break;
                case types::InternalType::ScilabDouble :
                {
                    types::Double * pDbl = static_cast<types::Double *>(pGT);
                    if (pDbl->isEmpty())
                    {
                        type = TIType::Type::EMPTY;
                    }
                    else if (pDbl->isComplex())
                    {
                        type = TIType::Type::COMPLEX;
                    }
                    else
                    {
                        type = TIType::Type::DOUBLE;
                    }
                    break;
                }
                case types::InternalType::ScilabBool :
                    type = TIType::Type::BOOLEAN;
                    break;
                case types::InternalType::ScilabPolynom :
                    type = TIType::Type::POLYNOMIAL;
                    break;
                case types::InternalType::ScilabSparse :
                    type = TIType::Type::SPARSE;
                    break;
                default :
                    type = TIType::Type::UNKNOWN;
            }

            t = TIType(type, pGT->getRows(), pGT->getCols());
        }
        else
        {
            t = TIType();
        }

        SymInfo si;
        si.current_type = t;
        symsinfo.emplace(sym, si);

        return t;
    }

    inline void set_sym_use(const symbol::Symbol & sym, SymInfo::Kind k)
    {
        MapSymInfo::iterator i = symsinfo.find(sym);
        if (i != symsinfo.end())
        {
            i->second.set(k);
        }
        else
        {
            symsinfo.emplace(sym, k);
        }
    }

    inline void set_sym_use(const symbol::Symbol & sym, SymInfo::Kind k1, SymInfo::Kind k2)
    {
        MapSymInfo::iterator i = symsinfo.find(sym);
        if (i != symsinfo.end())
        {
            i->second.set(k1, k2);
        }
        else
        {
            symsinfo.emplace(sym, SymInfo(k1, k2));
        }
    }

    inline void set_sym_type(const symbol::Symbol & sym, const TIType & t)
    {
        MapSymInfo::iterator i = symsinfo.find(sym);
        if (i != symsinfo.end())
        {
            i->second.current_type = t;
        }
        else
        {
            SymInfo si;
            si.current_type = t;
            symsinfo.emplace(sym, si);
        }
    }

    inline void setResult(Result & val)
    {
        _result = val;
    }

    inline void setResult(Result && val)
    {
        _result = val;
    }

    inline Result & getResult()
    {
        return _result;
    }

    void visit(ast::SimpleVar & e)
    {
        symbol::Symbol & sym = e.getSymbol();
        TIType typ = get_ti(sym);
        e.getDecorator().res = Result(typ, false, false);
        setResult(e.getDecorator().res);
        set_sym_use(e.getSymbol(), SymInfo::READ);
    }

    void visit(ast::DollarVar & e)
    {
        // nothing to do
    }

    void visit(ast::ColonVar & e)
    {
        // nothing to do
    }

    void visit(ast::ArrayListVar & e)
    {
        const ast::exps_t vars = e.getVars();
        for (ast::exps_t::const_iterator i = vars.begin(), end = vars.end(); i != end ; ++i)
        {
            (*i)->accept(*this);
        }
    }

    void visit(ast::DoubleExp & e)
    {
        e.getDecorator().res = Result(TIType(TIType::DOUBLE, 1, 1), false, true);
        setResult(e.getDecorator().res);
    }

    void visit(ast::BoolExp & e)
    {
        e.getDecorator().res = Result(TIType(TIType::BOOLEAN, 1, 1), false, true);
        setResult(e.getDecorator().res);
    }

    void visit(ast::StringExp & e)
    {
        e.getDecorator().res = Result(TIType(TIType::STRING, 1, 1), false, true);
        setResult(e.getDecorator().res);
    }

    void visit(ast::CommentExp & e)
    {
        // ignored
    }

    void visit(ast::NilExp & e)
    {
        // nothing to do
    }

    void visit(ast::CallExp & e)
    {
        e.getName().accept(*this);
        const ast::exps_t args = e.getArgs();
        for (ast::exps_t::const_iterator i = args.begin(), end = args.end(); i != end; ++i)
        {
            (*i)->accept(*this);
        }
    }

    void visit(ast::CellCallExp & e)
    {
        visit(static_cast<ast::CallExp &>(e));
    }

    void visit(ast::OpExp & e)
    {
        e.getLeft().accept(*this);
        Result LR = getResult();
        e.getRight().accept(*this);
        Result & RR = getResult();
        const TIType & LType = LR.getType();
        const TIType & RType = RR.getType();
        TIType resT;
        bool allocTmp = false;

        // We can released the temp vars
        if (LR.isTemp())
        {
            add_tmp(LType, -1);
        }
        if (RR.isTemp())
        {
            add_tmp(RType, -1);
        }

        //if left and right are constant, result is constant too
        if (e.getLeft().getDecorator().res.isConstant() && e.getRight().getDecorator().res.isConstant())
        {
            if (execAndReplace(e))
            {
                return;
            }
        }

        switch (e.getOper())
        {
            case ast::OpExp::plus :
            {
                if (replaceDAXPY(e))
                {
                    return;
                }
                //continue in generic case
            }
            case ast::OpExp::minus :
            case ast::OpExp::dottimes :
            {
                // TODO: check if the rules for addition and subtraction are the same
                resT = check_add(LType, RType);
                break;
            }
            case ast::OpExp::times :
            {
                if (replaceDGEMM(e))
                {
                    return;
                }
                // multiplication is not commutative for matrice pxq
                resT = check_times(LType, RType);
                break;
            }
        }

        if (resT.isknown() && !resT.isscalar())
        {
            // result is a matrix so we need a tmp
            add_tmp(resT.type, 1);
            allocTmp = true;
        }

        e.getDecorator().res = Result(resT, allocTmp, false);
        setResult(e.getDecorator().res);
    }

    void visit(ast::LogicalOpExp & e)
    {
        e.getLeft().accept(*this);
        e.getRight().accept(*this);
    }

    void visit(ast::AssignExp & e)
    {
        if (e.getLeftExp().isSimpleVar())
        {
            ast::SimpleVar & var = static_cast<ast::SimpleVar &>(e.getLeftExp());
            symbol::Symbol & sym = var.getSymbol();

            e.getRightExp().accept(*this);
            var.getDecorator().res = getResult();

            set_sym_use(sym, SymInfo::REPLACE);
            set_sym_type(sym, getResult().getType());
        }
        else
        {
            // TODO: handle this case
        }
    }

    void visit(ast::IfExp & e)
    {
        e.getTest().accept(*this);
        e.getThen().accept(*this);
        if (e.hasElse())
        {
            e.getElse().accept(*this);
        }
    }

    void visit(ast::WhileExp & e)
    {
        e.getTest().accept(*this);
        e.getBody().accept(*this);
    }

    void visit(ast::ForExp & e)
    {
        e.getVardec().accept(*this);
        e.getBody().accept(*this);

        MapSymInfo::const_iterator it = symsinfo.find(e.getVardec().getAs<ast::VarDec>()->getSymbol());
        if (it->second.read)
        {
            e.getVardec().getAs<ast::VarDec>()->getListInfo().setReadInLoop(true);
        }
    }

    void visit(ast::BreakExp & e)
    {
        // nothing to do
    }

    void visit(ast::ContinueExp & e)
    {
        // nothing to do
    }

    void visit(ast::TryCatchExp & e)
    {
        e.getTry().accept(*this);
        e.getCatch().accept(*this);
    }

    void visit(ast::SelectExp & e)
    {
        e.getSelect()->accept(*this);
        ast::exps_t* cases = e.getCases();
        for (ast::exps_t::const_iterator i = cases->begin(), end = cases->end(); i != end; ++i)
        {
            (*i)->accept(*this);
        }
        e.getDefaultCase()->accept(*this);
    }

    void visit(ast::CaseExp & e)
    {
        e.getTest()->accept(*this);
        e.getBody()->accept(*this);
    }

    void visit(ast::ReturnExp & e)
    {
        e.getExp().accept(*this);
    }

    void visit(ast::FieldExp & e)
    {
        // a.b.c <=> (a.b).c where a.b is the head and c is the tail

        //e.getHead()->accept(*this);
        //e.getTail()->accept(*this);
    }

    void visit(ast::NotExp & e)
    {
        e.getExp().accept(*this);
    }

    void visit(ast::TransposeExp & e)
    {
        e.getExp().accept(*this);
    }

    void visit(ast::MatrixExp & e)
    {
        const ast::exps_t lines = e.getLines();
        bool constant = true;
        for (ast::exps_t::const_iterator i = lines.begin(), itEnd = lines.end(); i != itEnd; ++i)
        {
            (*i)->accept(*this);
            if ((*i)->getDecorator().res.isConstant() == false)
            {
                constant = false;
            }
        }

        if (constant)
        {
            execAndReplace(e);
        }
    }

    void visit(ast::MatrixLineExp & e)
    {
        const ast::exps_t columns = e.getColumns();
        bool constant = true;
        for (ast::exps_t::const_iterator i = columns.begin(), itEnd = columns.end(); i != itEnd; ++i)
        {
            (*i)->accept(*this);
            if ((*i)->getDecorator().res.isConstant() == false)
            {
                constant = false;
            }
        }

        e.getDecorator().res = Result(e.getDecorator().res.getType(), e.getDecorator().res.isTemp(), constant);

    }

    void visit(ast::CellExp & e)
    {
        visit(static_cast<ast::MatrixExp &>(e));
    }

    void visit(ast::SeqExp & e)
    {
        const ast::exps_t exps = e.getExps();
        for (ast::exps_t::const_iterator i = exps.begin(), itEnd = exps.end(); i != itEnd; ++i)
        {
            (*i)->accept(*this);
        }
    }

    void visit(ast::ArrayListExp & e)
    {
        const ast::exps_t exps = e.getExps();
        for (ast::exps_t::const_iterator i = exps.begin(), itEnd = exps.end(); i != itEnd; ++i)
        {
            (*i)->accept(*this);
        }
    }

    void visit(ast::AssignListExp & e)
    {
        visit(static_cast<ast::ArrayListExp &>(e));
    }

    void visit(ast::VarDec & e)
    {
        // VarDec is only used in For loop for iterator declaration
        symbol::Symbol & sym = e.getSymbol();
        if (e.getInit().isListExp())
        {
            ast::ListExp & le = static_cast<ast::ListExp &>(e.getInit());
            if (le.getStart().isDoubleExp() && le.getStep().isDoubleExp() && le.getEnd().isDoubleExp())
            {
                ForList64 fl(static_cast<const ast::DoubleExp &>(le.getStart()).getValue(),
                             static_cast<const ast::DoubleExp &>(le.getStep()).getValue(),
                             static_cast<const ast::DoubleExp &>(le.getEnd()).getValue());
                e.setListInfo(fl);
                set_sym_use(sym, SymInfo::REPLACE, SymInfo::FOR_IT);
                set_sym_type(sym, fl.getType());
                // No need to visit the list (it has been visited just before)
            }
            else
            {
                e.setListInfo(ForList64());
                le.accept(*this);
            }
        }
    }

    void visit(ast::FunctionDec & e)
    {
        e.getArgs().accept(*this);
        e.getReturns().accept(*this);
        e.getBody().accept(*this);
    }

    void visit(ast::ListExp & e)
    {
        double start = std::numeric_limits<double>::quiet_NaN();
        double step = std::numeric_limits<double>::quiet_NaN();
        double end = std::numeric_limits<double>::quiet_NaN();

        if (e.getStart().isDoubleExp())
        {
            start = static_cast<const ast::DoubleExp &>(e.getStart()).getValue();
        }

        if (e.getStep().isDoubleExp())
        {
            step = static_cast<ast::DoubleExp &>(e.getStep()).getValue();
        }

        if (e.getEnd().isDoubleExp())
        {
            end = static_cast<ast::DoubleExp &>(e.getEnd()).getValue();
        }

        const_cast<ast::ListExp &>(e).setValues(start, step, end);
    }

    /* optimized */
    void visit(ast::OptimizedExp & e)
    {
        //gn� ??? Oo
    }

    void visit(ast::DAXPYExp & e)
    {
        //gn� ??? Oo
    }

    void visit(ast::DGEMMExp & e)
    {
        //gn� ??? Oo
    }

    bool replaceDAXPY(ast::OpExp& e)
    {
        bool ret = false;

        if (e.getOper() == ast::OpExp::plus)
        {
            ast::Exp& le = e.getLeft();
            ast::Exp& re = e.getRight();
            ast::Exp* a = NULL;
            ast::Exp* x = NULL;
            ast::Exp* y = NULL;

            // a * x(i,j) + y(i,j) or y(i,j) + a * x(i,j)
            // a .* x(i,j) + y(i,j) or y(i,j) + a .* x(i,j)
            if (le.isOpExp() &&
                    (le.getAs<ast::OpExp>()->getOper() == ast::OpExp::dottimes ||
                     le.getAs<ast::OpExp>()->getOper() == ast::OpExp::times))
            {
                ast::OpExp* dt = le.getAs<ast::OpExp>();
                y = &re;
                a = &dt->getLeft();
                x = &dt->getRight();
            }
            else if (re.isOpExp() &&
                     (re.getAs<ast::OpExp>()->getOper() == ast::OpExp::dottimes ||
                      re.getAs<ast::OpExp>()->getOper() == ast::OpExp::times))
            {
                ast::OpExp* rt = re.getAs<ast::OpExp>();
                y = &le;
                a = &rt->getLeft();
                x = &rt->getRight();
            }

            if (a && x && y)
            {
                ast::Exp* exp = new ast::DAXPYExp(e.getLocation(), *a, *x, *y);
                exp->setVerbose(e.isVerbose());
                exp->getDecorator().res = e.getDecorator().res;
                e.replace(exp);
                ret = true;
            }
        }
        return ret;
    }

    bool replaceDGEMM(ast::OpExp& e)
    {
        if (e.getOper() == ast::OpExp::times)
        {
            ast::Exp* a = &e.getLeft();
            ast::Exp* b = &e.getRight();
            bool transa = false;
            bool transb = false;

            if (a->isTransposeExp())
            {
                transa = true;
                a = &a->getAs<ast::TransposeExp>()->getExp();
            }

            if (b->isTransposeExp())
            {
                transb = true;
                b = &b->getAs<ast::TransposeExp>()->getExp();
            }

            ast::Exp* exp = new ast::DGEMMExp(e.getLocation(), *a, transa, *b, transb);
            exp->setVerbose(e.isVerbose());
            exp->getDecorator().res = analysis::Result(TIType(TIType::UNKNOWN, -1, -1), true, false);
            e.replace(exp);
            return true;

            //analysis::TIType ta = a->getDecorator().res.getType();
            //analysis::TIType tb = b->getDecorator().res.getType();

            //int ra = 0, ca = 0, rb = 0, cb = 0;
            //if(ta.type == analysis::TIType::DOUBLE && tb.type == analysis::TIType::DOUBLE)
            //{
            //    if(transa)
            //    {
            //        ra = ta.cols;
            //        ca = ta.rows;
            //        if(transb)
            //        {
            //            rb = tb.cols;
            //            cb = tb.rows;
            //        }
            //        else
            //        {
            //            rb = tb.rows;
            //            cb = tb.cols;
            //        }
            //    }
            //    else
            //    {
            //        ra = ta.rows;
            //        ca = ta.cols;
            //        if(transb)
            //        {
            //            rb = tb.cols;
            //            cb = tb.rows;
            //        }
            //        else
            //        {
            //            rb = tb.rows;
            //            cb = tb.cols;
            //        }
            //    }

            //    if(ca == rb && ca != -1)
            //    {
            //        ast::Exp* exp = new ast::DGEMMExp(e.getLocation(), *a, transa, *b, transb);
            //        exp->setVerbose(e.isVerbose());
            //        exp->getDecorator().res = analysis::Result(TIType(TIType::DOUBLE, ra, cb), true, false);
            //        e.replace(exp);
            //        std::cout << "DGEMMExp via inference" << std::endl;
            //        return true;
            //    }
            //}

            ////optimization via inference failed :x
            ////check exp type
            //if(a->isSimpleVar() && b->isSimpleVar())
            //{
            //    ast::Exp* exp = new ast::DGEMMExp(e.getLocation(), *a, transa, *b, transb);
            //    exp->setVerbose(e.isVerbose());
            //    exp->getDecorator().res = analysis::Result(TIType(TIType::UNKNOWN, -1, -1), true, false);
            //    e.replace(exp);
            //    std::cout << "DGEMMExp on simplevar" << std::endl;
            //    return true;
            //}
        }

        return false;
    }

    bool execAndReplace(ast::Exp& e)
    {
        //exec operation and substitute exp by result
        ast::ExecVisitor exec;

        try
        {
            e.accept(exec);
            InternalType* result = exec.getResult();
            ast::Exp* exp = result->getExp(e.getLocation());
            if (exp)
            {
                exp->setVerbose(e.isVerbose());
                e.replace(exp);
                exp->getDecorator().res = Result(e.getDecorator().res.getType(), e.getDecorator().res.isTemp(), true);
                setResult(exp->getDecorator().res);
                return true;
            }
        }
        catch (const ast::ScilabException& /*se*/)
        {
            //nothing to do, stop optimization phase and continue.
            std::cout << "optimization failed !" << std::endl;
        }

        return false;
    }
};

} // namespace analysis

#endif // __ANALYSIS_VISITOR_HXX__
