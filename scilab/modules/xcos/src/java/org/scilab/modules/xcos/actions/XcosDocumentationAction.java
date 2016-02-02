/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.actions;

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import javax.swing.JButton;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.bridge.CallScilabBridge;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * View Xcos documentation
 */
@SuppressWarnings(value = { "serial" })
public final class XcosDocumentationAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.XCOS_DOCUMENTATION;
    /** Icon name of the action */
    public static final String SMALL_ICON = "help-browser";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_F1;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = 0;

    /**
     * Constructor
     *
     * @param scilabGraph
     *            corresponding Scilab Graph
     */
    public XcosDocumentationAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Create a button for a graph toolbar
     *
     * @param scilabGraph
     *            corresponding Scilab Graph
     * @return the button
     */
    public static JButton createButton(ScilabGraph scilabGraph) {
        return createButton(scilabGraph, XcosDocumentationAction.class);
    }

    /**
     * Create a menu for a graph menubar
     *
     * @param scilabGraph
     *            corresponding Scilab Graph
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, XcosDocumentationAction.class);
    }

    /**
     * Action associated
     *
     * @param e
     *            the event
     * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(e);

        // action disabled when the cell is edited
        final ScilabComponent comp = ((ScilabComponent) graph.getAsComponent());
        if (comp.isEditing()) {
            return;
        }

        CallScilabBridge.openHelp("xcos");
    }

}
