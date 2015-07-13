/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO- Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "core_gw.hxx"
#include "function.hxx"
#include "macro.hxx"
#include "macrofile.hxx"
#include "tlist.hxx"
#include "string.hxx"
#include "configvariable.hxx"
#include "treevisitor.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_macr2tree(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected."), "macr2tree", 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "macr2tree", 1);
        return types::Function::Error;
    }

    types::InternalType* pIT = in[0];
    if (pIT->isMacro() == false && pIT->isMacroFile() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: macro expected.\n"), "macr2tree", 1);
        return types::Function::Error;
    }

    types::Macro* macro = NULL;
    if (pIT->isMacroFile())
    {
        types::MacroFile* pMF = pIT->getAs<types::MacroFile>();
        macro = pMF->getMacro();
    }
    else
    {
        macro = pIT->getAs<types::Macro>();
    }

    std::list<symbol::Variable*>* outputs = macro->getOutputs();
    std::list<symbol::Variable*>* inputs = macro->getInputs();
    ast::SeqExp* body = macro->getBody();

    //create a tlist "program"
    types::TList* l = new types::TList();
    types::String* s = new types::String(1, 6);
    s->set(0, L"program");
    s->set(1, L"name");
    s->set(2, L"outputs");
    s->set(3, L"inputs");
    s->set(4, L"statements");
    s->set(5, L"nblines");

    //header
    l->append(s);
    //name
    l->append(new types::String(macro->getName().c_str()));

    //outputs
    types::List* o = new types::List();
    for (auto p : *outputs)
    {
        o->append(ast::TreeVisitor::createVar(p->getSymbol().getName()));
    }

    l->append(o);

    //inputs
    types::List* i = new types::List();
    for (auto p : *inputs)
    {
        i->append(ast::TreeVisitor::createVar(p->getSymbol().getName()));
    }

    l->append(i);
    //statement
    ast::TreeVisitor v;
    body->accept(v);

    types::List* statement = new List();
    // add empty line between the "function" line and first body line
    int iNbEmptyLine = body->getLocation().first_line - macro->getFirstLine();
    for (int i = 0; i < iNbEmptyLine; ++i)
    {
        statement->append(v.getEOL());
    }

    // add body of function
    types::List* lBody = v.getList();
    for (int i = 0; i < lBody->getSize(); ++i)
    {
        statement->append(lBody->get(i));
    }
    lBody->killMe();

    // add empty line between last bidy line and the "endfunction" line
    iNbEmptyLine = macro->getLastLine() - body->getLocation().last_line;
    for (int i = 0; i < iNbEmptyLine; ++i)
    {
        statement->append(v.getEOL());
    }

    l->append(statement);
    //nb lines
    l->append(new types::Double(macro->getLastLine() - macro->getFirstLine() + 1));
    out.push_back(l);
    statement->killMe();
    return types::Function::OK;
}
