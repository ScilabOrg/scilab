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

import org.scilab.modules.gui.bridge.ScilabBridge;

/**
 * Interface for file chooser used by Scilab function such as uigetfile
 * @author Vincent COUVERT
 * @author Sylvestre KOUMAR
 */
public class ScilabFileChooser implements FileChooser {

    private SimpleFileChooser component;

    /**
     * Constructor
     */
    protected ScilabFileChooser() {
        component = ScilabBridge.createFileChooser();
    }

    /**
     * Constructor
     * @param figureId id of the exported figure
     */
    protected ScilabFileChooser(Integer figureId) {
        component = ScilabBridge.createExportFileChooser(figureId);
    }

    /**
     * Creates a Scilab FileChooser
     * @return the created file chooser
     */
    public static FileChooser createFileChooser() {
        return new ScilabFileChooser();
    }

    /**
     * Creates a Scilab ExportFileChooser
     * @param figureId int
     * @return the created export file chooser
     */
    public static FileChooser createExportFileChooser(Integer figureUID) {
        return new ScilabFileChooser(figureUID);
    }

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    public SimpleFileChooser getAsSimpleFileChooser() {
        return component;
    }

    /**
     * Set the title of the file chooser
     * @param title the title to set
     */
    public void setTitle(String title) {
        ScilabBridge.setTitle(this, title);
    }

    /**
     * Set the initial directory used for file search
     * @param path the default path
     */
    public void setInitialDirectory(String path) {
        ScilabBridge.setInitialDirectory(this, path);
    }

    /**
     * Display this chooser and wait for user selection
     */
    public void displayAndWait() {
        ScilabBridge.displayAndWait(this);
    }

    /**
     * Get the number of files selected
     * @return the number of files selected
     */
    public int getSelectionSize() {
        return ScilabBridge.getSelectionSize(this);
    }

    /**
     * Get the names of selected files
     * @return the names of selected files
     */
    public String[] getSelection() {
        return ScilabBridge.getSelection(this);
    }

    /**
     * Set the flag indicating that we want only select directories
     */
    public void setDirectorySelectionOnly() {
        ScilabBridge.setDirectorySelectionOnly(this);
    }

    /**
     * set the multiple file selection on the directory for the filechooser
     * @param multipleSelection for file(s)
     */
    public void setMultipleSelection(boolean multipleSelection) {
        ScilabBridge.setMultipleSelection(this, multipleSelection);
    }

    /**
     * Get the path of selected files
     * @return the path of selected files
     */
    public String getSelectionPathName() {
        return ScilabBridge.getSelectionPathName(this);
    }

    /**
     * Get the names of selected files
     * @return the names of selected files
     */
    public String[] getSelectionFileNames() {
        return ScilabBridge.getSelectionFileNames(this);
    }

    /**
     * Set the dialog type (save or open a file ?)
     * @param dialogType the dialog type
     */
    public void setUiDialogType(int dialogType) {
        ScilabBridge.setUiDialogType(this, dialogType);
    }
}
