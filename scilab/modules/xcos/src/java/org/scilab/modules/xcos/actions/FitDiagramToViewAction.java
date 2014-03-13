/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.actions;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import javax.swing.JButton;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.OneBlockDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Use maximum of space inside the diagram
 */
@SuppressWarnings(value = { "serial" })
public final class FitDiagramToViewAction extends OneBlockDependantAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.FIT_DIAGRAM_TO_VIEW;
    /** Icon name of the action */
    public static final String SMALL_ICON = "zoom-fit-drawing";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_SPACE;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    /**
     * Constructor
     *
     * @param scilabGraph
     *            associated diagram
     */
    public FitDiagramToViewAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Menu to add to the menubar
     *
     * @param scilabGraph
     *            associated diagram
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, FitDiagramToViewAction.class);
    }

    /**
     * Button to add to the toolbar
     *
     * @param scilabGraph
     *            associated diagram
     * @return the button
     */
    public static JButton createButton(ScilabGraph scilabGraph) {
        return createButton(scilabGraph, FitDiagramToViewAction.class);
    }

    /**
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(e);

        // action disabled when the cell is edited
        final ScilabComponent comp = ((ScilabComponent) graph.getAsComponent());
        if (comp.isEditing()) {
            return;
        }

        // If diagram is empty (has one default child) : do nothing.
        if (graph.getModel().getChildCount(graph.getDefaultParent()) < 1) {
            return;
        }

        /* Save the configuration */
        double oldZoomFactor = comp.getZoomFactor();

        comp.zoomAndCenterToCells(getGraph(null).getSelectionCells());

        /* Restore previous configuration */
        comp.setZoomFactor(oldZoomFactor);
    }
}
