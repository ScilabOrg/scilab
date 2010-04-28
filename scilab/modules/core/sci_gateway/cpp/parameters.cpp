#include <stdarg.h>

#include <parameters.hpp>

extern "C"
{
#include <stack-c.h>
#include <MALLOC.h>
#include <freeArrayOfString.h>
#include <Scierror.h>
#include <sciprint.h>
}

#include <api_scilab.h>


int init_parameters(int Pos, int ** param_addr)
{
  SciErr _SciErr;

  _SciErr = getVarAddressFromPosition(pvApiCtx, Pos, param_addr); 

  return 0;
}

int check_parameters(int * param_addr)
{
  int nb_param = 0, i;
  int m_label, n_label;
  int * len_label = NULL;
  char ** LabelList;
  SciErr _SciErr;
  
  int type;
  
  _SciErr= getVarType(pvApiCtx, param_addr, &type);
  if( type != sci_mlist){ return 1 ; }

  _SciErr = getListItemNumber(pvApiCtx, param_addr, &nb_param);

  if (nb_param!=0)
    {
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, NULL, NULL);
      len_label = (int *)MALLOC(m_label*n_label*sizeof(int));
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, NULL);
      LabelList = (char **)MALLOC(m_label*n_label*sizeof(char *));
      for(i=0; i<n_label*m_label; i++)
        {
          LabelList[i] = (char *)MALLOC((len_label[i]+1)*sizeof(char));
        }
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, LabelList);

      if (strcmp(LabelList[0],"plist") != 0) 
	{
	  if (len_label) FREE(len_label);
	  freeArrayOfString(LabelList, m_label*n_label);

	  return 1;
	}

      if (len_label) FREE(len_label);
      freeArrayOfString(LabelList, m_label*n_label);
    }
  
  return 0;
}

int has_label_partial(int * param_addr, const char * Label)
{
  int nb_param = 0, i, tmp_int = -1;
  int m_label, n_label;
  int * len_label = NULL;
  char ** LabelList;
  SciErr _SciErr;

  _SciErr = getListItemNumber(pvApiCtx, param_addr, &nb_param); 

  if (nb_param!=0)
    {
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, NULL, NULL); 
      len_label = (int *)MALLOC(m_label*n_label*sizeof(int));
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, NULL); 
      LabelList = (char **)MALLOC(m_label*n_label*sizeof(char *));
      for(i=0; i<n_label*m_label; i++)
        {
          LabelList[i] = (char *)MALLOC((len_label[i]+1)*sizeof(char));
        }
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, LabelList); 

      tmp_int = find_label_partial(LabelList, nb_param, Label);

      if (len_label) FREE(len_label);
      freeArrayOfString(LabelList, m_label*n_label);
    }
  
  return tmp_int;
}

int get_int_parameter(int * param_addr, const char * Label, int * value, int * found, int default_value, int Log, type_check check, ...)
{
  int nb_param = 0, pos_label, i;
  int m_label, n_label;
  int m_tmp, n_tmp;
  int * len_label = NULL;
  char ** LabelList;
  double * tmp_dbl;
  SciErr _SciErr;

  _SciErr = getListItemNumber(pvApiCtx, param_addr, &nb_param); 

  if (nb_param!=0)
    {
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, NULL, NULL); 
      len_label = (int *)MALLOC(m_label*n_label*sizeof(int));
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, NULL); 
      LabelList = (char **)MALLOC(m_label*n_label*sizeof(char *));
      for(i=0; i<n_label*m_label; i++)
        {
          LabelList[i] = (char *)MALLOC((len_label[i]+1)*sizeof(char));
        }
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, LabelList); 
    }

  pos_label = find_label(LabelList, nb_param, Label);
  *found = pos_label;

  if (len_label) FREE(len_label);
  freeArrayOfString(LabelList, m_label*n_label);

  if (pos_label!=-1)
    {
      _SciErr = getMatrixOfDoubleInList(pvApiCtx, param_addr, pos_label+1, &m_tmp, &n_tmp, &tmp_dbl); 
      if (!_SciErr.iErr)
	{
	  *value = (int)tmp_dbl[0];
	}
      else
	{
	  if (Log)
	    {
	      sciprint("%s: wrong parameter type. %s expected. Return default value %d.\n", "get_int_parameter","int",default_value);
	    }
	  *value = default_value;
	}
    }
  else
    {
      if (Log)
	{
	  sciprint("%s: parameter not found. Return default value %d.\n", "get_int_parameter",default_value);
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
  
  return 0;
}

int get_double_parameter(int * param_addr, const char * Label, double * value, int * found, double default_value, int Log, type_check check, ...)
{
  int nb_param = 0, pos_label, i;
  int m_label, n_label;
  int m_tmp, n_tmp;
  int * len_label = NULL;
  double * tmp_values = NULL;
  char ** LabelList;
  SciErr _SciErr;

  _SciErr = getListItemNumber(pvApiCtx, param_addr, &nb_param); 

  if (nb_param!=0)
    {
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, NULL, NULL); 
      len_label = (int *)MALLOC(m_label*n_label*sizeof(int));
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, NULL); 
      LabelList = (char **)MALLOC(m_label*n_label*sizeof(char *));
      for(i=0; i<n_label*m_label; i++)
        {
          LabelList[i] = (char *)MALLOC((len_label[i]+1)*sizeof(char));
        }
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, LabelList); 
    }

  pos_label = find_label(LabelList, nb_param, Label);
  *found = pos_label;

  if (len_label) FREE(len_label);
  freeArrayOfString(LabelList, m_label*n_label);

  if (pos_label!=-1)
    {
      _SciErr = getMatrixOfDoubleInList(pvApiCtx, param_addr, pos_label+1, &m_tmp, &n_tmp, &tmp_values); 
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

  return 0;
}

