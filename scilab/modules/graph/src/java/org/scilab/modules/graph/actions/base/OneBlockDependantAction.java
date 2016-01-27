/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */

package org.scilab.modules.graph.actions.base;

import org.scilab.modules.graph.ScilabGraph;

/**
 * Common class for actions that need at least 1 cell on the graph.
 */
public abstract class OneBlockDependantAction extends DefaultAction {
    /**
     * Default constructor
     * @param scilabGraph the graph to work on
     */
    public OneBlockDependantAction(ScilabGraph scilabGraph) {
        super(scilabGraph);

        MinimalNumberOfCellsConstraint constraint = new MinimalNumberOfCellsConstraint(1);
        constraint.install(this, scilabGraph);
    }
}
