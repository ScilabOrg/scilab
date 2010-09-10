/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __JAVASCI2_HELPER_H__
#define __JAVASCI2_HELPER_H__

#include "BOOL.h"
#include "sci_types.h"


typedef unsigned char byte;

char* getLastErrorMessageSingle(void);

sci_int_types getIntegerPrecision(char* variableName);

BOOL isComplexVar(char *variableName);

char ** getString(char* variableName, int *nbRow, int *nbCol);

int putString(char* variableName, char **variable, int nbRow, int nbCol);

int putDoubleComplex(char* variableName, double *variable, int nbRow, int nbCol);

double * getDouble(char* variableName, int *nbRow, int *nbCol);

int putDouble(char* variableName, double *variable, int nbRow, int nbCol);

BOOL * getBoolean(char* variableName, int *nbRow, int *nbCol);

int putBoolean(char* variableName, BOOL *variable, int nbRow, int nbCol);

byte * getByte(char* variableName, int *nbRow, int *nbCol);

int putByte(char* variableName, byte *variable, int nbRow, int nbCol);

byte * getUnsignedByte(char* variableName, int *nbRow, int *nbCol);

int putUnsignedByte(char* variableName, byte *variable, int nbRow, int nbCol);

short * getShort(char* variableName, int *nbRow, int *nbCol);

int putShort(char* variableName, short *variable, int nbRow, int nbCol);

unsigned short * getUnsignedShort(char* variableName, int *nbRow, int *nbCol);

int putUnsignedShort(char* variableName, unsigned short *variable, int nbRow, int nbCol);

int * getInt(char* variableName, int *nbRow, int *nbCol);

int putInt(char* variableName, int *variable, int nbRow, int nbCol);

unsigned int * getUnsignedInt(char* variableName, int *nbRow, int *nbCol);

int putUnsignedInt(char* variableName, unsigned int *variable, int nbRow, int nbCol);

double * getDoubleComplexReal(char* variableName, int *nbRow, int *nbCol);

double * getDoubleComplexImg(char* variableName, int *nbRow, int *nbCol);

#endif /* __JAVASCI2_HELPER_H__ */
/*--------------------------------------------------------------------------*/
