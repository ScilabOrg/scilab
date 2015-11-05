/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#ifndef __DYNLIB_COVERAGE_H__
#define __DYNLIB_COVERAGE_H__

#ifdef _MSC_VER
#if COVERAGE_EXPORTS
#define COVERAGE_IMPEXP __declspec (dllexport)
#else
#define COVERAGE_IMPEXP __declspec (dllimport)
#endif
#else
#if __GNUC__ >= 4
#define COVERAGE_IMPEXP __attribute__ ((visibility ("default")))
#else
#define COVERAGE_IMPEXP
#endif
#endif

#endif /* __DYNLIB_COVERAGE_H__ */
/*--------------------------------------------------------------------------*/
