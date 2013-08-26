/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graph.actions.base;

import org.scilab.modules.graph.ScilabGraph;

import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.view.mxGraphSelectionModel;

/**
 * Constraint which specify how many blocks must be selected on the block to
 * enable the action.
 */
public class SelectedNumberOfCellsConstraint extends ActionConstraint {
    private final int numberOfSelectedCells;

    /**
     * Default constructor
     *
     * @param numberOfSelectedCells
     *            The number of selected block needed to enable the action.
     */
    public SelectedNumberOfCellsConstraint(int numberOfSelectedCells) {
        this.numberOfSelectedCells = numberOfSelectedCells;
    }

    /**
     * Install
     * @param action the action
     * @param scilabGraph the graph
     * @see org.scilab.modules.graph.actions.base.ActionConstraint#install(org.scilab.modules.graph.actions.base.DefaultAction,
     *      org.scilab.modules.graph.ScilabGraph)
     */
    @Override
    public void install(DefaultAction action, ScilabGraph scilabGraph) {
        super.install(action, scilabGraph);

        scilabGraph.getSelectionModel().addListener(mxEvent.UNDO, this);
    }

    /**
     * Update the enable state
     * @param sender The sender of the event
     * @param evt the event
     * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
     */
    public void invoke(Object sender, mxEventObject evt) {
        mxGraphSelectionModel selection = (mxGraphSelectionModel) sender;
        Object[] cells = selection.getCells();
        setEnabled((cells != null) && (cells.length >= numberOfSelectedCells));
    }

}
