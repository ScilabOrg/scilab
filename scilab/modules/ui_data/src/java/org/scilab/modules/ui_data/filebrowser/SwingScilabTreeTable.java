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

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;

import javax.swing.AbstractCellEditor;
import javax.swing.JTable;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableCellRenderer;
import javax.swing.tree.DefaultTreeSelectionModel;
import javax.swing.tree.TreeModel;

/**
 * The tree table model abstract implementation
 * @author Calixte DENIZET
 */
public class SwingScilabTreeTable extends JTable {

    protected ScilabTreeTableCellRenderer tree;

    public SwingScilabTreeTable(ScilabTreeTableModel treeTableModel) {
        super();
        tree = new ScilabTreeTableCellRenderer(treeTableModel);
        super.setModel(new ScilabTreeTableModelAdapter(treeTableModel, tree));

        // Force the JTable and JTree to share their row selection models.
        tree.setSelectionModel(new DefaultTreeSelectionModel() {
                    {
                        setSelectionModel(listSelectionModel);
                    }
            });

        // Make the tree and table row heights the same.
        tree.setRowHeight(getRowHeight());

        // Install the tree editor renderer and editor.
        setDefaultRenderer(ScilabTreeTableModel.class, tree);
        setDefaultEditor(ScilabTreeTableModel.class, new ScilabTreeTableCellEditor());

        DefaultTableCellRenderer dtcr = new DefaultTableCellRenderer();
        dtcr.setHorizontalTextPosition(DefaultTableCellRenderer.LEFT);
        setDefaultRenderer(ScilabFileBrowserModel.FileSize.class, dtcr);

        setShowGrid(false);
        setBackground(Color.WHITE);
        setIntercellSpacing(new Dimension(0, 0));
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

    public class ScilabTreeTableCellRenderer extends JTree implements TableCellRenderer {

        protected int visibleRow;

        /**
         * {@inheritdoc}
         */
        public ScilabTreeTableCellRenderer(TreeModel model) {
            super(model);
            DefaultTreeCellRenderer renderer = new DefaultTreeCellRenderer() {

                    public Component getTreeCellRendererComponent(JTree tree, Object value, boolean selected,
                                                                  boolean expanded, boolean leaf, int row, boolean hasFocus) {
                        super.getTreeCellRendererComponent(tree, value, selected, expanded, leaf, row, hasFocus);
                        ScilabFileBrowserModel.FileNode fn = (ScilabFileBrowserModel.FileNode) value;
                        if (leaf) {
			    setLeafIcon(FileUtils.getIconForFile(fn.getFile()));
			}//System.out.println(getLeafIcon());
                        return this;
                    }
                };
            setCellRenderer(renderer);
        }

        /**
         * {@inheritdoc}
         */
        public void setBounds(int x, int y, int w, int h) {
            super.setBounds(x, 0, w, SwingScilabTreeTable.this.getHeight());
        }

        /**
         * {@inheritdoc}
         */
        public void paint(Graphics g) {
            g.translate(0, -visibleRow * getRowHeight());
            super.paint(g);
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
}
