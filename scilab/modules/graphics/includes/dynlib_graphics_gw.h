/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) - 2012 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#ifndef __DYNLIB_GRAPHICS_GW_H__
#define __DYNLIB_GRAPHICS_GW_H__

#ifdef _MSC_VER
#ifdef GRAPHICS_GW_EXPORTS
#define GRAPHICS_GW_IMPEXP __declspec(dllexport)
#else
#define GRAPHICS_GW_IMPEXP __declspec(dllimport)
#endif
#else
#if __GNUC__ >= 4
#define GRAPHICS_GW_IMPEXP __attribute__ ((visibility ("default")))
#else
#define GRAPHICS_GW_IMPEXP
#endif
#endif

#endif /* __DYNLIB_GUI_GW_H__ */
/*--------------------------------------------------------------------------*/

