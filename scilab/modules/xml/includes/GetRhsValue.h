/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/

#ifndef __XML_GETRHSVALUE__
#define __XML_GETRHSVALUE__(TYPE) bool getRhsValue(char * fname, int * addr, TYPE ** obj);

//__XML_GETRHSVALUE__(XMLDocument)
__XML_GETRHSVALUE__(XMLElement)
__XML_GETRHSVALUE__(XMLAttr)
__XML_GETRHSVALUE__(XMLNodeList)
__XML_GETRHSVALUE__(XMLNs)
bool getRhsValue(char * fname, int * addr, XMLDocument ** obj);
bool getRhsValue(char * fname, int * addr, std::string ** obj);
#endif
