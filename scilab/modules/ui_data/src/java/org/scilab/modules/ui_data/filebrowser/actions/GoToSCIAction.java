/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.ui_data.filebrowser.actions;

import java.io.File;

import javax.swing.JButton;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.ui_data.FileBrowser;
import org.scilab.modules.ui_data.filebrowser.FileUtils;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * Action to go to SCI
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class GoToSCIAction extends CommonCallBack {

    /**
     * {@inheritDoc}
     */
    public GoToSCIAction() {
        super(null);
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        File f = new File(System.getenv("SCI"));
        if (f.exists() && f.isDirectory() && f.canRead()) {
            InterpreterManagement.requestScilabExec("chdir('" + f.getAbsolutePath() + "')");
            FileBrowser.setBaseDir(f.getAbsolutePath());
        }
    }

    /**
     * @return the corresponding button
     */
    public static JButton createButton() {
        JButton button = new JButton();
        ScilabLAF.setDefaultProperties(button);
        button.addActionListener(new GoToSCIAction());
        button.setToolTipText(UiDataMessages.SCIFOLDER);
        button.setIcon(FileUtils.getSCIIcon());

        return button;
    }
}
