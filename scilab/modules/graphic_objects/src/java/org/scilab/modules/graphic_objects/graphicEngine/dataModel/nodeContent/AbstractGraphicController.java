package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent;

import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableController;

/**
 * Author: Lando Pierre
 * Date: june 15, 2010
 */
public class AbstractGraphicController<modelType extends IGraphicModel> extends AbstractDrawableController<modelType> implements IGraphicController<modelType> {

    public AbstractGraphicController(modelType model) {
        super(model);
    }

    
    public String getType() {
        return getModel().getType();
    }

    
    public String toString() {
        return getType();
    }
}