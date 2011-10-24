/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __STRING_GW_HXX__
#define __STRING_GW_HXX__

#include "dynlib_string_gw.h"

#include "cpp_gateway_prototype.hxx"

class StringModule
{
private :
   StringModule(){};
   ~StringModule(){};
public :
   EXTERN_STRING_GW static int Load();
};

CPP_GATEWAY_PROTOTYPE(sci_grep, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_stripblanks, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_regexp, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_part, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_length, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_strindex, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_strsubst, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_ascii, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_strcat, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_string, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_convstr, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_strncpy, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_strchr, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_strrchr, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_strstr, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_strrev, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_strtod, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_tokens, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_strcmp, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_isletter, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_strcspn, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_strtok, EXTERN_STRING_GW);
CPP_GATEWAY_PROTOTYPE(sci_strspn, EXTERN_STRING_GW);
#endif /* !__STRING_GW_HXX__ */
