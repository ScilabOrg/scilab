/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
<<<<<<< Updated upstream:scilab/modules/ui_data/src/java/org/scilab/modules/ui_data/variableeditor/SimpleVariableEditor.java
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
=======
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
>>>>>>> Stashed changes:scilab/modules/ui_data/src/java/org/scilab/modules/ui_data/variableeditor/SimpleVariableEditor.java
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.ui_data.variableeditor;

import org.scilab.modules.gui.tab.Tab;


/**
 * Interface for Variable editor.
 */

public interface SimpleVariableEditor extends Tab {

	/**
	 * Set data to be displayed
	 * @param data : the data displayed in JTable
	 */
	void setData(Object[][] data);

}
