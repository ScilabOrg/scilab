/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#ifndef __INT_API__
#define __INT_API__

#ifdef __cplusplus
extern "C" {
#endif


#define SCI_INT8		1
#define SCI_INT16		2
#define SCI_INT32		4
#define SCI_INT64		8

#define SCI_UINT8		11
#define SCI_UINT16	12
#define SCI_UINT32	14
#define SCI_UINT64	18

/********************************/
/*   integer matrix functions   */
/********************************/

/**
 * Get integer precision ( SCI_xINTx )
 * @param[in] _piAddress variable address
 * @param[out] _piPrecison return integer precision ( SCI_xINTx )
 */
SciErr getMatrixOfIntegerPrecision(void* _pvCtx, int* _piAddress, int* _piPrecision);

/**
 * Get integer 8 bit variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piData8 return pointer on 8 bits integer
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getMatrixOfInteger8(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, char** _pcData8);
SciErr getMatrixOfUnsignedInteger8(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned char** _pucData8);

/**
 * Get integer 16 bit variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piData16 return pointer on 16 bits integer
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getMatrixOfInteger16(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, short** _psData16);
SciErr getMatrixOfUnsignedInteger16(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned short** _pusData16);

/**
 * Get integer 32 bit variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piData32 return pointer on 32 bits integer
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getMatrixOfInteger32(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piData32);
SciErr getMatrixOfUnsignedInteger32(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned int** _puiData32);

/**
 * Get integer 64 bit variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piData64 return pointer on 64 bits integer
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
SciErr getMatrixOfInteger64(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, long long** _pllData64);
SciErr getMatrixOfUnsignedInteger64(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned long long** _pullData64);
#endif

/**
 * Get integer variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData8 array of integer 8 bits
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createMatrixOfInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, char* _pcData8);
SciErr createMatrixOfUnsignedInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned char* _pucData8);

/**
 * Get integer variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData16 array of integer 16 bits
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createMatrixOfInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, short* _psData16);
SciErr createMatrixOfUnsignedInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned short* _pusData16);

/**
 * Get integer variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData32 array of integer 32 bits
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createMatrixOfInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, int* _piData32);
SciErr createMatrixOfUnsignedInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned int* _puiData32);

/**
 * Get integer variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData64 array of integer 64 bits
 * @return if the operation successed (0) or not ( !0 )
 */

#ifdef __SCILAB_INT64__
SciErr createMatrixOfInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, long long* _pllData64);
SciErr createMatrixOfUnsignedInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned long long* _pullData64);
#endif

/**
 * Get integer variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _piData8 return array of integer 8 bits
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr allocMatrixOfInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, char** _pcData8);
SciErr allocMatrixOfUnsignedInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned char** _pucData8);

/**
 * Get integer variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _piData16 return array of integer 16 bits
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr allocMatrixOfInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, short** _psData16);
SciErr allocMatrixOfUnsignedInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned short** _pusData16);

/**
 * Get integer variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _piData32 return array of integer 32 bits
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr allocMatrixOfInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, int** _piData32);
SciErr allocMatrixOfUnsignedInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned int** _puiData32);

/**
 * Get integer variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _piData64 return array of integer 64 bits
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
SciErr allocMatrixOfInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, long long** _pllData64);
SciErr allocMatrixOfUnsignedInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned long long** _pullData64);
#endif

/**
 * Get integer variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData8 array of integer 8 bits
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createNamedMatrixOfInteger8(void* _pvCtx, char* _pstName, int _iRows, int _iCols, char* _pcData8);
SciErr createNamedMatrixOfUnsignedInteger8(void* _pvCtx, char* _pstName, int _iRows, int _iCols, unsigned char* _pucData8);

/**
 * Get integer variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData16 array of integer 16 bits
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createNamedMatrixOfInteger16(void* _pvCtx, char* _pstName, int _iRows, int _iCols, short* _psData16);
SciErr createNamedMatrixOfUnsignedInteger16(void* _pvCtx, char* _pstName, int _iRows, int _iCols, unsigned short* _pusData16);

/**
 * Get integer variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData32 array of integer 32 bits
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createNamedMatrixOfInteger32(void* _pvCtx, char* _pstName, int _iRows, int _iCols, int* _piData32);
SciErr createNamedMatrixOfUnsignedInteger32(void* _pvCtx, char* _pstName, int _iRows, int _iCols, unsigned int* _puiData32);

/**
 * Get integer variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData64 array of integer 64 bits
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
SciErr createNamedMatrixOfInteger64(void* _pvCtx, char* _pstName, int _iRows, int _iCols, long long* _pllData64);
SciErr createNamedMatrixOfUnsignedInteger64(void* _pvCtx, char* _pstName, int _iRows, int _iCols, unsigned long long* _pullData64);
#endif

/**
 * Get integer precision ( SCI_xINTx )
 * @param[in] _pstName variable name
 * @param[out] _piPrecison return integer precision ( SCI_xINTx )
 */
