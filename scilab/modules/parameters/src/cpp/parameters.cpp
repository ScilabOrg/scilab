#include <stdarg.h>

#include <parameters.h>

extern "C"
{
#include <stack-c.h>
#include <MALLOC.h>
#include <freeArrayOfString.h>
#include <Scierror.h>
#include <sciprint.h>
}

#include <api_scilab.h>

#define CHECK_ERROR if(_SciErr.iErr)            \
    {                                           \
      printError(&_SciErr, 0);                  \
      return _SciErr.iErr;                      \
    }

int find_label(int * param_addr, char const * const LabelToFind);
int find_label_partial(int * param_addr, char const * const LabelToFind);

int initPList(int Pos, int ** param_addr)
{
  SciErr _SciErr;

  _SciErr = getVarAddressFromPosition(pvApiCtx, Pos, param_addr); CHECK_ERROR;

  return _SciErr.iErr;
}

int checkPList(int * param_addr)
{
  int nb_param = 0, i, var_type;
  int m_label, n_label;
  int * len_label = NULL;
  char ** LabelList;
  SciErr _SciErr;

  _SciErr = getVarType(pvApiCtx, param_addr, &var_type); CHECK_ERROR;
  if (var_type!=sci_mlist)
    {
      return 1;
    }

  _SciErr = getListItemNumber(pvApiCtx, param_addr, &nb_param); CHECK_ERROR;

  if (nb_param!=0)
    {
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, NULL, NULL); CHECK_ERROR;
      len_label = (int *)MALLOC(m_label*n_label*sizeof(int));
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, NULL); CHECK_ERROR;
      LabelList = (char **)MALLOC(m_label*n_label*sizeof(char *));
      for(i=0; i<n_label*m_label; i++)
        {
          LabelList[i] = (char *)MALLOC((len_label[i]+1)*sizeof(char));
        }
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, LabelList); CHECK_ERROR;

      if (strcmp(LabelList[0],"plist") != 0) 
        {
          if (len_label) FREE(len_label);
          freeArrayOfString(LabelList, m_label*n_label);

          return 1;
        }

      if (len_label) FREE(len_label);
      freeArrayOfString(LabelList, m_label*n_label);
    }
  
  return _SciErr.iErr;
}

int hasPartialLabelInPList(int * param_addr, const char * Label)
{
  return find_label_partial(param_addr, Label);
}

int hasLabelInPList(int * param_addr, const char * Label)
{
  return find_label(param_addr, Label);
}

