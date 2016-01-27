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
 * === LICENSE_END ===
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.datatip;

import javax.swing.JPanel;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.graphic_objects.properties.Datatip;
import org.scilab.modules.gui.ged.graphic_objects.properties.GraphicObject;

/**
* Construction and startup of all components of the section: Data Properties.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class DataProperties extends SimpleSection {
    private JPanel sectionPanel;
    private static DataProperties instance;
    private Datatip datatip = new Datatip();
    private GraphicObject graphicObject = new GraphicObject();

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of datatip.
    */
    public DataProperties(Integer objectID) {
        super(MessagesGED.data_properties, "datatip");
        instance = this;
        sectionPanel = getSectionPanel();
        initComponents(objectID);
    }

    /**
     * Get instance
     * @return instance
     */
    public static DataProperties getInstance() {
        return instance;
    }

    /**
    * Add all the properties in this section.
    * @param objectID uid
    */
    @Override
    public final void initComponents(Integer objectID) {
        int row = 0;
        final int leftmargin = 0; //to inner components
        int column = 1; //first column

        datatip.tip3Component(sectionPanel, row++, column, leftmargin, objectID);

        datatip.tipData(sectionPanel, row++, column, leftmargin, objectID);

        graphicObject.tag(sectionPanel, row++, column, leftmargin, objectID);

        graphicObject.userData(sectionPanel, row++, column, leftmargin);
    }
}