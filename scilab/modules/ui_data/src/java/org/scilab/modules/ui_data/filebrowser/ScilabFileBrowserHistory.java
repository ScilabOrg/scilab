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

import java.util.ArrayList;
import java.util.List;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;

import org.scilab.modules.gui.events.callback.CallBack;

/**
 *
 * @author Calixte DENIZET
 */
public class ScilabFileBrowserHistory {

    private static final Icon PREVIOUSICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/go-previous.png");
    private static final Icon NEXTICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/go-next.png");
    
    private SwingScilabTreeTable stt;
    private List<String> history = new ArrayList<String>();
    private int position = -1;
    private JButton previous;
    private JButton next;

    public ScilabFileBrowserHistory(SwingScilabTreeTable stt) {
	this.stt = stt;
    }

    public void addPathInHistory(String path) {
	history.add(path);
	updateButton(0);
    }

    public JButton getPreviousButton() {
	if (previous == null) {
	    previous = new JButton(PREVIOUSICON);
	}
	previous.setAction(new CallBack(null) {
		public void callBack() {
		    stt.setBaseDir(history.get(position - 1));
		    setPositionInHistory(position - 1);
		}
	    });
	
	return previous;
    }

    public JButton getNextButton() {
	if (next == null) {
	    next = new JButton(NEXTICON);
	}
	next.setAction(new CallBack(null) {
		public void callBack() {
		    stt.setBaseDir(history.get(position + 1));
		    setPositionInHistory(position + 1);
		}
	    });
	
	return next;
    }

    private void setPositionInHistory(int pos) {
	position = pos;
	updateButton(pos);
    }

    private void updateButton(int pos) {
	previous.setEnabled(pos != 0);
	next.setEnabled(pos != history.size() - 1);
    }
}
