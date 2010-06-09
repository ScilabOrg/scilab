/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __CONFIGVARIABLE_HXX__
#define __CONFIGVARIABLE_HXX__

#include <list>
#include <string>
#include "dynlib_system_env.h"

class EXTERN_SYSTEM_ENV ConfigVariable
{
    //module list
private :
    static std::list<std::string> m_ModuleList;

public :
    static bool setModuleList(std::list<std::string>& _module_list);
    static std::list<std::string> getModuleList();


    //SCI
private :
    static std::string m_SCIPath;

public :
    static bool setSCIPath(std::string& _SCIPath);
    static std::string getSCIPath();

    //HOME
private :
    static std::string m_SciHome;

public :
    static bool setSCIHOME(std::string& _SciHome);
    static std::string getSCIHOME();

// Force Quit
private :
    static bool m_bForceQuit;

public : 
    static bool setForceQuit(bool _bForceQuit);
    static bool getForceQuit(void);

// Exit Status
private :
    static int m_iExitStatus;
    
public : 
    static bool setExitStatus(int _iExitStatus);
    static int getExitStatus(void);

};

#endif /* __CONFIGVARIABLE_HXX__ */
