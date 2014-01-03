/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 2013 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.figure;

import javax.swing.JPanel;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.graphic_objects.properties.Figure;

/**
* Construction and startup of all components of the section: Control.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Control extends SimpleSection {
    private static Control instance;
    private JPanel sectionPanel;
    private Figure figure = new Figure();

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of object.
    */
    public Control(Integer objectID) {
        super(MessagesGED.control, "figure");
        instance = this;
        sectionPanel = getSectionPanel();
        initComponents(objectID);
    }

    /**
     * Get instance
     * @return instance
     */
    public static Control getInstance() {
        return instance;
    }

    /**
    * Add all the properties in this section.
    * @param objectID uid
    */
    @Override
    public final void initComponents(Integer objectID) {
        int row = 0;
        final int leftmargin = 1; //to inner components
        int column = 1; //first column

        //Components of the property: Axes Size
        figure.axesSize(sectionPanel, row++, column, leftmargin, objectID);
        row++;

        //Components of the property: CloseRequestFcn.
        figure.closeRequestFcn(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Event Handler
        figure.eventHandler(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Figure Position
        figure.figurePosition(sectionPanel, row++, column, leftmargin, objectID);
        row++;

        //Components of the property: Figure Size.
        figure.figureSize(sectionPanel, row++, column, leftmargin, objectID);
        row++;

        //Components of the property: ResizeFcn.
        figure.resizeFcn(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Viewport.
        figure.viewport(sectionPanel, row++, column, leftmargin, objectID);
    }
}