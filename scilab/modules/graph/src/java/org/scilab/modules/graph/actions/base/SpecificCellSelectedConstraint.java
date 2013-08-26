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

import com.mxgraph.model.mxCell;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.view.mxGraphSelectionModel;

/**
 * Update a menu when a specific kind of cell is selected.
 */
public class SpecificCellSelectedConstraint extends ActionConstraint {
    private final Class < ? extends mxCell > kind;

    /**
     * Default constructor
     * @param kind The kind of cell which enable/disbale the action.
     */
    public SpecificCellSelectedConstraint(Class < ? extends mxCell > kind) {
        this.kind = kind;
    }

    /**
     * Install
     *
     * @param action
     *            the current action
     * @param scilabGraph
     *            tthe scilab graph
     * @see org.scilab.modules.graph.actions.base.ActionConstraint#install(org.scilab.modules.graph.actions.base.DefaultAction,
     *      org.scilab.modules.graph.ScilabGraph)
     */
    @Override
    public void install(DefaultAction action, ScilabGraph scilabGraph) {
        super.install(action, scilabGraph);

        scilabGraph.getSelectionModel().addListener(mxEvent.UNDO, this);
    }

    /**
     * @param sender the sender object
     * @param evt the event
     * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
     */
    public void invoke(Object sender, mxEventObject evt) {
        mxGraphSelectionModel selection = (mxGraphSelectionModel) sender;
        Object[] cells = selection.getCells();

        for (Object object : cells) {
            if (kind.isInstance(object)) {
                setEnabled(true);
                return;
            }
        }

        setEnabled(false);
    }

}
