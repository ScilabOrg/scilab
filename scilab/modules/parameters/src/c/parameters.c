/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Yann COLLETTE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/

#include <stdarg.h>

#include "parameters.h"

#include "stack-c.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "Scierror.h"
#include "sciprint.h"

#include "api_scilab.h"
#include "localization.h"

int commonFindLabel(void* pv_ctx, int * param_addr, char const * const label_to_find);
int commonFindLabelPartial(void* pv_ctx, int * param_addr, char const * const label_to_find);

SciErr initPList(void* pv_ctx, int iVar, int ** param_addr)
{
  SciErr _SciErr; _SciErr.iErr = 0; _SciErr.iMsgCount = 0;

  _SciErr = getVarAddressFromPosition(pv_ctx, iVar, param_addr);

  return _SciErr;
}

SciErr checkPList(void* pv_ctx, int * param_addr, int * result)
{
  int nb_param = 0, i, var_type;
  int m_label, n_label;
  int * len_label = NULL;
  char ** label_list;
  SciErr _SciErr; _SciErr.iErr = 0; _SciErr.iMsgCount = 0;

  *result = 0;

  _SciErr = getVarType(pv_ctx, param_addr, &var_type);
  if (var_type!=sci_mlist) {
    addErrorMessage(&_SciErr, API_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s expected"), "checkPList", "plist");
    return _SciErr;
  }

  _SciErr = getListItemNumber(pv_ctx, param_addr, &nb_param);

  if (nb_param!=0) {
    _SciErr = getMatrixOfStringInList(pv_ctx, param_addr, 1, &m_label, &n_label, NULL, NULL);
    len_label = (int *)MALLOC(m_label*n_label*sizeof(int));
    _SciErr = getMatrixOfStringInList(pv_ctx, param_addr, 1, &m_label, &n_label, len_label, NULL);
    label_list = (char **)MALLOC(m_label*n_label*sizeof(char *));
    for(i=0; i<n_label*m_label; i++) {
      label_list[i] = (char *)MALLOC((len_label[i]+1)*sizeof(char));
    }
    _SciErr = getMatrixOfStringInList(pv_ctx, param_addr, 1, &m_label, &n_label, len_label, label_list);
    if (strcmp(label_list[0],"plist") != 0) {
      if (len_label) FREE(len_label);
      freeArrayOfString(label_list, m_label*n_label);
      addErrorMessage(&_SciErr, API_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s expected"), "checkPList", "plist");
      return _SciErr;
    }
    
    if (len_label) FREE(len_label);
    freeArrayOfString(label_list, m_label*n_label);
  }
  
  *result = 1;

  return _SciErr;
}

SciErr hasPartialLabelInPList(void* pv_ctx, int * param_addr, const char * label, int * result)
{
  SciErr _SciErr;
  *result = commonFindLabelPartial(pv_ctx, param_addr, label);
  return _SciErr;
}

SciErr hasLabelInPList(void* pv_ctx, int * param_addr, const char * label, int * result)
{
  SciErr _SciErr;
  *result = commonFindLabel(pv_ctx, param_addr, label);
  return _SciErr;
}

SciErr getIntInPList(void* pv_ctx, int * param_addr, const char * label, int * value, int * found, 
                     int default_value, int Log, enum type_check check, ...)
{
  int pos_label, i;
  int m_tmp, n_tmp;
  double * tmp_dbl;
  SciErr _SciErr; _SciErr.iErr = 0; _SciErr.iMsgCount = 0;

  pos_label = commonFindLabel(pv_ctx, param_addr, label);
  *found = pos_label;

  if (pos_label!=-1) {
    _SciErr = getMatrixOfDoubleInList(pv_ctx, param_addr, pos_label+1, &m_tmp, &n_tmp, &tmp_dbl);
    if (!_SciErr.iErr) {
      *value = (int)tmp_dbl[0];
    } else {
      if (Log) {
        sciprint(_("%s: wrong parameter type. %s expected. Return default value %d.\n"), "getIntInPList","int",default_value);
      }
      *value = default_value;
    }
  } else {
    if (Log) {
      sciprint(_("%s: parameter not found. Return default value %d.\n"), "getIntInPList",default_value);
    }
    *value = default_value;
  }
  
  /* Now check parameters */
  
  if (check!=CHECK_NONE) {
    va_list vl;
    va_start(vl, check);
    int nb_value_to_check;
    int value_to_check;
    int check_res = 0;
    
    switch(check) {
    case CHECK_MIN:
      value_to_check = va_arg(vl,int);
      va_end(vl);
      if (value_to_check>*value) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong min bound for parameter %s: min bound %d, value %d\n"), "getIntInPList", label, value_to_check, *value);
        }
        *value = default_value;
	addErrorMessage(&_SciErr, 999, _("%s: wrong min bound for parameter %s: min bound %d, value %d\n"), "getIntInPList", label, value_to_check, *value);
        return _SciErr;
      }
      break;
    case CHECK_MAX:
      value_to_check = va_arg(vl,int);
      va_end(vl);
      if (value_to_check<*value) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong max bound for parameter %s: max bound %d, value %d\n"), "getIntInPList", label, value_to_check, *value);
        }
        *value = default_value;
	addErrorMessage(&_SciErr, 999, _("%s: wrong max bound for parameter %s: max bound %d, value %d\n"), "getIntInPList", label, value_to_check, *value);
        return _SciErr;
      }
      break;
    case CHECK_BOTH:
      // First value is the min bound
      value_to_check = va_arg(vl,int);
      if (value_to_check>*value) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong min bound for parameter %s: min bound %d, value %d\n"), "getIntInPList", label, value_to_check, *value);
        }
        *value = default_value;
        va_end(vl);
	addErrorMessage(&_SciErr, 999, _("%s: wrong min bound for parameter %s: min bound %d, value %d\n"), "getIntInPList", label, value_to_check, *value);
        return _SciErr;
      }
      // Second value is the max bound
      value_to_check = va_arg(vl,int);
      va_end(vl);
      if (value_to_check<*value) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong max bound for parameter %s: max bound %d, value %d\n"), "getIntInPList", label, value_to_check, *value);
        }
        *value = default_value;
	addErrorMessage(&_SciErr, 999, _("%s: wrong max bound for parameter %s: max bound %d, value %d\n"), "getIntInPList", label, value_to_check, *value);
        return _SciErr;
      }
      break;
    case CHECK_VALUES:
      // First parameters is int and contains the number of values to check
      nb_value_to_check = va_arg(vl,int);
      check_res = 0;
      for(i=0;i<nb_value_to_check; i++) {
        value_to_check = va_arg(vl,int);
        check_res = check_res || (value_to_check==*value);
      }
      
      if (!check_res) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong value for parameter %s: value %d\n"), "getIntInPList", label, *value);
          sciprint(_("%s: awaited parameters: "), "getIntInPList");
          va_start(vl, check);
          nb_value_to_check = va_arg(vl,int);
          for(i=0;i<nb_value_to_check; i++) {
            value_to_check = va_arg(vl,int);
            sciprint(" %d", value_to_check);
          }
          sciprint("\n");
        }
        
        *value = default_value;
        
        va_end(vl);
	addErrorMessage(&_SciErr, 999, _("%s: wrong value for parameter %s: value %d\n"), "getIntInPList", label, *value);
        return _SciErr;
      }
      
      va_end(vl);
      break;
    }
  }
  
  return _SciErr;
}

