/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.gui.editor.action;

import org.scilab.modules.gui.editor.action.BaseAction;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.editor.AxesHandler;

/**
* Implements the object to store PasteStyle related actions
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/
public class ActionPasteStyle extends BaseAction {

    Double[] colorMap;
    Integer background;
    Integer oldAxes;

    /**
    * Constructor
    *
    * @param newAxes The new axes that was pasted
    * @param oldAxes The old axes
    * @param colorMap The old colormap from the figure
    * @param background The old background color from the figure
    */
    public ActionPasteStyle(Integer newAxes, Integer oldAxes, Double[] colorMap, Integer background) {

        super(newAxes, (Integer)GraphicController.getController().getProperty(newAxes, GraphicObjectProperties.__GO_PARENT__));
        this.oldAxes = oldAxes;
        this.colorMap = colorMap;
        this.background = background;
    }

    /**
    * Undo, if the object exists
    */
    public void undo() {

        if (exists()) {
            AxesHandler.pasteAxesStyle(oldAxes, objectUID, false);
            Double[] tempColorMap = (Double[])GraphicController.getController().getProperty(parentUID, GraphicObjectProperties.__GO_COLORMAP__);
            Integer tempBackground = (Integer)GraphicController.getController().getProperty(parentUID, GraphicObjectProperties.__GO_BACKGROUND__);
            GraphicController.getController().setProperty(parentUID, GraphicObjectProperties.__GO_COLORMAP__, colorMap);
            GraphicController.getController().setProperty(parentUID, GraphicObjectProperties.__GO_BACKGROUND__, background);
            colorMap = tempColorMap;
            background = tempBackground;
        } else {
            objectUID = null;
        }
    }

    /**
    * Redo, if the object exists
    */
    public void redo() {

        if (exists()) {
            AxesHandler.pasteAxesStyle(objectUID, oldAxes, false);
            Double[] tempColorMap = (Double[])GraphicController.getController().getProperty(parentUID, GraphicObjectProperties.__GO_COLORMAP__);
            Integer tempBackground = (Integer)GraphicController.getController().getProperty(parentUID, GraphicObjectProperties.__GO_BACKGROUND__);
            GraphicController.getController().setProperty(parentUID, GraphicObjectProperties.__GO_COLORMAP__, colorMap);
            GraphicController.getController().setProperty(parentUID, GraphicObjectProperties.__GO_BACKGROUND__, background);
            colorMap = tempColorMap;
            background = tempBackground;
        } else {
            objectUID = null;
        }
    }

    public void dispose() {

        objectUID = null;
    }

    @Override
    public boolean exists() {

        if (GraphicController.getController().getObjectFromId(parentUID) != null &&
                GraphicController.getController().getObjectFromId(objectUID) != null &&
                GraphicController.getController().getObjectFromId(oldAxes) != null ) {

            return true;
        } else {
            return false;
        }
    }
}