SciErr getNamedMatrixOfIntegerPrecision(void* _pvCtx, char* _pstName, int* _piPrecision);

/**
 * Get integer named variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _piData8 return array of integer 8 bits
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr readNamedMatrixOfInteger8(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, char* _pcData8);
SciErr readNamedMatrixOfUnsignedInteger8(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, unsigned char* _pucData8);
/**
 * Get integer named variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _piData16 return array of integer 16 bits
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr readNamedMatrixOfInteger16(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, short* _psData16);
SciErr readNamedMatrixOfUnsignedInteger16(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, unsigned short* _pusData16);
/**
 * Get integer named variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _piData32 return array of integer 32 bits
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr readNamedMatrixOfInteger32(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piData32);
SciErr readNamedMatrixOfUnsignedInteger32(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, unsigned int* _puiData32);
/**
 * Get integer named variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _piData64 return array of integer 64 bits
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
SciErr readNamedMatrixOfInteger64(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, long long* _pllData64);
SciErr readNamedMatrixOfUnsignedInteger64(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, unsigned long long* _pullData64);
#endif

/* shortcut functions */

/**
 * check if the variable type is a integer
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int isIntegerType(void* _pvCtx, int* _piAddress);

/**
 * check if the variable type is a integer
 * @param[in] _pstName variable name
 * @return 1 for true and 0 for false
 */
int isNamedIntegerType(void* _pvCtx, char* _pstName);

/**
 * Get scalar integer variable data
 * @param[in] _piAddress variable address
 * @param[out] _pcData return integer value
 * @return if the operation successed (0) or not ( !0 )
 */
int getScalarInteger8(void* _pvCtx, int* _piAddress, char* _pcData);

/**
 * Get scalar integer variable data
 * @param[in] _piAddress variable address
 * @param[out] _psData return integer value
 * @return if the operation successed (0) or not ( !0 )
 */
int getScalarInteger16(void* _pvCtx, int* _piAddress, short* _psData);

/**
 * Get scalar integer variable data
 * @param[in] _piAddress variable address
 * @param[out] _piData return integer value
 * @return if the operation successed (0) or not ( !0 )
 */
int getScalarInteger32(void* _pvCtx, int* _piAddress, int* _piData);

/**
 * Get scalar integer variable data
 * @param[in] _piAddress variable address
 * @param[out] _pllData return integer value
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
int getScalarInteger64(void* _pvCtx, int* _piAddress, long long* _pllData);
#endif

/**
 * Get scalar integer variable data
 * @param[in] _piAddress variable address
 * @param[out] _pcData return integer value
 * @return if the operation successed (0) or not ( !0 )
 */
int getScalarUnsignedInteger8(void* _pvCtx, int* _piAddress, unsigned char* _pucData);

/**
 * Get scalar integer variable data
 * @param[in] _piAddress variable address
 * @param[out] _psData return integer value
 * @return if the operation successed (0) or not ( !0 )
 */
int getScalarUnsignedInteger16(void* _pvCtx, int* _piAddress, unsigned short* _pusData);

/**
 * Get scalar integer variable data
 * @param[in] _piAddress variable address
 * @param[out] _piData return integer value
 * @return if the operation successed (0) or not ( !0 )
 */
int getScalarUnsignedInteger32(void* _pvCtx, int* _piAddress, unsigned int* _puiData);

/**
 * Get scalar integer variable data
 * @param[in] _piAddress variable address
 * @param[out] _pllData return integer value
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
int getScalarUnsignedInteger64(void* _pvCtx, int* _piAddress, unsigned long long* _pullData);
#endif

/**
 * Get named scalar integer variable data
 * @param[in] _pstName variable name
 * @param[out] _pcData return integer value
 * @return if the operation successed (0) or not ( !0 )
 */
int getNamedScalarInteger8(void* _pvCtx, char* _pstName, char* _pcData);

/**
 * Get named scalar integer variable data
 * @param[in] _pstName variable name
 * @param[out] _psData return integer value
 * @return if the operation successed (0) or not ( !0 )
 */
int getNamedScalarInteger16(void* _pvCtx, char* _pstName, short* _psData);

/**
 * Get named scalar integer variable data
 * @param[in] _pstName variable name
 * @param[out] _piData return integer value
 * @return if the operation successed (0) or not ( !0 )
 */
int getNamedScalarInteger32(void* _pvCtx, char* _pstName, int* _piData);

/**
 * Get named scalar integer variable data
 * @param[in] _pstName variable name
 * @param[out] _pllData return integer value
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
int getNamedScalarInteger64(void* _pvCtx, char* _pstName, long long* _pllData);
#endif

/**
 * Get named scalar integer variable data
 * @param[in] _pstName variable name
 * @param[out] _pcData return integer value
 * @return if the operation successed (0) or not ( !0 )
 */
