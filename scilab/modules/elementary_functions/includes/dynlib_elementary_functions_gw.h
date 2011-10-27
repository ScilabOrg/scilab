/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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
	#if ELEM_FUNC_GW_EXPORTS
		#define EXTERN_EF_GW __declspec (dllexport)
	#else
		#define EXTERN_EF_GW __declspec (dllimport)
	#endif
#else
	#define EXTERN_EF_GW
#endif