int getIntInPList(int * param_addr, const char * Label, int * value, int * found, int default_value, int Log, type_check check, ...)
{
  int pos_label, i;
  int m_tmp, n_tmp;
  double * tmp_dbl;
  SciErr _SciErr;

  pos_label = find_label(param_addr, Label);
  *found = pos_label;

  if (pos_label!=-1)
    {
      _SciErr = getMatrixOfDoubleInList(pvApiCtx, param_addr, pos_label+1, &m_tmp, &n_tmp, &tmp_dbl); CHECK_ERROR;
      if (!_SciErr.iErr)
        {
          *value = (int)tmp_dbl[0];
        }
      else
        {
          if (Log)
            {
              sciprint("%s: wrong parameter type. %s expected. Return default value %d.\n", "getIntInPList","int",default_value);
            }
          *value = default_value;
        }
    }
  else
    {
      if (Log)
        {
          sciprint("%s: parameter not found. Return default value %d.\n", "getIntInPList",default_value);
        }
      *value = default_value;
    }

  //////////////////////////
  // Now check parameters //
  //////////////////////////

  if (check!=CHECK_NONE)
    {
      va_list vl;
      va_start(vl, check);
      int nb_value_to_check;
      int value_to_check;
      int check_res = 0;
      
      switch(check)
        {
        case CHECK_MIN:
          value_to_check = va_arg(vl,int);
          va_end(vl);
          if (value_to_check>*value)
            {
              if ((*found!=-1)&&(Log)) 
                {
                  sciprint("wrong min bound for parameter %s: min bound %d, value %d\n", Label, value_to_check, *value);
                }
              *value = default_value;
              return 1;
            }
          break;
        case CHECK_MAX:
          value_to_check = va_arg(vl,int);
          va_end(vl);
          if (value_to_check<*value)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong max bound for parameter %s: max bound %d, value %d\n", Label, value_to_check, *value);
                }
              *value = default_value;
              return 1;
            }
          break;
        case CHECK_BOTH:
          // First value is the min bound
          value_to_check = va_arg(vl,int);
          if (value_to_check>*value)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong min bound for parameter %s: min bound %d, value %d\n", Label, value_to_check, *value);
                }
              *value = default_value;
              va_end(vl);
              return 1;
            }
          // Second value is the max bound
          value_to_check = va_arg(vl,int);
          va_end(vl);
          if (value_to_check<*value)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong max bound for parameter %s: max bound %d, value %d\n", Label, value_to_check, *value);
                }
              *value = default_value;
              return 1;
            }
          break;
        case CHECK_VALUES:
          // First parameters is int and contains the number of values to check
          nb_value_to_check = va_arg(vl,int);
          check_res = 0;
          for(i=0;i<nb_value_to_check; i++)
            {
              value_to_check = va_arg(vl,int);
              check_res = check_res || (value_to_check==*value);
            }

          if (!check_res)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong value for parameter %s: value %d\n", Label, *value);
                  sciprint("awaited parameters: ");
                  va_start(vl, check);
                  nb_value_to_check = va_arg(vl,int);
                  for(i=0;i<nb_value_to_check; i++)
                    {
                      value_to_check = va_arg(vl,int);
                      sciprint(" %d", value_to_check);
                    }
                  sciprint("\n");
                }
              
              *value = default_value;
              
              va_end(vl);
              return 1;
            }
          
          va_end(vl);
          break;
        }
    }
  
  return _SciErr.iErr;
}

int getDoubleInPList(int * param_addr, const char * Label, double * value, int * found, double default_value, int Log, type_check check, ...)
{
  int pos_label, i;
  int m_tmp, n_tmp;
  double * tmp_values = NULL;
  SciErr _SciErr;

  pos_label = find_label(param_addr, Label);
  *found = pos_label;

  if (pos_label!=-1)
    {
      _SciErr = getMatrixOfDoubleInList(pvApiCtx, param_addr, pos_label+1, &m_tmp, &n_tmp, &tmp_values); CHECK_ERROR;
      if (!_SciErr.iErr)
        {
          *value = tmp_values[0];
        }
      else
        {
          if (Log)
            {
              sciprint("%s: wrong parameter type. %s expected. Return default value %f.\n", "get_double_parameter","double",default_value);
            }
          *value = default_value;
        }
    }
  else
    {
      if (Log)
        {
          sciprint("%s: parameter not found. Return default value %f.\n", "get_double_parameter",default_value);
        }
      *value = default_value;
    }

  //////////////////////////
  // Now check parameters //
  //////////////////////////

  if (check!=CHECK_NONE)
    {
      va_list vl;
      va_start(vl, check);
      int nb_value_to_check;
      double value_to_check;
      int check_res = 0;

      switch(check)
        {
        case CHECK_MIN:
          value_to_check = va_arg(vl,double);
          va_end(vl);
          if (value_to_check>*value)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong min bound for parameter %s: min bound %f, value %f\n", Label, value_to_check, *value);
                }
              *value = default_value;
              return 1;
            }
          break;
        case CHECK_MAX:
          value_to_check = va_arg(vl,double);
          va_end(vl);
          if (value_to_check<*value)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong max bound for parameter %s: max bound %f, value %f\n", Label, value_to_check, *value);
                }
              *value = default_value;
              return 1;
            }
          break;
        case CHECK_BOTH:
          // First value is the min bound
          value_to_check = va_arg(vl,double);
          if (value_to_check>*value)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong min bound for parameter %s: min bound %f, value %f\n", Label, value_to_check, *value);
                }
              *value = default_value;
              va_end(vl);
              return 1;
            }
          // Second value is the max bound
          value_to_check = va_arg(vl,double);
          va_end(vl);
          if (value_to_check<*value)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong max bound for parameter %s: max bound %f, value %f\n", Label, value_to_check, *value);
                }
              *value = default_value;
              return 1;
            }
          break;
        case CHECK_VALUES:
          // First parameters is int and contains the number of values to check
          nb_value_to_check = va_arg(vl,int);
          check_res = 0;
          for(i=0;i<nb_value_to_check; i++)
            {
              value_to_check = va_arg(vl,double);
              check_res = check_res || (value_to_check==*value);
            }

          if (!check_res)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong value for parameter %s: value %f\n", Label, *value);
                  sciprint("awaited parameters: ");
                  va_start(vl, check);
                  nb_value_to_check = va_arg(vl,int);
                  for(i=0;i<nb_value_to_check; i++)
                    {
                      value_to_check = va_arg(vl,double);
                      sciprint(" %f", value_to_check);
                    }
                  sciprint("\n");
                }

              *value = default_value;
              
              va_end(vl);
              return 1;
            }

          va_end(vl);
          break;
        }
    }

  return _SciErr.iErr;
}

