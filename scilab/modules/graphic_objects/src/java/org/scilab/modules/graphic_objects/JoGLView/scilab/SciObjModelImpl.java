package org.scilab.modules.graphic_objects.JoGLView.scilab;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableModel;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
 * Author: lando
 * Date: 21 sept. 2010
 */
public class SciObjModelImpl extends AbstractDrawableModel implements ISciObjModel {
    private final String id;
    private final String type;

    public SciObjModelImpl(String id) {
        this.id = id;
        type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);
    }

    public String getType() {
        return type;
    }

    public String getId() {
        return id;
    }
}
