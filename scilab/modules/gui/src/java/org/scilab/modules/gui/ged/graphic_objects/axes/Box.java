/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.axes;

import javax.swing.JPanel;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.graphic_objects.properties.Axes;

/**
* Construction and startup of all components of the section: Box.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Box extends SimpleSection {
    private JPanel sectionPanel;
    private static Box instance;
    private Axes axes = new Axes();

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of datatip.
    */
    public Box(Integer objectID) {
        super(MessagesGED.box, "axes");
        instance = this;
        sectionPanel = getSectionPanel();
        initComponents(objectID);
    }

    /**
     * Get instance
     * @return instance
     */
    public static Box getInstance() {
        return instance;
    }

    /**
    * Add all the properties in this section.
    * @param objectID uid
    */
    @Override
    public final void initComponents(Integer objectID) {
        int row = 0;
        final int leftmargin = 16; //to inner components
        int column = 0; //first column

        //Components of the property: Auto Scale
        axes.autoScale(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Data Bounds.
        //not implemented yet.

        //Components of the property: Hidden Axis Color.
        axes.hiddenAxisColor(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Tight Limits
        axes.tightLimits(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Zoom Box.
        //not implemented yet.
    }
}