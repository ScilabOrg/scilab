/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graph.actions;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.GraphActionManager;
import org.scilab.modules.graph.actions.base.VertexSelectionDependantAction;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.gui.menuitem.MenuItem;

import com.mxgraph.swing.handler.mxGraphTransferHandler;

/**
 * Copy manager
 */
@SuppressWarnings(value = { "serial" })
public final class CopyAction extends VertexSelectionDependantAction {
    /** Name of the action */
    public static final String NAME = ScilabGraphMessages.COPY;
    /** Icon name of the action */
    public static final String SMALL_ICON = "edit-copy";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_C;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    /**
     * Constructor
     * @param scilabGraph corresponding Scilab Graph
     */
    public CopyAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Create a menu for a graph menubar
     * @param scilabGraph corresponding Scilab Graph
     * @return the menu
     */
    public static MenuItem copyMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, CopyAction.class);
    }

    /**
     * Action associated
     * @param e the event
     * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        mxGraphTransferHandler.getCopyAction().actionPerformed(new ActionEvent(getGraph(e).getAsComponent(), e.getID(), e.getActionCommand()));

        // Enable the paste action
        GraphActionManager.setEnable(PasteAction.class, true);
    }
}
