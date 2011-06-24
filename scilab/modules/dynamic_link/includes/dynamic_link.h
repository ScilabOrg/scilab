/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*-----------------------------------------------------------------------------------*/
#ifndef __DYNAMIC_LINK_H__
#define __DYNAMIC_LINK_H__

#include "dynlib_dynamic_link.h"
#include "BOOL.h"
#include "machine.h" /* C2F */

#define ENTRYMAX 500

/**
* OBSOLETE 
* if *ilib == -1 
*    checks if routinename is a loaded
*    entry point 
*    the result is -1 if false 
*               or the number in the function table 
* @param[in] routinename
* @param[in/out] ilib
*/
DYNAMIC_LINK_IMPEXP void C2F(iislink)(char *routinename, int *ilib);


/**
* returns the ii functions 
* @param ii
* @param ptr on functions
*/
DYNAMIC_LINK_IMPEXP void GetDynFunc(int ii, void (**realop) ());

/**
* Search a function in the table 
* Search from end to top 
* @param 
* @param 
*/
DYNAMIC_LINK_IMPEXP int SearchInDynLinks(char *op, void (**realop) ());

/**
* unlink all linked files 
*/
DYNAMIC_LINK_IMPEXP void unlinkallsharedlib(void);

/**
* Unlink a shared lib 
* @param i (number of shared lib)
*/
DYNAMIC_LINK_IMPEXP void unlinksharedlib(int *i);


/**
* load a shared archive and call LoadDynLibrary
* the shared lib handler is stored in a Table 
* return value is == -1 if the LoadDynLibrary failed 
* @param loaded_file
*/
DYNAMIC_LINK_IMPEXP int Sci_dlopen(wchar_t* _pwstDynLibPath);

/**
* unload a shared library by calling FreeDynLibrary
* return value is == 0 if the FreeDynLibrary failed 
* @param loaded_file
*/
DYNAMIC_LINK_IMPEXP int Sci_dlclose(unsigned long _hLib);

/**
* This routine load the entryname ename 
* from shared lib ishared 
* @return TRUE or FALSE
*/
DYNAMIC_LINK_IMPEXP BOOL Sci_dlsym(wchar_t* _pwstEntryPointName, int _iLibID, BOOL _bFortran);

/**
* Delete entry points associated with shared lib ishared
* then delete the shared lib 
@param ishared
*/
DYNAMIC_LINK_IMPEXP void Sci_Delsym(int ishared);

/**
* @param sizearray returns size of string array returned
* @return string array with functions names loaded
*/
DYNAMIC_LINK_IMPEXP char **getNamesOfFunctionsInSharedLibraries(int *sizearray);

/**
* call link for scilab
* @param _iLibID                : Id of an existing shared lib otherwise -1
* @param _pwstLibraryName       : Dynamic library name
* @param _pwstEntryPointName    : List of functions name in dynamic library
* @param _iEntryPointSize       : Size of _pwstEntryPointName
* @param _bFortran              : Is a link on a fortran function
* @param _piErr                 : Error Id
* @return id                    : Dynamic Library ID
*/
DYNAMIC_LINK_IMPEXP int scilabLink(int _iLibID, wchar_t* _pwstLibraryName, wchar_t** _pwstEntryPointName, int _iEntryPointSize, BOOL _bFortran ,int *_piErr);

#endif /* __DYNAMIC_LINK_H__ */
/*-----------------------------------------------------------------------------------*/
