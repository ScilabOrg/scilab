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

#ifndef __LINT_MCCABE_VISITOR_HXX__
#define __LINT_MCCABE_VISITOR_HXX__

#include "dummyvisitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "alldec.hxx"

namespace slint
{

class McCabeVisitor : public ast::DummyVisitor
{

    unsigned int complexity;

public:

    McCabeVisitor() { }
    ~McCabeVisitor() { }

    inline unsigned int getComplexity(const ast::FunctionDec & fd)
    {
        complexity = 1;
        fd.getBody().accept(*this);

        return complexity;
    }

private:

    void visit(const ast::IfExp & e)
    {
        ++complexity;
        ast::DummyVisitor::visit(e);
    }

    void visit(const ast::WhileExp & e)
    {
        ++complexity;
        ast::DummyVisitor::visit(e);
    }

    void visit(const ast::ForExp & e)
    {
        ++complexity;
        ast::DummyVisitor::visit(e);
    }

    void visit(const ast::TryCatchExp & e)
    {
        ++complexity;
        ast::DummyVisitor::visit(e);
    }

    void visit(const ast::SelectExp & e)
    {
        complexity += static_cast<unsigned int>(e.getCases().size()) - 1;
        if (e.hasDefault())
        {
            ++complexity;
        }
        ast::DummyVisitor::visit(e);
    }

    void visit(const ast::ReturnExp & e)
    {
        ++complexity;
    }

    void visit(const ast::SeqExp & e)
    {
        ast::DummyVisitor::visit(e);
    }

    void visit(const ast::CaseExp & e)
    {
        ast::DummyVisitor::visit(e);
    }

    void visit(const ast::FunctionDec & e)
    {
    }

    void visit(const ast::CallExp & e)
    {
    }

    void visit(const ast::SimpleVar & e)
    {
    }

    void visit(const ast::NotExp & e)
    {
    }

    void visit(const ast::TransposeExp & e)
    {
    }

    void visit(const ast::MatrixExp & e)
    {
    }

    void visit(const ast::CellExp & e)
    {
    }

    void visit(const ast::OpExp & e)
    {
    }

    void visit(const ast::LogicalOpExp & e)
    {
    }

    void visit(const ast::ListExp & e)
    {
    }

    void visit(const ast::DollarVar & e)
    {
    }

    void visit(const ast::ColonVar & e)
    {
    }

    void visit(const ast::ArrayListVar & e)
    {
    }

    void visit(const ast::DoubleExp & e)
    {
    }

    void visit(const ast::BoolExp & e)
    {
    }

    void visit(const ast::StringExp & e)
    {
    }

    void visit(const ast::CommentExp & e)
    {
    }

    void visit(const ast::NilExp & e)
    {
    }

    void visit(const ast::CellCallExp & e)
    {
    }

    void visit(const ast::AssignExp & e)
    {
    }

    void visit(const ast::BreakExp & e)
    {
    }

    void visit(const ast::ContinueExp & e)
    {
    }

    void visit(const ast::FieldExp & e)
    {
    }

    void visit(const ast::MatrixLineExp & e)
    {
    }

    void visit(const ast::ArrayListExp & e)
    {
    }

    void visit(const ast::AssignListExp & e)
    {
    }

    void visit(const ast::VarDec & e)
    {
    }

    void visit(const ast::OptimizedExp & e)
    {
    }

    void visit(const ast::DAXPYExp & e)
    {
    }
};

} // namespace mcCabe

#endif // __LINT_MCCABE_VISITOR_HXX__
