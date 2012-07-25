package org.scilab.modules.gui.editor.action;

import org.scilab.modules.gui.editor.action.*;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

public class ActionTextEdit extends BaseAction {

    String[] FText, TText;

    public ActionTextEdit(String object, String[] FText, String[] TText) {

        super(object,(String)GraphicController.getController().getProperty(object, GraphicObjectProperties.__GO_PARENT__));
        this.FText = FText;
        this.TText = TText;
    }

    public void undo() {

        if (exists()) {
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_STRINGS__, FText);
        } else {
            objectUID = null;
        }
    }

    public void redo() {

        if (exists()) {
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_STRINGS__, TText);
        } else {
            objectUID = null;
        }
    }

    public void dispose() {

        objectUID = null;
    }
}


