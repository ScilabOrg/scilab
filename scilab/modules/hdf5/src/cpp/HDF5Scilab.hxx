/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __HDF5SCILAB_HXX__
#define __HDF5SCILAB_HXX__

#include "H5Object.hxx"
#include "H5File.hxx"
#include "H5Dataset.hxx"
#include "H5Data.hxx"

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include "HDF5Objects.h"
#include "sciprint.h"
#include "localization.h"
}

namespace org_modules_hdf5
{

class HDF5Scilab
{

public:

    static int getH5ObjectId(int * mlist, void * pvApiCtx);

    static bool isH5Object(int * mlist, void * pvApiCtx);

    static void scilabPrint(const std::string & str);

    static void readData(const char * filename, const char * name, int pos, void * pvApiCtx);
};
}

#endif // __HDF5SCILAB_HXX__
