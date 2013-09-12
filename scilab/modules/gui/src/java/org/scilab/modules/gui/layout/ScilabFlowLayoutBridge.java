/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.layout;

import org.scilab.modules.gui.bridge.layout.SwingScilabFlowLayout;

/**
 * Bridge for Scilab FlowLayouts in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabFlowLayoutBridge {

    /**
     * Constructor
     */
    protected ScilabFlowLayoutBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

    /**
     * Creates a Scilab FlowLayout
     * @return the created FlowLayout
     */
    public static FlowLayout createFlowLayout() {
        return new SwingScilabFlowLayout();
    }
}
