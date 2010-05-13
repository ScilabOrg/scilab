/* Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xpad.actions;

import java.util.List;

import javax.swing.JComponent;
import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.ScilabDocument;
import javax.swing.JEditorPane;
import org.scilab.modules.xpad.SearchManager;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 *
 * @author Antoine ELIAS
 *
 */
public final class FindPreviousAction extends DefaultAction {

    private static final long serialVersionUID = -9017015781643180532L;

    /**
     * Constructor
     * @param editor Xpad
     */
    protected FindPreviousAction(Xpad editor) {
        super(XpadMessages.FIND_NEXT, editor);
    }

    /**
     * DoAction
     */
    public void doAction() {
        try {
            int startPos = getEditor().getTextPane().getSelectionStart();
            int endPos = getEditor().getTextPane().getSelectionEnd();
            int startLine = ((ScilabDocument) getEditor().getTextPane().getDocument()).getDefaultRootElement().getElementIndex(startPos);
            int endLine = ((ScilabDocument) getEditor().getTextPane().getDocument()).getDefaultRootElement().getElementIndex(endPos);

            //don't manage multiple lines quick find
            if (startLine != endLine) {
                return;
            }

            String exp = null;
            if (startPos == endPos) {
                //nothing to search
                if (FindAction.getPreviousSearch() == null) {
                    return;
                } else {
                    exp = FindAction.getPreviousSearch();
                }
            } else {
                exp = getEditor().getTextPane().getDocument().getText(startPos, endPos - startPos);
            }

            JEditorPane xpadTextPane =  getEditor().getTextPane();
            ScilabDocument scilabStyle = ((ScilabDocument) xpadTextPane.getDocument());

            //search from current position to end document
            List<Integer[]> offsets = SearchManager.findWord(scilabStyle, exp, 0, scilabStyle.getLength(), false, false, false);
            if (offsets.size() != 0) {
                int index = -1;
                for (int i = 0; i < offsets.size(); i++) {
                    if (offsets.get(i)[0] >= startPos) {
                        index = i - 1;
                        break;
                    }
                }

                if (index == -1) {
                    index = offsets.size() - 1;
                }

                getEditor().getTextPane().select(offsets.get(index)[0], offsets.get(index)[1]);
            }
        } catch (BadLocationException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    /**
     * createMenu
     * @param editor Xpad
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(Xpad editor, KeyStroke key) {
        return createMenu(XpadMessages.FIND_PREVIOUS, null, new FindPreviousAction(editor), key);
    }

    /**
     * Put input map
     * @param textPane JTextpane
     * @param editor Editor
     * @param key KeyStroke
     */
    public static void putInInputMap(JComponent textPane, Xpad editor, KeyStroke key) {
	textPane.getInputMap().put(key, new FindPreviousAction(editor));
    }
    
}
