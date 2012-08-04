// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8830 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8830
//
// <-- Short Description -->
// In call_scilab, TerminateScilab  did not clear the last error.

// Create a C code to use call_scilab:

#include <stdio.h> 
#include <string.h>
 
#include "stack-c.h" /* Provide functions to access to the memory of Scilab */
#include "call_scilab.h" /* Provide functions to call Scilab engine */

/*------------------------------------------------------------*/
int main(void)
{
#ifdef _MSC_VER
	if ( StartScilab(NULL,NULL,NULL) == FALSE )
#else
	if ( StartScilab(getenv("SCI"),NULL,NULL) == FALSE )
#endif
		{
			fprintf(stderr,"Error while calling StartScilab\n");
			return -1;
		}

	
	int code=SendScilabJob("failedMyCurrentJob=%pi*3/0");
	
	if ( TerminateScilab(NULL) == FALSE ) {
		fprintf(stderr,"Error while calling TerminateScilab\n");
		return -2;
	}
    printf("getLastErrorValue %d\n",getLastErrorValue());
	return 0;
}
/*------------------------------------------------------------*/
