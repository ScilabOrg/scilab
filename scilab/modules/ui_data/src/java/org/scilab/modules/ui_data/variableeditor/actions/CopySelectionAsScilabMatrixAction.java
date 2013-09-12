/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.ui_data.variableeditor.actions;

import javax.swing.JTable;

import org.scilab.modules.commons.gui.ScilabKeyStroke;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;

/**
 * CopySelectionAsScilabMatrixAction class
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public final class CopySelectionAsScilabMatrixAction extends CopySelectionAction {

    private static final String KEY = "OSSCKEY shift E";
    private static final String COPYAS = "Copy as Scilab matrix";

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    private CopySelectionAsScilabMatrixAction(SwingScilabVariableEditor editor, String name) {
        super(editor, name, "[", "]", ",", ";");
    }

    /**
     * @param editor the editor
     * @param table where to put the action
     */
    public static void registerAction(SwingScilabVariableEditor editor, JTable table) {
        table.getActionMap().put(COPYAS, new CopySelectionAsScilabMatrixAction(editor, COPYAS));
        table.getInputMap().put(ScilabKeyStroke.getKeyStroke(KEY), COPYAS);
    }

    /**
     * Create a menu item
     * @param editor the associated editor
     * @param title the menu title
     * @return the menu item
     */
    public static MenuItem createMenuItem(SwingScilabVariableEditor editor, String title) {
        MenuItem menu = ScilabMenuItem.createMenuItem();
        menu.setCallback(new CopySelectionAsScilabMatrixAction(editor, title));
        menu.setText(title);
        ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(ScilabKeyStroke.getKeyStroke(KEY));

        return menu;
    }

    /**
     * Create a menu item as a SwingScilabMenuItem
     * @param editor the associated editor
     * @param title the menu title
     * @return the menu item
     */
    public static SwingScilabMenuItem createJMenuItem(SwingScilabVariableEditor editor, String title) {
        return (SwingScilabMenuItem) createMenuItem(editor, title).getAsSimpleMenuItem();
    }
}