int getStringInPList(int * param_addr, const char * Label, char ** value, int * found, const char * default_value, int Log, type_check check, ...)
{
  int pos_label, i;
  int m_label, n_label;
  int * len_label = NULL;
  char ** LabelList;
  SciErr _SciErr;

  pos_label = find_label(param_addr, Label);
  *found = pos_label;

  if (pos_label!=-1)
    {
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, pos_label+1, &m_label, &n_label, NULL, NULL); CHECK_ERROR;
      len_label = (int *)MALLOC(m_label*n_label*sizeof(int));
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, pos_label+1, &m_label, &n_label, len_label, NULL); CHECK_ERROR;
      LabelList = (char **)MALLOC(m_label*n_label*sizeof(char *));
      for(i=0; i<n_label*m_label; i++)
        {
          LabelList[i] = (char *)MALLOC((len_label[i]+1)*sizeof(char));
        }
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, pos_label+1, &m_label, &n_label, len_label, LabelList); CHECK_ERROR;

      if (!_SciErr.iErr)
        {
          if (LabelList[0])
            {
              *value = strdup(LabelList[0]);
            }
          else
            {
              if (Log)
                {
                  sciprint("%s: wrong parameter type. %s expected. Return default value %s.\n", "get_string_parameter","string",default_value);
                }
              *value = strdup(default_value);
            }
        }
      else
        {
          if (Log)
            {
              sciprint("%s: parameter not found. Return default value %s.\n", "get_string_parameter",default_value);
            }
          *value = strdup(default_value);
        }

      if (len_label) FREE(len_label);
      freeArrayOfString(LabelList, m_label*n_label);
    }
  else
    {
      *value = strdup(default_value);
    }

  //////////////////////////
  // Now check parameters //
  //////////////////////////

  if (check!=CHECK_NONE)
    {
      va_list vl;
      va_start(vl, check);
      int nb_value_to_check;
      char * value_to_check;
      int check_res = false;

      switch(check)
        {
        case CHECK_VALUES:
          // First parameters is int and contains the number of values to check
          nb_value_to_check = va_arg(vl,int);
          check_res = 0;
          for(i=0;i<nb_value_to_check; i++)
            {
              value_to_check = va_arg(vl,char *);
              check_res = check_res || (strcmp(value_to_check,*value)==0);
            }

          if (!check_res)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong value for parameter %s: value %s\n", Label, *value);
                  sciprint("awaited parameters: ");
                  va_start(vl, check);
                  nb_value_to_check = va_arg(vl,int);
                  for(i=0;i<nb_value_to_check; i++)
                    {
                      value_to_check = va_arg(vl,char *);
                      sciprint(" \"%s\"", value_to_check);
                    }
                  sciprint("\n");
                }

              FREE(*value);
              *value = strdup(default_value);
              
              va_end(vl);
              return 1;
            }

          va_end(vl);
          break;
        }
    }

  return _SciErr.iErr;
}

