/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __STATISTICS_GW_HXX__
#define __STATISTICS_GW_HXX__

#ifdef _MSC_VER
	#if STATISTICS_GW_EXPORTS
		#define EXTERN_STATISTICS_GW __declspec (dllexport)
	#else
		#define EXTERN_STATISTICS_GW __declspec (dllimport)
	#endif
#else
	#define EXTERN_STATISTICS_GW
#endif

class StatisticsModule
{
private :
   StatisticsModule(){};
   ~StatisticsModule(){};
public :
   EXTERN_STATISTICS_GW static int Load();
};

#endif /* !__STATISTICS_GW_HXX__ */
