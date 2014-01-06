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
package org.scilab.modules.gui.ged.graphic_objects.axes;

import javax.swing.JPanel;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.properties.ContouredObject;
import org.scilab.modules.gui.ged.graphic_objects.properties.Axes;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;

/**
* Construction and startup of all components of the section: Style/Appeareance.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Style extends SimpleSection {
    private JPanel sectionPanel;
    private static Style instance;
    private Axes axes = new Axes();
    private ContouredObject contouredObject = new ContouredObject();

    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of object.
    */
    public Style(Integer objectID) {
        super(MessagesGED.style_appearance, "axes");
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
    * Initialize the Components.
    */
    @Override
    public final void initComponents(Integer objectID) {
        int row = 0;
        final int leftmargin = 16; //to inner components
        int column = 0; //first column

        Integer parentFigure = (Integer) GraphicController.getController()
                               .getProperty(objectID, GraphicObjectProperties.__GO_PARENT_FIGURE__);

        //Components of the property: Box Type
        axes.box(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Filled
        axes.filled(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Background Color.
        contouredObject.backgroundColor(sectionPanel, row++, column, leftmargin, objectID, parentFigure);

        //Components of the property: Foreground Color.
        contouredObject.lineColor(sectionPanel, row++, column, leftmargin,
                                  objectID, parentFigure, "axes.Style", this);

        //Components of the property: Grid Position
        axes.gridPosition(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: X Grid Color
        axes.gridColorX(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Y Grid Color
        axes.gridColorY(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Line Style.
        contouredObject.lineStyle(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Thickness.
        contouredObject.thickness(sectionPanel, row++, column, leftmargin, objectID);
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    * @param UID objectID.
    */
    public void setForegroundColor(int scilabColor, Integer UID) {
        GraphicController.getController().setProperty(
            UID, GraphicObjectProperties.__GO_LINE_COLOR__, scilabColor);
    }
}