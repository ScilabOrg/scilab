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

package org.scilab.modules.ui_data.filebrowser;

import java.awt.Cursor;
import java.util.Enumeration;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import javax.swing.JTable;
import javax.swing.JTree;
import javax.swing.RowSorter;
import javax.swing.SortOrder;
import javax.swing.SwingUtilities;
import javax.swing.table.TableModel;
import javax.swing.tree.TreePath;

import org.scilab.modules.gui.bridge.window.SwingScilabWindow;

/**
 * A RowSorter to sort the files in the view
 * @author Calixte DENIZET
 */
public class FileBrowserRowSorter extends RowSorter<TableModel> {

    private JTree tree;
    private JTable table;
    private List<RowSorter.SortKey> sortKeys;

    /**
     * Default constructor
     * @param tree the associated JTree
     * @param table the associated JTable
     */
    public FileBrowserRowSorter(JTree tree, JTable table) {
        this.tree = tree;
        this.table = table;
    }

    /**
     * {@inheritDoc}
     */
    public void allRowsChanged() { }

    /**
     * {@inheritDoc}
     */
    public int convertRowIndexToModel(int index) {
        return index;
    }

    /**
     * {@inheritDoc}
     */
    public int convertRowIndexToView(int index) {
        return index;
    }

    /**
     * {@inheritDoc}
     */
    public TableModel getModel() {
        return table.getModel();
    }

    /**
     * {@inheritDoc}
     */
    public int getModelRowCount() {
        return tree.getRowCount();
    }

    /**
     * {@inheritDoc}
     */
    public List<RowSorter.SortKey> getSortKeys() {
        if (sortKeys == null) {
            List<RowSorter.SortKey> list = new ArrayList<RowSorter.SortKey>();
            list.add(new RowSorter.SortKey(0, SortOrder.ASCENDING));
            setSortKeys(list);
        }

        return sortKeys;
    }

    public int getViewRowCount() {
        return getModelRowCount();
    }

    /**
     * {@inheritDoc}
     */
    public void modelStructureChanged() { }

    /**
     * {@inheritDoc}
     */
    public void rowsDeleted(int firstRow, int endRow) { }

    /**
     * {@inheritDoc}
     */
    public void rowsInserted(int firstRow, int endRow) { }

    /**
     * {@inheritDoc}
     */
    public void rowsUpdated(int firstRow, int endRow) { }

    /**
     * {@inheritDoc}
     */
    public void rowsUpdated(int firstRow, int endRow, int column) { }

    /**
     * {@inheritDoc}
     */
    public void setSortKeys(List <? extends RowSorter.SortKey > keys) {
        this.sortKeys = Collections.unmodifiableList(new ArrayList<SortKey>(keys));
        fireSortOrderChanged();
    }

    /**
     * {@inheritDoc}
     */
    public void toggleSortOrder(int column) {
        SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, table).setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        ScilabFileBrowserModel model = (ScilabFileBrowserModel) tree.getModel();
        FileNode root = (FileNode) model.getRoot();
        Enumeration<TreePath> en = tree.getExpandedDescendants(new TreePath(root));
        root.toggleSortOrder(table.getColumnName(column));

        root.orderFiles();
        for (int i = 0; i < tree.getRowCount(); i++) {
            FileNode fn = (FileNode) tree.getPathForRow(i).getLastPathComponent();
            fn.orderFiles();
        }

        model.fireTreeStructureChanged(model, new TreePath(root).getPath(), null, null);
        if (en != null) {
            while (en.hasMoreElements()) {
                tree.expandPath(en.nextElement());
            }
        }

        SortOrder order = root.getOrder();
        List<RowSorter.SortKey> list = new ArrayList<RowSorter.SortKey>();
        list.add(new RowSorter.SortKey(column, order));
        setSortKeys(list);

        table.repaint();

        SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, table).setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
    }
}