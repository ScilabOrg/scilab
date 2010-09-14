/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.variableeditor.action;

import javax.swing.ImageIcon;

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;

/**
 * ReloadAction class
 * @author Calixte DENIZET
 */
public final class RefreshAction extends CallBack {

    private SwingScilabVariableEditor editor;

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    public RefreshAction(SwingScilabVariableEditor editor, String name) {
        super(name);
        this.editor = editor;
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        ((SwingEditvarTableModel) editor.getCurrentTable().getModel()).refreshMatrix();
    }

    /**
     * Create a button for a tool bar
     * @param title tooltip for the button
     * @param icon image icon
     * @param listener action listener
     * @return the button
     */
    public static PushButton createButton(SwingScilabVariableEditor editor, String title, String icon) {
        PushButton button = ScilabPushButton.createPushButton();
        ((SwingScilabPushButton) button.getAsSimplePushButton()).addActionListener(new RefreshAction(editor, title));
        button.setToolTipText(title);
        if (icon == null) {
            button.setText(title);
        } else {
            ((SwingScilabPushButton) button.getAsSimplePushButton()).setIcon(new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/actions/" + icon));
        }
        return button;
    }
}
