/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.ui_data.variablebrowser.actions;

import java.awt.Toolkit;
import java.awt.event.KeyEvent;

import javax.swing.ImageIcon;
import javax.swing.JButton;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;

/**
 * RefreshAction class
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public final class RefreshAction extends CommonCallBack {

    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_W;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    private static ImageIcon icon = new ImageIcon(FindIconHelper.findIcon("view-refresh"));

    /**
     * Constructor
     * @param name the name of the action
     */
    public RefreshAction(String name) {
        super(name);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void callBack() {
        try {
            ScilabInterpreterManagement.asynchronousScilabExec(null, "browsevar");
        } catch (InterpreterException e) {
            System.err.println(e);
        }
    }

    /**
     * Create a button for a tool bar
     * @param title tooltip for the button
     * @return the button
     */
    public static JButton createButton(String title) {
        JButton button = new JButton();
        ScilabLAF.setDefaultProperties(button);
        button.addActionListener(new RefreshAction(title));
        button.setToolTipText(title);
        button.setIcon(icon);

        return button;
    }

    /**
     * Create the menu for the menubar
     * @param label the menu label
     * @return the menu
     */
    public static MenuItem createMenuItem(String label) {
        MenuItem menuItem = ScilabMenuItem.createMenuItem();
        menuItem.setText(label);
        SwingScilabMenuItem swingItem = (SwingScilabMenuItem) menuItem.getAsSimpleMenuItem();
        swingItem.setCallback(new RefreshAction(label));
        swingItem.setIcon(icon);
        return menuItem;
    }
}
