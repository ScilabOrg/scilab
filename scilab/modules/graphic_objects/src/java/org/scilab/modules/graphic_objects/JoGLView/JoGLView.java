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

import org.scilab.modules.jvm.LoadClassPath;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.graphic_objects.util.FACTORY.AbstractFactoryManager;

public class JoGLView extends AbstractFactoryManager<SciBridge> implements GraphicView {
    private static JoGLView singleton = null;
    private JoGLView() {
        registerFactory("Figure", FigureBridge.getFactory());
        registerFactory("Polyline", PolylineBridge.getFactory());
    }

    public static JoGLView getJoGLView() {
        if (singleton == null) {
            System.out.println("pipo");
            LoadClassPath.loadOnUse("graphics");
            System.out.println("poil");

            singleton = new JoGLView();
        }
        return singleton;
    }

    public void updateObject(String id, String property) {}

    public void createObject(String id) {
        Object controller = GraphicController.getController().getObjectFromId(id);
        if (controller instanceof GraphicObject) {
            createStuff(((GraphicObject) controller).getType(), id);
        }
    }

    public void deleteObject(String id) {
        System.out.println("PEDRO : delete");
        //To change body of implemented methods use File | Settings | File Templates.
    }
}