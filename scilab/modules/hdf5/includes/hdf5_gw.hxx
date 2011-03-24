/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __HDF5_GW_HXX__
#define __HDF5_GW_HXX__

#include "funcmanager.hxx"
#include "context.hxx"

#ifdef _MSC_VER
	#if HDF5_GW_EXPORTS
		#define EXTERN_HDF5_GW __declspec (dllexport)
	#else
		#define EXTERN_HDF5_GW __declspec (dllimport)
	#endif
#else
	#define EXTERN_HDF5_GW
#endif

class Hdf5Module
{
private :
    Hdf5Module() {};
    ~Hdf5Module() {};

public :
	EXTERN_HDF5_GW static bool Load();
};

CPP_GATEWAY_PROTOTYPE(sci_export_to_hdf5);
CPP_GATEWAY_PROTOTYPE(sci_import_from_hdf5);

#endif /* !__FILEIO_GW_HXX__ */
