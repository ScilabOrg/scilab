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

import javax.swing.SwingUtilities;

import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.ui_data.filebrowser.SwingScilabTreeTable;

public class LoadFileAsGraphAction extends CallBack {

    private SwingScilabTreeTable table;

    public LoadFileAsGraphAction(SwingScilabTreeTable table) {
        super(null);
        this.table = table;
    }

    public void callBack() {
	String[] paths = table.getSelectedPaths();
        for (String path : paths) {
	    try {
                String cmd = "load_graph('" + path + "')";
                ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(cmd, true /* display */, true /* store in history */);
            } catch (NoClassDefFoundError e) {
                ScilabModalDialog.show((Tab) SwingUtilities.getAncestorOfClass(Tab.class, table), Messages.gettext("Feature not available in this mode..."));
            }
        }
    }
}
