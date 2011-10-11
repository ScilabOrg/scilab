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

package org.scilab.modules.ui_data.variableeditor;

import java.awt.Color;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.util.Arrays;
import java.util.Vector;

import javax.swing.BoundedRangeModel;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.TableModelEvent;
import javax.swing.table.JTableHeader;
import javax.swing.table.TableColumn;

import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.rowheader.HeaderRenderer;
import org.scilab.modules.ui_data.rowheader.RowHeader;

public class TableVariableEditor extends JTable {

    private JScrollPane scrollPane;
    private int clickedColumn;
    private int[] selectedRows = new int[0];
    private int[] selectedCols = new int[0];

    public TableVariableEditor() {
        super();
        scrollPane = new JScrollPane(this);
        scrollPane.getVerticalScrollBar().addAdjustmentListener(new AdjustmentListener() {
                public void adjustmentValueChanged(AdjustmentEvent e) {
                    BoundedRangeModel brm = scrollPane.getVerticalScrollBar().getModel();
                    if (brm.getMaximum() - brm.getExtent() - e.getValue() <= 1) {
                        addTenRows();
                    }
                }
            });

        scrollPane.getHorizontalScrollBar().addAdjustmentListener(new AdjustmentListener() {
                public void adjustmentValueChanged(AdjustmentEvent e) {
                    BoundedRangeModel brm = scrollPane.getHorizontalScrollBar().getModel();
                    if (brm.getMaximum() - brm.getExtent() - e.getValue() <= 1) {
                        addTenCols();
                    }
                }
            });

        scrollPane.getViewport().addComponentListener(new ComponentAdapter() {
                public void componentResized(ComponentEvent e) {
                    ((SwingEditvarTableModel) getModel()).enlarge(getMinimalRowNumber() + 1, getMinimalColumnNumber() + 1);
                    ((SwingEditvarTableModel) getModel()).fireTableChanged(new TableModelEvent(getModel()));
                }
            });

        getTableHeader().setDefaultRenderer(new HeaderRenderer(this, false));
        getTableHeader().addMouseListener(new MouseAdapter() {
                public void mousePressed(MouseEvent e) {
                    if (getTableHeader().getCursor().getType() == Cursor.DEFAULT_CURSOR) {
                        int modifiers = e.getModifiersEx();
                        JTable table = TableVariableEditor.this;
                        int column = table.getColumnModel().getColumnIndexAtX(e.getX());
                        if ((modifiers & MouseEvent.SHIFT_DOWN_MASK) != 0) {
                            table.setColumnSelectionInterval(column, clickedColumn);
                        } else {
                            if ((modifiers & MouseEvent.CTRL_DOWN_MASK) != 0) {
                                if (table.isColumnSelected(column)) {
                                    table.removeColumnSelectionInterval(column, column);
                                } else {
                                    table.addColumnSelectionInterval(column, column);
                                }
                            } else {
                                table.setColumnSelectionInterval(column, column);
                            }
                            clickedColumn = column;
                        }
                        table.setRowSelectionInterval(0, table.getRowCount() - 1);
                        table.requestFocus();
                    }
                }
            });

        getTableHeader().addMouseMotionListener(new MouseMotionAdapter() {
                public void mouseDragged(MouseEvent e) {
                    if (getTableHeader().getCursor().getType() == Cursor.DEFAULT_CURSOR) {
                        JTable table = TableVariableEditor.this;
                        Point p =  e.getPoint();
                        int column;
                        if (p.x >= table.getTableHeader().getHeaderRect(table.getColumnCount() - 1).x) {
                            column = table.getColumnCount() - 1;
                        } else {
                            column = table.getColumnModel().getColumnIndexAtX(p.x);
                        }

                        if (column == getColumnCount() - 1) {
                            ((TableVariableEditor) table).addTenCols();
                        }
                        table.scrollRectToVisible(table.getTableHeader().getHeaderRect(column));
                        table.setColumnSelectionInterval(clickedColumn, column);
                        table.setRowSelectionInterval(0, table.getRowCount() - 1);
                        table.requestFocus();
                    }
                }
            });

        getTableHeader().setReorderingAllowed(false);

        getSelectionModel().addListSelectionListener(new ListSelectionListener() {
                public void valueChanged(ListSelectionEvent e) {
                    int[] rows = getSelectedRows();
                    Vector<Integer> rowDiff = getDifference(rows, selectedRows);
                    JTable rowHeader = ((RowHeader) scrollPane.getRowHeader().getView()).getTable();
                    for (int i : rowDiff) {
                        rowHeader.repaint(rowHeader.getCellRect(i, 0, false));
                    }
                    selectedRows = rows;
                }
            });

        getColumnModel().getSelectionModel().addListSelectionListener(new ListSelectionListener() {
                public void valueChanged(ListSelectionEvent e) {
                    int[] cols = getSelectedColumns();
                    Vector<Integer> colDiff = getDifference(cols, selectedCols);
                    JTableHeader colHeader = getTableHeader();
                    for (int i : colDiff) {
                        colHeader.repaint(colHeader.getHeaderRect(i));
                    }
                    selectedCols = cols;
                }
            });

        scrollPane.setCorner(JScrollPane.UPPER_LEFT_CORNER, new JPanel());
        scrollPane.getCorner(JScrollPane.UPPER_LEFT_CORNER).addMouseListener(new MouseAdapter() {
                public void mousePressed(MouseEvent e) {
                    selectAll();
                }
            });

        setFillsViewportHeight(true);
        setRowHeight(18);
        scrollPane.setRowHeaderView(new RowHeader(this));
        setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
        setCellSelectionEnabled(true);
        setBackground(Color.WHITE);
        setAutoResizeMode(AUTO_RESIZE_OFF);
    }

