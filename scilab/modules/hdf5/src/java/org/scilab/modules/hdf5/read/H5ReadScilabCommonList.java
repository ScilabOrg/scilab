/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

package org.scilab.modules.hdf5.read;

import java.util.ArrayList;

import ncsa.hdf.hdf5lib.H5;
import ncsa.hdf.hdf5lib.HDF5Constants;
import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

public class H5ReadScilabCommonList {
   public static void readData(int dataSetId, ArrayList scilabList) throws NullPointerException, HDF5Exception {
	
	int nbElems = H5Read.getListDim(dataSetId); 
	byte[][] data = new byte[nbElems][8];

	if (H5Read.isEmpty(dataSetId)) {
	    return;
	}
	H5.H5Dread(dataSetId, HDF5Constants.H5T_STD_REF_OBJ,
		HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL, HDF5Constants.H5P_DEFAULT, data);
	
	
	for( int i = 0 ; i < nbElems ; ++i) {
	    int objectId = H5.H5Rdereference(dataSetId, HDF5Constants.H5R_OBJECT, data[i]);
	    Object localData = H5Read.getData(objectId);
	    scilabList.add(localData);
	}
	H5.H5Dclose(dataSetId);
    }
}
