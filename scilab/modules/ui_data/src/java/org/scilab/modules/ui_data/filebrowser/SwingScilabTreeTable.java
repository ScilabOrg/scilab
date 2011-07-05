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
import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.JTree;
import javax.swing.border.AbstractBorder;
import javax.swing.border.Border;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableCellRenderer;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.DefaultTreeSelectionModel;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;

/**
 * The tree table model abstract implementation
 * @author Calixte DENIZET
 */
public class SwingScilabTreeTable extends JTable {

    private static final int TIMETOREFRESH = 500;
    private static final Border BORDER = new AbstractBorder() {
	    public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
		g.setColor(Color.LIGHT_GRAY);
		g.drawLine(x, y, x, y + height);
	    }
	};

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

        DefaultTableCellRenderer dtcr = new DefaultTableCellRenderer() {
		public Component getTableCellRendererComponent(JTable table, Object value,
							       boolean selected, boolean focus,
							       int row, int col) {
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
	private Thread thread;
	private boolean isActive;

        /**
         * {@inheritdoc}
         */
        public ScilabTreeTableCellRenderer(TreeModel model) {
            super(model);
            DefaultTreeCellRenderer renderer = new DefaultTreeCellRenderer() {

                    public Component getTreeCellRendererComponent(JTree tree, Object value, boolean selected,
                                                                  boolean expanded, boolean leaf, int row, boolean hasFocus) {
                        ScilabFileBrowserModel.FileNode fn = (ScilabFileBrowserModel.FileNode) value;
                        if (leaf) {
			    this.setLeafIcon(fn.getIcon());
			} else {
			    this.setOpenIcon(FileUtils.getOpenedDirIcon());
			    this.setClosedIcon(FileUtils.getClosedDirIcon());
			}
			super.getTreeCellRendererComponent(tree, value, selected, expanded, leaf, row, hasFocus);

                        return this;
                    }
                };
            setCellRenderer(renderer);
	    setRootVisible(false);
	    startAutoRefresh();
        }

	private void startAutoRefresh() {
	    thread = new Thread(new Runnable() {
		    
		    public void run() {
			isActive = true;
			while (isActive) {
			    ScilabFileBrowserModel model = (ScilabFileBrowserModel) getModel();
			    boolean changed = false;
			    for (int i = 0; i < getRowCount(); i++) {
				TreePath path = getPathForRow(i);
				ScilabFileBrowserModel.FileNode fn = (ScilabFileBrowserModel.FileNode) path.getLastPathComponent();
				if (fn.hasChanged()) {
				    if (!changed) {
					changed = true;
				    }
				    fn.reset();
				    model.fireTreeStructureChanged(model, path.getPath(), new int[]{0}, new Object[]{fn});
				}
			    }

			    if (changed) {
				SwingScilabTreeTable.this.repaint();
			    }

			    try {
				thread.sleep(TIMETOREFRESH);
			    } catch (Exception e) {
				stopAutoRefresh();
			    }
			}
		    }
		});
	    try {
		thread.start();
	    } catch (Exception e) {
		stopAutoRefresh();
	    }
	}

	private void stopAutoRefresh() {
	    isActive = false;;
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
