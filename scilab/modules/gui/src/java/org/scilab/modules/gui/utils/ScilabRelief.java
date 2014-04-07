/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.Color;

import javax.swing.BorderFactory;
import javax.swing.border.Border;
import javax.swing.border.EtchedBorder;

/**
 * Borders for Widgets in Scilab GUIs definition
 * @author Vincent COUVERT
 */
public final class ScilabRelief {

    /**
     * Keywork used for FLAT relief property
     */
    public static final String FLAT = "flat";
    /**
     * Border used for FLAT relief property
     */
    public static final Border FLAT_BORDER = BorderFactory.createEmptyBorder();

    /**
     * Keyword used for GROOVE relief property
     */
    public static final String GROOVE = "groove";
    /**
     * Border used for GROOVE relief property
     */
    public static final Border GROOVE_BORDER = BorderFactory.createEtchedBorder(EtchedBorder.LOWERED);

    /**
     * Keyword used for RAISED relief property
     */
    public static final String RAISED = "raised";
    /**
     * Border used for RAISED relief property
     */
    public static final Border RAISED_BORDER = BorderFactory.createRaisedBevelBorder();

    /**
     * Keyword used for RIDGE relief property
     */
    public static final String RIDGE = "ridge";
    /**
     * Border used for RIDGE relief property
     */
    public static final Border RIDGE_BORDER = BorderFactory.createEtchedBorder(EtchedBorder.RAISED);

    /**
     * Keyword used for SOLID relief property
     */
    public static final String SOLID = "solid";
    /**
     * Border used for SOLID relief property
     */
    public static final Border SOLID_BORDER = BorderFactory.createLineBorder(Color.BLACK);

    /**
     * Border used for SUNKEN relief property
     */
    public static final String SUNKEN = "sunken";
    /**
     * Keyword used for SUNKEN relief property
     */
    public static final Border SUNKEN_BORDER = BorderFactory.createLoweredBevelBorder();
    /**
     * LaF border property
     */
    public static final String DEFAULT = "default";

    /**
     * Constructor
     */
    private ScilabRelief() {
        throw new UnsupportedOperationException();
    }

    /**
     * Get the border corresponding to the Scilab relief type
     * @param reliefType Scilab type for relief
     * @return the corresponding border
     */
    public static Border getBorderFromRelief(String reliefType, Border defaultBorder) {
        Border returnBorder = SUNKEN_BORDER;
        if (reliefType.equals(FLAT)) {
            returnBorder = FLAT_BORDER;
        } else if (reliefType.equals(GROOVE)) {
            returnBorder = GROOVE_BORDER;
        } else if (reliefType.equals(RAISED)) {
            returnBorder = RAISED_BORDER;
        } else if (reliefType.equals(RIDGE)) {
            returnBorder = RIDGE_BORDER;
        } else if (reliefType.equals(SOLID)) {
            returnBorder = SOLID_BORDER;
        } else if (reliefType.equals(DEFAULT)) {
            returnBorder = defaultBorder;
        }
        return returnBorder;
    }
}
