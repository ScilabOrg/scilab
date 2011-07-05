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

import java.io.File;

import org.scilab.modules.ui_data.FileBrowser;
import org.scilab.modules.ui_data.filebrowser.SwingScilabTreeTable;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.events.callback.CallBack;

public class ChangeCWDAction extends CallBack {

    private SwingScilabTreeTable table;

    public ChangeCWDAction(SwingScilabTreeTable table) {
        super(null);
        this.table = table;
    }

    public void callBack() {
	String baseDir = table.getComboBox().getBaseDir();
	File f = new File(baseDir);
        if (f.exists() && f.isDirectory() && f.canRead()) {
	    InterpreterManagement.requestScilabExec("chdir('" + table.getComboBox().getBaseDir() + "')");
	    FileBrowser.setBaseDir(table.getComboBox().getBaseDir());
	}
    }
}
