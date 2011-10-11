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

package org.scilab.modules.ui_data.variableeditor.actions;

import javax.swing.KeyStroke;
import javax.swing.ImageIcon;
import javax.swing.JTable;

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;
import org.scilab.modules.ui_data.variableeditor.TableVariableEditor;

/**
 * RefreshAction class
 * @author Calixte DENIZET
 */
public final class SizeColumnsToFitAction extends CallBack {

    private static final String KEY = "ctrl F";
    private static final String FIT = "Size to fit";

    private SwingScilabVariableEditor editor;

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    public SizeColumnsToFitAction(SwingScilabVariableEditor editor, String name) {
        super(name);
        this.editor = editor;
    }

    /**
     * @param editor the editor
     * @param table where to put the action
     */
    public static void registerAction(SwingScilabVariableEditor editor, JTable table) {
        table.getActionMap().put(FIT, new SizeColumnsToFitAction(editor, FIT));
        table.getInputMap().put(KeyStroke.getKeyStroke(KEY), FIT);
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        TableVariableEditor table = (TableVariableEditor) editor.getCurrentTable();
	table.sizeColumnsToFit();
    }

    /**
     * Create a button for a tool bar
     * @param editor the associated editor
     * @param title tooltip for the button
     * @return the button
     */
    public static PushButton createButton(SwingScilabVariableEditor editor, String title) {
        PushButton button = ScilabPushButton.createPushButton();
        ((SwingScilabPushButton) button.getAsSimplePushButton()).addActionListener(new SizeColumnsToFitAction(editor, title));
        button.setToolTipText(title);
        ImageIcon imageIcon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/actions/SizeColumnsToFit.png");
        ((SwingScilabPushButton) button.getAsSimplePushButton()).setIcon(imageIcon);

        return button;
    }
}