    public JScrollPane getScrollPane() {
        return scrollPane;
    }

    public int getMinimalRowNumber() {
        Dimension dim = scrollPane.getViewport().getSize();
        return (int) ((double) dim.height / getRowHeight()) + 1;
    }

    public int getMinimalColumnNumber() {
        Dimension dim = scrollPane.getViewport().getSize();
        return (int) ((double) dim.width / 75) + 1;
    }

    public void addTenRows() {
        int rowCount = getRowCount();
        ((SwingEditvarTableModel) getModel()).addRows(10);
        ((SwingEditvarTableModel) getModel()).fireTableRowsInserted(rowCount, rowCount + 10);
    }

    public void addTenCols() {
        ((SwingEditvarTableModel) getModel()).addColumns(10);
        int count = getColumnCount();
        for (int i = count; i < count + 10; i++) {
            addColumn(new TableColumn(i));
        }
    }

    public void sizeColumnsToFit() {
	SwingEditvarTableModel model = (SwingEditvarTableModel) getModel();
        for (int i = 0; i < model.getScilabMatrixColCount(); i++) {
            TableColumn col = getColumnModel().getColumn(i);
            int width = 75;
            for (int j = 0; j < model.getScilabMatrixRowCount(); j++) {
                Component c = getCellRenderer(j, i).getTableCellRendererComponent(this, getValueAt(j, i), false, false, j, i);
                if (c instanceof JLabel) {
                    JLabel label = (JLabel) c;
                    String text = label.getText();
                    if (text != null && !text.isEmpty()) {
                        int trueWidth = SwingUtilities.computeStringWidth(label.getFontMetrics(label.getFont()), label.getText());
                        width = Math.max(width, trueWidth);
                    }
                }
            }
            if (width != 75) {
                col.setPreferredWidth(width + 20);
            } else {
		col.setPreferredWidth(75);
	    }
        }
	invalidate();
	doLayout();
	repaint();
    }

    /**
     * Computes the symmetric difference between the sorted arrays a and b
     */
    private Vector<Integer> getDifference(int[] a, int[] b) {
        Vector<Integer> v = new Vector<Integer>(a.length + b.length);
        for (int i : a) {
            if (Arrays.binarySearch(b, i) < 0) {
                v.add(i);
            }
        }
        for (int i : b) {
            if (Arrays.binarySearch(a, i) < 0) {
                v.add(i);
            }
        }

        return v;
    }
}