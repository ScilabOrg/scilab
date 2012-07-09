/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos Cardinot
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged.curve;

/**
 * Manages the operation of the button show/hide for the current curve.
 *
 * @author cardinot
 */
public class HideCurve {
    public HideCurve(boolean hide) {
        if (hide) {
            BaseProperties.pBaseProperties.setVisible(false);
            BaseProperties.bBaseProperties.setSelected(true);

            Style.pStyle.setVisible(false);
            Style.bStyle.setSelected(true);
        } else {
            BaseProperties.pBaseProperties.setVisible(true);
            BaseProperties.bBaseProperties.setSelected(false);

            Style.pStyle.setVisible(true);
            Style.bStyle.setSelected(false);
        }
    }
}