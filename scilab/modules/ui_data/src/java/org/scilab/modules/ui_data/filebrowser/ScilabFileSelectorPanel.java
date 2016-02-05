/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

package org.scilab.modules.ui_data.filebrowser;

import java.awt.BorderLayout;

import javax.swing.JButton;
import javax.swing.JPanel;

import org.scilab.modules.ui_data.filebrowser.actions.ChangeCWDAction;

/**
 * The JPanel containing the combobox used to set the path and the two buttons
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class ScilabFileSelectorPanel extends JPanel {

    private SwingScilabTreeTable table;

    /**
     * Default constructor
     * @param table the table linked with the combobox
     */
    public ScilabFileSelectorPanel(SwingScilabTreeTable table) {
        super();
        this.table = table;
        init();
    }

    /**
     * Init the panel contents
     */
    private void init() {
        setLayout(new BorderLayout());
        JButton changeDirButton = ((ChangeCWDAction) table.getActionMap().get("cwd")).createButton();

        add(changeDirButton, BorderLayout.WEST);
        add(table.getComboBox(), BorderLayout.CENTER);
    }
}
