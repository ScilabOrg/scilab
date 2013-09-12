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
 * Common class for selection dependent actions.
 *
 * Children of this class will be activated when there something selected. If
 * not, the action will be disabled.
 */
public abstract class EdgeSelectionDependantAction extends DefaultAction {

    /**
     * Enable the selection if there is at least a vertex in the selection.
     */
    private final class EdgeSelectionDependantConstraint extends ActionConstraint {

        /**
         * Default constructor
         */
        public EdgeSelectionDependantConstraint() {
            super();
        }

        /**
         * @param action the action
         * @param scilabGraph the current graph
         * @see org.scilab.modules.graph.actions.base.ActionConstraint#install(org.scilab.modules.graph.actions.base.DefaultAction,
         *      org.scilab.modules.graph.ScilabGraph)
         */
        @Override
        public void install(DefaultAction action, ScilabGraph scilabGraph) {
            super.install(action, scilabGraph);

            scilabGraph.getSelectionModel().addListener(mxEvent.UNDO, this);
        }

        /**
         * @param sender the sender
         * @param evt the event
         * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
         */
        public void invoke(Object sender, mxEventObject evt) {
            mxGraphSelectionModel selection = (mxGraphSelectionModel) sender;
            Object[] cells = selection.getCells();

            boolean edgeFound = false;
            if (cells != null) {
                for (Object object : cells) {
                    if (object instanceof mxCell) {
                        mxCell cell = (mxCell) object;
                        edgeFound = cell.isEdge();
                    }

                    if (edgeFound) {
                        break;
                    }
                }

                setEnabled(edgeFound);
            }
        }

    }

    /**
     * Default constructor
     *
     * @param scilabGraph
     *            The associated graph
     */
    public EdgeSelectionDependantAction(ScilabGraph scilabGraph) {
        super(scilabGraph);

        if (scilabGraph != null) {
            EdgeSelectionDependantConstraint c = new EdgeSelectionDependantConstraint();
            c.install(this, scilabGraph);
        }
    }
}
