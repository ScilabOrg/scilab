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

package org.scilab.modules.ui_data.datatable;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.table.DefaultTableModel;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.ui_data.variableeditor.ScilabVariableEditor;
import org.scilab.modules.ui_data.variableeditor.celleditor.ScilabGenericCellEditor;

/**
 * Swing implementation of table model.
 * @author Allan SIMON
 * @author Calixte DENIZET
 */
public class SwingEditvarTableModel extends DefaultTableModel {

    private static final long serialVersionUID = -4255704246347716837L;
    private static final ActionListener EMPTYACTION = new ActionListener() {
            public void actionPerformed(ActionEvent e) { }
        };

    private int scilabMatrixRowCount;
    private int scilabMatrixColCount;
    private String type;
    private ScilabGenericCellEditor cellEditor;

    /**
     * Default construction setting table data.
     * @param data : the data to store.
     */
    public SwingEditvarTableModel(String type, Object[][] data, ScilabGenericCellEditor cellEditor) {
        scilabMatrixRowCount = data.length;
        scilabMatrixColCount = 0;
        if (scilabMatrixRowCount != 0) { // Not an empty matrix
            scilabMatrixColCount =  data[0].length;
        }
        this.type = type;
        this.setDataVector(data);
        this.cellEditor = cellEditor;
        expandTable();
    }

    /**
     * {@inheritDoc}
     */
    public void setDataVector(Object[][] data) {
        int cols = 0;
        if (data.length != 0) { // Not an empty matrix
            cols = data[0].length;
        }
        Integer[] identifiers = new Integer[cols];
        for (int i = 0; i < cols; ++i) {
            identifiers[i] = i + 1;
        }
        super.setDataVector(data, identifiers);

    }

    /**
     * @return the CellEditor used in this tableModel
     */
    public ScilabGenericCellEditor getCellEditor() {
        return cellEditor;
    }

    /**
     * @param row the row
     * @param col the col
     * @return the scilab expression corresponding to the value
     */
    public String getScilabValueAt(int row, int col) {
        String str = cellEditor.getDataAsScilabString(getValueAt(row, col));
        if (str.length() == 0) {
            str = getDefaultStringValue();
        }

        if (type.equals("String")) {
            str = "\"" + str + "\"";
        }

        return str;
    }