SciErr getDoubleInPList(void* pv_ctx, int * param_addr, const char * label, double * value, int * found, 
                        double default_value, int Log, enum type_check check, ...)
{
  int pos_label, i;
  int m_tmp, n_tmp;
  double * tmp_values = NULL;
  SciErr _SciErr; _SciErr.iErr = 0; _SciErr.iMsgCount = 0;

  pos_label = commonFindLabel(pv_ctx, param_addr, label);
  *found = pos_label;

  if (pos_label!=-1) {
    _SciErr = getMatrixOfDoubleInList(pv_ctx, param_addr, pos_label+1, &m_tmp, &n_tmp, &tmp_values);
    if (!_SciErr.iErr) {
      *value = tmp_values[0];
    } else {
      if (Log) {
        sciprint(_("%s: wrong parameter type. %s expected. Return default value %f.\n"), "getDoubleInPList","double",default_value);
      }
      *value = default_value;
    }
  } else {
    if (Log) {
      sciprint(_("%s: parameter not found. Return default value %f.\n"), "getDoubleInPList",default_value);
    }
    *value = default_value;
  }
  
  /* Now check parameters */
  
  if (check!=CHECK_NONE) {
    va_list vl;
    va_start(vl, check);
    int nb_value_to_check;
    double value_to_check;
    int check_res = 0;
    
    switch(check) {
    case CHECK_MIN:
      value_to_check = va_arg(vl,double);
      va_end(vl);
      if (value_to_check>*value) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong min bound for parameter %s: min bound %f, value %f\n"), "getDoubleInPList", label, value_to_check, *value);
        }
        *value = default_value;
	addErrorMessage(&_SciErr, 999, _("%s: wrong min bound for parameter %s: min bound %f, value %f\n"), "getDoubleInPList", label, value_to_check, *value);
        return _SciErr;
      }
      break;
    case CHECK_MAX:
      value_to_check = va_arg(vl,double);
      va_end(vl);
      if (value_to_check<*value) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong max bound for parameter %s: max bound %f, value %f\n"), "getDoubleInPList", label, value_to_check, *value);
        }
        *value = default_value;
	addErrorMessage(&_SciErr, 999, _("%s: wrong max bound for parameter %s: max bound %f, value %f\n"), "getDoubleInPList", label, value_to_check, *value);
        return _SciErr;
      }
      break;
    case CHECK_BOTH:
      /* First value is the min bound */
      value_to_check = va_arg(vl,double);
      if (value_to_check>*value) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong min bound for parameter %s: min bound %f, value %f\n"), "getDoubleInPList", label, value_to_check, *value);
        }
        *value = default_value;
        va_end(vl);
	addErrorMessage(&_SciErr, 999, _("%s: wrong min bound for parameter %s: min bound %f, value %f\n"), "getDoubleInPList", label, value_to_check, *value);
        return _SciErr;
      }
      /* Second value is the max bound */
      value_to_check = va_arg(vl,double);
      va_end(vl);
      if (value_to_check<*value) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong max bound for parameter %s: max bound %f, value %f\n"), "getDoubleInPList", label, value_to_check, *value);
        }
        *value = default_value;
	addErrorMessage(&_SciErr, 999, _("%s: wrong max bound for parameter %s: max bound %f, value %f\n"), "getDoubleInPList", label, value_to_check, *value);
        return _SciErr;
      }
      break;
    case CHECK_VALUES:
      /* First parameters is int and contains the number of values to check */
      nb_value_to_check = va_arg(vl,int);
      check_res = 0;
      for(i=0;i<nb_value_to_check; i++) {
        value_to_check = va_arg(vl,double);
        check_res = check_res || (value_to_check==*value);
      }
      
      if (!check_res) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong value for parameter %s: value %f\n"), "getDoubleInPList", label, *value);
          sciprint(_("%s: awaited parameters: "), "getDoubleInPList");
          va_start(vl, check);
          nb_value_to_check = va_arg(vl,int);
          for(i=0;i<nb_value_to_check; i++) {
            value_to_check = va_arg(vl,double);
            sciprint(" %f", value_to_check);
          }
          sciprint("\n");
        }
        
        *value = default_value;
        
        va_end(vl);
	addErrorMessage(&_SciErr, 999, _("%s: wrong value for parameter %s: value %f\n"), "getDoubleInPList", label, *value);
        return _SciErr;
      }
      
      va_end(vl);
      break;
    }
  }
  
  return _SciErr;
}

