package org.scilab.modules.gui.editor.action;

import org.scilab.modules.gui.editor.action.*;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.gui.editor.LegendHandler;


public class ActionLegend extends BaseAction {

    String[] oldLinks, newLinks;
    String[] oldText, newText;
    Double[] position;

    public ActionLegend(String parent, String[] links, String[] text, Double[] pos) {

        super(null,parent);
        if ( links != null ) {
		    oldLinks = new String[links.length];
            oldText = new String[links.length];
            for (Integer i = 0; i < links.length; i++ ) {
                oldLinks[i] = links[i];
                oldText[i] = text[i];
            }
        }
        objectUID = LegendHandler.searchLegend(parent);
		if (objectUID != null) {
		    newLinks = (String[])GraphicController.getController().getProperty(objectUID, GraphicObjectProperties.__GO_LINKS__);
            newText = (String[])GraphicController.getController().getProperty(objectUID, GraphicObjectProperties.__GO_TEXT_STRINGS__);
        }
        if (pos != null) {
            position = new Double[2];
            this.position[0] = pos[0];
            this.position[1] = pos[1];
        } else {
            position = null;
        }
    }

    public void undo() {

        objectUID = LegendHandler.searchLegend(parentUID);
        if (position == null) {
            position = (Double[])GraphicController.getController().getProperty(objectUID, GraphicObjectProperties.__GO_POSITION__);
        }
        if (objectUID == null) {
            objectUID = GraphicController.getController().askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_LEGEND__));
			Integer[] dimension = {oldLinks.length, 1};
			GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, dimension);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LINE_MODE__, true);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_STRINGS__, oldText);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LINKS__, oldLinks);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LEGEND_LOCATION__, 10);
			GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_POSITION__, position);
            setRelation(objectUID,parentUID);
        } else {
            if(oldLinks == null) {
                GraphicController.getController().removeRelationShipAndDelete(objectUID);
            } else {
                Integer[] dimension = {oldLinks.length, 1};
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, dimension);
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_STRINGS__, oldText);
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LINKS__, oldLinks);
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LEGEND_LOCATION__, 10);
			    GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_POSITION__, position);
            }
        }
    }

    public void redo() {

        objectUID = LegendHandler.searchLegend(parentUID);
        if (position == null) {
            position = (Double[])GraphicController.getController().getProperty(objectUID, GraphicObjectProperties.__GO_POSITION__);
        }
        if (objectUID == null) {
            objectUID = GraphicController.getController().askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_LEGEND__));
			Integer[] dimension = {newLinks.length, 1};
			GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, dimension);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LINE_MODE__, true);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_STRINGS__, newText);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LINKS__, newLinks);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LEGEND_LOCATION__, 10);
			GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_POSITION__, position);
            setRelation(objectUID,parentUID);
        } else {
            if(newLinks == null) {
                GraphicController.getController().removeRelationShipAndDelete(objectUID);
            } else {
                Integer[] dimension = {newLinks.length, 1};
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, dimension);
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_STRINGS__, newText);
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LINKS__, newLinks);
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LEGEND_LOCATION__, 10);
			    GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_POSITION__, position);
            }
        }
    }

    public void dispose() {

        objectUID = null;
        parentUID = null;
    }

    @Override
    public boolean exists() {

        if (GraphicController.getController().getObjectFromId(parentUID) != null) {
            return true;
        } else {
            return false;
        }
    }
}

        
