/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "api_scilab.h"

enum type_check {
  CHECK_NONE, 
  CHECK_SIZE, 
  CHECK_MIN, 
  CHECK_MAX, 
  CHECK_BOTH, 
  CHECK_VALUES};

/**
 * Get a pointer to an existing plist
 * @param[in] iVar position on the stack
 * @param[out] param_addr variable address
 * @return a SciErr structure
 */
SciErr initPList(void* pv_ctx, int iVar, int ** param_addr);

/**
 * Check that a pointer points to an existing plist
 * @param[in] param_addr variable address
 * @param[out] result !0 if it's a plist 0 otherwise
 * @return a SciErr structure
 */
SciErr checkPList(void* pv_ctx, int * param_addr, int * result);

/**
 * Check that a label exists in a plist
 * @param[in] param_addr variable address
 * @param[in] label the label to be checked
 * @param[out] the position of the label in the plist -1 otherwise
 * @return a SciErr structure
 */
SciErr hasLabelInPList(void* pv_ctx, int * param_addr, const char * label, int * result);

/**
 * Check that a label is partially matched in a plist
 * @param[in] param_addr variable address
 * @param[in] label the label to be checked
 * @param[out] the position of the label in the plist -1 otherwise
 * @return a SciErr structure
 */
SciErr hasPartialLabelInPList(void* pv_ctx, int * param_addr, const char * label, int * result);

/**
 * Get a C int associated to a label from a plist
 * @param[in] param_addr variable address
 * @param[in] label the label to be checked
 * @param[out] value the value stored in the plist
 * @param[out] found !-1 if the label has been found -1 otherwise
 * @param[in] default_value the value returned in 'value' if the label has not been found
 * @param[in] log display information messages during type_check
 * @param[in] check a type of checking in the value returned in 'value'
 * @return a SciErr structure
 */
SciErr getIntInPList(void* pv_ctx, int * param_addr, const char * label, int * value, int * found, 
                     int default_value, int log, enum type_check check, ...);

/**
 * Get a C double associated to a label from a plist
 * @param[in] param_addr variable address
 * @param[in] label the label to be checked
 * @param[out] value the value stored in the plist
 * @param[out] found !-1 if the label has been found -1 otherwise
 * @param[in] default_value the value returned in 'value' if the label has not been found
 * @param[in] log display information messages during type_check
 * @param[in] check a type of checking in the value returned in 'value'
 * @return a SciErr structure
 */
SciErr getDoubleInPList(void* pv_ctx, int * param_addr, const char * label, double * value, int * found, 
                        double default_value, int log, enum type_check check, ...);

/**
 * Get a C string associated to a label from a plist
 * @param[in] param_addr variable address
 * @param[in] label the label to be checked
 * @param[out] value the value stored in the plist
 * @param[out] found !-1 if the label has been found -1 otherwise
 * @param[in] default_value the value returned in 'value' if the label has not been found
 * @param[in] log display information messages during type_check
 * @param[in] check a type of checking in the value returned in 'value'
 * @return a SciErr structure
 */
SciErr getStringInPList(void* pv_ctx, int * param_addr, const char * label, char ** value, int * found, 
                        const char * default_value, int log, enum type_check check, ...);

/**
 * Get a C column vector of int associated to a label from a plist
 * @param[in] param_addr variable address
 * @param[in] label the label to be checked
 * @param[out] value the value stored in the plist
 * @param[out] found !-1 if the label has been found -1 otherwise
 * @param[in] default_value the value returned in 'value' if the label has not been found
 * @param[in] default_size the size of the vector of int that must be returned
 * @param[in] log display information messages during type_check
 * @param[in] check a type of checking in the value returned in 'value'
 * @return a SciErr structure
 */
SciErr getColVectorOfIntInPList(void* pv_ctx, int * param_addr, const char * label, int * value, int * found, 
                                int default_value, int default_size, int * size, int Log, enum type_check check, ...);

/**
 * Get a C column vector of double associated to a label from a plist
 * @param[in] param_addr variable address
 * @param[in] label the label to be checked
 * @param[out] value the value stored in the plist
 * @param[out] found !-1 if the label has been found -1 otherwise
 * @param[in] default_value the value returned in 'value' if the label has not been found
 * @param[in] default_size the size of the vector of int that must be returned
 * @param[in] log display information messages during type_check
 * @param[in] check a type of checking in the value returned in 'value'
 * @return a SciErr structure
 */
SciErr getColVectorOfDoubleInPList(void* pv_ctx, int * param_addr, const char * label, double * value, int * found, 
                                   double default_value, int default_size, int * size, int Log, enum type_check check, ...);

/**
 * Create a plist
 * @param[in] iVar the position on the stack of the plist
 * @param[in] param_addr variable address
 * @param[in] label_names a vector of labels for the plist
 * @param[in] nb_params the number of labels
 * @return a SciErr structure
 */
SciErr createPList(void* pv_ctx, int iVar, int ** param_addr, char ** label_names, int nb_params);

/**
 * Store a C int value in a plist and associates it to the given label
 * @param[in] iVar the position on the stack of the plist
 * @param[in] param_addr variable address
 * @param[in] label_name the label to which the value must be associated
 * @param[in] value the value to be stored in the plist
 * @return a SciErr structure
 */
SciErr createIntInPList(void* pv_ctx, int iVar, int * param_addr, char * label_name, int value);

/**
 * Store a C double value in a plist and associates it to the given label
 * @param[in] iVar the position on the stack of the plist
 * @param[in] param_addr variable address
 * @param[in] label_name the label to which the value must be associated
 * @param[in] value the value to be stored in the plist
 * @return a SciErr structure
 */
SciErr createDoubleInPList(void* pv_ctx, int iVar, int * param_addr, char * label_name, double value);

/**
 * Store a C string value in a plist and associates it to the given label
 * @param[in] iVar the position on the stack of the plist
 * @param[in] param_addr variable address
 * @param[in] label_name the label to which the value must be associated
 * @param[in] value the value to be stored in the plist
 * @return a SciErr structure
 */
SciErr createStringInPList(void* pv_ctx, int iVar, int * param_addr, char * label_name, char * value);

/**
 * Store a C column vector of int values in a plist and associates it to the given label
 * @param[in] iVar the position on the stack of the plist
 * @param[in] param_addr variable address
 * @param[in] label_name the label to which the value must be associated
 * @param[in] nb_values the size of the vector to be stored in the plist
 * @param[in] value the value to be stored in the plist
 * @return a SciErr structure
 */
SciErr createColVectorOfIntInPList(void* pv_ctx, int iVar, int * param_addr, char * label_name, int nb_values, int * value);

/**
 * Store a C column vector of double values in a plist and associates it to the given label
 * @param[in] iVar the position on the stack of the plist
 * @param[in] param_addr variable address
 * @param[in] label_name the label to which the value must be associated
 * @param[in] nb_values the size of the vector to be stored in the plist
 * @param[in] value the value to be stored in the plist
 * @return a SciErr structure
 */
SciErr createColVectorOfDoubleInPList(void* pv_ctx, int iVar, int * param_addr, char * label_name, int nb_values, double * value);

#endif
