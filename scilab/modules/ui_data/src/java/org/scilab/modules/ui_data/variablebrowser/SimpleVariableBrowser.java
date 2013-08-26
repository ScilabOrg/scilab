/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.ui_data.variablebrowser;

/**
 * Interface for Variable browser.
 */
public interface SimpleVariableBrowser {

    /**
     * Set columns title
     * @param columnNames : the columns Title
     */
    void setColumnNames(String[] columnNames);

    /**
     * Set data to be displayed
     * @param data : the data displayed in JTable
     */
    void setData(Object[][] data);

    /**
     * Update the row filtering
     */
    void updateRowFiltering();

}
