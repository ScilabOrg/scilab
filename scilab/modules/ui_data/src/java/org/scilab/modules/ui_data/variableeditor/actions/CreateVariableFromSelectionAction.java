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

package org.scilab.modules.ui_data.variableeditor.actions;

import java.awt.Toolkit;
import java.awt.datatransfer.StringSelection;

import javax.swing.KeyStroke;
import javax.swing.ImageIcon;
import javax.swing.JMenuItem;
import javax.swing.JTable;

import org.scilab.modules.commons.gui.ScilabKeyStroke;

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;

/**
 * RefreshAction class
 * @author Calixte DENIZET
 */
public final class CreateVariableFromSelectionAction extends CallBack {

    private static final String KEY = "OSSCKEY K";
    private static final String CREATE = "Create";

    private SwingScilabVariableEditor editor;

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    private CreateVariableFromSelectionAction(SwingScilabVariableEditor editor, String name) {
        super(name);
        this.editor = editor;
    }

    /**
     * @param editor the editor
     * @param table where to put the action
     */
    public static void registerAction(SwingScilabVariableEditor editor, JTable table) {
        table.getActionMap().put(CREATE, new CreateVariableFromSelectionAction(editor, CREATE));
        table.getInputMap().put(ScilabKeyStroke.getKeyStroke(KEY), CREATE);
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        JTable table = editor.getCurrentTable();
        int[] cols = table.getSelectedColumns();
        int[] rows = table.getSelectedRows();
        if (cols.length > 0 && rows.length > 0) {
            table.setColumnSelectionInterval(cols[0], cols[cols.length - 1]);
            table.setRowSelectionInterval(rows[0], rows[rows.length - 1]);

            SwingEditvarTableModel model = (SwingEditvarTableModel) table.getModel();
            int rowC = model.getScilabMatrixRowCount();
            int colC = model.getScilabMatrixColCount();

            if (rows[0] >= rowC || cols[0] >= colC) {
                return;
            }

            int rowE = Math.min(rows[rows.length - 1], rowC);
            int colE = Math.min(cols[cols.length - 1], colC);

            model.createVariable(rows[0], rowE, cols[0], colE);
        }
    }

    /**
     * Create a button for a tool bar
     * @param editor the associated editor
     * @param title tooltip for the button
     * @return the button
     */
    public static PushButton createButton(SwingScilabVariableEditor editor, String title) {
        PushButton button = ScilabPushButton.createPushButton();
        ((SwingScilabPushButton) button.getAsSimplePushButton()).addActionListener(new CreateVariableFromSelectionAction(editor, title));
        button.setToolTipText(title);
        ImageIcon imageIcon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/actions/variable-new.png");
        ((SwingScilabPushButton) button.getAsSimplePushButton()).setIcon(imageIcon);

        return button;
    }

    /**
     * Create a menu item
     * @param editor the associated editor
     * @param title the menu title
     * @return the menu item
     */
    public static JMenuItem createMenuItem(SwingScilabVariableEditor editor, String title) {
        JMenuItem mi = new JMenuItem(title);
        mi.addActionListener(new CreateVariableFromSelectionAction(editor, title));
        mi.setAccelerator(ScilabKeyStroke.getKeyStroke(KEY));

        return mi;
    }
}