SciErr getStringInPList(void* pv_ctx, int * param_addr, const char * label, char ** value, int * found, 
                        const char * default_value, int Log, enum type_check check, ...)
{
  int pos_label, i;
  int m_label, n_label;
  int * len_label = NULL;
  char ** label_list;
  SciErr _SciErr; _SciErr.iErr = 0; _SciErr.iMsgCount = 0;

  pos_label = commonFindLabel(pv_ctx, param_addr, label);
  *found = pos_label;

  if (pos_label!=-1) {
    _SciErr = getMatrixOfStringInList(pv_ctx, param_addr, pos_label+1, &m_label, &n_label, NULL, NULL);
    len_label = (int *)MALLOC(m_label*n_label*sizeof(int));
    _SciErr = getMatrixOfStringInList(pv_ctx, param_addr, pos_label+1, &m_label, &n_label, len_label, NULL);
    label_list = (char **)MALLOC(m_label*n_label*sizeof(char *));
    for(i=0; i<n_label*m_label; i++) {
      label_list[i] = (char *)MALLOC((len_label[i]+1)*sizeof(char));
    }
    _SciErr = getMatrixOfStringInList(pv_ctx, param_addr, pos_label+1, &m_label, &n_label, len_label, label_list);
    
    if (!_SciErr.iErr) {
      if (label_list[0]) {
        *value = strdup(label_list[0]);
      } else {
        if (Log) {
          sciprint(_("%s: wrong parameter type. %s expected. Return default value %s.\n"), "getStringInPList","string",default_value);
        }
        *value = strdup(default_value);
      }
    } else {
      if (Log) {
        sciprint(_("%s: parameter not found. Return default value %s.\n"), "getStringInPList",default_value);
      }
      *value = strdup(default_value);
    }
    
    if (len_label) FREE(len_label);
    freeArrayOfString(label_list, m_label*n_label);
  } else {
    *value = strdup(default_value);
  }
  
  /* Now check parameters */
  
  if (check!=CHECK_NONE) {
    va_list vl;
    va_start(vl, check);
    int nb_value_to_check;
    char * value_to_check;
    int check_res = 0;
    
    switch(check) {
    case CHECK_VALUES:
      /* First parameters is int and contains the number of values to check */
      nb_value_to_check = va_arg(vl,int);
      check_res = 0;
      for(i=0;i<nb_value_to_check; i++) {
        value_to_check = va_arg(vl,char *);
        check_res = check_res || (strcmp(value_to_check,*value)==0);
      }

      if (!check_res) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong value for parameter %s: value %s\n"), "getStringInPList", label, *value);
          sciprint(_("%s: awaited parameters: "), "getStringInPList");
          va_start(vl, check);
          nb_value_to_check = va_arg(vl,int);
          for(i=0;i<nb_value_to_check; i++) {
            value_to_check = va_arg(vl,char *);
            sciprint(" \"%s\"", value_to_check);
          }
          sciprint("\n");
        }
        
        FREE(*value);
        *value = strdup(default_value);
        
        va_end(vl);
	addErrorMessage(&_SciErr, 999, _("%s: wrong value for parameter %s: value %s\n"), "getStringInPList", label, *value);
        return _SciErr;
      }
      
      va_end(vl);
      break;
    }
  }
  
  return _SciErr;
}