/////////////////////////////////////
// get vector of double / integers //
/////////////////////////////////////

int getVectorOfIntInPList(int * param_addr, const char * Label, int * value, int * found, 
                          int default_value, int default_size, int * size, int Log, type_check check, ...)
{
  int pos_label, i;
  int m_tmp, n_tmp;
  double * tmp_dbl;
  SciErr _SciErr;

  *size = -1;

  pos_label = find_label(param_addr, Label);
  *found = pos_label;

  if (pos_label!=-1)
    {
      _SciErr = getMatrixOfDoubleInList(pvApiCtx, param_addr, pos_label+1, &m_tmp, &n_tmp, &tmp_dbl); CHECK_ERROR;
      if (!_SciErr.iErr)
        {
          *size  = m_tmp*n_tmp;
          *value = (int)tmp_dbl[0];
        }
      else
        {
          if (Log)
            {
              sciprint("%s: wrong parameter type. %s expected. Return default value %d.\n", "get_vec_int_parameter","int",default_value);
            }
          *size  = default_size;
          *value = default_value;
        }
    }
  else
    {
      if (Log)
        {
          sciprint("%s: parameter not found. Return default value %d.\n", "get_vec_int_parameter",default_value);
        }
      *size  = default_size;
      *value = default_value;
    }

  //////////////////////////
  // Now check parameters //
  //////////////////////////

  if (check!=CHECK_NONE)
    {
      va_list vl;
      va_start(vl, check);
      int nb_value_to_check;
      int value_to_check;
      int check_res = 0;

      switch(check)
        {
        case CHECK_SIZE:
          value_to_check = va_arg(vl,int);
          va_end(vl);
          if (value_to_check!=*size)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong size for parameter %s: %d requested, got %d\n", Label, value_to_check, *size);
                }
              *value = default_value;
              return 1;
            }
          break;
        case CHECK_MIN:
          value_to_check = va_arg(vl,int);
          va_end(vl);
          if (value_to_check>*value)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong min bound for parameter %s: min bound %d, value %d\n", Label, value_to_check, *value);
                }
              *value = default_value;
              return 1;
            }
          break;
        case CHECK_MAX:
          value_to_check = va_arg(vl,int);
          va_end(vl);
          if (value_to_check<*value)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong max bound for parameter %s: max bound %d, value %d\n", Label, value_to_check, *value);
                }
              *value = default_value;
              return 1;
            }
          break;
        case CHECK_BOTH:
          // First value is the min bound
          value_to_check = va_arg(vl,int);
          if (value_to_check>*value)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong min bound for parameter %s: min bound %d, value %d\n", Label, value_to_check, *value);
                }
              *value = default_value;
              va_end(vl);
              return 1;
            }
          // Second value is the max bound
          value_to_check = va_arg(vl,int);
          va_end(vl);
          if (value_to_check<*value)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong max bound for parameter %s: max bound %d, value %d\n", Label, value_to_check, *value);
                }
              *value = default_value;
              return 1;
            }
          break;
        case CHECK_VALUES:
          // First parameters is int and contains the number of values to check
          nb_value_to_check = va_arg(vl,int);
          check_res = 0;
          for(i=0;i<nb_value_to_check; i++)
            {
              value_to_check = va_arg(vl,int);
              check_res = check_res || (value_to_check==*value);
            }

          if (!check_res)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong value for parameter %s: value %d\n", Label, *value);
                  sciprint("awaited parameters: ");
                  va_start(vl, check);
                  nb_value_to_check = va_arg(vl,int);
                  for(i=0;i<nb_value_to_check; i++)
                    {
                      value_to_check = va_arg(vl,int);
                      sciprint(" %d", value_to_check);
                    }
                  sciprint("\n");
                }

              *value = default_value;

              va_end(vl);
              return 1;
            }
          
          va_end(vl);
          break;
        }
    }

  return _SciErr.iErr;
}

