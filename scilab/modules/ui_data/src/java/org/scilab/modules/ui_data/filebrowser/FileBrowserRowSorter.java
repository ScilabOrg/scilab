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

import java.util.List;

import javax.swing.JTree;
import javax.swing.RowSorter;

/**
 *
 * @author Calixte DENIZET
 */
public class FileBrowserRowSorter extends RowSorter<ScilabFileBrowserModel> {

    private ScilabFileBrowserModel model;
    private JTree tree;

    public FileBrowserRowSorter(ScilabFileBrowserModel model, Jtree tree) {
        this.model = model;
        this.tree = tree;
    }

    public void allRowsChanged() { }

    public int convertRowIndexToModel(int index) {
        return index;
    }

    public int convertRowIndexToView(int index) {
        return index
            }

    public ScilabFileBrowserModel getModel() {
        return model;
    }

    public int getModelRowCount() {
        return tree.getRowCount();
    }

    public List<? extends RowSorter.SortKey> getSortKeys() {
        return null;
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

    public void toggleSortOrder(int column) {

    }
}