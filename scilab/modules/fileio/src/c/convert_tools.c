/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "convert_tools.h"

void swap_generic(char* _in, char* _out, int _len)
{
    int i = 0;
    for (i = 0 ; i < _len ; i++)
    {
        _out[i] = _in[(_len - 1) - i];
    }
}
/*--------------------------------------------------------------------------*/
short swap_short(short _val)
{
    short res = 0;
    swap_generic((char*)&_val, (char*)&res, sizeof(short));
    return res;
}
/*--------------------------------------------------------------------------*/
long swap_long(long _val)
{
    long res = 0;
    swap_generic((char*)&_val, (char*)&res, sizeof(long));
    return res;
}
/*--------------------------------------------------------------------------*/
char swap_char(char _val)
{
    return _val;
}
/*--------------------------------------------------------------------------*/
void writeLong(long _val, FILE* _pF, int _iEndian)
{
    //reverse _val bytes and write
    long newVal = _val;
    if (_iEndian == BIG_ENDIAN)
    {
        newVal = swap_long(_val);
    }

    fwrite(&newVal, sizeof(unsigned char), 4, _pF);
}
/*--------------------------------------------------------------------------*/
void writeShort(short _val, FILE* _pF, int _iEndian)
{
    //reverse _val bytes and write
    short newVal = _val;
    if (_iEndian == BIG_ENDIAN)
    {
        newVal = swap_short(_val);
    }

    fwrite(&newVal, sizeof(unsigned char), 2, _pF);
}
/*--------------------------------------------------------------------------*/
void writeChar(short _val, FILE* _pF, int _iEndian)
{
    //reverse _val bytes and write
    fwrite(&_val, sizeof(unsigned char), 1, _pF);
}
/*--------------------------------------------------------------------------*/
long readLong(FILE* _pF, int _iEndian)
{
    long val = 0;
    fread(&val, sizeof(long), 1, _pF);
    if (_iEndian == BIG_ENDIAN)
    {
        return swap_long(val);
    }
    else
    {
        return val;
    }
}
/*--------------------------------------------------------------------------*/
short readShort(FILE* _pF, int _iEndian)
{
    short val = 0;
    fread(&val, sizeof(short), 1, _pF);
    if (_iEndian == BIG_ENDIAN)
    {
        return swap_short(val);
    }
    else
    {
        return val;
    }
}
/*--------------------------------------------------------------------------*/
char readChar(FILE* _pF, int _iEndian)
{
    char val = 0;
    fread(&val, sizeof(char), 1, _pF);
    return val;
}
/*--------------------------------------------------------------------------*/
int checkType(char _type)
{
    if (_type == 'l')
    {
        return TYPE_LONG;
    }

    if (_type == 's')
    {
        return TYPE_SHORT;
    }

    if (_type == 'c')
    {
        return TYPE_CHAR;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int checkEndian(char _endian)
{
    if (_endian == 'b')
    {
        return BIG_ENDIAN;
    }

    if (_endian == 'l')
    {
        return LITTLE_ENDIAN;
    }

    return 0;
}
