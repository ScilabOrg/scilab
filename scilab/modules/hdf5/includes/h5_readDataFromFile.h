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

#ifndef __H5_READDATATOFILE_H__
#define __H5_READDATATOFILE_H__

#include <hdf5.h>
#include "dynlib_hdf5_scilab.h"


#define unknow_type     -2
#define sci_undefined   -1
#define sci_void        0

HDF5_SCILAB_IMPEXP int isComplexData(int _iDatasetId);

HDF5_SCILAB_IMPEXP int getVariableNames(int _iFile, char **pstNameList);

HDF5_SCILAB_IMPEXP int getDataSetId(int  _iFile);

HDF5_SCILAB_IMPEXP int getDataSetIdFromName(int _iFile, char *_pstName);

HDF5_SCILAB_IMPEXP int getDatasetDims(int _iDatasetId, int *_piRows, int *_piCols);
HDF5_SCILAB_IMPEXP int getListDims(int _iDatasetId, int *_piItem);

HDF5_SCILAB_IMPEXP int getScilabTypeFromDataSet(int _iDatasetId);

HDF5_SCILAB_IMPEXP int getDatasetPrecision(int _iDatasetId, int* _piPrec);

HDF5_SCILAB_IMPEXP int getSparseDimension(int _iDatasetId, int* _piRows, int * _piCols, int* _piNbItem);

HDF5_SCILAB_IMPEXP int readEmptyMatrix(int _iDatasetId);

HDF5_SCILAB_IMPEXP int readDoubleMatrix(int _iDatasetId, int _iRows, int _iCols, double *_pdblData);
HDF5_SCILAB_IMPEXP int readDoubleComplexMatrix(int _iDatasetId, int _iRows, int _iCols, double *_pdblReal, double *_pdblImg);

HDF5_SCILAB_IMPEXP int readStringMatrix(int _iDatasetId, int _iRows, int _iCols, char **_pstData);

HDF5_SCILAB_IMPEXP int readBooleanMatrix(int _iDatasetId, int _iRows, int _iCols, int* _piData);

HDF5_SCILAB_IMPEXP int readPolyMatrix(int _iDatasetId, char* _pstVarname, int _iRows, int _iCols, int* _piNbCoef, double **_pdblData);
HDF5_SCILAB_IMPEXP int readPolyComplexMatrix(int _iDatasetId, char* _pstVarname, int _iRows, int _iCols, int* _piNbCoef, double **_pdblReal, double **_pdblImg);

HDF5_SCILAB_IMPEXP int readInterger8Matrix(int _iDatasetId, int _iRows, int _iCols, char* _pcData);
HDF5_SCILAB_IMPEXP int readInterger16Matrix(int _iDatasetId, int _iRows, int _iCols, short* _psData);
HDF5_SCILAB_IMPEXP int readInterger32Matrix(int _iDatasetId, int _iRows, int _iCols, int* _piData);
HDF5_SCILAB_IMPEXP int readInterger64Matrix(int _iDatasetId, int _iRows, int _iCols, long long* _pllData);

HDF5_SCILAB_IMPEXP int readUnsignedInterger8Matrix(int _iDatasetId, int _iRows, int _iCols, unsigned char* _pucData);
HDF5_SCILAB_IMPEXP int readUnsignedInterger16Matrix(int _iDatasetId, int _iRows, int _iCols, unsigned short* _pusData);
HDF5_SCILAB_IMPEXP int readUnsignedInterger32Matrix(int _iDatasetId, int _iRows, int _iCols, unsigned int* _puiData);
HDF5_SCILAB_IMPEXP int readUnsignedInterger64Matrix(int _iDatasetId, int _iRows, int _iCols, unsigned long long* _pullData);

HDF5_SCILAB_IMPEXP int readSparseComplexMatrix(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow,	int* _piColPos, double *_pdblReal, double *_pdblImg);
HDF5_SCILAB_IMPEXP int readSparseMatrix(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow,	int* _piColPos, double *_pdblReal);

HDF5_SCILAB_IMPEXP int readBooleanSparseMatrix(int _iDatasetId, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow,	int* _piColPos);

HDF5_SCILAB_IMPEXP int getListItemReferences(int _iDatasetId, hobj_ref_t** _piItemRef);

HDF5_SCILAB_IMPEXP int getListItemDataset(int _iDatasetId, void* _piItemRef, int _iItemPos, int* _piItemDataset);

HDF5_SCILAB_IMPEXP int deleteListItemReferences(int _iDatasetId, void* _piItemRef);

HDF5_SCILAB_IMPEXP  void reset_item_count();
#endif /* !__H5_READDATATOFILE_H__ */
