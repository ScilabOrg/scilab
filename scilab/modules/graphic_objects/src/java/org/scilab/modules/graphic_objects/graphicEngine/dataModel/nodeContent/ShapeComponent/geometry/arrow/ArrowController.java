package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.arrow;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.AbstractGraphicController;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 */
public class ArrowController extends AbstractGraphicController<IArrowModel> implements IArrowController {

    /**
     * Default constructor.
     */
    public ArrowController() {
        super(new ArrowModelImpl());
    }

    /**
     * Constructor
     * @param model the model used by this controller.
     */
    public ArrowController(IArrowModel model) {
        super(model);
    }

    
    public void setSize(float size) {
        if (getSize() != size) {
            getModel().setSize(size);
            fireUpdate();
        }
    }

    
    public float getSize() {
        return getModel().getSize();
    }

    
    public void setHeadRadius(float radius) {
        if (getHeadRadius() != radius) {
            getModel().setHeadRadius(radius);
            fireUpdate();
        }
    }

    
    public float getHeadRadius() {
        return getModel().getHeadRadius();
    }
}