int getVectorOfDoubleInPList(int * param_addr, const char * Label, double * value, int * found, 
                             double default_value, int default_size, int * size, int Log, type_check check, ...)
{
  int pos_label, i;
  int m_label, n_label;
  int m_tmp, n_tmp;
  int * len_label = NULL;
  double * tmp_values = NULL;
  char ** LabelList;
  SciErr _SciErr;

  *size = -1;

  pos_label = find_label(param_addr, Label);
  *found = pos_label;

  if (len_label) FREE(len_label);
  freeArrayOfString(LabelList, m_label*n_label);

  if (pos_label!=-1)
    {
      _SciErr = getMatrixOfDoubleInList(pvApiCtx, param_addr, pos_label+1, &m_tmp, &n_tmp, &tmp_values); CHECK_ERROR;
      if (!_SciErr.iErr)
        {
          *size  = m_tmp*n_tmp;
          *value = tmp_values[0];
        }
      else
        {
          if (Log)
            {
              sciprint("%s: wrong parameter type. %s expected. Return default value %f.\n", "get_vec_double_parameter","double",default_value);
            }
          *size  = default_size;
          *value = default_value;
        }
    }
  else
    {
      if (Log)
        {
          sciprint("%s: parameter not found. Return default value %f.\n", "get_vec_double_parameter",default_value);
        }
      *size  = default_size;
      *value = default_value;
    }

  //////////////////////////
  // Now check parameters //
  //////////////////////////

  if (check!=CHECK_NONE)
    {
      va_list vl;
      va_start(vl, check);
      int nb_value_to_check;
      double value_to_check;
      int check_res = false;

      switch(check)
        {
        case CHECK_SIZE:
          value_to_check = va_arg(vl,double);
          va_end(vl);
          if (value_to_check!=*size)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong size for parameter %s: %d requested, got %d\n", Label, value_to_check, *size);
                }
              *value = default_value;
              return 1;
            }
          break;
        case CHECK_MIN:
          value_to_check = va_arg(vl,double);
          va_end(vl);
          if (value_to_check>*value)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong min bound for parameter %s: min bound %f, value %f\n", Label, value_to_check, *value);
                }
              *value = default_value;
              return 1;
            }
          break;
        case CHECK_MAX:
          value_to_check = va_arg(vl,double);
          va_end(vl);
          if (value_to_check<*value)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong max bound for parameter %s: max bound %f, value %f\n", Label, value_to_check, *value);
                }
              *value = default_value;
              return 1;
            }
          break;
        case CHECK_BOTH:
          // First value is the min bound
          value_to_check = va_arg(vl,double);
          if (value_to_check>*value)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong min bound for parameter %s: min bound %f, value %f\n", Label, value_to_check, *value);
                }
              *value = default_value;
              va_end(vl);
              return 1;
            }
          // Second value is the max bound
          value_to_check = va_arg(vl,double);
          va_end(vl);
          if (value_to_check<*value)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong max bound for parameter %s: max bound %f, value %f\n", Label, value_to_check, *value);
                }
              *value = default_value;
              return 1;
            }
          break;
        case CHECK_VALUES:
          // First parameters is int and contains the number of values to check
          nb_value_to_check = va_arg(vl,int);
          check_res = 0;
          for(i=0;i<nb_value_to_check; i++)
            {
              value_to_check = va_arg(vl,double);
              check_res = check_res || (value_to_check==*value);
            }
          if (!check_res)
            {
              if ((*found!=-1)&&(Log))
                {
                  sciprint("wrong value for parameter %s: value %f\n", Label, *value);
                  sciprint("awaited parameters: ");
                  va_start(vl, check);
                  nb_value_to_check = va_arg(vl,int);
                  for(i=0;i<nb_value_to_check; i++)
                    {
                      value_to_check = va_arg(vl,double);
                      sciprint(" %f", value_to_check);
                    }
                  sciprint("\n");
                }

              *value = default_value;
              
              va_end(vl);
              return 1;
            }
          va_end(vl);
          break;
        }
    }
  
  return _SciErr.iErr;
}

