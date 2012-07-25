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
package org.scilab.modules.gui.editor.action;

import org.scilab.modules.gui.editor.action.*;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
* Implements the object to store Text Edit related actions
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/
public class ActionTextEdit extends BaseAction {

    /* Store the original text and the new text */
    String[] FText, TText;

    public ActionTextEdit(String object, String[] FText, String[] TText) {

        super(object,(String)GraphicController.getController().getProperty(object, GraphicObjectProperties.__GO_PARENT__));
        this.FText = FText;
        this.TText = TText;
    }

    /** 
    * Undo, Set the text to the original
    */
    public void undo() {

        if (exists()) {
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_STRINGS__, FText);
        } else {
            objectUID = null;
        }
    }

    /** 
    * Redo, Set the text to the new
    */
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


