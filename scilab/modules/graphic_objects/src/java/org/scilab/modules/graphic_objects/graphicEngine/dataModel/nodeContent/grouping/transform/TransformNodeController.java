package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.grouping.transform;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.AbstractGraphicController;
import org.scilab.modules.graphic_objects.graphicEngine.maths.IVector3f;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 */
public class TransformNodeController extends AbstractGraphicController<ITransformNodeModel> implements ITransformNodeController {
    
    public TransformNodeController(ITransformNodeModel model) {
        super(model);
    }

    public TransformNodeController() {
        this(new TransformNodeImpl());
    }

    
    public float[] getTransformMatrix() {
        return getModel().getTransformMatrix();
    }

    
    public void setTranslation(IVector3f translation) {
        getModel().setTranslation(translation);
        fireUpdate();
    }

    
    public float[] getInverseMatrix() {
        return getModel().getInverseMatrix();
    }

    public String getType() {
        return ITransformNodeModel.type;
    }
}
