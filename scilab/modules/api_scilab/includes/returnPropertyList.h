/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _RETURN_PROPERTY_LIST_H_
#define _RETURN_PROPERTY_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dynlib_api_scilab.h"

/**
 * Construct a new tlist for return in the console.
 * Only one tlist should be used at the same time.
 * @param nbElement Number of element of element in the list not including the name.
 * @param name      Array of string of size nbElement containing the names of the list
 *                  and its properties. Its size is nbElement + 1.
 * @return a pointer to the newly created list or NULL if an error occurred.
 */
API_SCILAB_IMPEXP
void* createReturnedList(int nbElements, char* elementName[]);

/**
 * add a new row vector in the list.
 * @param[in/out] void the returned tList
 * @param[in]     vector       values to be inserted of size nbValues
 * @return        0  if the call was successful
 */
API_SCILAB_IMPEXP
int addRowVectorToReturnedList(void* list, const double vector[], int nbValues);

/**
 * add a new column vector in the list.
 * @param[in/out] void the returned tList
 * @param[in]     vector       values to be inserted of size nbValues
 * @return        0  if the call was successful
 */
API_SCILAB_IMPEXP
int addColVectorToReturnedList(void* list, const double vector[], int nbValues);

/**
 * add a new column vector in the list.
 * @param[in/out] void the returned tList
 * @param[in]     matrix   values to be inserted of size nbRow x nbCol
 * @return        0  if the call was successful
 */
API_SCILAB_IMPEXP
int addMatrixToReturnedList(void* list, const double matrix[], int nbRow, int nbCol);

/**
 * add a new string column vector in the list.
 * @param[in/out] void the returned tList
 * @param[in]     vector   values to be inserted of size nbValues
 * @return        0  if the call was successful
 */
API_SCILAB_IMPEXP
int addStringColVectorToReturnedList(void* list, char* vector[], int nbValues);
API_SCILAB_IMPEXP
int addStringToReturnedList(void* list, char * str);
#ifdef __cplusplus
}
#endif
#endif /* _RETURN_PROPERTY_LIST_H_ */
