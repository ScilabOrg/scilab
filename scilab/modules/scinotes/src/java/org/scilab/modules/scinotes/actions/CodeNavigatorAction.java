/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
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

import javax.swing.JButton;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.tabfactory.CodeNavigatorTab;
import org.scilab.modules.scinotes.utils.NavigatorWindow;

/**
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
@SuppressWarnings("serial")
public final class CodeNavigatorAction extends DefaultAction {

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public CodeNavigatorAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return createMenu
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new CodeNavigatorAction(label, editor), key);
    }

    /**
     * Action !!
     */
    public void doAction() {
        getEditor().addNavigator();
    }

    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new CodeNavigatorAction(tooltip, editor));
    }
}