/* get vector of double / integers */

SciErr getColVectorOfIntInPList(void* pv_ctx, int * param_addr, const char * label, int * value, int * found, 
                                int default_value, int default_size, int * size, int Log, enum type_check check, ...)
{
  int pos_label, i;
  int m_tmp, n_tmp;
  double * tmp_dbl;
  SciErr _SciErr; _SciErr.iErr = 0; _SciErr.iMsgCount = 0;

  *size = -1;

  pos_label = commonFindLabel(pv_ctx, param_addr, label);
  *found = pos_label;

  if (pos_label!=-1) {
    _SciErr = getMatrixOfDoubleInList(pv_ctx, param_addr, pos_label+1, &m_tmp, &n_tmp, &tmp_dbl);
    if (!_SciErr.iErr) {
      *size  = m_tmp*n_tmp;
      *value = (int)tmp_dbl[0];
    } else {
      if (Log) {
        sciprint(_("%s: wrong parameter type. %s expected. Return default value %d.\n"), "getColVectorOfIntInPList","int",default_value);
      }
      *size  = default_size;
      *value = default_value;
    }
  } else {
    if (Log) {
      sciprint(_("%s: parameter not found. Return default value %d.\n"), "getColVectorOfIntInPList",default_value);
    }
    *size  = default_size;
    *value = default_value;
  }
  
  /* Now check parameters */
  
  if (check!=CHECK_NONE) {
    va_list vl;
    va_start(vl, check);
    int nb_value_to_check;
    int value_to_check;
    int check_res = 0;
    
    switch(check) {
    case CHECK_SIZE:
      value_to_check = va_arg(vl,int);
      va_end(vl);
      if (value_to_check!=*size) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong size for parameter %s: %d requested, got %d\n"), "getColVectorOfIntInPList", label, value_to_check, *size);
        }
        *value = default_value;
	addErrorMessage(&_SciErr, 999, _("%s: wrong size for parameter %s: %d requested, got %d\n"), "getColVectorOfIntInPList", label, value_to_check, *size);
        return _SciErr;
      }
      break;
    case CHECK_MIN:
      value_to_check = va_arg(vl,int);
      va_end(vl);
      if (value_to_check>*value) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong min bound for parameter %s: min bound %d, value %d\n"), "getColVectorOfIntInPList", label, value_to_check, *value);
        }
        *value = default_value;
	addErrorMessage(&_SciErr, 999, _("%s: wrong min bound for parameter %s: min bound %d, value %d\n"), "getColVectorOfIntInPList", label, value_to_check, *value);
        return _SciErr;
      }
      break;
    case CHECK_MAX:
      value_to_check = va_arg(vl,int);
      va_end(vl);
      if (value_to_check<*value) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong max bound for parameter %s: max bound %d, value %d\n"), "getColVectorOfIntInPList", label, value_to_check, *value);
        }
        *value = default_value;
	addErrorMessage(&_SciErr, 999, _("%s: wrong max bound for parameter %s: max bound %d, value %d\n"), "getColVectorOfIntInPList", label, value_to_check, *value);
        return _SciErr;
      }
      break;
    case CHECK_BOTH:
      /* First value is the min bound */
      value_to_check = va_arg(vl,int);
      if (value_to_check>*value) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong min bound for parameter %s: min bound %d, value %d\n"), "getColVectorOfIntInPList", label, value_to_check, *value);
        }
        *value = default_value;
        va_end(vl);
	addErrorMessage(&_SciErr, 999, _("%s: wrong min bound for parameter %s: min bound %d, value %d\n"), "getColVectorOfIntInPList", label, value_to_check, *value);
        return _SciErr;
      }
      /* Second value is the max bound */
      value_to_check = va_arg(vl,int);
      va_end(vl);
      if (value_to_check<*value) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong max bound for parameter %s: max bound %d, value %d\n"), "getColVectorOfIntInPList", label, value_to_check, *value);
        }
        *value = default_value;
	addErrorMessage(&_SciErr, 999, _("%s: wrong max bound for parameter %s: max bound %d, value %d\n"), "getColVectorOfIntInPList", label, value_to_check, *value);
        return _SciErr;
      }
      break;
    case CHECK_VALUES:
      /* First parameters is int and contains the number of values to check */
      nb_value_to_check = va_arg(vl,int);
      check_res = 0;
      for(i=0;i<nb_value_to_check; i++) {
        value_to_check = va_arg(vl,int);
        check_res = check_res || (value_to_check==*value);
      }

      if (!check_res) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong value for parameter %s: value %d\n"), "getColVectorOfIntInPList", label, *value);
          sciprint(_("%s: awaited parameters: "), "getColVectorOfIntInPList");
          va_start(vl, check);
          nb_value_to_check = va_arg(vl,int);
          for(i=0;i<nb_value_to_check; i++) {
            value_to_check = va_arg(vl,int);
            sciprint(" %d", value_to_check);
          }
          sciprint("\n");
        }
        
        *value = default_value;
        
        va_end(vl);
	addErrorMessage(&_SciErr, 999, _("%s: wrong value for parameter %s: value %d\n"), "getColVectorOfIntInPList", label, *value);
        return _SciErr;
      }
      
      va_end(vl);
      break;
    }
  }
  
  return _SciErr;
}

