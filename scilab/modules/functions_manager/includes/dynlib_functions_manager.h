/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#if FUNC_MAN_EXPORTS
#define FUNCMAN_IMEXP __declspec (dllexport)
#else
#define FUNCMAN_IMEXP __declspec (dllimport)
#endif
#else
#if __GNUC__ >= 4
#define FUNCMAN_IMEXP __attribute__ ((visibility ("default")))
#else
#define FUNCMAN_IMEXP
#endif
#endif
