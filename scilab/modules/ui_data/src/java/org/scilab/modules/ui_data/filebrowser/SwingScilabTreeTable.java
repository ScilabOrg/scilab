/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.filebrowser;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Insets;
import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;

import javax.swing.AbstractCellEditor;
import javax.swing.ActionMap;
import javax.swing.InputMap;
import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.JTree;
import javax.swing.Timer;
import javax.swing.border.AbstractBorder;
import javax.swing.border.Border;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableColumn;
import javax.swing.tree.TreePath;

import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.ui_data.filebrowser.actions.ChangeCWDAction;
import org.scilab.modules.ui_data.filebrowser.actions.EditFileWithDefaultAppAction;
import org.scilab.modules.ui_data.filebrowser.actions.ExecuteFileInConsoleAction;
import org.scilab.modules.ui_data.filebrowser.actions.ExecuteFileInXcosAction;
import org.scilab.modules.ui_data.filebrowser.actions.LoadFileAsGraphAction;
import org.scilab.modules.ui_data.filebrowser.actions.LoadFileInScilabAction;
import org.scilab.modules.ui_data.filebrowser.actions.OpenFileInSciNotesAction;
import org.scilab.modules.ui_data.filebrowser.actions.OpenFileWithDefaultAppAction;
import org.scilab.modules.ui_data.filebrowser.actions.ValidateAction;

/**
 * The tree table model abstract implementation
 * @author Calixte DENIZET
 */
public class SwingScilabTreeTable extends JTable {

    private static final Insets INSETS = new Insets(0, 2, 0, 0);

    private static final Border BORDER = new AbstractBorder() {
            public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
                g.setColor(Color.LIGHT_GRAY);
                g.drawLine(x, y, x, y + height);
            }

            public Insets getBorderInsets(Component c) {
                return INSETS;
            }

