/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.CompoundUndoManager;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.ScilabDocument;

/**
 * CapitalizeAction Class
 * @author Calixte DENIZET
 */
public final class CapitalizeAction extends DefaultAction {

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public CapitalizeAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = (ScilabEditorPane) getEditor().getTextPane();
        ScilabDocument doc = (ScilabDocument) sep.getDocument();
        String str;
        int start = sep.getSelectionStart();
        int end = sep.getSelectionEnd();

        if (start == end) {
            try {
                str = doc.getText(start, 1).toUpperCase();
                doc.mergeEditsBegin();
                ((CompoundUndoManager) doc.getUndoManager()).enableOneShot(true);
                sep.select(start, start + 1);
                sep.replaceSelection(str);
                doc.mergeEditsEnd();
                ((CompoundUndoManager) doc.getUndoManager()).enableOneShot(false);
            } catch (BadLocationException e) { }
        } else {
            str = sep.getSelectedText();
            boolean nonLetter = true;
            char[] buf = str.toCharArray();
            for (int i = 0; i < buf.length; i++) {
                char c = buf[i];
                if (Character.isLetter(c)) {
                    if (nonLetter) {
                        buf[i] = Character.toUpperCase(c);
                        nonLetter = false;
                    }
                } else {
                    nonLetter = true;
                }
            }

            doc.mergeEditsBegin();
            sep.replaceSelection(new String(buf));
            sep.select(start, end);
            doc.mergeEditsEnd();
        }
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new CapitalizeAction(label, editor), key);
    }
}