int get_string_parameter(int * param_addr, const char * Label, char ** value, int * found, const char * default_value, int Log, type_check check, ...)
{
  int nb_param = 0, pos_label, i;
  int m_label, n_label;
  int * len_label = NULL;
  char ** LabelList;
  SciErr _SciErr;

  _SciErr = getListItemNumber(pvApiCtx, param_addr, &nb_param); 

  if (nb_param!=0)
    {
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, NULL, NULL); 
      len_label = (int *)MALLOC(m_label*n_label*sizeof(int));
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, NULL); 
      LabelList = (char **)MALLOC(m_label*n_label*sizeof(char *));
      for(i=0; i<n_label*m_label; i++)
        {
          LabelList[i] = (char *)MALLOC((len_label[i]+1)*sizeof(char));
        }
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, LabelList); 
    }

  pos_label = find_label(LabelList, nb_param, Label);
  *found = pos_label;

  if (len_label) FREE(len_label);
  freeArrayOfString(LabelList, m_label*n_label);

  if (pos_label!=-1)
    {
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, pos_label+1, &m_label, &n_label, NULL, NULL); 
      len_label = (int *)MALLOC(m_label*n_label*sizeof(int));
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, pos_label+1, &m_label, &n_label, len_label, NULL); 
      LabelList = (char **)MALLOC(m_label*n_label*sizeof(char *));
      for(i=0; i<n_label*m_label; i++)
        {
          LabelList[i] = (char *)MALLOC((len_label[i]+1)*sizeof(char));
        }
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, pos_label+1, &m_label, &n_label, len_label, LabelList); 

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

  return 0;
}

/////////////////////////////////////
// get vector of double / integers //
/////////////////////////////////////

int get_vec_int_parameter(int * param_addr, const char * Label, int * value, int * found, 
			  int default_value, int default_size, int * size, int Log, type_check check, ...)
{
  int nb_param = 0, pos_label, i;
  int m_label, n_label;
  int m_tmp, n_tmp;
  int * len_label = NULL;
  char ** LabelList;
  double * tmp_dbl;
  SciErr _SciErr;

  *size = -1;

  _SciErr = getListItemNumber(pvApiCtx, param_addr, &nb_param); 

  if (nb_param!=0)
    {
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, NULL, NULL); 
      len_label = (int *)MALLOC(m_label*n_label*sizeof(int));
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, NULL); 
      LabelList = (char **)MALLOC(m_label*n_label*sizeof(char *));
      for(i=0; i<n_label*m_label; i++)
        {
          LabelList[i] = (char *)MALLOC((len_label[i]+1)*sizeof(char));
        }
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, LabelList); 
    }

  pos_label = find_label(LabelList, nb_param, Label);
  *found = pos_label;

  if (len_label) FREE(len_label);
  freeArrayOfString(LabelList, m_label*n_label);

  if (pos_label!=-1)
    {
      _SciErr = getMatrixOfDoubleInList(pvApiCtx, param_addr, pos_label+1, &m_tmp, &n_tmp, &tmp_dbl); 
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

  return 0;
}

int get_vec_double_parameter(int * param_addr, const char * Label, double * value, int * found, 
			     double default_value, int default_size, int * size, int Log, type_check check, ...)
{
  int nb_param = 0, pos_label, i;
  int m_label, n_label;
  int m_tmp, n_tmp;
  int * len_label = NULL;
  double * tmp_values = NULL;
  char ** LabelList;
  SciErr _SciErr;

  *size = -1;

  _SciErr = getListItemNumber(pvApiCtx, param_addr, &nb_param); 

  if (nb_param!=0)
    {
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, NULL, NULL); 
      len_label = (int *)MALLOC(m_label*n_label*sizeof(int));
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, NULL); 
      LabelList = (char **)MALLOC(m_label*n_label*sizeof(char *));
      for(i=0; i<n_label*m_label; i++)
        {
          LabelList[i] = (char *)MALLOC((len_label[i]+1)*sizeof(char));
        }
      _SciErr = getMatrixOfStringInList(pvApiCtx, param_addr, 1, &m_label, &n_label, len_label, LabelList); 
    }

  pos_label = find_label(LabelList, nb_param, Label);
  *found = pos_label;

  if (len_label) FREE(len_label);
  freeArrayOfString(LabelList, m_label*n_label);

  if (pos_label!=-1)
    {
      _SciErr = getMatrixOfDoubleInList(pvApiCtx, param_addr, pos_label+1, &m_tmp, &n_tmp, &tmp_values); 
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
  
  return 0;
}

///////////////////////
// Utility functions //
///////////////////////

int find_label_partial(char ** LabelList, int nblabels, char const * const LabelToFind)
{
  int Pos = -1, i;

  if (LabelList!=NULL)
    {
      for(i=0; i<nblabels; i++)
	{
	  // A bug in scilab: if the mlist contains only the type, the C API returns nblabels==2 !!
	  if (LabelList[i]!=NULL)
	    {
	      if (strncmp(LabelList[i],LabelToFind,strlen(LabelToFind))==0)
		{
		  Pos = i;
		  return Pos;
		} 
	    }
	} 
    }
  return Pos;
} 

int find_label(char ** LabelList, int nblabels, char const * const LabelToFind)
{
  int Pos = -1, i;

  if (LabelList!=NULL)
    {
      for(i=0; i<nblabels; i++)
	{
	  // A bug in scilab: if the mlist contains only the type, the C API returns nblabels==2 !!
	  if (LabelList[i]!=NULL)
	    {
	      if (strcmp(LabelList[i],LabelToFind)==0)
		{
		  Pos = i;
		  return Pos;
		} 
	    }
	} 
    }
  return Pos;
}

