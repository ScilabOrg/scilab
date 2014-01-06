/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.surface;

import javax.swing.JPanel;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.properties.ContouredObject;
import org.scilab.modules.gui.ged.graphic_objects.properties.Surface;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;

/**
* Construction and startup of all components of the section: Style/Appeareance.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Style extends SimpleSection {
    private JPanel sectionPanel;
    private static Style instance;
    private Integer parentFigure;
    private Surface surface = new Surface();
    private ContouredObject contouredObject = new ContouredObject();

    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of surface.
    */
    public Style(Integer objectID) {
        super(MessagesGED.style_appearance, "surface");
        instance = this;
        sectionPanel = getSectionPanel();
        initComponents(objectID);
    }

    /**
     * Get instance
     * @return instance
     */
    public static Style getInstance() {
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
        parentFigure = (Integer) GraphicController.getController()
                       .getProperty(objectID, GraphicObjectProperties.__GO_PARENT_FIGURE__);

        //Components of the property: Color Flag.
        surface.colorFlag(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Color Mode.
        surface.colorFlag(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Hidden Color.
        surface.hiddenColor(sectionPanel, row++, column, leftmargin, objectID, parentFigure);

        //Components of the property: Foreground Color.
        contouredObject.foregroundColor(sectionPanel, row++, column, leftmargin,
                                        objectID, parentFigure);

        //Components of the property: Thickness.
        contouredObject.thickness(sectionPanel, row++, column, leftmargin, objectID);
    }
}