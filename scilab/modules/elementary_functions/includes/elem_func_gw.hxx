/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __ELEM_FUNC_H__
#define __ELEM_FUNC_H__

#include "funcmanager.hxx"
#include "context.hxx"

#ifdef _MSC_VER
	#if ELEM_FUNC_GW_EXPORTS
		#define EXTERN_EF_GW __declspec (dllexport)
	#else
		#define EXTERN_EF_GW __declspec (dllimport)
	#endif
#else
	#define EXTERN_EF_GW 
#endif

class ElemFuncModule
{
private :
	ElemFuncModule(){};
	~ElemFuncModule(){};
public :
	EXTERN_EF_GW static bool Load();
};


Function::ReturnValue sci_isequal(types::typed_list &in, int _iRetCount, types::typed_list &out);
Function::ReturnValue sci_size(types::typed_list &in, int _iRetCount, types::typed_list &out);

#endif /* __ELEM_FUNC_H__ */
