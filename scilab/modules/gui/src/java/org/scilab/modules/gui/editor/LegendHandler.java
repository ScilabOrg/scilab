/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.editor;

import java.util.Arrays;
import java.util.List;
import java.util.ArrayList;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.*;

import org.scilab.modules.gui.editor.ObjectSearcher;
import org.scilab.modules.gui.editor.PolylineHandler;


/**
* Implements all legend manipulation functions for the editor.
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/
public class LegendHandler {

    private static String searchLegend(String uid) {
        Integer childCount = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_CHILDREN_COUNT__);
        String[] child = (String[])GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_CHILDREN__);
        for (Integer i = 0; i < childCount; i++) {
            String type = (String)GraphicController.getController().getProperty(child[i], GraphicObjectProperties.__GO_TYPE__);
            if (type == GraphicObjectProperties.__GO_LEGEND__) {
                return child[i];
            }
        }
        return null;
    }

    /**
    * Set or create a legend for one polyline.
    *
    * @param axes The axes that will recieve the legend.
    * @param polyline The polyline to set/create legend.
    * @param text The text for the legend.
    */

    public static void setLegend(String axes, String polyline, String text) {

        if (text == null || text == "") {
            return;
        }
        String legend = searchLegend(axes);
        Integer[] dimension = { 0, 0 };
        ArrayList<String> texts = new ArrayList<String>();
        ArrayList<String> links = new ArrayList<String>();
        texts.add(text);
        if (legend != null) {
            String[] textOld = (String[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_TEXT_STRINGS__);
            String[] linksOld = (String[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_LINKS__);
            Integer length = linksOld.length;
            for (Integer i = 0; i < length; i++) {
                if (linksOld[i] != polyline) {
                    texts.add(textOld[i]);
                    links.add(linksOld[i]);
                } else {
                    removeLegend(axes, polyline);
                    setLegend(axes, polyline, text);
                    return;
                }
            }
			PolylineHandler.getInstance().delete(legend);
        }
		legend = GraphicController.getController().askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_LEGEND__));
        GraphicController.getController().setGraphicObjectRelationship(axes, legend);

        links.add(polyline);
        String[] textToSet = new String[texts.size()];
        texts.toArray(textToSet);
        String[] linksToSet = new String[links.size()];
        links.toArray(linksToSet);
        dimension[0] = linksToSet.length;
        dimension[1] = 1;
        if (dimension[0] != 0) {
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, dimension);
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_LINE_MODE__, true);
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_TEXT_STRINGS__, textToSet);
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_LINKS__, linksToSet);
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_LEGEND_LOCATION__, 0);
        }
    }

    /**
    * Remove a legend from axes.
    *
    * @param axes The axes to remove the legend.
    * @param polyline The polyline that is linked to the legend.
    */

    public static void removeLegend(String axes, String polyline) {

        String legend = searchLegend(axes);
        Integer[] dimension = { 0, 0 };
        ArrayList<String> texts = new ArrayList<String>();
        ArrayList<String> links = new ArrayList<String>();
        if (legend == null) {
            return;
        } else {
            String[] textOld = (String[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_TEXT_STRINGS__);
            String[] linksOld = (String[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_LINKS__);
            Integer remove = -1;
            for (Integer i = 0; i < linksOld.length; i++) {
                if (linksOld[i] != polyline) {
                    links.add(linksOld[i]);
                } else {
                    remove = i;
                }
            }
            for (Integer i = 0; i < textOld.length; i++) {
                 if ( i != (textOld.length - remove - 1)) {
                     texts.add(textOld[i]);
                 }
            }
        }
        String[] textToSet = new String[texts.size()];
        texts.toArray(textToSet);
        String[] linksToSet = new String[links.size()];
        links.toArray(linksToSet);
        dimension[0] = linksToSet.length;
        dimension[1] = 1;
        if (dimension[0] != 0) {
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, dimension);
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_LINE_MODE__, true);
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_TEXT_STRINGS__, textToSet);
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_LINKS__, linksToSet);
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_LEGEND_LOCATION__, 1);
        } else {
            PolylineHandler.getInstance().delete(legend);
        }
    }

    /**
    * Get the text of the legend.
    *
    * @param axes The axes where is the legend.
    * @param polyline The polyline that is linked to the legend.
    * @return The text of the legend.
    */

    public static String getLegendText(String axes, String polyline) {

        String legend = searchLegend(axes);
        if (legend == null) {
            return null;
        } else {
            String[] textOld = (String[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_TEXT_STRINGS__);
            String[] linksOld = (String[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_LINKS__);
            for (Integer i = 0; i < linksOld.length; i++) {
                if (linksOld[i] == polyline) {
                    return textOld[linksOld.length-i-1];
                }
            }
        }
        return null;
    }
}
