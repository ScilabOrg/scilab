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

package org.scilab.modules.ui_data.filebrowser;

import javax.swing.SwingUtilities;

import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.ui_data.tabfactory.FileBrowserTab;
import org.scilab.modules.ui_data.tabfactory.FileBrowserTabFactory;
import org.scilab.modules.ui_data.utils.UiDataMessages;

public final class ScilabFileBrowser {

    private static SwingScilabFileBrowser instance;

    static {
        ScilabTabFactory.getInstance().addTabFactory(FileBrowserTabFactory.getInstance());
    }

    /**
     * Constructor
     */
    private ScilabFileBrowser() {
        TextBox infobar = ScilabTextBox.createTextBox();
        instance = new SwingScilabFileBrowser();
        instance.addInfoBar(infobar);
        instance.setTitle(UiDataMessages.FILE_BROWSER);
    }

    public static SwingScilabFileBrowser createFileBrowserTab() {
        if (instance == null) {
            new ScilabFileBrowser();
        }

        return instance;
    }

    /**
     * @return true if an instance of BrowseVar already exists.
     */
    public static boolean isFileBrowserOpened() {
        return instance != null;
    }

    /**
     * Get the variable browser singleton with specified columns title.
     * @return the File Browser
     */
    public static SwingScilabFileBrowser getFileBrowser() {
        if (instance == null) {
            boolean success = WindowsConfigurationManager.restoreUUID(SwingScilabFileBrowser.FILEBROWSERUUID);
            if (!success) {
                FileBrowserTab.getFileBrowserInstance();
                SwingScilabWindow window = (SwingScilabWindow) ScilabWindow.createWindow().getAsSimpleWindow();
                window.addTab(instance);
                window.setLocation(0, 0);
                window.setSize(500, 500);
                window.setVisible(true);
            }
        } else {
            SwingScilabWindow window = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, instance);
            window.setVisible(true);
            window.toFront();
        }

        return instance;
    }

    /**
     * Close File Browser
     */
    public static void closeFileBrowser() {
        if (instance != null) {
            instance.close();
        }
    }

    /**
     * Close File Browser
     */
    public void close() {
        instance = null;
    }

    /**
     * {@inheritDoc}
     */
    public void setVisible(boolean status) {
        instance.setVisible(status);
    }
}
