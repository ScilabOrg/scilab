/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2008 - DIGITEO - Sylvestre KOUMAR
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.filechooser;

/**
 * Interface for file chooser used by Scilab function such as uigetfile
 * @author Vincent COUVERT
 * @author Sylvestre KOUMAR
 */
public interface FileChooser {

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    SimpleFileChooser getAsSimpleFileChooser();

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
     * Set the flag indicating that we want only select files
     * @param multipleSelection enable the multiple selection
     */
    void setMultipleSelection(boolean multipleSelection);

    /**
     * Get the path name of selected files
     * @return the path name of selected files
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
