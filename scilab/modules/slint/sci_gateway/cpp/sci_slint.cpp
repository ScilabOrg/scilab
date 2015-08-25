/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include <iostream>

#include "SLint.hxx"
#include "SLintScilabResult.hxx"
#include "checkers/FunctionNameChecker.hxx"
#include "checkers/FunctionArgsChecker.hxx"
#include "checkers/VariablesChecker.hxx"
#include "checkers/UselessArgChecker.hxx"
#include "checkers/UselessRetChecker.hxx"
#include "checkers/VariableNameChecker.hxx"
#include "checkers/RedefinitionChecker.hxx"
#include "checkers/SingleInstrChecker.hxx"
#include "checkers/EmptyBlockChecker.hxx"
#include "checkers/SemicolonAtEOLChecker.hxx"
#include "checkers/MopenMcloseChecker.hxx"
#include "checkers/McCabeChecker.hxx"
#include "checkers/DecimalChecker.hxx"
#include "checkers/PrintfChecker.hxx"
#include "checkers/TodoChecker.hxx"
#include "checkers/LineLengthChecker.hxx"
#include "checkers/IllegalCallsChecker.hxx"
#include "checkers/LinesCountChecker.hxx"
#include "checkers/NaNChecker.hxx"
#include "checkers/EqEqChecker.hxx"
#include "checkers/UselessOpChecker.hxx"
#include "checkers/UnreachableCodeChecker.hxx"
#include "checkers/DeprecatedChecker.hxx"
#include "checkers/SelectChecker.hxx"
#include "checkers/ImplicitListChecker.hxx"
#include "checkers/StructChecker.hxx"
#include "checkers/LoadSaveChecker.hxx"
#include "checkers/OldNotChecker.hxx"
#include "checkers/SpacesAroundOpChecker.hxx"
#include "checkers/BreaksInLoopChecker.hxx"
#include "checkers/NestedBlocksChecker.hxx"
#include "checkers/BracketedExpChecker.hxx"
#include "checkers/NotNotChecker.hxx"
#include "checkers/SpacesInArgsChecker.hxx"

#include "string.hxx"
#include "slint_gw.hxx"

#include <iostream>
#include <string>

extern "C"
{
#include "os_string.h"
#include "expandPathVariable.h"
#include "Scierror.h"
#include "localization.h"
#include "os_string.h"
#include "mopen.h"
#include "mclose.h"
#include "fullpath.h"
#include "PATH_MAX.h"
}

using namespace std;
using namespace types;
using namespace ast;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_slint(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: at least %d expected.\n"), "slint", 1);
        return Function::Error;
    }

    if (!in[0]->isString())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "slint" , 1);
        return Function::Error;
    }

    try
    {
        slint::SLintScilabResult results;
        slint::SLint slint(results);
        slint.setFiles(in[0]->getAs<types::String>());
        slint::SLintOptions options;

        options.add(new slint::RedefinitionChecker(), ast::Exp::SIMPLEVAR);
        options.add(new slint::VariablesChecker(), ast::Exp::FUNCTIONDEC, ast::Exp::SIMPLEVAR, ast::Exp::VARDEC);

        options.add(new slint::FunctionNameChecker(L"[a-z]*", -1, -1), ast::Exp::FUNCTIONDEC);
        options.add(new slint::FunctionArgsChecker(), ast::Exp::FUNCTIONDEC);
        options.add(new slint::UselessArgChecker(), ast::Exp::FUNCTIONDEC, ast::Exp::SIMPLEVAR);
        options.add(new slint::UselessRetChecker(), ast::Exp::FUNCTIONDEC, ast::Exp::SIMPLEVAR);
        options.add(new slint::VariableNameChecker(L"[a-zA-Z0-9]*", -1, -1), ast::Exp::SIMPLEVAR, ast::Exp::VARDEC);
        options.add(new slint::SingleInstrChecker(), ast::Exp::SEQEXP);
        options.add(new slint::EmptyBlockChecker(), ast::Exp::SEQEXP);
        options.add(new slint::SemicolonAtEOLChecker(), ast::Exp::SEQEXP);
        options.add(new slint::MopenMcloseChecker(true), ast::Exp::FUNCTIONDEC, ast::Exp::CALLEXP);
        options.add(new slint::McCabeChecker(30), ast::Exp::FUNCTIONDEC);
        options.add(new slint::DecimalChecker(L'e', true), ast::Exp::DOUBLEEXP);
        options.add(new slint::PrintfChecker(), ast::Exp::CALLEXP);
        options.add(new slint::LineLengthChecker(80), ast::Exp::FUNCTIONDEC);
        options.add(new slint::LinesCountChecker(200), ast::Exp::FUNCTIONDEC);
        options.add(new slint::TodoChecker(), ast::Exp::COMMENTEXP);
        options.add(new slint::NaNChecker(), ast::Exp::OPEXP);
        options.add(new slint::EqEqChecker(), ast::Exp::OPEXP);
        options.add(new slint::UselessOpChecker(), ast::Exp::OPEXP);
        options.add(new slint::UnreachableCodeChecker(), ast::Exp::SEQEXP);
        options.add(new slint::DeprecatedChecker(), ast::Exp::CALLEXP);
        options.add(new slint::SelectChecker(), ast::Exp::SELECTEXP);
        options.add(new slint::ImplicitListChecker(), ast::Exp::LISTEXP);
        options.add(new slint::StructChecker(L"", -1, -1), ast::Exp::CALLEXP, ast::Exp::SIMPLEVAR, ast::Exp::FIELDEXP);
        options.add(new slint::LoadSaveChecker(), ast::Exp::CALLEXP);
        options.add(new slint::OldNotChecker(), ast::Exp::NOTEXP);
        options.add(new slint::SpacesAroundOpChecker(), ast::Exp::OPEXP, ast::Exp::ASSIGNEXP);
        options.add(new slint::SpacesInArgsChecker(), ast::Exp::CALLEXP, ast::Exp::FUNCTIONDEC);
        options.add(new slint::BreaksInLoopChecker(1, 1), ast::Exp::FOREXP, ast::Exp::WHILEEXP, ast::Exp::BREAKEXP, ast::Exp::CONTINUEEXP);
        options.add(new slint::NestedBlocksChecker(5), ast::Exp::IFEXP, ast::Exp::WHILEEXP, ast::Exp::FOREXP, ast::Exp::SELECTEXP);
        options.add(new slint::BracketedExpChecker(), ast::Exp::OPEXP);
        options.add(new slint::NotNotChecker(), ast::Exp::NOTEXP);

        std::vector<std::wstring> names = {L"pause"};
        options.add(new slint::IllegalCallsChecker(names), ast::Exp::SIMPLEVAR);

        slint.setOptions(options);
        slint.check();
        results.getResults();
    }
    catch (slint::PCREException & e)
    {
        Scierror(999, _("%s: %s\n"), "slint", e.what(), 1);
        return Function::Error;
    }
    catch (slint::FileException & e)
    {
        Scierror(999, _("%s: %s\n"), "slint", e.what(), 1);
        return Function::Error;
    }

    return Function::OK;
}
