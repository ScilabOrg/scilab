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

import java.awt.Component;

import javax.swing.ImageIcon;
import javax.swing.JMenu;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.SwingUtilities;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.TableModelEvent;
import javax.swing.table.TableModel;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.rowheader.RowHeader;
import org.scilab.modules.ui_data.utils.UiDataMessages;
import org.scilab.modules.ui_data.variableeditor.actions.CopyAction;
import org.scilab.modules.ui_data.variableeditor.actions.CreateVariableFromSelectionAction;
import org.scilab.modules.ui_data.variableeditor.actions.CutAction;
import org.scilab.modules.ui_data.variableeditor.actions.PasteAction;
import org.scilab.modules.ui_data.variableeditor.actions.PlotAction;
import org.scilab.modules.ui_data.variableeditor.actions.RedoAction;
import org.scilab.modules.ui_data.variableeditor.actions.RefreshAction;
import org.scilab.modules.ui_data.variableeditor.actions.SetPrecisionShortAction;
import org.scilab.modules.ui_data.variableeditor.actions.SetPrecisionShorteAction;
import org.scilab.modules.ui_data.variableeditor.actions.SetPrecisionLongAction;
import org.scilab.modules.ui_data.variableeditor.actions.SetPrecisionLongeAction;
import org.scilab.modules.ui_data.variableeditor.actions.SizeColumnsToFitAction;
import org.scilab.modules.ui_data.variableeditor.actions.SupprAction;
import org.scilab.modules.ui_data.variableeditor.actions.UndoAction;
import org.scilab.modules.ui_data.variableeditor.renderers.RendererFactory;
import org.scilab.modules.ui_data.variableeditor.celleditor.CellEditorFactory;
import org.scilab.modules.ui_data.variableeditor.celleditor.ScilabGenericCellEditor;
import org.scilab.modules.ui_data.variableeditor.undo.CellsUndoManager;

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

    private static final String APOS = "'";
    private static final long serialVersionUID = 1L;

    private ScilabTabbedPane tabPane;
    private PushButton refreshButton;
    private PushButton undoButton;
    private PushButton redoButton;

    /**
     * Create a JTable with data Model.
     * @param name the variable name
     * @param type the variable type
     * @param data : The JTable data.
     */
    public SwingScilabVariableEditor(String name, String type, Object[][] data) {
        super(UiDataMessages.VARIABLE_EDITOR);
        setWindowIcon(new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/32x32/apps/rrze_table.png").getImage());
        refreshButton = RefreshAction.createButton(this, UiDataMessages.REFRESH);
        undoButton = UndoAction.createButton(this, UiDataMessages.UNDO);
        redoButton = RedoAction.createButton(this, UiDataMessages.REDO);
        enableUndoButton(false);
        enableRedoButton(false);
        tabPane = new ScilabTabbedPane(this);
        tabPane.addChangeListener(new ChangeListener() {
                public void stateChanged(ChangeEvent e) {
                    String name = tabPane.getScilabTitleAt(tabPane.getSelectedIndex());
                    if (name.length() != 0) {
                        name = name.substring(PREFIX.length());
                        String tooltip = "";
                        if (name != null && getCurrentModel() != null) {
                            tooltip = UiDataMessages.REFRESH + APOS + name + APOS;
                            String type = ((SwingEditvarTableModel) getCurrentModel()).getType();
                            String title = UiDataMessages.VARIABLE_EDITOR + " - " + name + "  (" + type + ")";
                            setName(title);
                            SwingScilabWindow window = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, tabPane);
                            if (window != null) {
                                window.setTitle(title);
                            }
                        }
                        CellsUndoManager undoManager = ((SwingEditvarTableModel) getCurrentModel()).getUndoManager();
                        enableUndoButton(undoManager.canUndo());
                        enableRedoButton(undoManager.canRedo());
                        refreshButton.setToolTipText(tooltip);
                    }
                }
            });
        setContentPane(tabPane);
        setData(name, type, data);
        ToolBar toolBar = ScilabToolBar.createToolBar();
        toolBar.add(refreshButton);
        toolBar.addSeparator();
        toolBar.add(CutAction.createButton(this, UiDataMessages.CUT));
        toolBar.add(CopyAction.createButton(this, UiDataMessages.COPY));
        toolBar.add(PasteAction.createButton(this, UiDataMessages.PASTE));
        toolBar.addSeparator();
        toolBar.add(undoButton);
        toolBar.add(redoButton);
        toolBar.addSeparator();
        toolBar.add(CreateVariableFromSelectionAction.createButton(this, UiDataMessages.CREATE));
        toolBar.addSeparator();
        toolBar.add(SizeColumnsToFitAction.createButton(this, UiDataMessages.FIT));
        toolBar.addSeparator();
        toolBar.add(SetPrecisionShortAction.createButton(this, UiDataMessages.SHORT));
        toolBar.add(SetPrecisionShorteAction.createButton(this, UiDataMessages.SHORTE));
        toolBar.add(SetPrecisionLongAction.createButton(this, UiDataMessages.LONG));
        toolBar.add(SetPrecisionLongeAction.createButton(this, UiDataMessages.LONGE));

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
    public ScilabTabbedPane getTabPane() {
        return tabPane;
    }

    /**
     * @return the current table
     */
    public JTable getCurrentTable() {
        Component c = tabPane.getSelectedComponent();
        if (c != null) {
            return (JTable) ((JScrollPane) c).getViewport().getComponent(0);
        }

        return null;
    }

    /**
     * @return the current table
     */
    public TableModel getCurrentModel() {
        JTable t = getCurrentTable();
        if (t != null) {
            return t.getModel();
        }

        return null;
    }

    /**
     * @return a popup menu for the table
     */
    public JPopupMenu createPopupMenu() {
        JPopupMenu popup = new JPopupMenu();
        popup.add(CutAction.createMenuItem(this, UiDataMessages.CUT));
        popup.add(CopyAction.createMenuItem(this, UiDataMessages.COPY));
        popup.add(PasteAction.createMenuItem(this, UiDataMessages.PASTE));

        popup.addSeparator();

        popup.add(CreateVariableFromSelectionAction.createMenuItem(this, UiDataMessages.CREATE));

        popup.addSeparator();

        JMenu plots = new JMenu("plots");
        popup.add(plots);

        plots.add(PlotAction.createMenuItem(this, "plot2d", 0));
        plots.add(PlotAction.createMenuItem(this, "Matplot", 1));
        plots.add(PlotAction.createMenuItem(this, "grayplot", 2));
        plots.add(PlotAction.createMenuItem(this, "Sgrayplot", 3));
        plots.add(PlotAction.createMenuItem(this, "champ", 4));
        plots.add(PlotAction.createMenuItem(this, "histplot", 5));
        plots.add(PlotAction.createMenuItem(this, "mesh", 6));
        plots.add(PlotAction.createMenuItem(this, "surf", 7));

        return popup;
    }

    /**
     * {@inheritDoc}
     */
    public void setData(String name, String type, Object[][] data) {
        TableVariableEditor table = new TableVariableEditor();

        CopyAction.registerAction(this, table);
        CutAction.registerAction(this, table);
        PasteAction.registerAction(this, table);
        SupprAction.registerAction(this, table);
        UndoAction.registerAction(this, table);
        RedoAction.registerAction(this, table);
        SizeColumnsToFitAction.registerAction(this, table);
        SetPrecisionShortAction.registerAction(this, table);
        SetPrecisionShorteAction.registerAction(this, table);
        SetPrecisionLongAction.registerAction(this, table);
        SetPrecisionLongeAction.registerAction(this, table);
        CreateVariableFromSelectionAction.registerAction(this, table);

        table.setComponentPopupMenu(createPopupMenu());

        ScilabGenericCellEditor cellEditor = (ScilabGenericCellEditor) CellEditorFactory.createCellEditor(type);
        int rows = Math.max(((TableVariableEditor) table).getMinimalRowNumber(), table.getModel().getRowCount());
        int cols = table.getModel().getColumnCount();

        SwingEditvarTableModel dataModel = new SwingEditvarTableModel(this, name, type, data, cellEditor, rows, cols);

        dataModel.addTableModelListener(getRowHeader((TableVariableEditor) table));
        getRowHeader((TableVariableEditor) table).tableChanged(new TableModelEvent(dataModel));
        table.setModel(dataModel);
        table.setDefaultEditor(Object.class, cellEditor);
        table.setDefaultRenderer(Object.class, RendererFactory.createRenderer(type));

        tabPane.add(PREFIX + name, table.getScrollPane());
        tabPane.setSelectedComponent(table.getScrollPane());
    }

    /**
     * @param table the table to update
     * @param name the name of the variable
     * @param type the type of the matrix
     * @param data the matrix datas
     */
    public void updateData(JTable table, String name, String type, Object[][] data) {
        ScilabGenericCellEditor cellEditor = (ScilabGenericCellEditor) CellEditorFactory.createCellEditor(type);
        int rows = Math.max(((TableVariableEditor) table).getMinimalRowNumber(), table.getModel().getRowCount());
        int cols = table.getModel().getColumnCount();
        CellsUndoManager undo = null;
        if (table.getModel() instanceof SwingEditvarTableModel) {
            undo = ((SwingEditvarTableModel) table.getModel()).getUndoManager();
        }

        SwingEditvarTableModel dataModel = (SwingEditvarTableModel) table.getModel();
        getRowHeader((TableVariableEditor) table).tableChanged(new TableModelEvent(dataModel));
        dataModel.changeData(type, data, cellEditor, rows, cols);
        table.setDefaultEditor(Object.class, cellEditor);
        table.setDefaultRenderer(Object.class, RendererFactory.createRenderer(type));
    }

    /**
     * @param table the table to update
     * @param name the name of the variable
     * @param type the type of the matrix
     * @param data the matrix datas
     */
    public void updateData(JTable table, String name, String type, Object[][] data, double[] rowsIndex, double[] colsIndex) {
        SwingEditvarTableModel dataModel = (SwingEditvarTableModel) table.getModel();
        if (!type.equals(dataModel.getType())) {
            dataModel.refresh();
        } else {
            for (int i = 0; i < rowsIndex.length; i++) {
                for (int j = 0; j < colsIndex.length; j++) {
                    int r = (int) rowsIndex[i] - 1;
                    int c = (int) colsIndex[j] - 1;
                    dataModel.setValueAtAndUpdate(false, false, data[i][j], r, c);
                }
            }
        }
    }

    /**
     * @param c the component in the tabbedpane
     * @param name the name of the variable
     * @param type the type of the matrix
     * @param data the matrix datas
     */
    public void updateData(Component c, String name, String type, Object[][] data) {
        tabPane.setSelectedComponent(c);
        JTable table = (JTable) ((JScrollPane) c).getViewport().getComponent(0);
        updateData(table, name, type, data);
    }

    /**
     * @param c the component in the tabbedpane
     * @param name the name of the variable
     * @param type the type of the matrix
     * @param data the matrix datas
     */
    public void updateData(Component c, String name, String type, Object[][] data, double[] rowsIndex, double[] colsIndex) {
        tabPane.setSelectedComponent(c);
        JTable table = (JTable) ((JScrollPane) c).getViewport().getComponent(0);
        updateData(table, name, type, data, rowsIndex, colsIndex);
    }

    /**
     * @return the row header used in this table
     */
    public RowHeader getRowHeader(TableVariableEditor table) {
        return (RowHeader) table.getScrollPane().getRowHeader().getView();
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
     * @param b true to enable the button
     */
    public void enableUndoButton(boolean b) {
        if (undoButton != null) {
            undoButton.setEnabled(b);
        }
    }

    /**
     * @param b true to enable the button
     */
    public void enableRedoButton(boolean b) {
        if (redoButton != null) {
            redoButton.setEnabled(b);
        }
    }
}
