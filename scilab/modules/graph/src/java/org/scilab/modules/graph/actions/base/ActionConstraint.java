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

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import org.scilab.modules.graph.ScilabGraph;

import com.mxgraph.util.mxEventSource.mxIEventListener;

/**
 * Specify function called to add constraint on an action.
 */
public abstract class ActionConstraint implements mxIEventListener {
    private boolean enabled;
    private DefaultAction action;

    /**
     * Install this constraint on a graph for the specific action.
     *
     * Override to action to install the current listener on a graph. Call the
     * super.install(...) in order to automatically register your action.
     *
     * @param action
     *            the constrained action
     * @param scilabGraph
     *            where to install constraint
     */
    public void install(final DefaultAction action, ScilabGraph scilabGraph) {
        this.action = action;

        /**
         * assume that at installation time, the constraint is not valid.
         */
        setEnabled(false);

        /*
         * Disable actions when cells are locked
         */
        scilabGraph.addPropertyChangeListener(new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent evt) {
                if (evt.getPropertyName().equals("cellsLocked")) {
                    boolean locked = (Boolean) evt.getNewValue();
                    action.setEnabled(!locked);
                }
            }
        });

        // If the current constraint is not valid : force setEnable(false).
        // This will force the current constraint.
        action.addPropertyChangeListener(new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent evt) {
                if (!isEnabled()) {
                    action.setEnabled(false);
                }
            }
        });
    }

    /**
     * @return That status of the action according to implementation conditions.
     */
    public boolean isEnabled() {
        return enabled;
    }

    /**
     * @param enabled Update the enable status of this constraint.
     */
    public void setEnabled(boolean enabled) {
        this.enabled = enabled;

        action.setEnabled(enabled);
    }
}
