/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import java.util.StringTokenizer;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * HelpOnKeywordAction Class
 * @author Calixte DENIZET
 */
public class HelpOnKeywordInPopupAction extends HelpOnKeywordAction {

    private static final long serialVersionUID = -7189187227299495917L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public HelpOnKeywordInPopupAction(String name, SciNotes editor) {
        super(name, editor);
        isPopup = true;
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        StringTokenizer token = new StringTokenizer(label, ";\uff1b");
        final String label1 = token.nextToken();
        final String label2 = token.nextToken();
        return createMenu(label1, label2, editor, key, new HelpOnKeywordInPopupAction(label1 + SciNotesMessages.DOTS, editor));
    }
}
