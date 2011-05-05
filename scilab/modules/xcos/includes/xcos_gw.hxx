/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __XCOS_GW_HXX__
#define __XCOS_GW_HXX__

#ifdef _MSC_VER
   #if XCOS_GW_EXPORTS
      #define EXTERN_XCOS_GW __declspec (dllexport)
   #else
      #define EXTERN_XCOS_GW __declspec (dllimport)
   #endif
#else
   #define EXTERN_XCOS_GW
#endif

class XcosModule
{
private :
    XcosModule() {};
    ~XcosModule() {};

    static bool loadedDep;
    static void LoadDeps(void);

public :
    EXTERN_XCOS_GW static bool Load();
};

#endif /* !__XCOS_GW_HXX__ */
