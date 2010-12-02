/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef PARALLEL_GW_HXX
#define PARALLEL_GW_HXX

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

class ParallelModule
{
private :
	ParallelModule(){};
	~ParallelModule(){};
public :
	EXTERN_EF_GW static bool Load();
};

CPP_GATEWAY_PROTOTYPE(sci_parallel_run);

#endif /* __ELEM_FUNC_H__ */
