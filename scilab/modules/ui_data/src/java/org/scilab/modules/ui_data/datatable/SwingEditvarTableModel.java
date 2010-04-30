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

	/**
	 * Default construction setting table data.
	 * @param data : the data to store.
	 */
	public SwingEditvarTableModel(Type[][] data) {
		this.setDataVector(data);
	}

	/**
	 * {@inheritDoc}
	 */
	public void setDataVector(Object[][] data) {
		int cols = data[0].length;
		Integer[] identifiers = new Integer[cols]; 
		for (int i = 0; i < cols; ++i) {
			identifiers[i] = i;
		}
		super.setDataVector(data, identifiers);
	}


}
