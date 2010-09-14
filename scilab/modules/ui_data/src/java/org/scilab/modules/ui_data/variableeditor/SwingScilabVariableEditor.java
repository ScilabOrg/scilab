/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.variableeditor;

import java.awt.Color;
import java.awt.Component;
import java.awt.Toolkit;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.StringSelection;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.io.IOException;
import java.util.StringTokenizer;

import javax.swing.AbstractAction;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTable;
import javax.swing.KeyStroke;
import javax.swing.ListSelectionModel;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.rowheader.RowHeader;
import org.scilab.modules.ui_data.rowheader.RowHeaderModel;
import org.scilab.modules.ui_data.utils.UiDataMessages;
import org.scilab.modules.ui_data.variableeditor.action.RefreshAction;
import org.scilab.modules.ui_data.variableeditor.renderers.RendererFactory;
import org.scilab.modules.ui_data.variableeditor.celleditor.CellEditorFactory;
import org.scilab.modules.ui_data.variableeditor.celleditor.ScilabGenericCellEditor;

/**
 * Swing implementation of Scilab Variable Editor
 * uses JTable
 * @author Allan SIMON
 * @author Calixte DENIZET
 */
public class SwingScilabVariableEditor extends SwingScilabTab implements Tab, SimpleVariableEditor {

    /**
     * Prefix used in the tabs titles.
     */
    public static final String PREFIX = "Var - ";

    private static final long serialVersionUID = 1L;

    private SwingEditvarTableModel dataModel;
    private JTabbedPane tabPane;
    private JScrollPane scrollPane;

    /**
     * Create a JTable with data Model.
     * @param data : The JTable data.
     */
    public SwingScilabVariableEditor(String name, String type, Object[][] data) {
        super(Messages.gettext("Variable Editor"));
        tabPane = new JTabbedPane();
        setContentPane(tabPane);
        setData(name, type, data);
	ToolBar toolBar = ScilabToolBar.createToolBar();
	PushButton pb = RefreshAction.createButton(this, UiDataMessages.REFRESH, "view-refresh.png");
	toolBar.add(pb);
	addToolBar(toolBar);
    }

    /**
     * {@inheritDoc}
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        setInfoBar(infoBarToAdd);
    }

    /**
     * {@inheritDoc}
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        setMenuBar(menuBarToAdd);
    }

    /**
     * {@inheritDoc}
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        setToolBar(toolBarToAdd);
    }

    /**
     * @return the TabbedPane used in this window
     */
    public JTabbedPane getTabPane() {
        return tabPane;
    }

    /**
     * @return the current table
     */
    public JTable getCurrentTable() {
        return (JTable) ((JScrollPane) tabPane.getSelectedComponent()).getViewport().getComponent(0);
    }

    /**
     * {@inheritDoc}
     */
    public void setData(String name, String type, Object[][] data) {
        JTable table = new JTable();
        scrollPane = new JScrollPane(table);
        table.setFillsViewportHeight(true);
        table.setAutoResizeMode(CENTER);
        table.setRowHeight(25);

        updateData(table, type, data);

        table.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
        table.setCellSelectionEnabled(true);

        table.setBackground(Color.WHITE);
        tabPane.add(PREFIX + name, scrollPane);
        tabPane.setSelectedComponent(scrollPane);
    }

    /**
     * @param table the table to update
     * @param type the type of the matrix
     * @param data the matrix datas
     */
    public void updateData(JTable table, String type, Object[][] data) {
        ScilabGenericCellEditor cellEditor = (ScilabGenericCellEditor) CellEditorFactory.createCellEditor(type);
        if (table.getModel() instanceof SwingEditvarTableModel) {
            cellEditor.setExpressions(((SwingEditvarTableModel) table.getModel()).getCellEditor().getExpressions());
        }
        dataModel = new SwingEditvarTableModel(type, data, cellEditor);
        table.setModel(dataModel);
        RowHeaderModel rowHeaderModel = new RowHeaderModel(dataModel);
        RowHeader rowHeader = new RowHeader(rowHeaderModel, table);
        scrollPane.setRowHeaderView(rowHeader);
        table.setDefaultEditor(Object.class, cellEditor);
        table.setDefaultRenderer(Object.class, RendererFactory.createRenderer(type));
        addCopyPaste(table);
    }

