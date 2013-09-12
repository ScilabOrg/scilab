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

package org.scilab.modules.xcos.block.actions.alignement;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Align block on the middle
 */
@SuppressWarnings(value = { "serial" })
public final class AlignBlockActionMiddle extends AlignBlockAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.ALIGN_MIDDLE;
    /** Icon name of the action */
    public static final String SMALL_ICON = "align-vertical-center";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = 0;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = 0;

    /**
     * Default constructor
     *
     * @param scilabGraph
     *            the graph to work on
     */
    public AlignBlockActionMiddle(ScilabGraph scilabGraph) {
        super(scilabGraph, XcosConstants.ALIGN_MIDDLE);
    }

    /**
     * Create the menu associated with this action.
     *
     * @param scilabGraph
     *            the associated graph
     * @return The associated menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, AlignBlockActionMiddle.class);
    }
}
