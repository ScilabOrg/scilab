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
package org.scilab.modules.gui.ged.axes;

/**
 * Highest class - Prepares methods dependent on JPanel.
 * @author cardinot
 */
public class Roof extends javax.swing.JPanel {

    /**
    * Build methods of overwriting.
    */
    public Roof() {
        axisRulersComponents();
        labelComponents();
        positionComponents();
        styleComponents();
        initLayout();
        dialogBackgroundColor();
        dialogForegroundColor();
        dialogFontColor();
    }

    /**
    * It has all the components of the section Axis Rulers.
    */
    public void axisRulersComponents() { };

    /**
    * It has all the components of the section Label.
    */
    public void labelComponents() { };

    /**
    * It has all the components of the section Position.
    */
    public void positionComponents() { };

    /**
    * It has all the components of the section Style/Appareance.
    */
    public void styleComponents() { };

    /**
    * Creates layout of all sections.
    */
    public void initLayout() { };

    /**
    * JDialog - Selection of background colors.
    */
    public void dialogBackgroundColor() { };

    /**
    * JDialog - Selection of foreground colors.
    */
    public void dialogForegroundColor() { };

    /**
    * JDialog - Selection of font colors.
    */
    public void dialogFontColor() { };
}