    /**
     * @param c the component in the tabbedpane
     * @param type the type of the matrix
     * @param data the matrix datas
     */
    public void updateData(Component c, String type, Object[][] data) {
        tabPane.setSelectedComponent(c);
        JTable table = (JTable) ((JScrollPane) c).getViewport().getComponent(0);
        updateData(table, type, data);
    }

    /**
     * {@inheritDoc}
     */
    public SimpleTab getAsSimpleTab() {
        return this;
    }

    /**
     * {@inheritDoc}
     */
    public Window getParentWindow() {
        return (Window) UIElementMapper.getCorrespondingUIElement(getParentWindowId());
    }

    /**
     * @param table where to add copy/paste action
     */
    private void addCopyPaste(final JTable table) {
        KeyStroke copy = KeyStroke.getKeyStroke("ctrl C");
        KeyStroke paste = KeyStroke.getKeyStroke("ctrl V");
        AbstractAction copyAction = new AbstractAction() {
                public void actionPerformed(ActionEvent e) {
                    int[] cols = table.getSelectedColumns();
                    int[] rows = table.getSelectedRows();
                    table.setColumnSelectionInterval(cols[0], cols[cols.length - 1]);
                    table.setRowSelectionInterval(rows[0], rows[rows.length - 1]);
                    StringBuffer buf = new StringBuffer();
                    for (int i = rows[0]; i <= rows[rows.length - 1]; i++) {
                        for (int j = cols[0]; j <= cols[cols.length - 1]; j++) {
                            String exp = ((SwingEditvarTableModel) table.getModel()).getCellEditor().getExpression(i, j);
                            if (exp != null) {
                                buf.append("=" + exp);
                            } else {
                                buf.append(((SwingEditvarTableModel) table.getModel()).getScilabValueAt(i, j));
                            }
                            if (j < cols[cols.length - 1]) {
                                buf.append("\t");
                            }
                        }
                        buf.append("\n");
                    }
                    StringSelection sel  = new StringSelection(buf.toString());
                    Toolkit.getDefaultToolkit().getSystemClipboard().setContents(sel, sel);
                }
            };

        AbstractAction pasteAction = new AbstractAction() {
                public void actionPerformed(ActionEvent e) {
                    int col = table.getSelectedColumn();
                    int row = table.getSelectedRow();
                    table.setColumnSelectionInterval(col, col);
                    table.setRowSelectionInterval(row, row);
                    String str = "";
                    try {
                        str = (String) Toolkit.getDefaultToolkit().getSystemClipboard().getContents(this).getTransferData(DataFlavor.stringFlavor);
                    } catch (UnsupportedFlavorException ex1) {
                        System.err.println(ex1);
                    } catch (IOException ex2) {
                        System.err.println(ex2);
                    }
                    StringTokenizer rElems = new StringTokenizer(str, "\n");
                    for (int i = 0; rElems.hasMoreTokens(); i++) {
                        StringTokenizer cElems = new StringTokenizer(rElems.nextToken(), "\t");
                        for (int j = 0; cElems.hasMoreTokens(); j++) {
                            String value = (String) cElems.nextToken();
                            ((SwingEditvarTableModel) table.getModel()).setValueAtAndUpdate(false, value, row + i, col + j);
                        }
                    }
                    ((SwingEditvarTableModel) table.getModel()).updateMatrix();
                }
            };

        table.getActionMap().put("Copy", copyAction);
        table.getInputMap().put(copy, "Copy");
        table.getActionMap().put("Paste", pasteAction);
        table.getInputMap().put(paste, "Paste");
    }
}