SciErr getColVectorOfDoubleInPList(void* pv_ctx, int * param_addr, const char * label, double * value, int * found, 
                                   double default_value, int default_size, int * size, int Log, enum type_check check, ...)
{
  int pos_label, i;
  int m_label, n_label;
  int m_tmp, n_tmp;
  int * len_label = NULL;
  double * tmp_values = NULL;
  char ** label_list;
  SciErr _SciErr; _SciErr.iErr = 0; _SciErr.iMsgCount = 0;

  *size = -1;

  pos_label = commonFindLabel(pv_ctx, param_addr, label);
  *found = pos_label;

  if (len_label) FREE(len_label);
  freeArrayOfString(label_list, m_label*n_label);

  if (pos_label!=-1) {
    _SciErr = getMatrixOfDoubleInList(pv_ctx, param_addr, pos_label+1, &m_tmp, &n_tmp, &tmp_values);
    if (!_SciErr.iErr) {
      *size  = m_tmp*n_tmp;
      *value = tmp_values[0];
    } else {
      if (Log) {
        sciprint(_("%s: wrong parameter type. %s expected. Return default value %f.\n"), "getColVectorOfDoubleInPList","double",default_value);
      }
      *size  = default_size;
      *value = default_value;
    }
  } else {
    if (Log) {
      sciprint(_("%s: parameter not found. Return default value %f.\n"), "getColVectorOfDoubleInPList",default_value);
    }
    *size  = default_size;
    *value = default_value;
  }
  
  /* Now check parameters */

  if (check!=CHECK_NONE) {
    va_list vl;
    va_start(vl, check);
    int nb_value_to_check;
    double value_to_check;
    int check_res = 0;
    
    switch(check) {
    case CHECK_SIZE:
      value_to_check = va_arg(vl,double);
      va_end(vl);
      if (value_to_check!=*size) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong size for parameter %s: %d requested, got %d\n"), "getColVectorOfDoubleInPList", label, value_to_check, *size);
        }
        *value = default_value;
	addErrorMessage(&_SciErr, 999, _("%s: wrong size for parameter %s: %d requested, got %d\n"), "getColVectorOfDoubleInPList", label, value_to_check, *size);
        return _SciErr;
      }
      break;
    case CHECK_MIN:
      value_to_check = va_arg(vl,double);
      va_end(vl);
      if (value_to_check>*value) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong min bound for parameter %s: min bound %f, value %f\n"), "getColVectorOfDoubleInPList", label, value_to_check, *value);
        }
        *value = default_value;
	addErrorMessage(&_SciErr, 999, _("%s: wrong min bound for parameter %s: min bound %f, value %f\n"), "getColVectorOfDoubleInPList", label, value_to_check, *value);
        return _SciErr;
      }
      break;
    case CHECK_MAX:
      value_to_check = va_arg(vl,double);
      va_end(vl);
      if (value_to_check<*value) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong max bound for parameter %s: max bound %f, value %f\n"), "getColVectorOfDoubleInPList", label, value_to_check, *value);
        }
        *value = default_value;
	addErrorMessage(&_SciErr, 999, _("%s: wrong max bound for parameter %s: max bound %f, value %f\n"), "getColVectorOfDoubleInPList", label, value_to_check, *value);
        return _SciErr;
      }
      break;
    case CHECK_BOTH:
      /* First value is the min bound */
      value_to_check = va_arg(vl,double);
      if (value_to_check>*value) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong min bound for parameter %s: min bound %f, value %f\n"), "getColVectorOfDoubleInPList", label, value_to_check, *value);
        }
        *value = default_value;
        va_end(vl);
	addErrorMessage(&_SciErr, 999, _("%s: wrong min bound for parameter %s: min bound %f, value %f\n"), "getColVectorOfDoubleInPList", label, value_to_check, *value);
        return _SciErr;
      }
      /* Second value is the max bound */
      value_to_check = va_arg(vl,double);
      va_end(vl);
      if (value_to_check<*value) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong max bound for parameter %s: max bound %f, value %f\n"), "getColVectorOfDoubleInPList", label, value_to_check, *value);
        }
        *value = default_value;
	addErrorMessage(&_SciErr, 999, _("%s: wrong max bound for parameter %s: max bound %f, value %f\n"), "getColVectorOfDoubleInPList", label, value_to_check, *value);
        return _SciErr;
      }
      break;
    case CHECK_VALUES:
      /* First parameters is int and contains the number of values to check */
      nb_value_to_check = va_arg(vl,int);
      check_res = 0;
      for(i=0;i<nb_value_to_check; i++) {
        value_to_check = va_arg(vl,double);
        check_res = check_res || (value_to_check==*value);
      }
      if (!check_res) {
        if ((*found!=-1)&&(Log)) {
          sciprint(_("%s: wrong value for parameter %s: value %f\n"), "getColVectorOfDoubleInPList", label, *value);
          sciprint(_("%s: awaited parameters: "), "getColVectorOfDoubleInPList");
          va_start(vl, check);
          nb_value_to_check = va_arg(vl,int);
          for(i=0;i<nb_value_to_check; i++) {
            value_to_check = va_arg(vl,double);
            sciprint(" %f", value_to_check);
          }
          sciprint("\n");
        }
        
        *value = default_value;
        
        va_end(vl);
	addErrorMessage(&_SciErr, 999, _("%s: wrong value for parameter %s: value %f\n"), "getColVectorOfDoubleInPList", label, *value);
        return _SciErr;
      }
      va_end(vl);
      break;
    }
  }
  
  return _SciErr;
}

