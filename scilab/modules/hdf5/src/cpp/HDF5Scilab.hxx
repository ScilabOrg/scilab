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
#include "localization.h"
}

namespace org_modules_hdf5
{

    class HDF5Scilab
    {

    public:

        static int getH5ObjectId(int * mlist, void * pvApiCtx)
            {
                int * id = 0;
                int row, col;

                SciErr err = getMatrixOfInteger32InList(pvApiCtx, mlist, 2, &row, &col, &id);

                if (err.iErr)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot get H5Object id"));
                }

                return *id;
            }

        static bool isH5Object(int * mlist, void * pvApiCtx)
            {
                char * mlist_type[2];
                int type;
                int rows, cols;
                int lengths[2];

                SciErr err = getVarType(pvApiCtx, mlist, &type);

                if (err.iErr || type != sci_mlist)
                {
                    return false;
                }

                err = getMatrixOfStringInList(pvApiCtx, mlist, 1, &rows, &cols, 0, 0);
                if (err.iErr || rows != 1 || cols != 2)
                {
                    return false;
                }

                err = getMatrixOfStringInList(pvApiCtx, mlist, 1, &rows, &cols, lengths, 0);
                if (err.iErr)
                {
                    return false;
                }

                for (int i = 0; i < 2; i++)
                {
                    mlist_type[i] = new char[lengths[i] + 1];
                }

                err = getMatrixOfStringInList(pvApiCtx, mlist, 1, &rows, &cols, lengths, mlist_type);
                if (err.iErr)
                {
                    return false;
                }

		bool ret = mlist_type[0] == __SCILAB_MLIST_H5OBJECT__ && mlist_type[1] == "_id";

		for (int i = 0; i < 2; i++)
                {
                    delete[] mlist_type[i];
                }

		return ret;
            }

	static void readData(const char * filename, const char * name, int pos, void * pvApiCtx)
	    {
		hid_t file;
		hid_t obj;
		herr_t err;
		H5O_info_t info;
		H5File * h5file = 0;
		H5Dataset * dataset = 0;
		H5Data * data = 0;
		
		if (!H5Fis_hdf5(filename))
		{
		    throw H5Exception(__LINE__, __FILE__, _("Invalid hdf5 file: %s"), filename);
		}
		
		file = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
		if (file < 0)
		{
		    throw H5Exception(__LINE__, __FILE__, _("Cannot open the given hdf5 file: %s"), filename);
		}

		if (H5Lexists(file, name, H5P_DEFAULT) <= 0)
		{
		    H5Fclose(file);
		    throw H5Exception(__LINE__, __FILE__, _("Invalid path: %s"), name);
		} 

		obj = H5Oopen(file, name, H5P_DEFAULT);
		if (obj < 0)
		{
		    H5Fclose(file);
		    throw H5Exception(__LINE__, __FILE__, _("Cannot retrieve object: %s"), name);
		}

		err = H5Oget_info(obj, &info);
		if (err < 0)
		{
		    H5Fclose(file);
		    H5Oclose(obj);
		    throw H5Exception(__LINE__, __FILE__, _("Cannot retrieve object information: %s"), name);
		}

		if (info.type != H5O_TYPE_DATASET)
		{
		    H5Fclose(file);
		    H5Oclose(obj);
		    throw H5Exception(__LINE__, __FILE__, _("Not a dataset: %s"), name);
		}

		H5Fclose(file);
		H5Oclose(obj);

		try
		{
		    h5file = new H5File(strdup(filename), "/", true);
		    dataset = new H5Dataset(*h5file, strdup(name));
		}
		catch (H5Exception & e)
		{
		    if (h5file)
		    {
			delete h5file;
		    }
		    throw;
		}

		try
		{
		    data = &(dataset->getData());
		    data->toScilab(pvApiCtx, pos);
		}
		catch (H5Exception & e)
		{
		    if (h5file)
		    {
			delete h5file;
		    }
		    throw;
		}

		if (h5file)
		{
		    delete h5file;
		}	    
	    }
    };
}

#endif // __HDF5SCILAB_HXX__
