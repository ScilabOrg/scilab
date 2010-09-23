package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.rectangle;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.AbstractGraphicController;

/**
 * Author: Lando Pierre
 * Date: june 25, 2010
 */
public class RectangleController extends AbstractGraphicController<IRectangleModel> implements IRectangleController {

    public RectangleController() {
        super(new RectangleModelImpl());
    }
    
    public RectangleController(IRectangleModel model) {
        super(model);
    }

    public RectangleController(float width, float height) {
        super(new RectangleModelImpl(width, height));
    }

    public RectangleController(float width, float height, Origin origin) {
        super(new RectangleModelImpl(width, height, origin));
    }

    
    public void setOrigin(Origin origin) {
        if (getOrigin() != origin) {
            getModel().setOrigin(origin);
            fireUpdate();
        }
    }

    
    public Origin getOrigin() {
        return getModel().getOrigin();
    }

    
    public void setHeight(float height) {
        if (getHeight() != height) {
            getModel().setHeight(height);
            fireUpdate();
        }
    }

    
    public float getHeight() {
        return getModel().getHeight();
    }

    
    public void setWidth(float width) {
        if (getWidth() != width) {
            getModel().setWidth(width);
            fireUpdate();
        }
    }

    
    public float getWidth() {
        return getModel().getWidth();
    }
}
