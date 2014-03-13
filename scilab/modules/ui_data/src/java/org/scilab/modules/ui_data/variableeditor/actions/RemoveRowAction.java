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

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JMenuItem;
import javax.swing.JTable;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.variableeditor.TableVariableEditor;

/**
 * Set Precision Action class
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public final class RemoveRowAction extends CommonCallBack {

    private final JTable table;

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    public RemoveRowAction(JTable table, String name) {
        super(name);
        this.table = table;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void callBack() {
        int[] rows = table.getSelectedRows();
        if (rows.length == 0) {
            rows = new int[] {((TableVariableEditor) table).getPopupRow()};
        }
        SwingEditvarTableModel model = (SwingEditvarTableModel) table.getModel();
        model.removeRows(rows);
        table.clearSelection();
    }

    /**
     * Create a button for a tool bar
     * @param editor the associated editor
     * @param title tooltip for the button
     * @return the button
     */
    public static JButton createButton(JTable table, String title) {
        JButton button = new JButton();
        ScilabLAF.setDefaultProperties(button);
        button.addActionListener(new RemoveRowAction(table, title));
        button.setToolTipText(title);
        ImageIcon imageIcon = new ImageIcon(FindIconHelper.findIcon("short"));
        button.setIcon(imageIcon);

        return button;
    }

    /**
     * Create a menu item
     * @param editor the associated editor
     * @param title the menu title
     * @return the menu item
     */
    public static JMenuItem createMenuItem(JTable table, String title) {
        JMenuItem mi = new JMenuItem(title);
        mi.addActionListener(new RemoveRowAction(table, title));

        return mi;
    }
}
