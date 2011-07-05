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

import java.util.Enumeration;
import java.util.List;

import javax.swing.JTable;
import javax.swing.JTree;
import javax.swing.RowSorter;
import javax.swing.table.TableModel;
import javax.swing.tree.TreePath;

/**
 *
 * @author Calixte DENIZET
 */
public class FileBrowserRowSorter extends RowSorter<TableModel> {

    private JTree tree;
    private JTable table;

    public FileBrowserRowSorter(JTree tree, JTable table) {
        this.tree = tree;
        this.table = table;
    }

    public void allRowsChanged() { }

    public int convertRowIndexToModel(int index) {
        return index;
    }

    public int convertRowIndexToView(int index) {
        return index;
    }

    public TableModel getModel() {
        return table.getModel();
    }

    public int getModelRowCount() {
        return tree.getRowCount();
    }

    public List<? extends RowSorter.SortKey> getSortKeys() {
        return new java.util.ArrayList(0);
    }

    public int getViewRowCount() {
        return getModelRowCount();
    }

    public void modelStructureChanged() { }

    public void rowsDeleted(int firstRow, int endRow) { }

    public void rowsInserted(int firstRow, int endRow) { }

    public void rowsUpdated(int firstRow, int endRow) { }

    public void rowsUpdated(int firstRow, int endRow, int column) { }

    public void setSortKeys(List<? extends RowSorter.SortKey> keys) { }

    public void toggleSortOrder(int column) {System.out.println("hello="+column);
        ScilabFileBrowserModel model = (ScilabFileBrowserModel) tree.getModel();
        Enumeration<TreePath> en = tree.getExpandedDescendants(tree.getPathForRow(0));
	//System.out.println(tree.isExpanded(tree.getExpandedDescendants(tree.getPathForRow(0)).nextElement()));
        model.toggleSortOrder(table.getColumnName(column));
	for (int i = 0; i < tree.getRowCount(); i++) {
            ScilabFileBrowserModel.FileNode fn = (ScilabFileBrowserModel.FileNode) tree.getPathForRow(i).getLastPathComponent();
	    fn.reset();
	}

	model.fireTreeStructureChanged(model, tree.getPathForRow(0).getPath(), null, null);
	while (en.hasMoreElements()) {
	    TreePath p = en.nextElement();System.out.println(p);
	    tree.expandPath(p);
	}
	table.repaint();
    }
}