/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - S/E - Sylvestre Ledru
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
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;
import org.scilab.modules.ui_data.variablebrowser.SwingScilabVariableBrowser;
import org.scilab.modules.ui_data.BrowseVar;
import org.scilab.modules.types.ScilabTypeEnum;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.asynchronousScilabExec;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
/**
 * Export to Csv class
 * @author Sylvestre Ledru
 */
@SuppressWarnings(value = { "serial" })
public final class ExportToCsvAction extends CommonCallBack {

    private static final String KEY = "OSSCKEY S";
    private static final String CREATE = "Export to CSV";
    private static final int GAP = 5;

    private final SwingScilabDockablePanel editor;

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    public ExportToCsvAction(SwingScilabDockablePanel editor, String name) {
        super(name);
        this.editor = editor;
    }

    /**
     * @param editor the editor
     * @param table where to put the action
     */
    public static void registerAction(SwingScilabDockablePanel editor, JTable table) {
        table.getActionMap().put(CREATE, new ExportToCsvAction(editor, CREATE));
        table.getInputMap().put(ScilabKeyStroke.getKeyStroke(KEY), CREATE);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void callBack() {
        String scilabCommand = "__export__csv__=uiputfile();if __export__csv__<>'' then csvWrite(%s, __export__csv__);clear __export__csv__; end;";
        if (editor instanceof SwingScilabVariableEditor) {
            JTable table = ((SwingScilabVariableEditor)editor).getCurrentTable();
            SwingEditvarTableModel model = (SwingEditvarTableModel) table.getModel();
            model.execCommand(String.format(scilabCommand, model.getVarName()));
        } else {
            // Browse var
            String variableName;
            int clickedRow = ((SwingScilabVariableBrowser)editor).getTable().getSelectedRow();

            // Does nothing if no variable selected
            if (clickedRow != -1) {
                variableName = ((SwingScilabVariableBrowser)editor).getTable().getValueAt(clickedRow, BrowseVar.NAME_COLUMN_INDEX).toString();
                int varType = Integer.parseInt(((SwingScilabVariableBrowser)editor).getTable().getModel().getValueAt(clickedRow, BrowseVar.TYPE_COLUMN_INDEX).toString());

                try {
                    // The export is only available for double (complex or not) and strings
                    if (varType == ScilabTypeEnum.sci_matrix.swigValue() || varType == ScilabTypeEnum.sci_strings.swigValue()) {
                        asynchronousScilabExec(null, String.format(scilabCommand, variableName));
                    } else {
                        asynchronousScilabExec(null, "messagebox('" + Messages.gettext("Type not supported for this operation") + "')");
                    }
                } catch (InterpreterException e1) {
                    System.err.println("An error in the interpreter has been catched: " + e1.getLocalizedMessage());
                }

            }
        }
    }

    /**
     * Create a menu item
     * @param editor the associated editor
     * @param title the menu title
     * @return the menu item
     */
    public static MenuItem createMenuItem(SwingScilabDockablePanel editor, String title) {
        MenuItem menu = ScilabMenuItem.createMenuItem();
        menu.setCallback(new ExportToCsvAction(editor, title));
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
    public static SwingScilabMenuItem createJMenuItem(SwingScilabDockablePanel editor, String title) {
        return (SwingScilabMenuItem) createMenuItem(editor, title).getAsSimpleMenuItem();
    }

}
