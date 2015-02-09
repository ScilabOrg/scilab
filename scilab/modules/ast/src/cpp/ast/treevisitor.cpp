/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include "treevisitor.hxx"
#include "execvisitor.hxx"
#include "token.hxx"

namespace ast
{
void TreeVisitor::visit(const SeqExp  &e)
{
    types::List* lst = new types::List();
    lst->append(getEOL());

    int last_line = -1;
    for (auto it : e.getExps())
    {
        int fisrt = it->getLocation().first_line;
        while (last_line != -1 && last_line < it->getLocation().first_line)
        {
            lst->append(getEOL());
            ++last_line;
        }

        last_line = it->getLocation().last_line;

        it->accept(*this);
        if (it->isAssignExp() ||
                it->isCommentExp() ||
                it->isForExp() ||
                it->isWhileExp() ||
                it->isTryCatchExp() ||
                it->isSelectExp() ||
                it->isIfExp())
        {
            lst->append(getList());
        }
        else
        {
            //add ans = before expression
            types::TList* tl = new types::TList();
            types::String* s = new types::String(1, 4);
            s->set(0, L"equal");
            s->set(1, L"expression");
            s->set(2, L"lhs");
            s->set(3, L"endsymbol");
            //header
            tl->append(s);
            //expression
            tl->append(getList());
            //lhs
            types::List* lhs = new types::List();
            lhs->append(createVar(L"ans"));
            tl->append(lhs);
            //endsymbol
            tl->append(getVerbose(*it));
            lst->append(tl);
        }
    }

    lst->append(getEOL());
    l = lst;
}

void TreeVisitor::visit(const CommentExp  &e)
{
    types::TList* tl = new types::TList();
    types::String* s = new types::String(1, 2);
    s->set(0, L"comment");
    s->set(1, L"text");
    tl->append(s);
    tl->append(new types::String(e.getComment().c_str()));
    l = tl;
}

void TreeVisitor::visit(const SimpleVar &e)
{
    l = createVar(e.getSymbol().getName());
}

void TreeVisitor::visit(const DoubleExp &e)
{
    ExecVisitor exec;
    e.accept(exec);
    l = createConst(exec.getResult());
}

void TreeVisitor::visit(const StringExp &e)
{
    ExecVisitor exec;
    e.accept(exec);
    l = createConst(exec.getResult());
}

void TreeVisitor::visit(const BoolExp &e)
{
    ExecVisitor exec;
    e.accept(exec);
    l = createConst(exec.getResult());
}

void TreeVisitor::visit(const DollarVar &e)
{
    l = createVar(L"$");
}

void TreeVisitor::visit(const ColonVar &e)
{
    l = createVar(L":");
}

void TreeVisitor::visit(const MatrixExp &e)
{
    exps_t lines = e.getLines();

    if (lines.size() == 0)
    {
        l = createConst(Double::Empty());
        return;
    }

    if (lines.size() == 1)
    {
        lines.front()->accept(*this);
        return;
    }

    types::List* sub = createOperation();
    types::List* ope = new types::List();
    sub->append(ope);

    int idx = 0;
    for (auto it : lines)
    {
        it->accept(*this);

        if (idx >= 2)
        {
            sub->append(new types::String(L"cc"));
            //create a new operation
            //put previous stage in lhs and
            //result in rhs
            types::List* lst = createOperation();
            types::List* tmp = new types::List();
            tmp->append(sub);
            //add EOL
            tmp->append(getEOL());

            lst->append(tmp);
            ope = tmp;
            sub = lst;
        }

        ope->append(getList());

        if (idx == 0)
        {
            //add EOL
            ope->append(getEOL());
        }
        ++idx;
    }

    sub->append(new types::String(L"cc"));
    l = sub;
}

void TreeVisitor::visit(const MatrixLineExp &e)
{
    exps_t columns = e.getColumns();
    if (columns.size() == 1)
    {
        columns.front()->accept(*this);
        return;
    }

    types::List* sub = createOperation();
    types::List* ope = new types::List();
    sub->append(ope);

    int idx = 0;
    int last_line = -1;
    for (auto it : columns)
    {
        it->accept(*this);

        if (idx >= 2)
        {
            sub->append(new types::String(L"rc"));
            //create a new operation
            //put previous stage in lhs and
            //result in rhs
            types::List* lst = createOperation();
            types::List* tmp = new types::List();
            tmp->append(sub);

            lst->append(tmp);
            ope = tmp;
            sub = lst;
        }

        ope->append(getList());
        ++idx;
    }

    sub->append(new types::String(L"rc"));
    l = sub;
}

void TreeVisitor::visit(const OpExp &e)
{
    types::List* ope = createOperation();
    types::List* sub = new types::List();

    if (e.getOper() != OpExp::unaryMinus)
    {
        e.getLeft().accept(*this);
        sub->append(getList());
    }
    e.getRight().accept(*this);
    sub->append(getList());
    ope->append(sub);

    types::String* s = new types::String(1, 1);
    switch (e.getOper())
    {
        // Arithmetics.
        case OpExp::plus:
            ope->append(new types::String(SCI_PLUS));
            break;
        case OpExp::unaryMinus:
        case OpExp::minus:
            ope->append(new types::String(SCI_MINUS));
            break;
        case OpExp::times:
            ope->append(new types::String(SCI_TIMES));
            break;
        case OpExp::rdivide:
            ope->append(new types::String(SCI_RDIVIDE));
            break;
        case OpExp::ldivide:
            ope->append(new types::String(SCI_LDIVIDE));
            break;
        case OpExp::power:
            ope->append(new types::String(SCI_POWER));
            break;
        // Element wise.
        case OpExp::dottimes:
            ope->append(new types::String(SCI_DOTTIMES));
            break;
        case OpExp::dotrdivide:
            ope->append(new types::String(SCI_DOTRDIVIDE));
            break;
        case OpExp::dotldivide:
            ope->append(new types::String(SCI_DOTLDIVIDE));
            break;
        case OpExp::dotpower:
            ope->append(new types::String(SCI_DOTPOWER));
            break;
        // Kroneckers
        case OpExp::krontimes:
            ope->append(new types::String(SCI_KRONTIMES));
            break;
        case OpExp::kronrdivide:
            ope->append(new types::String(SCI_KRONRDIVIDE));
            break;
        case OpExp::kronldivide:
            ope->append(new types::String(SCI_KRONLDIVIDE));
            break;
        // Control
        case OpExp::controltimes:
            ope->append(new types::String(SCI_CONTROLTIMES));
            break;
        case OpExp::controlrdivide:
            ope->append(new types::String(SCI_CONTROLRDIVIDE));
            break;
        case OpExp::controlldivide:
            ope->append(new types::String(SCI_CONTROLLDIVIDE));
            break;
        // Comparisons
        case OpExp::eq:
            ope->append(new types::String(SCI_EQ));
            break;
        case OpExp::ne:
            ope->append(new types::String(SCI_NE));
            break;
        case OpExp::lt:
            ope->append(new types::String(SCI_LT));
            break;
        case OpExp::le:
            ope->append(new types::String(SCI_LE));
            break;
        case OpExp::gt:
            ope->append(new types::String(SCI_GT));
            break;
        case OpExp::ge:
            ope->append(new types::String(SCI_GE));
            break;
        default:
            ope->append(new types::String(L"BAD OPERATOR"));
            break;
    }

    l = ope;
}

void TreeVisitor::visit(const LogicalOpExp &e)
{
    types::List* ope = createOperation();
    types::List* sub = new types::List();

    if (e.getOper() != OpExp::unaryMinus)
    {
        e.getLeft().accept(*this);
        sub->append(getList());
    }
    e.getRight().accept(*this);
    sub->append(getList());
    ope->append(sub);

    types::String* s = new types::String(1, 1);
    switch (e.getOper())
    {
        case LogicalOpExp::logicalAnd:
            ope->append(new types::String(SCI_AND));
            break;
        case LogicalOpExp::logicalOr:
            ope->append(new types::String(SCI_OR));
            break;
        case LogicalOpExp::logicalShortCutAnd:
            ope->append(new types::String(SCI_ANDAND));
            break;
        case LogicalOpExp::logicalShortCutOr:
            ope->append(new types::String(SCI_OROR));
            break;
        default:
            ope->append(new types::String(L"BAD LOGICAL OPERATOR"));
            break;
    }

    l = ope;
}

void TreeVisitor::visit(const IfExp  &e)
{
    types::TList* tl = new types::TList();
    bool hasElse = e.hasElse() && e.getElse().isCommentExp() == false;
    //header
    types::String* s = new types::String(1, 5);
    s->set(0, L"ifthenelse");
    s->set(1, L"expression");
    s->set(2, L"then");
    s->set(3, L"elseifs"); //always empty
    s->set(4, L"else");
    tl->append(s);

    //expression -> condition
    e.getTest().accept(*this);
    tl->append(getList());

    //then
    e.getThen().accept(*this);
    tl->append(getList());

    //elseif
    tl->append(new types::List());

    //else
    if (hasElse)
    {
        e.getElse().accept(*this);
        tl->append(getList());
    }
    else
    {
        tl->append(new types::List());
    }
    l = tl;
}

void TreeVisitor::visit(const ListExp  &e)
{
    types::List* ope = createOperation();
    types::List* sub = new types::List();

    e.getStart().accept(*this);
    sub->append(getList());

    if (e.hasExplicitStep())
    {
        e.getStep().accept(*this);
        sub->append(getList());
    }

    e.getEnd().accept(*this);
    sub->append(getList());

    ope->append(sub);
    ope->append(new types::String(L":"));
    l = ope;
}

void TreeVisitor::visit(const AssignExp &e)
{
    types::List* assign = createAssign();

    //expression : what to assign
    e.getRightExp().accept(*this);
    assign->append(getList());

    double* dlhs = NULL;
    if (e.getRightExp().isCallExp())
    {
        types::TList* tl = getList()->getAs<types::TList>();
        types::Double* lhsnb = tl->get(tl->getSize() - 1)->getAs<types::Double>();
        dlhs = lhsnb->get();
    }

    //lhs : who to assign
    Exp& left = e.getLeftExp();
    if (left.isSimpleVar())
    {
        left.accept(*this);
        types::List* lhs = new types::List();
        lhs->append(getList());
        assign->append(lhs);
        if (dlhs)
        {
            dlhs[0] = 1;//lhs = 1
        }
    }

    if (left.isCellCallExp())
    {
        //not yet manage
    }

    if (left.isCallExp())
    {
        CallExp* call = left.getAs<CallExp>();
        types::List* ins = createOperation();
        types::List* lhs = new types::List();
        //varname
        call->getName().accept(*this);
        lhs->append(getList());

        //indexes
        for (auto arg : call->getArgs())
        {
            arg->accept(*this);
            lhs->append(getList());
        }
        if (dlhs)
        {
            dlhs[0] = 1;//lhs = 1
        }
        ins->append(lhs);

        //operator
        ins->append(new types::String(L"ins"));
        types::List* tmp = new types::List();
        tmp->append(ins);
        assign->append(tmp);
    }

    if (left.isAssignListExp())
    {
        AssignListExp* lst = left.getAs<AssignListExp>();
        types::List* lhs = new types::List();
        for (auto exp : lst->getExps())
        {
            exp->accept(*this);
            lhs->append(getList());
        }
        if (dlhs)
        {
            dlhs[0] = static_cast<double>(lst->getExps().size());
        }


        assign->append(lhs);
    }

    if (left.isFieldExp())
    {
        FieldExp* field = left.getAs<FieldExp>();
        types::List* ins = createOperation();
        types::List* lhs = new types::List();

        field->getHead()->accept(*this);
        lhs->append(getList());

        if (field->getTail()->isSimpleVar())
        {
            SimpleVar* pVar = field->getTail()->getAs<SimpleVar>();
            lhs->append(createConst(new types::String(pVar->getSymbol().getName().c_str())));
        }
        else
        {
            //never occur ?
            field->accept(*this);
            lhs->append(getList());
        }

        ins->append(lhs);

        //operator
        ins->append(new types::String(L"ins"));
        types::List* tmp = new types::List();
        tmp->append(ins);
        assign->append(tmp);
    }

    //verbose ?
    assign->append(getVerbose(e));
    l = assign;
}

void TreeVisitor::visit(const CallExp &e)
{
    types::TList* call = new types::TList();

    //header
    types::String* varstr = new types::String(1, 4);
    varstr->set(0, L"funcall");
    varstr->set(1, L"rhs");
    varstr->set(2, L"name");
    varstr->set(3, L"lhsnb");
    call->append(varstr);

    //rhs
    types::List* rhs = new types::List();
    for (auto arg : e.getArgs())
    {
        arg->accept(*this);
        rhs->append(getList());
    }
    call->append(rhs);

    //name
    call->append(new types::String(e.getName().getAs<SimpleVar>()->getSymbol().getName().c_str()));

    //lhsnb
    //use default value 1
    //parent exp like assign can adapt it.
    call->append(new types::Double(1));

    l = call;
}

void TreeVisitor::visit(const ForExp &e)
{
    types::TList* f = new types::TList();

    //header
    types::String* varstr = new types::String(1, 3);
    varstr->set(0, L"for");
    varstr->set(1, L"expression");
    varstr->set(2, L"statements");
    f->append(varstr);

    //expression
    //create a AssignExp to call visitor it
    VarDec* vardec = e.getVardec().getAs<VarDec>();
    SimpleVar* var = new SimpleVar(*vardec->getLocation().clone(), *new symbol::Symbol(vardec->getSymbol()));
    Exp* init = vardec->getInit().clone();
    AssignExp* assign = new AssignExp(*vardec->getLocation().clone(), *var, *init);
    assign->setVerbose(true);

    assign->accept(*this);
    f->append(getList());
    delete assign;

    //statements
    e.getBody().accept(*this);
    f->append(getList());
    l = f;
}

void TreeVisitor::visit(const FieldExp &e)
{
    types::List* ext = createOperation();
    types::List* ope = new types::List();

    //function or varname
    e.getHead()->accept(*this);
    ope->append(getList());

    //arguments
    if (e.getTail()->isSimpleVar())
    {
        const SimpleVar* pVar = e.getTail()->getAs<SimpleVar>();
        ope->append(createConst(new types::String(pVar->getSymbol().getName().c_str())));
    }
    else
    {
        ope->append(getList());
    }

    //operator
    ext->append(ope);
    ext->append(new types::String(L"ext"));
    l = ext;
}

void TreeVisitor::visit(const TryCatchExp &e)
{
    types::TList* tc = new types::TList();

    //header
    types::String* varstr = new types::String(1, 3);
    varstr->set(0, L"trycatch");
    varstr->set(1, L"trystat");
    varstr->set(2, L"catchstat");
    tc->append(varstr);

    //trystat
    e.getTry().accept(*this);
    tc->append(getList());

    //catchstat
    e.getCatch().accept(*this);
    tc->append(getList());

    l = tc;
}

void TreeVisitor::visit(const WhileExp &e)
{
    types::TList* w = new types::TList();

    //header
    types::String* varstr = new types::String(1, 3);
    varstr->set(0, L"while");
    varstr->set(1, L"expression");
    varstr->set(2, L"statements");
    w->append(varstr);

    //expression
    e.getTest().accept(*this);
    w->append(getList());

    //statements
    e.getBody().accept(*this);
    w->append(getList());
    l = w;
}

void TreeVisitor::visit(const ContinueExp &e)
{
    l = createVar(L"continue");
}

void TreeVisitor::visit(const BreakExp &e)
{
    l = createVar(L"break");
}

void TreeVisitor::visit(const ReturnExp &e)
{
    if (e.isGlobal())
    {
        l = createVar(L"resume");
    }
    else
    {
        types::List* ext = createOperation();
        types::List* ope = new types::List();

        //function or varname
        ope->append(new types::String(L"return"));

        //arguments
        for (auto arg : e.getExp().getAs<ArrayListExp>()->getExps())
        {
            arg->accept(*this);
            ope->append(getList());
        }

        //operator
        ext->append(ope);
        ext->append(new types::String(L"ext"));
        l = ext;
    }
}

void TreeVisitor::visit(const SelectExp &e)
{
    types::TList* select = new types::TList();

    //header
    types::String* varstr = new types::String(1, 4);
    varstr->set(0, L"selectcase");
    varstr->set(1, L"expression");
    varstr->set(2, L"cases");
    varstr->set(3, L"else");
    select->append(varstr);

    //expression
    types::List* cond = new types::List();
    e.getSelect()->accept(*this);
    cond->append(getList());
    cond->append(getEOL());
    select->append(cond);


    //cases
    types::List* cases = new types::List();
    for (auto c : *e.getCases())
    {
        c->accept(*this);
        cases->append(getList());
    }

    select->append(cases);

    //default
    if (e.hasDefault())
    {
        e.getDefaultCase()->accept(*this);
        select->append(getList());
    }
    else
    {
        select->append(new types::List());

    }
    l = select;
}

void TreeVisitor::visit(const CaseExp &e)
{
    types::TList* c = new types::TList();

    //header
    types::String* varstr = new types::String(1, 3);
    varstr->set(0, L"case");
    varstr->set(1, L"expression");
    varstr->set(2, L"then");
    c->append(varstr);

    //expression
    e.getTest()->accept(*this);
    c->append(getList());

    //then
    e.getBody()->accept(*this);
    c->append(getList());

    l = c;
}

void TreeVisitor::visit(const ArrayListExp  &e)
{
    types::List* ext = createOperation();
    types::List* ope = new types::List();

    //function or varname
    ope->append(new types::String(L""));

    //arguments
    for (auto arg : e.getExps())
    {
        arg->accept(*this);
        ope->append(getList());
    }

    //operator
    ext->append(ope);
    ext->append(new types::String(L"ext"));
    l = ext;
}

void TreeVisitor::visit(const NotExp  &e)
{
    types::List* ext = createOperation();
    types::List* ope = new types::List();

    //function or varname
    e.getExp().accept(*this);
    ope->append(getList());

    //operator
    ext->append(ope);
    ext->append(new types::String(L"~"));
    l = ext;
}

void TreeVisitor::visit(const TransposeExp  &e)
{
    types::List* ext = createOperation();
    types::List* ope = new types::List();

    //function or varname
    e.getExp().accept(*this);
    ope->append(getList());

    //operator
    ext->append(ope);
    ext->append(new types::String(L"'"));
    l = ext;
}

void TreeVisitor::visit(const FunctionDec  &e)
{
    //types::TList* function = new types::TList();

    ////header
    //types::String* varstr = new types::String(1, 3);
    //varstr->set(0, L"inline");
    //varstr->set(1, L"prototype");
    //varstr->set(2, L"definition");
    //function->append(varstr);

    ////add returns
    //std::wstring returns;
    //returns += L"[";

    //const ArrayListVar *pListRet = e.getReturns().getAs<ArrayListVar>();
    //exps_t rets = pListRet->getVars();
    //int idx = 0;
    //for (auto ret : rets)
    //{
    //    if (idx > 0)
    //    {
    //        returns += L", ";
    //    }

    //    SimpleVar* s = ret->getAs<SimpleVar>();
    //    returns += s->getSymbol().getName();
    //    ++idx;
    //}
    //
    l = new types::List();
}

types::InternalType* TreeVisitor::getEOL()
{
    if (eol)
    {
        return eol;
    }

    eol = new types::List();
    eol->append(new types::String(L"EOL"));
    return eol;
}

types::List* TreeVisitor::createVar(const std::wstring& str)
{
    types::TList* var = new types::TList();
    types::String* varstr = new types::String(1, 2);
    varstr->set(0, L"variable");
    varstr->set(1, L"name");
    var->append(varstr);
    var->append(new types::String(str.c_str()));
    return var;
}

types::List* TreeVisitor::createConst(types::InternalType* pIT)
{
    types::TList* var = new types::TList();
    types::String* varstr = new types::String(1, 2);
    varstr->set(0, L"cste");
    varstr->set(1, L"value");
    var->append(varstr);
    var->append(pIT);
    return var;
}

types::List* TreeVisitor::createOperation()
{
    types::TList* var = new types::TList();
    types::String* varstr = new types::String(1, 3);
    varstr->set(0, L"operation");
    varstr->set(1, L"operands");
    varstr->set(2, L"operator");
    var->append(varstr);
    return var;
}

types::List* TreeVisitor::createAssign()
{
    types::TList* var = new types::TList();
    types::String* varstr = new types::String(1, 4);
    varstr->set(0, L"equal");
    varstr->set(1, L"expression");
    varstr->set(2, L"lhs");
    varstr->set(3, L"endsymbol");
    var->append(varstr);
    return var;
}

types::InternalType* TreeVisitor::getVerbose(const Exp& e)
{
    if (e.isVerbose())
    {
        return new types::String(L"");
    }
    else
    {
        return new types::String(L";");
    }
}
}