SciErr createPList(void* pv_ctx, int iVar, int ** param_addr, char ** label_names, int nb_params)
{
  SciErr _SciErr; _SciErr.iErr = 0; _SciErr.iMsgCount = 0;
  int i;
  char ** label_list = NULL;

  _SciErr = createMList(pv_ctx, iVar, nb_params+1, param_addr);
  label_list = (char **)MALLOC((nb_params+1)*sizeof(char *));
  label_list[0] = strdup("plist");
  for(i=1;i<=nb_params;i++) {
    label_list[i] = strdup(label_names[i-1]);
  }

  _SciErr = createMatrixOfStringInList(pv_ctx, iVar, *param_addr, 1, 1, nb_params+1, label_list);
  
  if (label_list) freeArrayOfString(label_list, nb_params+1);
  
  return _SciErr;
}

SciErr createIntInPList(void* pv_ctx, int iVar, int * param_addr, char * label_name, int value)
{
  int itemPos = -1;
  double tmp_val[1];
  SciErr _SciErr; _SciErr.iErr = 0; _SciErr.iMsgCount = 0;

  itemPos = commonFindLabel(pv_ctx, param_addr, label_name) + 1;

#ifdef DEBUG
  sciprint("DEBUG: addIntInPList - itemPos = %d label_name = %s\n", itemPos, label_name);
#endif

  tmp_val[0] = (double)value;
  _SciErr = createMatrixOfDoubleInList(pv_ctx, iVar, param_addr, itemPos, 1, 1, tmp_val);

  return _SciErr;
}

