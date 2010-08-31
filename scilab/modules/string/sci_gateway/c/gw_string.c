/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET , Cong WU
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
	{NULL, ""}, //length
	{sci_part,"part"},
	{sci_string,"string"},
	{sci_convstr,"convstr"},
	{sci_emptystr,"emptystr"},
	{sci_str2code,"str2code"},
	{sci_code2str,"code2str"},
	{sci_strcat,"strcat"},
	{sci_strindex,"strindex"},
	{NULL, ""}, //strsubst
	{sci_ascii,"ascii"},
	{NULL, ""}, //grep
	{sci_tokens,"tokens"},
	{NULL, ""}, //strsplit
	{NULL, ""}, //stripblank
	{sci_strcmp,"strcmp"},
	{sci_isletter,"isletter"},
	{NULL, ""}, //isdigit
	{NULL ,""}, //isalphanum
	{NULL, ""}, //isascii
	{sci_strcspn,"strcspn"},
	{sci_strncpy,"strncpy"},
	{sci_strrchr,"strrchr"},
	{sci_strchr,"strchr"},
	{sci_strstr,"strstr"},
	{sci_strrev,"strrev"},
	{sci_strtok,"strtok"},
	{sci_strspn,"strspn"},
	{sci_strtod,"strtod"},
	{NULL, ""} //regexp
};
/*--------------------------------------------------------------------------*/
int gw_string(void)
{  
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/

