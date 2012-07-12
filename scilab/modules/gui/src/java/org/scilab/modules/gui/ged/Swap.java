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
package org.scilab.modules.gui.ged;

import javax.swing.JPanel;
import org.scilab.modules.gui.editor.AxesHandler;
import org.scilab.modules.gui.ged.axes.Axes;
import org.scilab.modules.gui.ged.curve.Curve;
import org.scilab.modules.gui.ged.figure.Figure;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
* Manager object exchange.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Swap {
    public Swap(String select, String objectID, Integer clickX, Integer clickY) {
        if (select.equals("curve")) {
            curve(objectID);
        } else {
            Integer[] position = new Integer[2];
            position[0] = clickX;
            position[1] = clickY;
            String axesID = AxesHandler.clickedAxes(objectID, position);
            Double[] axesMargins = (Double[]) GraphicController.getController()
                                   .getProperty(axesID, GraphicObjectProperties.__GO_MARGINS__);
            Double left, right, top, bottom;

            left = axesMargins[0];
            right = axesMargins[1];
            top = axesMargins[2];
            bottom = axesMargins[3];

            Integer[] axesSize = (Integer[])GraphicController.getController()
                                 .getProperty(objectID, GraphicObjectProperties.__GO_AXES_SIZE__);
            Integer x, y;

            x = axesSize[0];
            y = axesSize[1];

            Double leftBorder, rightBorder, bottomBorder, topBorder;

            leftBorder = left*x;
            rightBorder = x - right*x;
            bottomBorder = bottom*y;
            topBorder = y - top*y;

            if (position[0] > leftBorder && position[0] < rightBorder && position[1] > bottomBorder && position[1] < topBorder) {
                axes(axesID);
            } else {
                figure(objectID);
            }
        }
    }

    private void axes(String axesID) {
        Inspector.pReceive.removeAll();
        JPanel received = new Axes(axesID);
        received.setBounds(2, 0, 348, 780);
        received.setVisible(true);
        Inspector.pReceive.add(received);
        Inspector.pReceive.setPreferredSize(new java.awt.Dimension(370, 780));
        Inspector.selectaux = "axes";
    }

    private void curve(String objectID) {
        Inspector.pReceive.removeAll();
        JPanel received = new Curve(objectID);
        received.setBounds(2, 0, 348, 300);
        received.setVisible(true);
        Inspector.pReceive.add(received);
        Inspector.pReceive.setPreferredSize(new java.awt.Dimension(370, 300));
        Inspector.selectaux = "curve";
    }

    private void figure(String objectID) {
        Inspector.pReceive.removeAll();
        JPanel received = new Figure(objectID);
        received.setBounds(2, 0, 348, 600);
        received.setVisible(true);
        Inspector.pReceive.add(received);
        Inspector.pReceive.setPreferredSize(new java.awt.Dimension(370, 600));
        Inspector.selectaux = "figure";
    }
}
