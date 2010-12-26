/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2009 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include "gw_mpi.h"
#include "sci_mpi.h"

int sci_mpi_unserialize (char *fname, unsigned long fname_len)
{
	int flag;
	CheckRhs(1,1);
	CheckLhs(0,1);
	sciprint("unserialize\b");

    FILE *pFile = fopen ( "/tmp/serialisation.mpi" , "rb" );
    if (pFile==NULL) {sprintf(stderr,"error while reading"); exit (1);}
    int count;
//    sci_types Scilab;
    char Scilab;
    double *data;
    int i=0;
    fread(&count, sizeof(int), 1, pFile);
    printf("count: %d\n",count);

    fread(&Scilab, sizeof(sci_types), 1, pFile);
    
    printf("type: %d\n",Scilab);
    data = (double*)malloc(count*sizeof(double));
    fread(&data, sizeof(double), count, pFile);
    for (i=0; i<count; i++) {
        printf("data[%d]: %f\n",i, data[i]);
    }
    fclose(pFile);
	LhsVar(1)= 0;
	C2F(putlhsvar)();
	return 0;
}
