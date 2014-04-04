/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import java.awt.event.ActionListener;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JOptionPane;
import javax.swing.KeyStroke;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * Default action in SciNotes
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
public class DefaultAction extends CommonCallBack {

    private static final long serialVersionUID = 3597772070169671017L;

    private final SciNotes editor;

    /**
     * Constructor
     * @param editor associated SciNotes instance
     */
    private DefaultAction(SciNotes editor) {
        super(SciNotesMessages.DEFAULT + SciNotesMessages.DOTS);
        this.editor = editor;
    }

    /**
     * Constructor
     * @param label action name
     * @param editor associated editor
     */
    protected DefaultAction(String label, SciNotes editor) {
        super(label);
        this.editor = editor;
    }

    /**
     * Get associated editor
     * @return editor instance
     */
    public SciNotes getEditor() {
        return editor;
    }

    /**
     * Create a button for a tool bar
     * @param title tooltip for the button
     * @param icon image icon
     * @param listener action listener
     * @return the button
     */
    protected static JButton createButton(String title, String icon, ActionListener listener) {
        JButton button = new JButton();
        button.addActionListener(listener);
        button.setToolTipText(title);
        if (icon == null) {
            button.setText(title);
        } else {
            button.setIcon(new ImageIcon(FindIconHelper.findIcon(icon)));
        }
        return button;
    }

    /**
     * Create a menu for a menu bar
     * @param title label for the menu
     * @param icon image icon
     * @param listener action listener
     * @param keyStroke menu shortcut
     * @return the button
     */
    protected static MenuItem createMenu(String title, String icon, DefaultAction listener, KeyStroke keyStroke) {
        MenuItem menu = ScilabMenuItem.createMenuItem();
        menu.setCallback(listener);
        menu.setText(title);

        if (keyStroke != null) {
            ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(keyStroke);
        }

        return menu;
    }

    /**
     * Default action
     */
    public void doAction() {
        JOptionPane.showMessageDialog(getEditor(), "Not Implemented Now !!!", null, JOptionPane.ERROR_MESSAGE);
    }

    /**
     * Default action
     */
    @Override
    public void callBack() {
        doAction();
    }

}
