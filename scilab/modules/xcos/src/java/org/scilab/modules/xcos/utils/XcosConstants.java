/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement David
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.utils;

import org.scilab.modules.graph.utils.ScilabGraphConstants;

/**
 * Contains all the constants used trough the source code.
 */
public final class XcosConstants extends ScilabGraphConstants {

    /** Define the default block width on the palette */
    public static final int PALETTE_BLOCK_WIDTH = 100;
    /** Define the default block height on the palette */
    public static final int PALETTE_BLOCK_HEIGHT = 100;

    /** Define the default block height on the palette */
    public static final double PALETTE_BLOCK_ICON_RATIO = 1.5;

    /** Define the default block horizontal margin on the palette */
    public static final int PALETTE_HMARGIN = 5;
    /** Define the default block vertical margin on the palette */
    public static final int PALETTE_VMARGIN = 5;

    /** Define the maximum number of char that might be represented as style */
    public static final int MAX_CHAR_IN_STYLE = 24;

    /* Events */
    /**
     * When a block changed
     */
    public static final String EVENT_BLOCK_UPDATED = "block";
    /** Change event old name */
    public static final String EVENT_CHANGE_OLD = "old";
    /** Change event new name */
    public static final String EVENT_CHANGE_NEW = "new";

    /* SCI environment */
    /** Path from SCI or SCIHOME to the Xcos configuration directory */
    public static final String XCOS_ETC = "/modules/xcos/etc";

    /** This class is a static singleton, thus it must not be instantiated */
    private XcosConstants() {
    }
}
