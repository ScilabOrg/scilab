/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - Calixte DENIZET
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
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.ScilabDocument;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * Class to handle "select all"
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
public final class SelectAllAction extends DefaultAction {

    /**
     * Constructor
     */
    private SelectAllAction(Xpad editor) {
        super(XpadMessages.SELECT_ALL, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ((ScilabDocument) getEditor().getTextPane().getDocument()).mergeEditsEnd();
        ((ScilabDocument) getEditor().getTextPane().getDocument()).mergeEditsBegin();
        getEditor().getTextPane().getActionMap().get(DefaultEditorKit.selectAllAction).actionPerformed(null);
    }

    /**
     * createMenu
     * @param editor Xpad
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(Xpad editor, KeyStroke key) {
        return createMenu(XpadMessages.SELECT_ALL, null, new SelectAllAction(editor), key);
    }

    /**
     * Put input map
     * @param textPane JTextpane
     * @param editor Editor
     * @param key KeyStroke
     */
    public static void putInInputMap(JComponent textPane, Xpad editor, KeyStroke key) {
        textPane.getInputMap().put(key, new SelectAllAction(editor));
    }
}
