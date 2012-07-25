package org.scilab.modules.gui.editor.action;

import org.scilab.modules.gui.editor.action.*;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.editor.PolylineHandler;
import org.scilab.modules.gui.editor.LegendHandler;

public class ActionMove extends BaseAction {

    Integer[] myClick, myNewClick;
    boolean isLegend;

    public ActionMove(String objectUID, Integer[] click, Integer[] newClick, boolean isLegend) {

        super(objectUID, (String)GraphicController.getController().getProperty(objectUID, GraphicObjectProperties.__GO_PARENT__));
        myClick = new Integer[2];
        myNewClick = new Integer[2];
        myClick[0] = click[0];
        myClick[1] = click[1];
        myNewClick[0] = newClick[0];
        myNewClick[1] = newClick[1];
		this.isLegend = isLegend;
    }

    public void undo() {

        if (exists()) {

            if (!isLegend) {
                PolylineHandler.getInstance().dragPolyline(objectUID, myNewClick, myClick);
            } else {
                LegendHandler.dragLegend(objectUID, myNewClick, myClick);
            }
        } else {
            objectUID = null;
        }
    }

    public void redo() {

        if (exists()) {
            if (!isLegend) {
                PolylineHandler.getInstance().dragPolyline(objectUID, myClick, myNewClick);
            } else {
                LegendHandler.dragLegend(objectUID, myClick, myNewClick);
            }
        } else {
            objectUID = null;
        }
    }

    public void dispose() {

        objectUID = null;
    }
}