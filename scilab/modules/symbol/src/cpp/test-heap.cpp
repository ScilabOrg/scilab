/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <iostream>
#include <assert.h>
#include "types.hxx"
#include "int.hxx"
#include "float.hxx"
#include "string.hxx"
#include "symbol.hxx"
#include "heap.hxx"

using namespace types;

using symbol::Symbol;
using symbol::Heap;


int heap_test()
{
	int *piVal		= NULL;
	int *piJaune	= NULL;
	double *pReal	= NULL;
	double *pImg	= NULL;
	int *pb1		= NULL;
	float *pfReal	= NULL;
	float *pfImg	= NULL;

	Int32 IntVal(2,2, &piVal);
	Double dblI(2,2, &pReal, &pImg);
    Int32 IntJaune(2,2, &piJaune);
    Bool Bool1(2,2, &pb1);
    Float Float1(2,2, &pfReal, &pfImg);

	piVal[0]		= 2008;
	piVal[1]		= 1492;
	piVal[2]		= 1515;
	piVal[3]		= 1789;

	pb1[0]			= 1;
	pb1[1]			= 0;
	pb1[2]			= 0;
	pb1[3]			= 1;

	pReal[0]		= +1e-05;
	pImg[0]			= +1e20;
	pReal[1]		= +1e-05;
	pImg[1]			= -1e20;
	pReal[2]		= -1e-05;
	pImg[2]			= +1e20;
	pReal[3]		= -1e-05;
	pImg[3]			= -1e20;

	piJaune[0]	= 51;
	piJaune[1]	= 1664;
	piJaune[2]	= 1664;
	piJaune[3]	= 51;

	pfReal[0]		= 0.1f;
	pfReal[1]		= -0.2f;
	pfReal[2]		= 0.3f;
	pfReal[3]		= -0.4f;

	pfImg[0]		= -0.9f;
	pfImg[1]		= 0.8f;
	pfImg[2]		= -0.7f;
	pfImg[3]		= 0.6f;

	String szvalue(L"value");

    String szjaune(2,2);
	szjaune.set(0,0, L"jaune");
	szjaune.set(0,1, L"bleu");
	szjaune.set(1,0, L"rouge");
	szjaune.set(1,1, L"vert");

    Heap tas;
    Symbol foo(L"foo");
    Symbol pourcentI(L"%i");
    Symbol booltest(L"boobool");
    Symbol floattest(L"floattest");
    Symbol bar(L"bar");
    Symbol scicos(L"scicos");

    // foo = 1664
    tas.put(foo, IntVal);
    // bar2 = "value"
    tas.put(bar, szvalue);
    // %i = "%i"
    tas.put(pourcentI, dblI);
    // floattest = "[x.x]"
    tas.put(floattest, Float1);

    // scicos::scicos = 51
    //tas.put(symbol::Symbol(L"scicos"), scicos, IntJaune);
    // scicos::foo = 51
    //tas.put(symbol::Symbol(L"scicos"), foo, szjaune);
    // scicos::boobool = "t,f,f,t"
    //tas.put(symbol::Symbol(L"scicos"), booltest, Bool1);

    std::wcout << L"-----------------" << std::endl;
    std::wcout << L" Heap Overview :" << std::endl;
    std::wcout << L"-----------------" << std::endl;
    std::wcout << tas;
    std::wcout << L"-----------------" << std::endl;

    assert(tas.get(foo) == &IntVal);
    assert(tas.get(bar) == &szvalue);
    //assert(tas.get(symbol::Symbol(L"scicos"), scicos) == &IntJaune);
    //assert(tas.get(symbol::Symbol(L"scicos"), foo) == &szjaune);
    //assert(tas.get(symbol::Symbol(L"scicos"), bar) == NULL);

    return 0;
}

int main(void)
{
    heap_test();
    return 0;
}
