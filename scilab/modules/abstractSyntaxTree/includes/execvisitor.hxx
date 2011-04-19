/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef AST_EXECVISITOR_HXX
#define AST_EXECVISITOR_HXX

#include "runvisitor.hxx"

extern "C"
{
#include "dynlib_ast.h"
}

namespace ast
{
    class EXTERN_AST ExecVisitor : public RunVisitorT<ExecVisitor>
	{
        void visit (const SeqExp  &e);

        void visit (const MatrixExp &e);

        void visit (const MatrixLineExp &e);

        void visit (const CellExp &e);

        void visit (const StringExp &e);

        void visit (const CommentExp &e);

        void visit (const IntExp &e);

        void visit (const FloatExp &e);

        void visit (const DoubleExp &e);

        void visit (const BoolExp &e);

        void visit (const NilExp &e);

        void visit (const SimpleVar &e);

        void visit (const ColonVar &e);

        void visit (const DollarVar &e);

        void visit (const ArrayListVar &e);

        void visit (const FieldExp &e);

        void visit (const OpExp &e);

        void visit (const LogicalOpExp &e);

        void visit (const AssignExp &e);

        void visit (const CellCallExp &e);

        void visit (const CallExp &e);

        void visit (const IfExp &e);

        void visit (const TryCatchExp &e);

        void visit (const WhileExp &e);

        void visit (const ForExp &e);

        void visit (const BreakExp &e);

        void visit (const ContinueExp &e);

        void visit (const ReturnExp &e);

        void visit (const SelectExp &e);

        void visit (const CaseExp &e);

        void visit (const ArrayListExp &e);

        void visit (const AssignListExp &e);

        void visit (const NotExp &e);

        void visit (const TransposeExp &e);

        void visit (const VarDec &e);

        void visit (const FunctionDec &e);

        void visit(const ListExp &e);
    };
}
#endif // !AST_EXECVISITOR_HXX
