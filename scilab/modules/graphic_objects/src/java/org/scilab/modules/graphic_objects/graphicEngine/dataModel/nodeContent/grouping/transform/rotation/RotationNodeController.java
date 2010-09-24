package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.grouping.transform.rotation;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.AbstractGraphicController;
import org.scilab.modules.graphic_objects.graphicEngine.maths.IVector3f;
import org.scilab.modules.graphic_objects.graphicEngine.maths.UnitaryQuaternion;

/**
 * Author: Lando Pierre
 * Date: june 29, 2010
 */
public class RotationNodeController extends AbstractGraphicController<IRotationNodeModel> implements IRotationNodeController {
    
    public RotationNodeController(IRotationNodeModel model) {
        super(model);
    }

    public RotationNodeController() {
        this(new RotationNodeModelImpl());
    }

    
    public void setOrientation(UnitaryQuaternion orientation) {
        // TODO : test equality.
        getModel().setOrientation(orientation);
        fireUpdate();
    }

    
    public UnitaryQuaternion getOrientation() {
        return getModel().getOrientation();
    }

    
    public float[] getTransformMatrix() {
        return getModel().getTransformMatrix();
    }

    
    public void setTranslation(IVector3f translation) {
        // TODO remove
    }

    
    public float[] getInverseMatrix() {
        return getModel().getInverseMatrix();
    }
}
