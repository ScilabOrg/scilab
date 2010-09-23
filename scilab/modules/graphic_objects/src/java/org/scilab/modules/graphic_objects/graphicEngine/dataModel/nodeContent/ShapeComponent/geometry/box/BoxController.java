package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.box;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.AbstractGraphicController;
import org.scilab.modules.graphic_objects.graphicEngine.maths.IVector3f;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 */
public class BoxController extends AbstractGraphicController<IBoxModel> implements IBoxController {
    
    public BoxController(IBoxModel model) {
        super(model);
    }

    public BoxController() {
        super(new BoxModelImpl());
    }

    
    public void setSize(IVector3f size) {
        getModel().setSize(size);
        fireUpdate();
    }

    
    public IVector3f getSize() {
        return getModel().getSize(); 
    }

    
    public void setSolid(boolean solidness) {
        getModel().setSolid(solidness);
        fireUpdate();
    }

    
    public boolean getSolid() {
        return getModel().getSolid();
    }
}
