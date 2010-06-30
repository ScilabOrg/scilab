/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "CommandHistory.hxx"

/* fake methods for nogui mode */
/* configure --without-gui */

namespace org_scilab_modules_history_manager_commandhistory {

    CommandHistory::~CommandHistory() {}
    CommandHistory::CommandHistory(JavaVM * jvm_) {}
    void CommandHistory::appendLine (JavaVM * jvm_, char * lineToAppend){}
    void CommandHistory::loadFromFile (JavaVM * jvm_){}
    void CommandHistory::initialize (JavaVM * jvm_){}
    void CommandHistory::reset (JavaVM * jvm_){}
    void CommandHistory::deleteLine (JavaVM * jvm_, int lineNumber){}
}

