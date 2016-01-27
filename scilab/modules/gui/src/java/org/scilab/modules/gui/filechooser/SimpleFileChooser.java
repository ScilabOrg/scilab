/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2008 - DIGITEO - Sylvestre KOUMAR
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */

package org.scilab.modules.gui.filechooser;

/**
 * Interface for SimpleFileChooser the object associated to Scilab File Choosers
 * @author Vincent COUVERT
 * @author Sylvestre KOUMAR
 */
public interface SimpleFileChooser {

    /**
     * Set the title of the file chooser
     * @param title the title to set
     */
    void setTitle(String title);

    /**
     * Set the initial directory used for file search
     * @param path the default path
     */
    void setInitialDirectory(String path);

    /**
     * Display this chooser and wait for user selection
     */
    void displayAndWait();

    /**
     * Get the number of files selected
     * @return the number of files selected
     */
    int getSelectionSize();

    /**
     * Get the names of selected files
     * @return the names of selected files
     */
    String[] getSelection();

    /**
     * Set the flag indicating that we want only select directories
     */
    void setDirectorySelectionOnly();

    /**
     * Set the flag indicating that we can select multiple files
     * @param multipleSelection enable multiple selection
     */
    void setMultipleSelection(boolean multipleSelection);

    /**
     * Get the path of selected files
     * @return the path of selected files
     */
    String getSelectionPathName();

    /**
     * Get the names of selected files
     * @return the names of selected files
     */
    String[] getSelectionFileNames();

    /**
     * Set the dialog type (save or open a file ?)
     * @param dialogType the dialog type
     */
    void setUiDialogType(int dialogType);
}
