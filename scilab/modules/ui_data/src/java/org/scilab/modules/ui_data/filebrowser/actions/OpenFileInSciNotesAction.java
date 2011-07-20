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

import java.lang.reflect.Method;

import javax.swing.JTree;
import javax.swing.tree.TreePath;

import org.scilab.modules.ui_data.FileBrowser;
import org.scilab.modules.ui_data.filebrowser.ScilabFileBrowserModel;
import org.scilab.modules.ui_data.filebrowser.SwingScilabTreeTable;
import org.scilab.modules.gui.events.callback.CallBack;

public class OpenFileInSciNotesAction extends CallBack {

    private SwingScilabTreeTable table;

    public OpenFileInSciNotesAction(SwingScilabTreeTable table) {
	super(null);
	this.table = table;
    }

    public void callBack() {
	Class scinotesClass = null;
	Method scinotes = null;
	try {
            scinotesClass = Class.forName("org.scilab.modules.scinotes.SciNotes");
            scinotes = scinotesClass.getMethod("scinotes", new Class[] {String.class});
        } catch (ClassNotFoundException e) {
            System.err.println("Could not find SciNotes class");
            e.printStackTrace();
        } catch (SecurityException e) {
            System.err.println("Security error: Could not access to SciNotes class");
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            System.err.println("Could not access to scinotesWithText method from object SciNotes");
            e.printStackTrace();
        }
	if (scinotesClass != null && scinotes != null) {
	    final Method scin = scinotes;
	    String[] paths = table.getSelectedPaths();
	    for (final String path : paths) {
		Thread t = new Thread(new Runnable() {
			public void run() {
			    try {
				scin.invoke(null, new Object[]{path});
			    } catch (Exception e) { e.printStackTrace(); }
			}
		    });
		try {
		    t.start();
		} catch (Exception e) { }
	    }
	}
    }
}
