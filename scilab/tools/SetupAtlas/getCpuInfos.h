/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __GETCPUINFOS_H__
#define __GETCPUINFOS_H__
/*--------------------------------------------------------------------------*/
#include <wchar.h>
/*--------------------------------------------------------------------------*/
#define CPU_FAMILY_ERROR -1
#define CPU_MODEL_ERROR -1
#define CPU_STEPPING_ERROR -1
#define CPU_FREQUENCY_ERROR -1
#define CPU_PLATFORMID_ERROR -1
/*--------------------------------------------------------------------------*/
#define DEFAULT_ERROR_MESSAGE L"ERROR"
/*--------------------------------------------------------------------------*/
/**
* get CPU vendor name (GenuineIntel,AuthenticAMD, ...)
* @return CPU vendor name
*/
wchar_t *getCpuVendor(void);

/**
* get Processor Name String
* example :
* Intel(R) Core(TM)2 Quad CPU    Q6600  @ 2.40GHz
* @return Processor Name String
*/
wchar_t *getProcessorNameString(void);

/**
* get Platform ID
* @return Platform ID
*/
int getCpuPlatformID(void);

/**
* get CPU Identifier
* example :
* Intel64 Family 6 Model 15 Stepping 11
* @return CPU Identifier
*/
wchar_t *getCpuIdentifier(void);

/**
* get CPU frequency
* @return CPU frequency
*/
int getCpuFrequency(void);

/**
* get CPU Family
* @return CPU Family
*/
int getCpuFamily(void);

/**
* get CPU Model
* @return CPU Model
*/
int getCpuModel(void);

/**
* get CPU Stepping
* @return CPU Stepping
*/
int getCpuStepping(void);

/**
* get CPU Architecture used
* example :
* Intel64 or x86
* @return CPU Architecture
*/
wchar_t *getCpuArchitecture(void);
/*--------------------------------------------------------------------------*/
#endif /* __GETCPUINFOS_H__ */
/*--------------------------------------------------------------------------*/

