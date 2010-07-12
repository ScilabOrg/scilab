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
#include "parser.hxx"
#include "funcmanager.hxx"
#include "context.hxx"
#include "functions_gw.hxx"
#include "setenvvar.hxx"
#include "execvisitor.hxx"
#include "mutevisitor.hxx"
#include "yaspio.hxx"

#include <iostream>
#include <fstream>
#include <string>

extern "C"
{
#include "MALLOC.h"

#ifndef _MSC_VER
#include "stricmp.h"
#endif
#include "cluni0.h"
#include "PATH_MAX.h"
#include "prompt.h"
#include "stricmp.h"
}


using namespace std;
using namespace types;
using namespace ast;
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_execstr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	bool bErrCatch		= false;
	wchar_t* pstMsg     = NULL;
	Exp* pExp           = NULL;
	wchar_t *pstCommand = NULL;
    Parser parser;

	if(in.size() < 1 || in.size() > 3)
	{
		return Function::Error;
	}

	if(in.size() > 1)
	{
		if(in[1]->getType() == InternalType::RealString)
		{//errcatch
			String* pS = in[1]->getAsString();
			if(pS->size_get() != 1)
			{
				return Function::Error;
			}

			if(wcsicmp(pS->string_get(0), L"errcatch") == 0)
			{
				bErrCatch = true;
			}
			else
			{
				wchar_t stErr[1024];
#ifdef _MSC_VER
				swprintf_s(stErr, 1024, L"\"%s\" value is not a valid value for exec function", pS->string_get(0));
#else
				swprintf(stErr, 1024, L"\"%s\" value is not a valid value for exec function", pS->string_get(0));
#endif
				YaspWriteW(stErr);
				return Function::Error;
			}
		}
		else
		{//not managed
			YaspWriteW(L"Bad 2nd parameter type in execstr call");
			return Function::Error;
		}

		//3rd parameter
		if(in.size() == 3)
		{
			if(in[2]->getType() == InternalType::RealString)
			{
				String* pS = in[1]->getAsString();
				if(pS->size_get() != 1)
				{
					return Function::Error;
				}

				pstMsg = pS->string_get(0);
			}
			else
			{//not managed
				YaspWriteW(L"Bad 3rd parameter type in execstr call");
				return Function::Error;
			}
		}
	}

	if(in[0]->getType() == InternalType::RealString)
	{//1st argument is a scilab source code, parse and execute it
		String* pS = in[0]->getAsString();

		//must be a vector
		if(pS->rows_get() != 1 && pS->cols_get() != 1)
		{
			return Function::Error;
		}

		int iTotalLen = pS->size_get(); //add \n after each string
		for(int i = 0 ; i < pS->size_get() ; i++)
		{
			iTotalLen += (int)wcslen(pS->string_get(i));
		}

		pstCommand = (wchar_t*)MALLOC(sizeof(wchar_t) * (iTotalLen + 1));//+1 for null termination

		int iPos = 0;
		for(int i = 0 ; i < pS->size_get() ; i++)
		{
			wcscpy(pstCommand + iPos, pS->string_get(i));
			iPos = (int)wcslen(pstCommand);
			pstCommand[iPos++] = L'\n';
			pstCommand[iPos] = 0;
		}

		parser.parse(pstCommand);
		if(parser.getExitStatus() !=  Parser::Succeded)
		{
			YaspWriteW(parser.getErrorMessage());
			FREE(pstCommand);
			return Function::Error;
		}

		pExp = parser.getTree();
	}
	else
	{//not managed
		YaspWrite("Bad 1st parameter type in execstr call");
		return Function::Error;
	}

	if(pExp == NULL)
	{
		return Function::Error;
	}

	MuteVisitor mute;
	pExp->accept(mute);

	std::list<Exp *>::iterator j;
	std::list<Exp *>LExp = ((SeqExp*)pExp)->exps_get();

	for(j = LExp.begin() ; j != LExp.end() ; j++)
	{
		try
		{
			//excecute script
			ExecVisitor execMe;
			(*j)->accept(execMe);
		}
		catch(std::wstring st)
		{
			//print error
			YaspWriteW(pstCommand);
			YaspWriteW(L"\n");
			YaspWriteW(st.c_str());
			return Function::Error;
		}
	}

	parser.freeTree();
	FREE(pstCommand);
	return Function::OK;
}
/*--------------------------------------------------------------------------*/
