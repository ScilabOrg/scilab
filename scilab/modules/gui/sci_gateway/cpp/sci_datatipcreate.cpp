/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DatatipCreate.hxx"
#include "ScilabView.hxx"

extern "C"
{
#include "stack-c.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
#include "BOOL.h"
#include "MALLOC.h"
}

using namespace org_scilab_modules_gui_datatip;

int sci_datatipcreate(char *fname, unsigned long fname_len)
{
    char const * figureUid;
    char const * datatip_handle;

    CheckLhs(0, 1);
    CheckRhs(3, 3);

	int tamanho1, tamanho2;
	int endereco_fig_num;
	int endereco_posx;
	int endereco_posy;

	GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &tamanho1, &tamanho2, &endereco_fig_num);
	if (tamanho1 != 1 && tamanho2 != 1)
	{
            //error
	}

	GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &tamanho1, &tamanho2, &endereco_posx);
	if (tamanho1 != 1 && tamanho2 != 1)
	{
            //error
	}


	GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &tamanho1, &tamanho2, &endereco_posx);
	if (tamanho1 != 1 && tamanho2 != 1)
	{
            //error
	}

    try
    {

		int fig_num = *istk(endereco_fig_num);
		int posx = *istk(endereco_posx);
		int posy = *istk(endereco_posy);
        figureUid = ScilabView::getFigureFromIndex(fig_num);

        if (Rhs == 3)
        {

            datatip_handle = DatatipCreate::createDatatip(getScilabJavaVM(), figureUid, posx, posy);
        }
    }

    //return datatip_handle;


	int *datatip_handle_ptr = new int[1];

	(*datatip_handle_ptr) = datatip_handle;


	int m = 1, n = 1;

    CreateVarFromPtr(Rhs + 1, MATRIX_OF_INTEGER_DATATYPE, &m, &n, &datatip_handle_ptr);


	LhsVar(1) = Rhs + 1;
    PutLhsVar();

	//delete datatip_handle_ptr;

	return TRUE;

}
