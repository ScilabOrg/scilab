/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.ui_data.datatable;

import javax.swing.table.DefaultTableModel;


/**
 * Swing implementation of table model.
 * @param <Type> Type of data to be stored by the model
 */
public class SwingEditvarTableModel<Type> extends DefaultTableModel {

	private static final long serialVersionUID = -4255704246347716837L;
	private int scilabMatrixRowCount;
	private int scilabMatrixColCount;

	/**
	 * Default construction setting table data.
	 * @param data : the data to store.
	 */
	public SwingEditvarTableModel(Type[][] data) {
		scilabMatrixColCount =  data[0].length;
		scilabMatrixRowCount =  data.length;
		this.setDataVector(data);
		expandTable();
		
	}

	/**
	 * {@inheritDoc}
	 */
	public void setDataVector(Type[][] data) {
		int cols = data[0].length;
		Integer[] identifiers = new Integer[cols]; 
		for (int i = 0; i < cols; ++i) {
			identifiers[i] = i+1;
		}
		super.setDataVector(data, identifiers);
		
	}
	
	
	@Override
	public void setValueAt(Object value, int row, int col) {


		Object defaultValue = "";
		
		
		for (int i = scilabMatrixRowCount; i <= Math.max(row, scilabMatrixRowCount-1); i++){
			for(int j = 0; j <= Math.max(col,scilabMatrixColCount-1); j++) {

				super.setValueAt(defaultValue, i, j);
			}
		}
		
		for (int i = scilabMatrixColCount; i <= Math.max(col,scilabMatrixColCount-1); i++){
			for(int j = 0; j <= Math.max(row, scilabMatrixRowCount-1); j++) {
				super.setValueAt(defaultValue, j, i);
			}
		}
		
		super.setValueAt(value, row, col);
		

		if(col > scilabMatrixColCount || row > scilabMatrixRowCount) {
			if (col > scilabMatrixColCount){
				scilabMatrixColCount = col+1;
			}
			if (row > scilabMatrixRowCount) {
				scilabMatrixRowCount = row+1;
			}
			expandTable();
		}
		


		
	}
	
	public void expandTable() {
		int newTableColCount = 42;
		int newTableRowCount = 42;
		
		// row
		if (getColumnCount() > 32) {
			newTableColCount = scilabMatrixColCount + 10;
		}
		for(int i = getColumnCount(); i < newTableColCount; i++) {
			addColumn(i+1);
		}
		
		// col
		if (getRowCount() > 32) {
			newTableRowCount = scilabMatrixRowCount + 10;
		}
		for(Integer i = getRowCount(); i < newTableRowCount; i++) {
			Type[] newCol =(Type[]) new Object[scilabMatrixRowCount];
			addRow(newCol);

		}
	}
	

}
