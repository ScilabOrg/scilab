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

package org.scilab.modules.ui_data.filebrowser;

import java.awt.BorderLayout;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.SwingUtilities;

import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.ui_data.filebrowser.actions.ChangeCWDAction;

public class ScilabFileBrowserComponent extends JPanel {

    private SwingScilabFileBrowser filebrowser;
    private ScilabFileSelectorComboBox combobox;
    private SwingScilabTreeTable stt;

    public ScilabFileBrowserComponent() {
	super(new BorderLayout());
	stt = new SwingScilabTreeTable(new ScilabFileBrowserModel());
	combobox = new ScilabFileSelectorComboBox("/");
	combobox.setAction(new ChangeCWDAction(combobox));
	add(combobox, BorderLayout.PAGE_START);
	add(new JScrollPane(stt), BorderLayout.CENTER);
    }

    public void setBaseDir(final String baseDir) {
	long t = System.currentTimeMillis();
	SwingUtilities.invokeLater(new Runnable() {
		public void run() {
		    combobox.setBaseDir(baseDir);
		    stt.setBaseDir(baseDir);
		}
	    });
	System.out.println(System.currentTimeMillis() - t);
    }

    public SwingScilabFileBrowser getFileBrowser() {
	return filebrowser;
    }
}