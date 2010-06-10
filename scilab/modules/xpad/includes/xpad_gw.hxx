/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#ifndef __XPAD_GW_HXX__
#define __XPAD_GW_HXX__

#ifdef _MSC_VER
   #if XPAD_GW_EXPORTS
      #define EXTERN_XPAD_GW __declspec (dllexport)
   #else
      #define EXTERN_XPAD_GW __declspec (dllimport)
   #endif
#else
   #define EXTERN_XPAD_GW 
#endif

class XpadModule
{
private :
   XpadModule(){};
   ~XpadModule(){};
public :
   EXTERN_XPAD_GW static bool Load();
};

#endif /* !__XPAD_GW_HXX__ */
