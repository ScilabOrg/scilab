package org.scilab.modules.hdf5.write;
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

import ncsa.hdf.hdf5lib.H5;
import ncsa.hdf.hdf5lib.HDF5Constants;
import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;
import ncsa.hdf.hdf5lib.exceptions.HDF5LibraryException;

import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;


public class H5Write {  

	public static int createFile(String fileName) {
		try {
			return H5.H5Fcreate(fileName,  HDF5Constants.H5F_ACC_TRUNC, HDF5Constants.H5P_DEFAULT, HDF5Constants.H5P_DEFAULT);
		} catch (HDF5LibraryException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return 0;
		} catch (NullPointerException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return 0;
		}
	}

	public static void closeFile(int fileId) {
		try {
			H5.H5Fclose(fileId);
		} catch (HDF5LibraryException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public static void createAttribute(int datasetId, String attributeName, String attributeValue) throws NullPointerException, HDF5Exception {
		int tid = H5.H5Tcopy(HDF5Constants.H5T_C_S1);
		H5.H5Tset_size(tid, attributeValue.length());
		long[] attributeDims = {1};
		int attributespaceId = H5.H5Screate_simple(1, attributeDims, null);
		int attributeId = H5.H5Acreate(datasetId, attributeName, tid, attributespaceId, HDF5Constants.H5P_DEFAULT);
		H5.H5Awrite(attributeId, tid, attributeValue.getBytes());
		H5.H5Aclose(attributeId);
		H5.H5Sclose(attributespaceId);
		H5.H5Tclose(tid);
	}

	public static void createIntAttribute(int datasetId, String attributeName, int attributeValue) throws NullPointerException, HDF5Exception {
		long[] attributeDims = {1};
		int attributespaceId = H5.H5Screate_simple(1, attributeDims, null);
		int attributeId = H5.H5Acreate(datasetId, attributeName, HDF5Constants.H5T_NATIVE_INT, 
			attributespaceId, HDF5Constants.H5P_DEFAULT);
		H5.H5Awrite(attributeId, HDF5Constants.H5T_NATIVE_INT, new Integer[]{attributeValue});
		H5.H5Aclose(attributeId);
		H5.H5Sclose(attributespaceId);
	}

	//    public static void writeInDataSet(int fileId, String dataSetName, int[] data) throws NullPointerException, HDF5Exception {
	//	int size = data.length;
	//
	//	long[] dims = {size};
	//	int dataspaceId = H5.H5Screate_simple(1, dims, null);
	//	int datasetId = H5.H5Dcreate(fileId, "/" + dataSetName,
	//		HDF5Constants.H5T_NATIVE_INT, dataspaceId,
	//		HDF5Constants.H5P_DEFAULT);
	//	H5.H5Dwrite(datasetId, HDF5Constants.H5T_NATIVE_INT,
	//		HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL,
	//		HDF5Constants.H5P_DEFAULT, data);
	//	createAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS, H5ScilabConstant.SCILAB_CLASS_INT32);
	//	H5.H5Dclose(datasetId);
	//	H5.H5Sclose(dataspaceId);
	//    }
	//
	//    public static void writeInDataSet(int fileId, String dataSetName, int[][] data) throws NullPointerException, HDF5Exception {
	//	int rows = data[0].length;
	//	int cols = data.length;
	//
	//	long[] dims = {rows, cols};
	//	int dataspaceId = H5.H5Screate_simple(2, dims, null);
	//	int datasetId = H5.H5Dcreate(fileId, "/" + dataSetName,
	//		HDF5Constants.H5T_NATIVE_INT, dataspaceId,
	//		HDF5Constants.H5P_DEFAULT);
	//	H5.H5Dwrite(datasetId, HDF5Constants.H5T_NATIVE_INT,
	//		HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL,
	//		HDF5Constants.H5P_DEFAULT, data);
	//	createAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS, H5ScilabConstant.SCILAB_CLASS_INT32);
	//	H5.H5Dclose(datasetId);
	//	H5.H5Sclose(dataspaceId);
	//    }

	public static void writeInDataSet(int file_id, String dataSetName, ScilabType data) throws HDF5Exception {
		if(data instanceof ScilabString) {
			H5WriteScilabString.writeInDataSet(file_id, dataSetName, (ScilabString) data); 
		}else if(data instanceof ScilabList) {
			H5WriteScilabList.writeInDataSet(file_id, dataSetName, (ScilabList) data); 
		}else if(data instanceof ScilabTList) {
			H5WriteScilabTList.writeInDataSet(file_id, dataSetName, (ScilabTList) data); 
		}else if(data instanceof ScilabMList) { 
			H5WriteScilabMList.writeInDataSet(file_id, dataSetName, (ScilabMList) data); 
		}else if(data instanceof ScilabDouble) {
			H5WriteScilabDouble.writeInDataSet(file_id, dataSetName, (ScilabDouble) data); 
		}else if(data instanceof ScilabBoolean) {
			H5WriteScilabBoolean.writeInDataSet(file_id, dataSetName, (ScilabBoolean) data); 
		}else if(data instanceof ScilabInteger) { 
			H5WriteScilabInteger.writeInDataSet(file_id, dataSetName, (ScilabInteger) data); 
		}
	}


}
