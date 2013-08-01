/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.utils;

import com.mxgraph.util.mxEvent;

/**
 * Add Xcos specialized events to mxGraph
 */
public final class XcosEvent extends mxEvent {

    /**
     * Fired when something as been done and the cell need to be updated
     */
    public static final String FORCE_CELL_VALUE_UPDATE = "ForceCellValueUpdate";

    /**
     * Fired when a {@link org.scilab.modules.xcos.block.SuperBlock} I/O block
     * as been updated by the user
     */
    public static final String IO_PORT_VALUE_UPDATED = "IoPortValueUpdated";

    /**
     * Fired when an {@link org.scilab.modules.xcos.block.SuperBlock} as been
     * updated by the user
     */
    public static final String SUPER_BLOCK_UPDATED = "superBlockUpdated";

    /**
     * Fired when the diagram has been updated since previous execution
     */
    public static final String DIAGRAM_UPDATED = "DiagramUpdated";

    /**
     * Fired when a block values has been updated.
     */
    public static final String UPDATE_BLOCK_VALUES = "UpdateBlocksValues";

    /**
     * Fired when a block values has been updated.
     */
    public static final String BLOCK_VALUES_UPDATED = "BlockValuesUpdated";

    /** This class is a static singleton, thus it must not be instantiated */
    private XcosEvent() {
    }
}
