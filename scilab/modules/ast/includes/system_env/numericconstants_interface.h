/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "dynlib_ast.h"
#include "machine.h"

AST_IMPEXP double nc_eps();
AST_IMPEXP double nc_safe_min();
AST_IMPEXP double nc_base();
AST_IMPEXP double nc_eps_machine();
AST_IMPEXP double nc_num_mantissa_digits();
AST_IMPEXP double nc_rounded();
AST_IMPEXP double nc_exp_min();
AST_IMPEXP double nc_double_min();
AST_IMPEXP double nc_exp_max();
AST_IMPEXP double nc_double_max();

//Fortran
AST_IMPEXP double C2F(nc_eps)();
AST_IMPEXP double C2F(nc_safe_min)();
AST_IMPEXP double C2F(nc_base)();
AST_IMPEXP double C2F(nc_eps_machine)();
AST_IMPEXP double C2F(nc_num_mantissa_digits)();
AST_IMPEXP double C2F(nc_rounded)();
AST_IMPEXP double C2F(nc_exp_min)();
AST_IMPEXP double C2F(nc_double_min)();
AST_IMPEXP double C2F(nc_exp_max)();
AST_IMPEXP double C2F(nc_double_max)();
