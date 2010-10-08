package org.scilab.modules.renderer.JoGLView;

import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.jvm.LoadClassPath;

/**
 * @author Pierre Lando
 */
public final class CallGraphicJoGLView {
    private static boolean notLoaded = true;
    private static void load() {
        if(notLoaded) {
            LoadClassPath.loadOnUse("graphics");
            notLoaded = false;
        }
    }

    public static void  createJoGLView(String id) {
        load();
        GraphicObject controller = GraphicController.getController().getObjectFromId(id);
        if (controller != null && controller instanceof Figure) {
            FigureBridge.createFigure(id);
        }
    }
}
