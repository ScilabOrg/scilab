package org.scilab.modules.gui.editor.action;

import org.scilab.modules.gui.editor.action.*;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

public class ActionPaste extends BaseAction {

    String TParent;
    boolean isDuplicated;

    public ActionPaste(String object, String FParent, String TParent, boolean isDuplicated) {

        super(object, FParent);
        this.TParent = TParent;
        this.isDuplicated = isDuplicated;
    }

    public void undo() {

        if (exists()) {
            if (isDuplicated) {
                removeRelation(objectUID);
            } else {
                setRelation(objectUID, parentUID);
            }
        } else {
            objectUID = null;
        }
    }

    public void redo() {

        if (exists()) {
            setRelation(objectUID, TParent);
        } else {
            objectUID = null;
        }
    }

    public void dispose() {

        objectUID = null;
    }
}