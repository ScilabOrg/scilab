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
import javax.swing.text.Element;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;

/**
 * Class for paste action
 * @author Calixte DENIZET
 */
public class RepeatSelectionOnColumnAction extends DefaultAction {

    /**
     * Constructor
     * @param name the name of the action
     * @param editor to associate
     */
    public RepeatSelectionOnColumnAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Action !
     */
    public void doAction() {
        ScilabEditorPane pane = getEditor().getTextPane();
        ScilabDocument doc = (ScilabDocument) pane.getDocument();
        Element root = doc.getDefaultRootElement();
        String str = pane.getSelectedText();
        boolean select = false;

        if (str == null) {
            try {
                str = doc.getText(pane.getCaretPosition(), 1);
            } catch (BadLocationException e) { }
        } else {
            select = true;
        }

        if (str != null) {
            doc.mergeEditsBegin();
            int start = pane.getColumnSelectionStart();
            int end = pane.getColumnSelectionEnd();
            int index = root.getElementIndex(end);
            int col = start - root.getElement(root.getElementIndex(start)).getStartOffset();
            if (index == root.getElementCount() - 1) {
                try {
                    doc.insertString(root.getElement(index).getEndOffset() - 1, "\n", null);
                } catch (BadLocationException e) { }
            }

            Element elem = root.getElement(index + 1);
            end = elem.getEndOffset() - 1;
            String insert = whites(elem.getStartOffset() + col - end);
            try {
                doc.insertString(end, insert, null);
            } catch (BadLocationException e) { }

            int pos = root.getElement(index + 1).getStartOffset() + col;
            int[][] positions = PasteColumnAction.pasteColumnString(doc, str, pos);

            doc.mergeEditsEnd();
            if (select) {
                pane.select(positions);
            } else {
                pane.setCaretPosition(pos);
            }
        }
    }

    /**
     * Generates a string of whites
     * @param n the whites number
     * @param the string
     */
    private static String whites(int n) {
        if (n > 0) {
            char[] buf = new char[n];
            for (int i = 0; i < n; i++) {
                buf[i] = ' ';
            }

            return new String(buf);
        }
        return "";
    }

    /**
     * Create the MenuItem for print action
     * @param label label of the menu
     * @param editor Editor
     * @param key KeyStroke
     * @return a MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new RepeatSelectionOnColumnAction(label, editor), key);
    }
}
