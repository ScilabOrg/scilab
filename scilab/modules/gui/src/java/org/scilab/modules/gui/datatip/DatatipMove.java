/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.datatip;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.graphic_objects.PolylineData;
import java.util.ArrayList;

import org.scilab.modules.gui.datatip.DatatipCreate;

/**
 * Move a datatip over a polyline
 * @author Gustavo Barbosa Libotte
 */
public class DatatipMove {

    public static Integer[] datatipBounds = { 0 , 0 };
    public static double[] coordsToMove = { 0.0 , 0.0 };
    public static Double[] datatipPositionRight = { 0.0 , 0.0 , 0.0 };
    public static Double[] datatipPositionLeft = { 0.0 , 0.0 , 0.0 };
    public static double rightX = 0.0;
    public static double leftX = 0.0;
    public static double rightY = 0.0;
    public static double leftY = 0.0;

    /**
    * Move a select datatip along the curve to the right direction
    *
    * @param figureUid Figure unique identifier.
    * @param polylinesUid Arraylist containing polylines unique identifier which datatips belongs.
    * @param indexToMove Integer containing the index of selected datatip unique identifier in Arraylist
    * @param datatipsCoord Arraylist containing all created datatip's user coordinates.
    * @param datatipsUid Arraylist containing all created datatip's unique identifier.
    * @param markersUid Arraylist containing all created merker's unique identifier.
    * @return Arraylist containing all created datatip's user coordinates updated with moved coordintes.
    */
    public static ArrayList<Double> moveRight (String figureUid, ArrayList<String> polylinesUid, Integer indexToMove, ArrayList<Double> datatipsCoord, ArrayList<String> datatipsUid, ArrayList<String> markersUid) {

        String polylineUid = polylinesUid.get (indexToMove / 2);
        double[] polylineDataX = (double[]) PolylineData.getDataX (polylineUid);
        double[] polylineDataY = (double[]) PolylineData.getDataY (polylineUid);
        coordsToMove[0] = datatipsCoord.get (indexToMove);
        coordsToMove[1] = datatipsCoord.get (indexToMove + 1);
        for (int i = 1 ; i < (polylineDataX.length - 1) ; i = i + 2) {
            if (coordsToMove[0] >= polylineDataX[i - 1] & coordsToMove[0] <= polylineDataX[i]) {
                rightX = polylineDataX[i + 1];
                rightY = polylineDataY[i + 1];
            }
        }
        String datatipUid = datatipsUid.get (indexToMove / 2);
        String markerUid = markersUid.get (indexToMove / 2);
        double[] posRight = { rightX , rightY , 0.0 };
        String[] datatipLabel = DatatipCreate.setDatatipLabel (posRight);
        datatipBounds = DatatipCreate.getDatatipBounds (datatipLabel);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, datatipBounds);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_TEXT_STRINGS__, datatipLabel);
        datatipPositionRight = DatatipCreate.setDatatipPosition (posRight);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_POSITION__, datatipPositionRight);
        GraphicController.getController().setProperty(markerUid, GraphicObjectProperties.__GO_POSITION__, datatipPositionRight);
        datatipsCoord.set (indexToMove, rightX);
        datatipsCoord.set (indexToMove + 1, rightY);
        return datatipsCoord;
    }

    /**
    * Move a select datatip along the curve to the left direction
    *
    * @param figureUid Figure unique identifier.
    * @param polylinesUid Arraylist containing polylines unique identifier which datatips belongs.
    * @param indexToMove Integer containing the index of selected datatip unique identifier in Arraylist
    * @param datatipsCoord Arraylist containing all created datatip's user coordinates.
    * @param datatipsUid Arraylist containing all created datatip's unique identifier.
    * @param markersUid Arraylist containing all created merker's unique identifier.
    * @return Arraylist containing all created datatip's user coordinates updated with moved coordintes.
    */
    public static ArrayList<Double> moveLeft (String figureUid, ArrayList<String> polylinesUid, Integer indexToMove, ArrayList<Double> datatipsCoord, ArrayList<String> datatipsUid, ArrayList<String> markersUid) {

        String polylineUid = polylinesUid.get (indexToMove / 2);
        double[] polylineDataX = (double[]) PolylineData.getDataX (polylineUid);
        double[] polylineDataY = (double[]) PolylineData.getDataY (polylineUid);
        coordsToMove[0] = datatipsCoord.get (indexToMove);
        coordsToMove[1] = datatipsCoord.get (indexToMove + 1);
        for (int i = 1 ; i < (polylineDataX.length - 1) ; i = i + 2) {
            if (coordsToMove[0] >= polylineDataX[i] & coordsToMove[0] <= polylineDataX[i + 1]) {
                leftX = polylineDataX[i - 1];
                leftY = polylineDataY[i - 1];
            }
        }
        String datatipUid = datatipsUid.get (indexToMove / 2);
        String markerUid = markersUid.get (indexToMove / 2);
        double[] posLeft = { leftX , leftY , 0.0 };
        String[] datatipLabel = DatatipCreate.setDatatipLabel (posLeft);
        datatipBounds = DatatipCreate.getDatatipBounds (datatipLabel);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, datatipBounds);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_TEXT_STRINGS__, datatipLabel);
        datatipPositionLeft = DatatipCreate.setDatatipPosition (posLeft);
        GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_POSITION__, datatipPositionLeft);
        GraphicController.getController().setProperty(markerUid, GraphicObjectProperties.__GO_POSITION__, datatipPositionLeft);
        datatipsCoord.set (indexToMove, leftX);
        datatipsCoord.set (indexToMove + 1, leftY);
        return datatipsCoord;
    }
}