int getNamedScalarUnsignedInteger8(void* _pvCtx, char* _pstName, unsigned char* _pucData);

/**
 * Get named scalar integer variable data
 * @param[in] _pstName variable name
 * @param[out] _psData return integer value
 * @return if the operation successed (0) or not ( !0 )
 */
int getNamedScalarUnsignedInteger16(void* _pvCtx, char* _pstName, unsigned short* _pusData);

/**
 * Get named scalar integer variable data
 * @param[in] _pstName variable name
 * @param[out] _piData return integer value
 * @return if the operation successed (0) or not ( !0 )
 */
int getNamedScalarUnsignedInteger32(void* _pvCtx, char* _pstName, unsigned int* _puiData);

/**
 * Get named scalar integer variable data
 * @param[in] _pstName variable name
 * @param[out] _pllData return integer value
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
int getNamedScalarUnsignedInteger64(void* _pvCtx, char* _pstName, unsigned long long* _pullData);
#endif

/**
 * create scalar integer variable data
 * @param[in] _piAddress variable name
 * @param[in] _pcData array of integer 8 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int createScalarInteger8(void* _pvCtx, int _iVar, char _cData);

/**
 * create scalar integer variable data
 * @param[in] _piAddress variable name
 * @param[in] _psData array of integer 16 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int createScalarInteger16(void* _pvCtx, int _iVar, short _sData);

/**
 * create scalar integer variable data
 * @param[in] _piAddress variable name
 * @param[in] _piData array of integer 32 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int createScalarInteger32(void* _pvCtx, int _iVar, int _iData);

/**
 * create scalar integer variable data
 * @param[in] _piAddress variable name
 * @param[in] _pllData array of integer 64 bits
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
int createScalarInteger64(void* _pvCtx, int _iVar, long long _llData);
#endif

/**
 * create scalar integer variable data
 * @param[in] _piAddress variable name
 * @param[in] _pucData array of unsigned integer 8 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int createScalarUnsignedInteger8(void* _pvCtx, int _iVar, unsigned char _ucData);

/**
 * create scalar integer variable data
 * @param[in] _piAddress variable name
 * @param[in] _pusData array of unsigned integer 16 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int createScalarUnsignedInteger16(void* _pvCtx, int _iVar, unsigned short _usData);

/**
 * create scalar integer variable data
 * @param[in] _piAddress variable name
 * @param[in] _puiData array of unsigned integer 32 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int createScalarUnsignedInteger32(void* _pvCtx, int _iVar, unsigned int _uiData);

/**
 * create scalar integer variable data
 * @param[in] _piAddress variable name
 * @param[in] _pullData array of unsigned integer 64 bits
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
int createScalarUnsignedInteger64(void* _pvCtx, int _iVar, unsigned long long _ullData);
#endif

/**
 * create named scalar integer variable data
 * @param[in] _pstName variable name
 * @param[in] _pcData array of integer 8 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedScalarInteger8(void* _pvCtx, char* _pstName, char _cData);

/**
 * create named scalar integer variable data
 * @param[in] _pstName variable name
 * @param[in] _psData array of integer 16 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedScalarInteger16(void* _pvCtx, char* _pstName, short _sData);

/**
 * create named scalar integer variable data
 * @param[in] _pstName variable name
 * @param[in] _piData array of integer 32 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedScalarInteger32(void* _pvCtx, char* _pstName, int _iData);

/**
 * create named scalar integer variable data
 * @param[in] _pstName variable name
 * @param[in] _pllData array of integer 64 bits
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
int createNamedScalarInteger64(void* _pvCtx, char* _pstName, long long _llData);
#endif

/**
 * create named scalar integer variable data
 * @param[in] _pstName variable name
 * @param[in] _pucData array of unsigned integer 8 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedScalarUnsignedInteger8(void* _pvCtx, char* _pstName, unsigned char _ucData);

/**
 * create named scalar integer variable data
 * @param[in] _pstName variable name
 * @param[in] _pusData array of unsigned integer 16 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedScalarUnsignedInteger16(void* _pvCtx, char* _pstName, unsigned short _usData);

/**
 * create named scalar integer variable data
 * @param[in] _pstName variable name
 * @param[in] _puiData array of unsigned integer 32 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedScalarUnsignedInteger32(void* _pvCtx, char* _pstName, unsigned int _uiData);

/**
 * create named scalar integer variable data
 * @param[in] _pstName variable name
 * @param[in] _pullData array of unsigned integer 64 bits
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
int createNamedScalarUnsignedInteger64(void* _pvCtx, char* _pstName, unsigned long long _ullData);
#endif


#ifdef __cplusplus
}
#endif
#endif /* __INT_API__ */
