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
import java.awt.Color;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JViewport;
import javax.swing.SwingUtilities;

import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.ui_data.filebrowser.actions.ChangeCWDAction;

public class ScilabFileBrowserComponent extends JPanel {

    private SwingScilabFileBrowser filebrowser;
    private SwingScilabTreeTable stt;

    public ScilabFileBrowserComponent() {
	super(new BorderLayout());
	ScilabFileSelectorComboBox combobox = new ScilabFileSelectorComboBox();
	stt = new SwingScilabTreeTable(new ScilabFileBrowserModel(), combobox);
	Color bg = Color.WHITE;
	stt.setBackground(bg);
        add(combobox, BorderLayout.PAGE_START);
	JScrollPane jsp = new JScrollPane(stt);
	jsp.getViewport().setScrollMode(JViewport.BACKINGSTORE_SCROLL_MODE);
	jsp.getViewport().setBackground(bg);
	add(jsp, BorderLayout.CENTER);
    }

    public void setBaseDir(final String baseDir) {
	SwingUtilities.invokeLater(new Runnable() {
		public void run() {
		    stt.setBaseDir(baseDir);
		}
	    });
    }

    public SwingScilabFileBrowser getFileBrowser() {
	return filebrowser;
    }
}