int createPList(int Pos, int ** param_addr, char ** LabelNames, int nb_params)
{
  SciErr _SciErr;
  int i;
  char ** LabelList = NULL;

  _SciErr = createMList(pvApiCtx, Pos, nb_params+1, param_addr); CHECK_ERROR;
  LabelList = (char **)MALLOC((nb_params+1)*sizeof(char *));
  LabelList[0] = strdup("plist");
  for(i=1;i<=nb_params;i++) 
    {
      LabelList[i] = strdup(LabelNames[i-1]);
    }

  _SciErr = createMatrixOfStringInList(pvApiCtx, Pos, *param_addr, 1, 1, nb_params+1, LabelList); CHECK_ERROR;

  if (LabelList) freeArrayOfString(LabelList, nb_params+1);
  
  return _SciErr.iErr;
}

int createIntInPList(int iVar, int * param_addr, char * LabelName, int value)
{
  int itemPos = -1;
  double tmp_val[1];
  SciErr _SciErr;

  itemPos = find_label(param_addr, LabelName) + 1;

#ifdef DEBUG
  sciprint("DEBUG: addIntInPList - itemPos = %d LabelName = %s\n", itemPos, LabelName);
#endif

  // Now add the value
  tmp_val[0] = (double)value;
  _SciErr = createMatrixOfDoubleInList(pvApiCtx, iVar, param_addr, itemPos, 1, 1, tmp_val); CHECK_ERROR;

  return _SciErr.iErr;
}

int createDoubleInPList(int iVar, int * param_addr, char * LabelName, double value)
{
  int itemPos = -1;
  double tmp_val[1];
  SciErr _SciErr;

  itemPos = find_label(param_addr, LabelName) + 1;

#ifdef DEBUG
  sciprint("DEBUG: addDoubleInPList - itemPos = %d LabelName = %s\n", itemPos, LabelName);
#endif

  // Now add the value
  tmp_val[0] = value;
  _SciErr = createMatrixOfDoubleInList(pvApiCtx, iVar, param_addr, itemPos, 1, 1, tmp_val); CHECK_ERROR;

  return _SciErr.iErr;
}

int createStringInPList(int iVar, int * param_addr, char * LabelName, char * value)
{
  int itemPos = -1;
  char * tmp_val[1];
  SciErr _SciErr;

  itemPos = find_label(param_addr, LabelName) + 1;

#ifdef DEBUG
  sciprint("DEBUG: addStringInPList - itemPos = %d LabelName = %s\n", itemPos, LabelName);
#endif

  // Now add the value
  tmp_val[0] = strdup(value);
  _SciErr = createMatrixOfStringInList(pvApiCtx, iVar, param_addr, itemPos, 1, 1, tmp_val); CHECK_ERROR;

  if (tmp_val[0]) free(tmp_val[0]);

  return _SciErr.iErr;
}

