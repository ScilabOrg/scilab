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

import java.awt.Desktop;
import java.io.File;

import javax.swing.SwingUtilities;

import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.ui_data.filebrowser.SwingScilabTreeTable;
import org.scilab.modules.ui_data.utils.UiDataMessages;

public class OpenFileWithDefaultAppAction extends CallBack {

    private SwingScilabTreeTable table;

    public OpenFileWithDefaultAppAction(SwingScilabTreeTable table) {
        super(null);
        this.table = table;
    }

    public static boolean isSupported() {
	return Desktop.isDesktopSupported() && Desktop.getDesktop().isSupported(Desktop.Action.OPEN);
    }

    public void callBack() {
	Desktop dt = Desktop.getDesktop();
	File[] files = table.getSelectedFiles();
        for (File file : files) {
	    try {
		dt.open(file);
	    } catch (Exception e) {
		ScilabModalDialog.show((Tab) SwingUtilities.getAncestorOfClass(Tab.class, table), String.format(UiDataMessages.CANNOTOPEN, file.getName()));
	    }
	}
    }
}
