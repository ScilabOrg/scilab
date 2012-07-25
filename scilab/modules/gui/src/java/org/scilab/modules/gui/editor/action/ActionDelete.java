package org.scilab.modules.gui.editor.action;

import org.scilab.modules.gui.editor.action.*;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

public class ActionDelete extends BaseAction {


    public ActionDelete(String object, String parentUID) {

        super(object, parentUID);
    }

    public void undo() {

        if (exists()) {
            setRelation(objectUID, parentUID);
        } else {
            objectUID = null;
        }
    }

    public void redo() {

        if (exists()) {
            removeRelation(objectUID);
        } else {
            objectUID = null;
        }
    }

    public void dispose() {

        objectUID = null;
    }
}
           