    /**
     * @param mustUpdate if true, update in Scilab (useful with paste action)
     * @param value the value
     * @param row the row
     * @param col the col
     */
    public void setValueAtAndUpdate(boolean mustUpdate, Object value, int row, int col) {
        if (value == null || (value instanceof String && ((String) value).length() == 0)) {
            return;
        }

        boolean defaultAdded = false;
        for (int i = scilabMatrixRowCount; i <= Math.max(row, scilabMatrixRowCount - 1); i++) {
            for (int j = 0; j <= Math.max(col, scilabMatrixColCount - 1); j++) {
                super.setValueAt(getDefaultValue(), i, j);
                if (!defaultAdded) {
                    defaultAdded = true;
                }
            }
        }

        for (int i = scilabMatrixColCount; i <= Math.max(col, scilabMatrixColCount - 1); i++) {
            for (int j = 0; j <= Math.max(row, scilabMatrixRowCount - 1); j++) {
                super.setValueAt(getDefaultValue(), j, i);
                if (!defaultAdded) {
                    defaultAdded = true;
                }
            }
        }

        String exp = value.toString();

        if (exp.startsWith("=")) {
            exp = exp.substring(1);
            super.setValueAt(exp, row, col);
            // keep the expression
            if (exp.length() > 0) {
                cellEditor.addExpression(exp, row, col);
            }
        } else {
            cellEditor.removeExpression(row, col);
            super.setValueAt(value, row, col);
        }

        if (col >= scilabMatrixColCount || row >= scilabMatrixRowCount) {
            if (col >= scilabMatrixColCount) {
                scilabMatrixColCount = col + 1;
            }
            if (row >= scilabMatrixRowCount) {
                scilabMatrixRowCount = row + 1;
            }
            expandTable();
        }

        if (mustUpdate) {
            if (defaultAdded) {
                updateMatrix();
            } else {
                updateMatrix(exp, row, col);
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public void setValueAt(Object value, int row, int col) {
        setValueAtAndUpdate(true, value, row, col);
    }

    /**
     * Update all the matrix on the Scilab's side.
     */
    public void updateMatrix() {
        String matrix = getDataAsScilabString(getMatrix());
        String variableName = ScilabVariableEditor.getVariableEditor().getVariablename();
        String expr = variableName + "=" + matrix;
        String command = buildScilabRequest(expr);
        execCommand(command);
    }

    /**
     * Expand the table
     */
    public void expandTable() {
        int newTableColCount = 42;
        int newTableRowCount = 42;
        // row
        if (getColumnCount() > 32) {
            newTableColCount = scilabMatrixColCount + 10;
        }
        for (int i = getColumnCount(); i < newTableColCount; i++) {
            addColumn(i + 1);
        }
        // col
        if (getRowCount() > 32) {
            newTableRowCount = scilabMatrixRowCount + 10;
        }
        for (Integer i = getRowCount(); i < newTableRowCount; i++) {
            Object[] newRow = new Object[newTableColCount];
            addRow(newRow);
        }
    }

    /**
     * Refresh the current matrix
     */
    public void refreshMatrix() {
	execCommand("editvar(\"" + ScilabVariableEditor.getVariableEditor().getVariablename() + "\");");
    }

    /**
     * @return the default value according to this type
     */
    private Object getDefaultValue() {
        if (type.equals("String")) {
            return "";
        }

        if (type.equals("Double")) {
            return new Double(0.0);
        }

        if (type.equals("Integer")) {
            return new Integer(0);
        }

        if (type.equals("Complex")) {
            return new Double[]{0.0, 0.0};
        }

        if (type.equals("Boolean")) {
            return Boolean.FALSE;
        }

        return null;
    }

    /**
     * @return the default representation according to the type
     */
    private String getDefaultStringValue() {
        if (type.equals("String")) {
            return "";
        }

        if (type.equals("Double") || type.equals("Complex") || type.equals("Integer")) {
            return "0";
        }

        if (type.equals("Boolean")) {
            return "%f";
        }

        return null;
    }

    /**
     * @param value the value to update on the Scilab's side
     * @param row the row coord
     * @param col the col coord
     */
    private void updateMatrix(Object value, int row, int col) {
        String variableName = ScilabVariableEditor.getVariableEditor().getVariablename();
        String val = getScilabValueAt(row, col);
        String coords = "";
        if (scilabMatrixRowCount != 1 || scilabMatrixColCount != 1) {
            coords = "(" + (row + 1) + "," + (col + 1) + ")";
        }
        String expr = variableName + coords + "=" + getDataAsScilabString(val);
        String command = buildScilabRequest(expr);
        execCommand(command);
    }

    /**
     * @param command the command to execute.
     */
    private void execCommand(String command) {
        try {System.out.println("command="+command);
            ScilabInterpreterManagement.asynchronousScilabExec(EMPTYACTION, command);
        } catch (InterpreterException e1) {
            System.err.println("An error in the interpreter has been catched: " + e1.getLocalizedMessage());
        }
    }

    /**
     * @param expr the expr to put in the request.
     * @return the request
     */
    private String buildScilabRequest(String expr) {
        StringBuilder command = new StringBuilder();
        command.append("if execstr(\"" + expr + ";" + cellEditor.getAllExpressions() + "\", \"errcatch\") <> 0 then ");
        command.append("messagebox(\"Could not edit variable: \" + lasterror() + \"\"");
        command.append(",\"Variable editor\", \"error\", \"modal\");");
        command.append("end ");
        command.append("editvar(\"" + ScilabVariableEditor.getVariableEditor().getVariablename() + "\");");
        return command.toString();
    }

    /**
     * @return the Scilab matrix for this JTable
     */
    private String getMatrix() {
        String str = "[";
        for (int i = 0; i < scilabMatrixRowCount; i++) {
            for (int j = 0; j < scilabMatrixColCount; j++) {
                if (j < scilabMatrixColCount - 1) {
                    str += getScilabValueAt(i, j) + ",";
                } else if (i < scilabMatrixRowCount - 1) {
                    str += getScilabValueAt(i, j) + ";";
                } else {
                    str += getScilabValueAt(i, j) + "]";
                }
            }
        }

        return str;
    }

    /**
     * @param str the string where replace " and '
     */
    private String getDataAsScilabString(String str) {
        String data = str.replace("\"", "\"\""); // Change " to "" because added in an execstr command
        data = data.replace("'", "''"); // Change ' to '' because added in an execstr command

        return data;
    }
}
