/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2016 - Scilab Enteprises - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.ui_data.filebrowser.actions;

import java.awt.event.ActionEvent;
import java.io.File;
import javax.swing.AbstractAction;
import javax.swing.JButton;
import javax.swing.filechooser.FileSystemView;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.ui_data.FileBrowser;

/**
 * Change to any directory action
 */
public class ChangeToAnyDirAction extends AbstractAction {
    final File path;

    public ChangeToAnyDirAction(File path) {
        super(FileSystemView.getFileSystemView().getSystemDisplayName(path),
              FileSystemView.getFileSystemView().getSystemIcon(path));

        this.path = path;
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (path.exists() && path.isDirectory() && path.canRead()) {
            InterpreterManagement.requestScilabExec("chdir('" + path.getAbsolutePath() + "');");
            FileBrowser.setBaseDir(path.getAbsolutePath());
        }
    }

    public static JButton createButton(File path) {
        JButton btn = new JButton(new ChangeToAnyDirAction(path));

        btn.setToolTipText(FileSystemView.getFileSystemView().getSystemTypeDescription(path));
        ScilabLAF.setDefaultProperties(btn);

        return btn;
    }
}
