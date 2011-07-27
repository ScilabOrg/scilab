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

import java.awt.Component;
import java.awt.Container;
import java.awt.Cursor;
import java.awt.Graphics;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;

import javax.swing.JTable;
import javax.swing.JTree;
import javax.swing.SwingUtilities;
import javax.swing.SwingWorker;
import javax.swing.table.TableCellRenderer;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.DefaultTreeSelectionModel;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;
import javax.swing.event.TreeExpansionEvent;
import javax.swing.event.TreeExpansionListener;
import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;
import javax.swing.event.TreeWillExpandListener;

import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.events.callback.CallBack;

/**
 * The table renderer to render the tree in the first column of the JTable
 */
public class ScilabTreeTableCellRenderer extends JTree implements TableCellRenderer {

    private SwingScilabTreeTable table;
    private SwingWorker worker;

    protected int visibleRow;

    /**
     * {@inheritdoc}
     */
    public ScilabTreeTableCellRenderer(SwingScilabTreeTable table, TreeModel model) {
        super(model);
        this.table = table;
        DefaultTreeCellRenderer renderer = new DefaultTreeCellRenderer() {

                public Component getTreeCellRendererComponent(JTree tree, Object value, boolean selected, boolean expanded, boolean leaf, int row, boolean hasFocus) {
                    FileNode fn = (FileNode) value;
                    super.getTreeCellRendererComponent(tree, value, selected, expanded, leaf, row, hasFocus);
                    if (leaf) {
                        if (fn instanceof ScilabFileBrowserModel.ParentNode) {
                            this.setLeafIcon(FileUtils.getUpDirIcon());
                        } else {
                            this.setLeafIcon(fn.getIcon());
                        }
                    } else if (fn.isUserHome()) {
                        this.setClosedIcon(FileUtils.getClosedUserHomeIcon());
                        this.setOpenIcon(FileUtils.getOpenUserHomeIcon());
                    } else if (fn.isSCI()) {
                        this.setClosedIcon(FileUtils.getSCIIcon());
                        this.setOpenIcon(FileUtils.getOpenSCIIcon());
                    } else {
                        this.setClosedIcon(FileUtils.getClosedDirIcon());
                        this.setOpenIcon(FileUtils.getOpenDirIcon());
                    }

                    return this;
                }
            };

        /* Force the JTable and JTree to share their row selection models.
           And let the table to handle the selection rather than the tree; */
        setSelectionModel(new DefaultTreeSelectionModel() {
                    {
                        ScilabTreeTableCellRenderer.this.table.setSelectionModel(listSelectionModel);
                    }

                public void setSelectionPaths(TreePath[] pPaths) { }

                public void addSelectionPaths(TreePath[] paths) { }

                public void removeSelectionPaths(TreePath[] paths) { }
            });

        setCellRenderer(renderer);
        setRootVisible(true);
        setRowHeight(table.getRowHeight());
        setLargeModel(true);
        setEditable(true);
        setToggleClickCount(0);

        addMouseListener(new MouseAdapter() {
                public void mousePressed(MouseEvent e) {
                    int selRow = getRowForLocation(e.getX(), e.getY());
                    TreePath selPath = getPathForLocation(e.getX(), e.getY());
                    if (selRow != -1) {
                        switch (e.getClickCount()) {
                        case 1 :
                            /*int sel = SwingScilabTreeTable.this.getSelectedRow();
                              System.out.println(sel+":::"+selRow+":::"+isEditable());
                              if (sel == selRow) {
                              System.out.println(getCellEditor().getTreeCellEditorComponent(ScilabTreeTableCellRenderer.this, getCellEditor().getCellEditorValue(), true, false, true, selRow));
                              }
                              e.consume();*/
                            break;
                        case 2:
                            ((CallBack) ScilabTreeTableCellRenderer.this.table.getActionMap().get("validate")).callBack();
                            e.consume();
                        }
                    }
                }

                public void mouseReleased(MouseEvent e) { }
            });
    }

    /**
     * {@inheritDoc}
     */
    protected TreeModelListener createTreeModelListener() {
        return new TreeModelListener() {
            public void treeNodesChanged(TreeModelEvent e) { }

            public void treeNodesInserted(TreeModelEvent e) { }

            public void treeStructureChanged(TreeModelEvent e) { }

            public void treeNodesRemoved(TreeModelEvent e) { }
        };
    }

    /**
     * Stop the refresh
     */
    public void stopRefresh() {
        if (worker != null) {
            worker.cancel(true);
            worker = null;
        }
    }

    /**
     * Start the refresh
     */
    public void startRefresh() {
        final ScilabFileBrowserModel model = (ScilabFileBrowserModel) getModel();
        final TreePath[] paths;
        try {
            paths = getPathBetweenRows(0, getRowCount() - 1);
        } catch (Exception e) {
            return;
        }

        int[] selectedRows = table.getSelectedRows();
        final TreePath[] selectedPaths = new TreePath[selectedRows.length];
        for (int i = 0; i < selectedRows.length; i++) {
            selectedPaths[i] = getPathForRow(selectedRows[i]);
        }
        final List<TreePath> list = new ArrayList<TreePath>();
        worker = new SwingWorker<Void, Void>() {

            protected Void doInBackground() throws Exception {
		for (TreePath path : paths) {
                    FileNode fn = (FileNode) path.getLastPathComponent();
		    if (fn.hasChanged()) {
                        fn.reset();
                        list.add(path);
                    }
                }
                for (TreePath path : list) {
                    Object[] p = path.getPath();
                    model.fireTreeStructureChanged(model, p, new int[]{0}, new Object[]{p[p.length - 1]});
                }

                return null;
            }

            protected void done() {
                for (TreePath path : list) {
                    TreePath parent = path.getParentPath();
                    if (parent == null) {
                        parent = path;
                    }
                    collapsePath(parent);
                    expandPath(parent);
                }
                if (!list.isEmpty()) {
                    for (TreePath path : selectedPaths) {
                        if (isVisible(path)) {
                            int r = getRowForPath(path);
                            if (r != -1) {
                                table.addRowSelectionInterval(r, r);
                            }
                        }
                    }
                }
            }
        };

        worker.execute();
    }

    /**
     * {@inheritdoc}
     */
    public void setBounds(int x, int y, int w, int h) {
        super.setBounds(x, 0, w, table.getHeight());
    }

    /**
     * {@inheritdoc}
     */
    public void paint(Graphics g) {
	g.translate(0, -visibleRow * getRowHeight());
        try {
            ui.update(g, this);
        } catch (NullPointerException e) {
            // Occurs sometimes...
            g.translate(0, visibleRow * getRowHeight());
            paint(g);
        }
    }

    /**
     * {@inheritdoc}
     */
    public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
        if (isSelected) {
            setBackground(table.getSelectionBackground());
        } else {
            setBackground(table.getBackground());
        }
        visibleRow = row;

        return this;
    }
}
