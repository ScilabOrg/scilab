/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.rowheader;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Insets;
import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import javax.swing.JTable;
import javax.swing.border.AbstractBorder;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.DefaultTableModel;

import org.scilab.modules.ui_data.variableeditor.TableVariableEditor;

/**
 * Class for the RowHeader
 * @author Calixte DENIZET
 */
public class RowHeader extends JPanel implements TableModelListener {

    private static final Insets INSETS = new Insets(1, 0, 0, 0);

    private JTable rowTable;
    private DefaultTableModel model;
    private int clickedRow;

    /**
     * Constructor
     * @param table the table
     */
    public RowHeader(final JTable table) {
        super(new BorderLayout());
        table.getModel().addTableModelListener(this);
        model = new DefaultTableModel(0, 1) {
                public boolean isCellEditable(int iRowIndex, int iColumnIndex) {
                    return false;
                }
            };
        rowTable = new JTable(model);
        setBorder(new AbstractBorder() {
		public Insets getBorderInsets(Component c) {
		    return INSETS;
		}
		
		public Insets getBorderInsets(Component c, Insets insets) {
		    return INSETS;
		}		 
		
		public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
		    g.setColor(table.getGridColor().darker());
		    g.drawLine(x, y, x + width, y);
		} 
	    });
        rowTable.setDragEnabled(false);
        rowTable.setGridColor(table.getGridColor().darker());
        rowTable.setShowVerticalLines(true);
        rowTable.setModel(model);
        rowTable.setRowHeight(table.getRowHeight());
        rowTable.setRowMargin(table.getRowMargin());
        rowTable.setIntercellSpacing(table.getIntercellSpacing());
        rowTable.setDefaultEditor(Object.class, null);
        rowTable.setCellEditor(null);
        rowTable.setDefaultRenderer(Object.class, new HeaderRenderer(table, true));
	rowTable.doLayout();
        add(rowTable);
        doLayout();
        rowTable.addMouseListener(new MouseAdapter() {
                public void mousePressed(MouseEvent e) {
                    int modifiers = e.getModifiersEx();
                    int row  = rowTable.rowAtPoint(e.getPoint());
                    if ((modifiers & MouseEvent.SHIFT_DOWN_MASK) != 0) {
                        table.setRowSelectionInterval(row, clickedRow);
                    } else {
                        if ((modifiers & MouseEvent.CTRL_DOWN_MASK) != 0) {
			    if (table.isRowSelected(row)) {
				table.removeRowSelectionInterval(row, row);
			    } else {
				table.addRowSelectionInterval(row, row);
			    }
                        } else {
                            table.setRowSelectionInterval(row, row);
                        }
                        clickedRow = row;
                    }
                    table.setColumnSelectionInterval(0, table.getColumnCount() - 1);
                    table.requestFocus();
                }
            });

        rowTable.addMouseMotionListener(new MouseMotionAdapter() {
                public void mouseDragged(MouseEvent e) {
		    Point p = e.getPoint();
		    int row;
		    if (p.y >= rowTable.getCellRect(rowTable.getRowCount() - 1, 0, false).y) {
			row = rowTable.getRowCount() - 1;
		    } else {
			row = rowTable.rowAtPoint(p);
		    }
		    if (row == rowTable.getRowCount() - 1) {
			((TableVariableEditor) table).addTenRows();
		    }
		    table.scrollRectToVisible(table.getCellRect(row, 0, false));
                    table.setRowSelectionInterval(clickedRow, row);
                    table.setColumnSelectionInterval(0, table.getColumnCount() - 1);
                    table.requestFocus();
                }
            });
    }

    /**
     * {@inheritDoc}
     */
    public void tableChanged(TableModelEvent e) {
        DefaultTableModel dtm = (DefaultTableModel) e.getSource();
        int N = dtm.getRowCount() - model.getRowCount();
        for (int i = 0; i < N; i++) {
            model.addRow(new Object[1]);
        }
    }

    public JTable getTable() {
	return rowTable;
    }
}
