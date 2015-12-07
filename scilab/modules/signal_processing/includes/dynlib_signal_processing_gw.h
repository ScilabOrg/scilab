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
#ifndef __DYNLIB_SIGNAL_PROCESSING_GW_H__
#define __DYNLIB_SIGNAL_PROCESSING_GW_H__

#ifdef _MSC_VER
#if SIGNAL_PROCESSING_GW_EXPORTS
#define EXTERN_SIGNAL_PROCESSING_GW __declspec (dllexport)
#else
#define EXTERN_SIGNAL_PROCESSING_GW __declspec (dllimport)
#endif
#else
#if __GNUC__ >= 4
#define EXTERN_SIGNAL_PROCESSING_GW __attribute__ ((visibility ("default")))
#else
#define EXTERN_SIGNAL_PROCESSING_GW
#endif
#endif

#endif /* __DYNLIB_SIGNAL_PROCESSING_GW_H__ */
/*--------------------------------------------------------------------------*/

