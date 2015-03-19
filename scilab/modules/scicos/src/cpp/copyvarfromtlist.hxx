/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
 *  Copyright (C) DIGITEO - 2009 - Allan CORNET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __COPYVARFROMTLIST_HXX__
#define __COPYVARFROMTLIST_HXX__

#include "types.hxx"
#include "internal.hxx"

/* CopyVarFromTList
*    Copy a Scilab object in a tlist 'header' to the variable 'element'
*
* Calling sequence :
*  bool CopyVarFromTList(types::InternalType* &element, types::InternalType* header, const int i)
*
* Input parameters : element : InternalType*, pointer to the returned element
*                    header : TList*, pointer to the tlist containing 'element'.
*                    i : integer, the index of 'element' in 'header'
*
* Output : 'false' if failed, 'true' else.
*
* Examples of use
*
* 1 -  put the third element of a tlist 'header' in 'element' :
*
*  types::InternalType* element;
*  types::TList* tlist = get_il_state();
*  CopyVarFromTList(element, tlist, 4)
*/

bool CopyVarFromTList(types::InternalType* &element, types::InternalType* header, const int i);

#endif /* __COPYVARFROMTLIST_HXX__ */
