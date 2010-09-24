package org.scilab.modules.graphic_objects.JoGLView;

import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;

/**
 * Author: lando
 * Date: 21 sept. 2010
 */
public class FiguresManager implements GraphicView {

    public void updateObject(String id, String property) {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    public void createObject(String id) {
        Object model = GraphicController.getController().getObjectFromId(id);
        if (model instanceof Figure) {
            new FigureFrame(id);
        }
    }

    public void deleteObject(String id) {
        //To change body of implemented methods use File | Settings | File Templates.
    }
}