int createVectorOfIntInPList(int iVar, int * param_addr, char * LabelName, int nb_values, int * value)
{
  int itemPos = -1, i;
  double * tmp_val = NULL;
  SciErr _SciErr;

  itemPos = find_label(param_addr, LabelName) + 1;

#ifdef DEBUG
  sciprint("DEBUG: addVectorOfIntInPList - itemPos = %d LabelName = %s\n", itemPos, LabelName);
#endif

  // Now add the value
  tmp_val = (double *)MALLOC(nb_values*sizeof(double));
  for(i=0;i<nb_values;i++) tmp_val[i] = (double)value[i];
  _SciErr = createMatrixOfDoubleInList(pvApiCtx, iVar, param_addr, itemPos, nb_values, 1, tmp_val); CHECK_ERROR;

  if (tmp_val) FREE(tmp_val);

  return _SciErr.iErr;
}

int createVectorOfDoubleInPList(int iVar, int * param_addr, char * LabelName, int nb_values, double * value)
{
  int itemPos = -1;
  SciErr _SciErr;

  itemPos = find_label(param_addr, LabelName) + 1;

#ifdef DEBUG
  sciprint("DEBUG: addVectorOfDoubleInPList - itemPos = %d LabelName = %s\n", itemPos, LabelName);
#endif

  // Now add the value
  _SciErr = createMatrixOfDoubleInList(pvApiCtx, iVar, param_addr, itemPos, nb_values, 1, value); CHECK_ERROR;

  return _SciErr.iErr;
}

///////////////////////
// Utility functions //
///////////////////////

int find_label_partial(int * param_addr, char const * const LabelToFind)
{
  int Pos = -1, i;
  int m_label, n_label;
  int * len_label = NULL;
  char ** LabelList = NULL;
  SciErr _SciErr;

  _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, NULL, NULL); CHECK_ERROR;
  len_label = (int *)MALLOC(m_label*n_label*sizeof(int));
  _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, NULL); CHECK_ERROR;
  LabelList = (char **)MALLOC(m_label*n_label*sizeof(char *));
  for(i=0; i<n_label*m_label; i++)
    {
      LabelList[i] = (char *)MALLOC((len_label[i]+1)*sizeof(char));
    }
  _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, LabelList); CHECK_ERROR;

  if (LabelList!=NULL)
    {
      for(i=0; i<m_label*n_label; i++)
        {
          // A bug in scilab: if the mlist contains only the type, the C API returns m_label*n_label==2 !!
          if (LabelList[i]!=NULL)
            {
              if (strncmp(LabelList[i],LabelToFind,strlen(LabelToFind))==0)
                {
                  Pos = i;

                  if (len_label) FREE(len_label);
                  freeArrayOfString(LabelList, m_label*n_label);

                  return Pos;
                } 
            }
        } 
    }

  if (len_label) FREE(len_label);
  freeArrayOfString(LabelList, m_label*n_label);

  return Pos;
} 

int find_label(int * param_addr, char const * const LabelToFind)
{
  int Pos = -1, i;
  int m_label, n_label;
  int * len_label = NULL;
  char ** LabelList = NULL;
  SciErr _SciErr;

  _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, NULL, NULL); CHECK_ERROR;
  len_label = (int *)MALLOC(m_label*n_label*sizeof(int));
  _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, NULL); CHECK_ERROR;
  LabelList = (char **)MALLOC(m_label*n_label*sizeof(char *));
  for(i=0; i<n_label*m_label; i++)
    {
      LabelList[i] = (char *)MALLOC((len_label[i]+1)*sizeof(char));
    }
  _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, LabelList); CHECK_ERROR;

  if (LabelList!=NULL)
    {
      for(i=0; i<m_label*n_label; i++)
        {
          // A bug in scilab: if the mlist contains only the type, the C API returns m_label*n_label==2 !!
          if (LabelList[i]!=NULL)
            {
              if (strcmp(LabelList[i],(char *)LabelToFind)==0)
                {
                  Pos = i;

                  if (len_label) FREE(len_label);
                  freeArrayOfString(LabelList, m_label*n_label);

                  return Pos;
                } 
            }
        } 
    }

  if (len_label) FREE(len_label);
  freeArrayOfString(LabelList, m_label*n_label);

  return Pos;
}
