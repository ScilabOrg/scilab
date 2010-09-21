package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.sphere;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.AbstractGraphicController;

/**
 * Author: Lando Pierre
 * Date: june 25, 2010
 */
public class SphereController extends AbstractGraphicController<ISphereModel> implements ISphereController {

    public SphereController() {
        super(new SphereModelImpl());
    }

    public SphereController(float radius) {
        super(new SphereModelImpl(radius));
    }

    public SphereController(ISphereModel model) {
        super(model);
    }

    
    public void setRadius(float radius) {
        if (getRadius() != radius) {
            getModel().setRadius(radius);
            fireUpdate();
        }
    }

    
    public float getRadius() {
        return getModel().getRadius();
    }
}
