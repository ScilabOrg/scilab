/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.ui_data.variableeditor;

import java.awt.Component;
import java.util.HashMap;
import java.util.Map;

import javax.swing.JTable;
import javax.swing.SwingUtilities;
import javax.swing.event.ChangeListener;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.localization.Messages;

/**
 * Class ScilabVariableEditor
 * Implements a ScilabWindow containing Variable Editor (JTable)
 */
public final class ScilabVariableEditor extends SwingScilabWindow implements VariableEditor {

    private static Map<String, Component> map = new HashMap();

    private static ScilabVariableEditor instance;

    private static SwingScilabVariableEditor editorTab;
    private static ScilabTabbedPane tabPane;

    /**
     * Constructor
     * @param type the variable type
     * @param data the JTable data.
     * @param variableName the name in Scilab of the variable being edited.
     */
    private ScilabVariableEditor(String type, Object[][] data, String variableName) {
        super();
        editorTab = new SwingScilabVariableEditor(variableName, type, data);
        tabPane = editorTab.getTabPane();
        TextBox infobar = ScilabTextBox.createTextBox();
        editorTab.addInfoBar(infobar);
        addTab(editorTab);
        map.put(variableName, tabPane.getSelectedComponent());
    }

    /**
     * Close Variable Editor
     */
    public void close() {
        SwingScilabWindow editvarWindow = SwingScilabWindow.allScilabWindows.get(editorTab.getParentWindowId());
        ChangeListener[] cl = tabPane.getChangeListeners();
        for (int i = 0; i < cl.length; i++) {
            tabPane.removeChangeListener(cl[i]);
        }
        instance = null;
        map.clear();
        super.close();
    }

    /**
     * Close the edition of the variable
     * @param name the variable
     */
    public static void close(String name) {
        map.remove(name);
    }

    /**
     * Set data displayed in JTable
     * @param name the variable name
     * @param type the variable type
     * @param data : data to be displayed in JTable
     */
    public void updateData(String name, String type, Object[][] data) {
        if (map.containsKey(name)) {
            editorTab.updateData(map.get(name), name, type, data);
        } else {
            editorTab.setData(name, type, data);
            map.put(name, tabPane.getSelectedComponent());
        }
    }

    /**
     * Set data displayed in JTable
     * @param name the variable name
     * @param type the variable type
     * @param data : data to be displayed in JTable
     */
    public void updateData(String name, String type, Object[][] data, double[] rowsIndex, double[] colsIndex) {
        if (map.containsKey(name)) {
            ((SwingScilabVariableEditor) editorTab).updateData(map.get(name), name, type, data, rowsIndex, colsIndex);
        }
    }

    /**
     * Get the variable editor singleton
     * @return the Variable Editor
     */
    public static ScilabVariableEditor getVariableEditor() {
        return instance;
    }

    /**
     * Get the variable editor singleton with specified data.
     * @param type the variable type
     * @param data : the data to fill the editor with
     * @param variableName : the scilab name of the variable being edited.
     * @return the Variable Editor
     */
    public static ScilabVariableEditor getVariableEditor(final String type, final Object[][] data, final String variableName) {
        if (instance == null) {
            instance = new ScilabVariableEditor(type, data, variableName);
            instance.setVisible(true);
        } else {
            SwingScilabWindow window = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, editorTab);
            window.setVisible(true);
            window.toFront();
            final JTable table = ((SwingScilabVariableEditor) editorTab).getCurrentTable();
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    int r = -1;
                    int c = -1;

                    if (table != null) {
                        r = table.getSelectedRow();
                        c = table.getSelectedColumn();
                    }
                    boolean b = map.containsKey(variableName);
                    instance.updateData(variableName, type, data);
                    if (b && r != -1 && c != -1) {
                        table.setRowSelectionInterval(r, r);
                        table.setColumnSelectionInterval(c, c);
                    }
                }
            });
        }

        editorTab.setName(Messages.gettext("Variable Editor") + " - " + variableName + "  (" + type + ")");
        return instance;
    }

    public static void refreshVariableEditor(final String type, final Object[][] data, double[] rowsIndex, double[] colsIndex, final String variableName) {
        if (instance != null) {
            instance.updateData(variableName, type, data, rowsIndex, colsIndex);
        }
    }

    /**
     * {@inheritDoc}
     */
    public String getVariablename() {
        String title = tabPane.getScilabTitleAt(tabPane.getSelectedIndex());
        return title.substring(SwingScilabVariableEditor.PREFIX.length());
    }

    /**
     * {@inheritDoc}
     */
    public void setVariableName(String variableName) {
        tabPane.setTitleAt(tabPane.getSelectedIndex(), SwingScilabVariableEditor.PREFIX + variableName);
    }

    /**
     * {@inheritDoc}
     */
    public void setVisible(boolean status) {
        super.setVisible(status);
        editorTab.setVisible(status);
    }

    /**
     * This method is called by the OSXAdapter class when the specific Mac
     * OS X about menu is called. It is the only case where this method
     * should be used
     */
    public void OSXabout() {
        InterpreterManagement.requestScilabExec("about()");
    }

    public void OSXquit() {
        InterpreterManagement.requestScilabExec("exit()");
    }
}