            public Insets getBorderInsets(Component c, Insets insets) {
                return INSETS;
            }
        };

    private Timer refreshTimer;
    private static int refreshTime = 1000;

    protected ScilabTreeTableCellRenderer tree;
    protected ScilabFileSelectorComboBox combobox;
    protected ScilabFileBrowserHistory history;

    public SwingScilabTreeTable(ScilabTreeTableModel treeTableModel, ScilabFileSelectorComboBox combobox) {
        super();
        this.combobox = combobox;
        combobox.setTreeTable(this);
        history = new ScilabFileBrowserHistory(this);
        tree = new ScilabTreeTableCellRenderer(this, treeTableModel);
        super.setModel(new ScilabTreeTableModelAdapter(treeTableModel, tree));

        // Install the tree editor renderer and editor.
        setDefaultRenderer(ScilabTreeTableModel.class, tree);
        setDefaultEditor(ScilabTreeTableModel.class, new ScilabTreeTableCellEditor());

        DefaultTableCellRenderer dtcr = new DefaultTableCellRenderer() {
                public Component getTableCellRendererComponent(JTable table, Object value, boolean selected, boolean focus, int row, int col) {
                    Component c = super.getTableCellRendererComponent(table, value, selected, focus, row, col);
                    if (col == 1) {
                        JLabel jl = (JLabel) c;
                        jl.setBorder(BORDER);
                    }
                    return c;
                }
            };
        dtcr.setHorizontalTextPosition(DefaultTableCellRenderer.LEFT);
        setDefaultRenderer(ScilabFileBrowserModel.FileSize.class, dtcr);

        setShowGrid(false);
        setIntercellSpacing(new Dimension(0, 0));
        setRowSorter(new FileBrowserRowSorter(tree, this));
        setAutoResizeMode(AUTO_RESIZE_NEXT_COLUMN);

        addMouseListener(new MouseAdapter() {
                public void mousePressed(MouseEvent e) {
                    /* Crappy workaround (the repaint()) to avoid weird painting of the jtree when
                       the user click on a row and outside the label and its expand icon */
                    int col = columnAtPoint(new Point(e.getX(), e.getY()));
                    if (getColumnClass(col) == ScilabTreeTableModel.class) {
                        editingRow = tree.getClosestRowForLocation(e.getX(), e.getY());
                        repaint();
                    }
                }

                public void mouseClicked(MouseEvent e) {
                    /* Workaround to guarantee that the table will have the focus to allow keyboard navigation */
                    int col = columnAtPoint(new Point(e.getX(), e.getY()));
                    if (getColumnClass(col) == ScilabTreeTableModel.class) {
                        requestFocus();
                    }
                }
            });

	TableCellRenderer renderer = getTableHeader().getDefaultRenderer();

	initActions();
        refreshTimer = new Timer(refreshTime, new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    tree.startRefresh();
                }
            });
    }

    public static void setRefreshTime(int time) {
	refreshTime = time;
    }

    public ScilabFileSelectorComboBox getComboBox() {
        return combobox;
    }

    public String[] getSelectedPaths() {
        int[] rows = getSelectedRows();
        String[] paths = new String[rows.length];
        for (int i = 0; i < rows.length; i++) {
            TreePath path = tree.getPathForRow(rows[i]);
            FileNode fn = (FileNode) path.getLastPathComponent();
            paths[i] = fn.getFile().getAbsolutePath();
        }

        return paths;
    }

    public File[] getSelectedFiles() {
        int[] rows = getSelectedRows();
        File[] files = new File[rows.length];
        for (int i = 0; i < rows.length; i++) {
            TreePath path = tree.getPathForRow(rows[i]);
            FileNode fn = (FileNode) path.getLastPathComponent();
            files[i] = fn.getFile();
        }

        return files;
    }

    public int getRowHeight(int row) {
        return getRowHeight();
    }

    public boolean isOpaque() {
        return false;
    }

    public void setBaseDir(String baseDir) {
        ScilabFileBrowserModel model = (ScilabFileBrowserModel) tree.getModel();
        combobox.setBaseDir(baseDir);
        if (!baseDir.equals(model.getBaseDir())) {
            refreshTimer.stop();
            tree.stopRefresh();
            tree.collapsePath(new TreePath(model.getRoot()));
            model.setBaseDir(baseDir, this);
        }
    }

    public void reload() {
        ScilabFileBrowserModel model = (ScilabFileBrowserModel) tree.getModel();
        tree.setModel(model);
        tree.setRowHeight(getRowHeight());
        tree.setLargeModel(true);
        TreePath path = new TreePath(model.getRoot());
        tree.collapsePath(path);
        tree.expandPath(path);
        editingRow = 0;
        refreshTimer.start();
    }

    /* Workaround for BasicTableUI anomaly. Make sure the UI never tries to
     * paint the editor. The UI currently uses different techniques to
     * paint the renderers and editors and overriding setBounds() below
     * is not the right thing to do for an editor. Returning -1 for the
     * editing row in this case, ensures the editor is never painted.
     */
    public int getEditingRow() {
        if (getColumnClass(editingColumn) == ScilabTreeTableModel.class) {
            return -1;
        } else {
            return editingRow;
        }
    }

    public class ScilabTreeTableCellEditor extends AbstractCellEditor implements TableCellEditor {

        /**
         * {@inheritdoc}
         */
        public Component getTableCellEditorComponent(JTable table, Object value, boolean isSelected, int r, int c) {
            return tree;
        }

        /**
         * {@inheritdoc}
         */
        public Object getCellEditorValue() {
            return null;
        }
    }

    private void initActions() {
        ActionMap actions = getActionMap();
        actions.put("scinotes", new OpenFileInSciNotesAction(this));
        actions.put("xcos", new ExecuteFileInXcosAction(this));
        actions.put("console", new ExecuteFileInConsoleAction(this));
        actions.put("load", new LoadFileInScilabAction(this));
        actions.put("graph", new LoadFileAsGraphAction(this));
        actions.put("cwd", new ChangeCWDAction(this));
        if (EditFileWithDefaultAppAction.isSupported()) {
            actions.put("edit", new EditFileWithDefaultAppAction(this));
        }
        if (OpenFileWithDefaultAppAction.isSupported()) {
            actions.put("open", new OpenFileWithDefaultAppAction(this));
        }
        actions.put("validate", new ValidateAction(this));

        combobox.setAction((CallBack) actions.get("cwd"));
        InputMap map = getInputMap();
        //map.put(
    }
}
