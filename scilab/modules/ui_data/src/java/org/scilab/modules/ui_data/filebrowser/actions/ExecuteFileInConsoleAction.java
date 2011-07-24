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

package org.scilab.modules.ui_data.filebrowser.actions;

import org.scilab.modules.ui_data.filebrowser.SwingScilabTreeTable;

public class ExecuteFileInConsoleAction extends ExecuteCommandAction {

    private static final String EXEC = "exec('%s',-1)";

    public ExecuteFileInConsoleAction(SwingScilabTreeTable table) {
        super(table, EXEC);
    }
}
