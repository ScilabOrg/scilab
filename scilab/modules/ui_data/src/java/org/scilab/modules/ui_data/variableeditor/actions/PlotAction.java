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

package org.scilab.modules.ui_data.variableeditor.actions;

import java.awt.Toolkit;
import java.awt.datatransfer.StringSelection;

import javax.swing.KeyStroke;
import javax.swing.ImageIcon;
import javax.swing.JMenuItem;
import javax.swing.JTable;

import org.scilab.modules.commons.gui.ScilabKeyStroke;

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;
import org.scilab.modules.ui_data.UI_data;
import org.scilab.modules.ui_data.datatable.SwingEditvarTableModel;
import org.scilab.modules.ui_data.variableeditor.SwingScilabVariableEditor;

/**
 * RefreshAction class
 * @author Calixte DENIZET
 */
public final class PlotAction extends CallBack {

    public static final int PLOT2D = 0;
    public static final int MATPLOT = 1;
    public static final int GRAYPLOT = 2;
    public static final int SGRAYPLOT = 3;
    public static final int CHAMP = 4;
    public static final int HISTPLOT = 5;
    public static final int MESH = 6;
    public static final int SURF = 7;

    private static final String COM_PLOT2D = "plot2d(%s)";
    private static final String COM_MATPLOT = "Matplot(%s)";
    private static final String COM_GRAYPLOT = "grayplot(1:size(%s,1),1:size(%s,2),%s)";
    private static final String COM_SGRAYPLOT = "Sgrayplot(1:size(%s,1),1:size(%s,2),%s)";
    private static final String COM_CHAMP = "champ(1:size(%s,1),1:size(%s,2),real(%s),imag(%s))";
    private static final String COM_HISTPLOT = "histplot(10,%s)";
    private static final String COM_MESH = "mesh(%s)";
    private static final String COM_SURF = "surf(%s)";
    
    private static final String[] COMMANDS = new String[]{COM_PLOT2D, COM_MATPLOT, COM_GRAYPLOT, COM_SGRAYPLOT, COM_CHAMP, COM_HISTPLOT, COM_MESH, COM_SURF};

    private static final String CREATE = "Create";

    private SwingScilabVariableEditor editor;
    private int type;

    /**
     * Constructor
     * @param editor the editor
     * @param name the name of the action
     */
    private PlotAction(SwingScilabVariableEditor editor, String name, int type) {
        super(name);
        this.editor = editor;
	this.type = type;
    }

    /**
     * {@inheritDoc}
     */
    public void callBack() {
        JTable table = editor.getCurrentTable();
        int[] cols = table.getSelectedColumns();
        int[] rows = table.getSelectedRows();
        if (cols.length > 0 && rows.length > 0) {
            table.setColumnSelectionInterval(cols[0], cols[cols.length - 1]);
            table.setRowSelectionInterval(rows[0], rows[rows.length - 1]);

            SwingEditvarTableModel model = (SwingEditvarTableModel) table.getModel();
            int rowC = model.getScilabMatrixRowCount();
            int colC = model.getScilabMatrixColCount();

            if (rows[0] >= rowC || cols[0] >= colC) {
                return;
            }

            int rowE = Math.min(rows[rows.length - 1], rowC);
            int colE = Math.min(cols[cols.length - 1], colC);

	    String varName = UI_data.getUnnamedVariable();
	    String com = COMMANDS[type];
	    switch (type) {
	    case 0:
	    case 1:
	    case 5:
	    case 6:
	    case 7:
		com = String.format(com, varName);
		break;
	    case 2:
	    case 3:
		com = String.format(com, varName, varName, varName);
		break;
	    case 4:
		com = String.format(com, varName, varName, varName, varName);
		break;
	    }

            model.execPlot(rows[0], rowE, cols[0], colE, varName, com);
        }
    }

    /**
     * Create a button for a tool bar
     * @param editor the associated editor
     * @param title tooltip for the button
     * @return the button
     */
    public static PushButton createButton(SwingScilabVariableEditor editor, String title) {
        PushButton button = ScilabPushButton.createPushButton();
        ((SwingScilabPushButton) button.getAsSimplePushButton()).addActionListener(new PlotAction(editor, title, 0));
        button.setToolTipText(title);
        ImageIcon imageIcon = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/edit-copy.png");
        ((SwingScilabPushButton) button.getAsSimplePushButton()).setIcon(imageIcon);

        return button;
    }

    /**
     * Create a menu item
     * @param editor the associated editor
     * @param title the menu title
     * @return the menu item
     */
    public static JMenuItem createMenuItem(SwingScilabVariableEditor editor, String title, int type) {
        JMenuItem mi = new JMenuItem(title);
        mi.addActionListener(new PlotAction(editor, title, type));

        return mi;
    }
}