SciErr createDoubleInPList(void* pv_ctx, int iVar, int * param_addr, char * label_name, double value)
{
  int itemPos = -1;
  double tmp_val[1];
  SciErr _SciErr; _SciErr.iErr = 0; _SciErr.iMsgCount = 0;

  itemPos = commonFindLabel(pv_ctx, param_addr, label_name) + 1;

#ifdef DEBUG
  sciprint("DEBUG: addDoubleInPList - itemPos = %d label_name = %s\n", itemPos, label_name);
#endif

  tmp_val[0] = value;
  _SciErr = createMatrixOfDoubleInList(pv_ctx, iVar, param_addr, itemPos, 1, 1, tmp_val);

  return _SciErr;
}

SciErr createStringInPList(void* pv_ctx, int iVar, int * param_addr, char * label_name, char * value)
{
  int itemPos = -1;
  char * tmp_val[1];
  SciErr _SciErr; _SciErr.iErr = 0; _SciErr.iMsgCount = 0;

  itemPos = commonFindLabel(pv_ctx, param_addr, label_name) + 1;

#ifdef DEBUG
  sciprint("DEBUG: addStringInPList - itemPos = %d label_name = %s\n", itemPos, label_name);
#endif

  tmp_val[0] = strdup(value);
  _SciErr = createMatrixOfStringInList(pv_ctx, iVar, param_addr, itemPos, 1, 1, tmp_val);

  if (tmp_val[0]) free(tmp_val[0]);

  return _SciErr;
}

SciErr createColVectorOfIntInPList(void* pv_ctx, int iVar, int * param_addr, char * label_name, int nb_values, int * value)
{
  int itemPos = -1, i;
  double * tmp_val = NULL;
  SciErr _SciErr; _SciErr.iErr = 0; _SciErr.iMsgCount = 0;

  itemPos = commonFindLabel(pv_ctx, param_addr, label_name) + 1;

#ifdef DEBUG
  sciprint("DEBUG: addVectorOfIntInPList - itemPos = %d label_name = %s\n", itemPos, label_name);
#endif

  tmp_val = (double *)MALLOC(nb_values*sizeof(double));
  for(i=0;i<nb_values;i++) tmp_val[i] = (double)value[i];
  _SciErr = createMatrixOfDoubleInList(pv_ctx, iVar, param_addr, itemPos, nb_values, 1, tmp_val);

  if (tmp_val) FREE(tmp_val);

  return _SciErr;
}

