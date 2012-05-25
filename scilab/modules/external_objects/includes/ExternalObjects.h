/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __EXTERNALOBJECTS_H__
#define __EXTERNALOBJECTS_H__

#define __USE_DEPRECATED_STACK_FUNCTIONS__

#define FIELDS_LENGTH 3

#define VOID_OBJECT -1

#define EXTERNAL_INVALID -1
#define EXTERNAL_OBJECT 0
#define EXTERNAL_CLASS 1
#define EXTERNAL_VOID 2

#define EXTERNAL_ENV_ID_POSITION 2
#define EXTERNAL_OBJ_ID_POSITION 3

#define _NOTHING 0
#define _NULL 1
#define _SINGLE_DOUBLE 2
#define _ROW_DOUBLE 3
#define _MAT_DOUBLE 4
#define _SINGLE_STRING 5
#define _ROW_STRING 6
#define _MAT_STRING 7
#define _SINGLE_BOOLEAN 8
#define _ROW_BOOLEAN 9
#define _MAT_BOOLEAN 10
#define _SINGLE_CHAR 11
#define _ROW_CHAR 12
#define _MAT_CHAR 13
#define _SINGLE_UCHAR 14
#define _ROW_UCHAR 15
#define _MAT_UCHAR 16
#define _SINGLE_SHORT 17
#define _ROW_SHORT 18
#define _MAT_SHORT 19
#define _SINGLE_USHORT 20
#define _ROW_USHORT 21
#define _MAT_USHORT 22
#define _SINGLE_INT 23
#define _ROW_INT 24
#define _MAT_INT 25
#define _SINGLE_UINT 26
#define _ROW_UINT 27
#define _MAT_UINT 28
#define _SINGLE_LONG 29
#define _ROW_LONG 30
#define _MAT_LONG 31
#define _SINGLE_ULONG 32
#define _ROW_ULONG 33
#define _MAT_ULONG 34
#define _SINGLE_FLOAT 35
#define _ROW_FLOAT 36
#define _MAT_FLOAT 37
#define _SINGLE_COMPLEX 38
#define _ROW_COMPLEX 39
#define _MAT_COMPLEX 40

typedef enum {Nothing = _NOTHING, Null = _NULL,
              SingleDouble = _SINGLE_DOUBLE, RowDouble = _ROW_DOUBLE, MatDouble = _MAT_DOUBLE,
              SingleString = _SINGLE_STRING, RowString = _ROW_STRING, MatString = _MAT_STRING,
              SingleBoolean = _SINGLE_BOOLEAN, RowBoolean = _ROW_BOOLEAN, MatBoolean = _MAT_BOOLEAN,
              SingleChar = _SINGLE_CHAR, RowChar = _ROW_CHAR, MatChar = _MAT_CHAR,
              SingleUChar = _SINGLE_UCHAR, RowUChar = _ROW_UCHAR, MatUChar = _MAT_UCHAR,
              SingleShort = _SINGLE_SHORT, RowShort = _ROW_SHORT, MatShort = _MAT_SHORT,
              SingleUShort = _SINGLE_USHORT, RowUShort = _ROW_USHORT, MatUShort = _MAT_USHORT,
              SingleInt = _SINGLE_INT, RowInt = _ROW_INT, MatInt = _MAT_INT,
              SingleUInt = _SINGLE_UINT, RowUInt = _ROW_UINT, MatUInt = _MAT_UINT,
              SingleLong = _SINGLE_LONG, RowLong = _ROW_LONG, MatLong = _MAT_LONG,
              SingleULong = _SINGLE_ULONG, RowULong = _ROW_ULONG, MatULong = _MAT_ULONG,
              SingleFloat = _SINGLE_FLOAT, RowFloat = _ROW_FLOAT, MatFloat = _MAT_FLOAT,
              SingleComplex = _SINGLE_COMPLEX, RowComplex = _ROW_COMPLEX, MatComplex = _MAT_COMPLEX
             } VariableType;

#endif //__EXTERNALOBJECTS_H__
