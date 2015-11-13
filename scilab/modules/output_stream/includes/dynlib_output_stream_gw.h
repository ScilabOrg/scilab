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
#ifndef __DYNLIB_OUTPUT_STREAM_GW_H__
#define __DYNLIB_OUTPUT_STREAM_GW_H__

#ifdef _MSC_VER
#ifdef OUTPUT_STREAM_GW_EXPORTS
#define OUTPUT_STREAM_IMPEXP_GW __declspec(dllexport)
#else
#define OUTPUT_STREAM_IMPEXP_GW __declspec(dllimport)
#endif
#else
#if __GNUC__ >= 4
#define OUTPUT_STREAM_IMPEXP_GW __attribute__ ((visibility ("default")))
#else
#define OUTPUT_STREAM_IMPEXP_GW
#endif
#endif

#endif /* __DYNLIB_OUTPUT_STREAM_GW_H__ */
/*--------------------------------------------------------------------------*/