SciErr createColVectorOfDoubleInPList(void* pv_ctx, int iVar, int * param_addr, char * label_name, int nb_values, double * value)
{
  int itemPos = -1;
  SciErr _SciErr; _SciErr.iErr = 0; _SciErr.iMsgCount = 0;

  itemPos = commonFindLabel(pv_ctx, param_addr, label_name) + 1;

#ifdef DEBUG
  sciprint("DEBUG: addVectorOfDoubleInPList - itemPos = %d label_name = %s\n", itemPos, label_name);
#endif

  _SciErr = createMatrixOfDoubleInList(pv_ctx, iVar, param_addr, itemPos, nb_values, 1, value);

  return _SciErr;
}

/* Utility functions */

int commonFindLabelPartial(void* pv_ctx, int * param_addr, char const * const label_to_find)
{
  int Pos = -1, i;
  int m_label, n_label;
  int * len_label = NULL;
  char ** label_list = NULL;
  SciErr _SciErr; _SciErr.iErr = 0; _SciErr.iMsgCount = 0;

  _SciErr = getMatrixOfStringInList(pv_ctx, param_addr, 1, &m_label, &n_label, NULL, NULL);
  len_label = (int *)MALLOC(m_label*n_label*sizeof(int));
  _SciErr = getMatrixOfStringInList(pv_ctx, param_addr, 1, &m_label, &n_label, len_label, NULL);
  label_list = (char **)MALLOC(m_label*n_label*sizeof(char *));
  for(i=0; i<n_label*m_label; i++) {
    label_list[i] = (char *)MALLOC((len_label[i]+1)*sizeof(char));
  }
  _SciErr = getMatrixOfStringInList(pv_ctx, param_addr, 1, &m_label, &n_label, len_label, label_list);
  
  if (label_list!=NULL) {
    for(i=0; i<m_label*n_label; i++) {
      /* A bug in scilab: if the mlist contains only the type, the C API returns m_label*n_label==2 !! */
      if (label_list[i]!=NULL) {
        if (strncmp(label_list[i],label_to_find,strlen(label_to_find))==0) {
          Pos = i;
          
          if (len_label) FREE(len_label);
          freeArrayOfString(label_list, m_label*n_label);
          
          return Pos;
        } 
      }
    } 
  }
  
  if (len_label) FREE(len_label);
  freeArrayOfString(label_list, m_label*n_label);

  return Pos;
} 

int commonFindLabel(void* pv_ctx, int * param_addr, char const * const label_to_find)
{
  int Pos = -1, i;
  int m_label, n_label;
  int * len_label = NULL;
  char ** label_list = NULL;
  SciErr _SciErr; _SciErr.iErr = 0; _SciErr.iMsgCount = 0;

  _SciErr = getMatrixOfStringInList(pv_ctx, param_addr, 1, &m_label, &n_label, NULL, NULL);
  len_label = (int *)MALLOC(m_label*n_label*sizeof(int));
  _SciErr = getMatrixOfStringInList(pv_ctx, param_addr, 1, &m_label, &n_label, len_label, NULL);
  label_list = (char **)MALLOC(m_label*n_label*sizeof(char *));
  for(i=0; i<n_label*m_label; i++) {
    label_list[i] = (char *)MALLOC((len_label[i]+1)*sizeof(char));
  }
  _SciErr = getMatrixOfStringInList(pv_ctx, param_addr, 1, &m_label, &n_label, len_label, label_list);
  
  if (label_list!=NULL) {
    for(i=0; i<m_label*n_label; i++) {
      /* A bug in scilab: if the mlist contains only the type, the C API returns m_label*n_label==2 !! */
      if (label_list[i]!=NULL) {
        if (strcmp(label_list[i],(char *)label_to_find)==0) {
          Pos = i;
          
          if (len_label) FREE(len_label);
          freeArrayOfString(label_list, m_label*n_label);
          
          return Pos;
        } 
      }
    } 
  }
  
  if (len_label) FREE(len_label);
  freeArrayOfString(label_list, m_label*n_label);
  
  return Pos;
}
