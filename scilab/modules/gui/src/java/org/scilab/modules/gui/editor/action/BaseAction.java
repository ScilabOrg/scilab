package org.scilab.modules.gui.editor.action;

import org.scilab.modules.gui.editor.action.*;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

public abstract class BaseAction implements Action{

    String objectUID;
    String parentUID;

    public BaseAction(String objectUID, String parentUID) {

         this.objectUID = objectUID;
         this.parentUID = parentUID;
    }

    public boolean exists() {

        if (GraphicController.getController().getObjectFromId(objectUID) != null && GraphicController.getController().getObjectFromId(parentUID) != null) {
            return true;
        } else {
            return false;
        }
    }

    public void setRelation(String object, String parent) {

        if (exists()) {
            GraphicController.getController().setGraphicObjectRelationship(parent, object);
        } else {
            GraphicController.getController().removeRelationShipAndDelete(object);
            objectUID = null;
            parentUID = null;
        }
    }

    public void removeRelation(String object) {

            GraphicController.getController().setGraphicObjectRelationship("", object);
    }
}