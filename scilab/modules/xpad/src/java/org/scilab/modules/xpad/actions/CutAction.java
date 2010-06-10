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

import javax.swing.KeyStroke;
import javax.swing.JComponent;
import javax.swing.text.DefaultEditorKit;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.ScilabDocument;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * CutAction Class
 * @author Bruno JOFRET
 *
 */
public final class CutAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -4831313579986185630L;
    
    /**
     * Constructor 
     * @param editor Xpad
     */
    private CutAction(Xpad editor) {
	super(XpadMessages.CUT, editor);
    }
    
    /**
     * doAction
     */
    public void doAction() {
	ScilabDocument doc = (ScilabDocument) getEditor().getTextPane().getDocument();
	doc.mergeEditsBegin();
	getEditor().getTextPane().getActionMap().get(DefaultEditorKit.cutAction).actionPerformed(null);
	doc.mergeEditsEnd();
    }

    /**
     * createMenu
     * @param editor Xpad 
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(Xpad editor, KeyStroke key) {
	return createMenu(XpadMessages.CUT, null, new CutAction(editor), key);
    }
    
    /**
     * createButton
     * @param editor Xpad
     * @return PushButton
     */
    public static PushButton createButton(Xpad editor) {
	return createButton(XpadMessages.CUT, "edit-cut.png", new CutAction(editor));
    }
    
    /**
     * Put input map
     * @param textPane JTextpane
     * @param editor Editor
     * @param key KeyStroke
     */
    public static void putInInputMap(JComponent textPane, Xpad editor, KeyStroke key) {
	textPane.getInputMap().put(key, new CutAction(editor));
    }
}
