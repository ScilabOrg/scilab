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

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import javax.swing.JCheckBoxMenuItem;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.HelpOnTypingManager;

/**
 * ActivateHelpOnTypingForKeywordsAction Class
 * @author Calixte DENIZET
 *
 */
public final class ActivateHelpOnTypingForKeywordsAction extends DefaultCheckAction {

    private static final long serialVersionUID = 3793710651553964430L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public ActivateHelpOnTypingForKeywordsAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        HelpOnTypingManager.getInstance().enableKeywords(getState());
        SciNotes.activateHelpOnTyping();
    }

    /**
     * createCheckBoxMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return CheckBoxMenuItem
     */
    public static CheckBoxMenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        final CheckBoxMenuItem cb = createCheckBoxMenu(label, null, new ActivateHelpOnTypingForKeywordsAction(label, editor), key);
        cb.setChecked(HelpOnTypingManager.getInstance().isKeywordsActive());
        ((JCheckBoxMenuItem) cb.getAsSimpleCheckBoxMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent e) {
                cb.setChecked(HelpOnTypingManager.getInstance().isKeywordsActive());
            }
        });

        return cb;
    }
}
