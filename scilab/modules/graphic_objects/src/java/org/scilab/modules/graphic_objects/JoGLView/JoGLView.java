/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.JoGLView;

import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.jvm.LoadClassPath;

public class JoGLView implements GraphicView {
    private static JoGLView singleton = null;
    
    private JoGLView() {
    }

    public static JoGLView getJoGLView() {
        if (singleton == null) {
            System.out.println("-- LoadClassPath.loadOnUse('graphics')");
            LoadClassPath.loadOnUse("graphics");
            singleton = new JoGLView();
        }
        return singleton;
    }

    public void updateObject(String id, String property) {
    }

    public void createObject(String id) {
        GraphicObject controller = GraphicController.getController().getObjectFromId(id);
        if (controller != null && controller instanceof Figure) {
            FigureBridge.createFigure(id);
        }
    }

    public void deleteObject(String id) {
        System.out.println("PEDRO : delete");
    }
}