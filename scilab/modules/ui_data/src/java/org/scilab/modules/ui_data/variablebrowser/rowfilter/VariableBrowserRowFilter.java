/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.variablebrowser.rowfilter;

import java.util.HashSet;

import javax.swing.RowFilter;

import org.scilab.modules.types.ScilabTypeEnum;

public class VariableBrowserRowFilter extends RowFilter<Object, Object>{

	private static final int TYPE_INDEX = 3;
	
	private HashSet<ScilabTypeEnum> filteredValues;
	
	public VariableBrowserRowFilter() {
		super();
		filteredValues = new HashSet<ScilabTypeEnum>();
	}
	
	public VariableBrowserRowFilter(HashSet<ScilabTypeEnum> filteredValues) {
		this.filteredValues = filteredValues;
	}
	
	@Override
	public boolean include(Entry< ? extends Object, ? extends Object> entry) {
		Integer currentRowVariableclassNumber =  (Integer) entry.getValue(TYPE_INDEX);
		
		try {
			if (filteredValues.contains(ScilabTypeEnum.swigToEnum(currentRowVariableclassNumber))) {
				return false;
			}
		} catch (IllegalArgumentException e) {
			// This case should never occur
			// But there is a bug in Scilab global variables type detection
			// So we show these "unknown type" variables by default (See bug #7333)
			return true;
		}
		
		return true;
	}

}
