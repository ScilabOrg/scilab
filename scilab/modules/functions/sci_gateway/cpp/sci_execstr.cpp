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
#include "AnalysisVisitor.hxx"
#include "parser.hxx"
#include "functions_gw.hxx"
//#include "debugvisitor.hxx"
#include "execvisitor.hxx"
#include "mutevisitor.hxx"
#include "printvisitor.hxx"
#include "visitor_common.hxx"
#include "scilabWrite.hxx"
#include "scilabexception.hxx"
#include "configvariable.hxx"
#include "context.hxx"
#include "runner.hxx"
#include "threadmanagement.hxx"

#include <iostream>
#include <fstream>
#include <string>

extern "C"
{
#include "sci_malloc.h"
#include "os_string.h"
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "os_string.h"
}

#define MUTE_FLAG       L"n"
#define NO_MUTE_FLAG    L"m"

using namespace std;
using namespace types;
using namespace ast;
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_execstr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iErr            = 0;
    bool bErrCatch		= false;
    bool bMute          = false;
    wchar_t* pstMsg     = NULL;
    Exp* pExp           = NULL;
    wchar_t *pstCommand = NULL;
    Parser parser;

    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "execstr" , 1, 3);
        return Function::Error;
    }

    //2nd parameter
    if (in.size() > 1)
    {
        //errcatch
        if (in[1]->isString() == false || in[1]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "execstr", 2);
            return Function::Error;
        }

        String* pS = in[1]->getAs<types::String>();
        if (os_wcsicmp(pS->get(0), L"errcatch") == 0)
        {
            bErrCatch = true;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: 'errcatch' expected.\n"), "execstr", 2);
            return Function::Error;
        }

        bMute = true;
    }

    //3rd parameter
    if (in.size() == 3)
    {
        if (in[2]->isString() == false || in[2]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "execstr", 3);
            return Function::Error;
        }

        if (os_wcsicmp(in[2]->getAs<types::String>()->get(0), MUTE_FLAG) == 0)
        {
            bMute = true;
        }
        else if (os_wcsicmp(in[2]->getAs<types::String>()->get(0), NO_MUTE_FLAG) == 0)
        {
            bMute = false;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), "execstr", 3, MUTE_FLAG, NO_MUTE_FLAG);
            return Function::Error;
        }
    }

    //1st argument
    if (in[0]->isDouble() && in[0]->getAs<Double>()->getSize() == 0)
    {
        // execstr([])
        out.push_back(Double::Empty());
        return Function::OK;
    }

    if (in[0]->isString() == false || (in[0]->getAs<types::String>()->getRows() != 1 && in[0]->getAs<types::String>()->getCols() != 1))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Vector of strings expected.\n"), "execstr", 1);
        return Function::Error;
    }

    String* pS = in[0]->getAs<types::String>();
    int iTotalLen = pS->getSize(); //add \n after each string
    for (int i = 0 ; i < pS->getSize() ; i++)
    {
        iTotalLen += (int)wcslen(pS->get(i));
    }

    pstCommand = (wchar_t*)MALLOC(sizeof(wchar_t) * (iTotalLen + 1));//+1 for null termination

    for (int i = 0, iPos = 0 ; i < pS->getSize() ; i++)
    {
        wcscpy(pstCommand + iPos, pS->get(i));
        iPos = (int)wcslen(pstCommand);
        pstCommand[iPos++] = L'\n';
        pstCommand[iPos] = 0;
    }

    parser.parse(pstCommand);
    FREE(pstCommand);
    if (parser.getExitStatus() !=  Parser::Succeded)
    {
        if (bErrCatch)
        {
            out.push_back(new Double(999));
            //to lock last error information
            ConfigVariable::setLastErrorCall();
            ConfigVariable::setLastErrorMessage(parser.getErrorMessage());
            ConfigVariable::setLastErrorNumber(999);
            return Function::OK;
        }
        else
        {
            char* pst = wide_string_to_UTF8(parser.getErrorMessage());
            Scierror(999, "%s", pst);
            FREE(pst);
            return Function::Error;
        }
    }

    if (ConfigVariable::getSerialize())
    {
        ast::Exp* temp = parser.getTree();
        if (ConfigVariable::getTimed())
        {
            pExp = callTyper(temp, L"execstr");
        }
        else
        {
            pExp = callTyper(temp);
        }

        delete temp;
    }
    else
    {
        pExp = parser.getTree();
    }

    if (pExp == NULL)
    {
        return Function::Error;
    }

    //save current prompt mode
    int oldVal = ConfigVariable::getPromptMode();
    ConfigVariable::setPromptMode(-1);

    if (ConfigVariable::getAnalyzerOptions() == 1)
    {
        analysis::AnalysisVisitor analysis;
        pExp->accept(analysis);
        //ast::DebugVisitor debugMe;
        //pExp->accept(debugMe);
    }

    ast::exps_t LExp = pExp->getAs<SeqExp>()->getExps();

    types::ThreadId* pThreadMe = ConfigVariable::getThread(__GetCurrentThreadKey());

    for (ast::exps_t::iterator j = LExp.begin(), itEnd = LExp.end(); j != itEnd; ++j)
    {
        try
        {
            if (pThreadMe && pThreadMe->getInterrupt())
            {
                ThreadManagement::SendAstPendingSignal();
                pThreadMe->suspend();
            }

            // Set the real location of this exp contained in the string of the input of execstr
            // in case where execstr is called in a file.
            (*j)->getLocation().first_line = ConfigVariable::getMacroFirstLines();
            (*j)->getLocation().last_line += (ConfigVariable::getMacroFirstLines() - 1);

            //excecute script
            ExecVisitor execMe;
            (*j)->accept(execMe);

            //to manage call without ()
            if (execMe.getResult() != NULL && execMe.getResult()->isCallable())
            {
                Callable *pCall = execMe.getResult()->getAs<Callable>();
                types::typed_list out;
                types::typed_list in;
                types::optional_list opt;

                try
                {
                    ExecVisitor execCall;
                    Function::ReturnValue Ret = pCall->call(in, opt, 1, out, &execCall);

                    if (Ret == Callable::OK)
                    {
                        if (out.size() == 0)
                        {
                            execMe.setResult(NULL);
                        }
                        else if (out.size() == 1)
                        {
                            out[0]->DecreaseRef();
                            execMe.setResult(out[0]);
                        }
                        else
                        {
                            for (int i = 0; i < static_cast<int>(out.size()); i++)
                            {
                                out[i]->DecreaseRef();
                                execMe.setResult(i, out[i]);
                            }
                        }
                    }
                    else if (Ret == Callable::Error)
                    {
                        throw ast::ScilabMessage(L"", ConfigVariable::getLastErrorNumber(), (*j)->getLocation());
                    }
                }
                catch (ast::ScilabMessage sm)
                {
                    if (ConfigVariable::getLastErrorFunction() == L"")
                    {
                        ConfigVariable::setLastErrorFunction(pCall->getName());
                    }

                    throw sm;
                }
            }

            //update ans variable.
            if (execMe.getResult() != NULL && execMe.getResult()->isDeletable())
            {
                InternalType* pITAns = execMe.getResult();
                symbol::Context::getInstance()->put(symbol::Symbol(L"ans"), pITAns);
                if ((*j)->isVerbose() && bErrCatch == false)
                {
                    std::wostringstream ostr;
                    ostr << L" ans  =" << std::endl;
                    ostr << std::endl;
                    pITAns->toString(ostr);
                    ostr << std::endl;
                    scilabWriteW(ostr.str().c_str());
                }
            }
        }
        catch (ast::ScilabMessage sm)
        {
            ConfigVariable::fillWhereError(sm.GetErrorLocation().first_line);
            if (bErrCatch == false && bMute == false)
            {
                sm.SetErrorLocation((*j)->getLocation());
                throw sm;
            }

            ConfigVariable::resetWhereError();
            iErr = ConfigVariable::getLastErrorNumber();
            break;
        }
        catch (ast::ScilabError se)
        {
            ConfigVariable::fillWhereError(se.GetErrorLocation().first_line);
            // check on error number because error message can be empty.
            if (ConfigVariable::getLastErrorNumber() == 0)
            {
                ConfigVariable::setLastErrorMessage(se.GetErrorMessage());
                ConfigVariable::setLastErrorNumber(se.GetErrorNumber());
                ConfigVariable::setLastErrorLine(se.GetErrorLocation().first_line);
                ConfigVariable::setLastErrorFunction(wstring(L""));
            }

            //store message
            iErr = ConfigVariable::getLastErrorNumber();
            if (bErrCatch == false)
            {
                //restore previous prompt mode
                ConfigVariable::setPromptMode(oldVal);
                se.SetErrorLocation((*j)->getLocation());
                throw se;
            }

            ConfigVariable::resetWhereError();
            break;
        }
    }

    //restore previous prompt mode and silent mode
    ConfigVariable::setPromptMode(oldVal);

    if (bErrCatch)
    {
        out.push_back(new Double(iErr));
        //to lock last error information
        ConfigVariable::setLastErrorCall();
        // allow print
        ConfigVariable::resetError();
    }

    delete pExp;
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
