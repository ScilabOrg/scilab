/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xpad.actions;

import java.awt.event.KeyEvent;

import javax.swing.JComponent;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.style.TabManager;
import org.scilab.modules.xpad.utils.XpadMessages;

public class TabifyAction extends DefaultAction {

	private TabManager tabManager= new TabManager();
	
	private TabifyAction(Xpad editor) {
		super(XpadMessages.TABIFY_SELECTION, editor);
	}
	
	public synchronized void doAction()
	{
		int position_start = getEditor().getTextPane().getSelectionStart();
		int position_end   = getEditor().getTextPane().getSelectionEnd();
		ScilabStyleDocument scilabDocument = (ScilabStyleDocument) getEditor().getTextPane().getStyledDocument();
		int line_start     = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).getDefaultRootElement().getElementIndex(position_start);
		int line_end       = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).getDefaultRootElement().getElementIndex(position_end);
		if(position_start == position_end) {
			tabManager.insertTab(scilabDocument, position_start);
		} else {
			if( line_start == line_end ) {
				// A part of the line is selected : Insert a Tab at the beginning of the line
				int offset = tabManager.tabifyLine(scilabDocument, line_start);
				getEditor().getTextPane().setSelectionStart(position_start + offset);
				getEditor().getTextPane().setSelectionEnd(position_end + offset);
			} else {
				// several lines are selected
				int offset = tabManager.tabifyLines(scilabDocument, line_start, line_end);
				getEditor().getTextPane().setSelectionStart(position_start + offset);
				getEditor().getTextPane().setSelectionEnd(position_end + offset * (line_end - line_start + 1));
			}
		}
	}
	
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.TABIFY_SELECTION , null, new TabifyAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_TAB,0));
	}
	public static void putInInputMap(JComponent textPane, Xpad editor) {
		textPane.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_TAB,0), new TabifyAction(editor));
		return;
	}
}
