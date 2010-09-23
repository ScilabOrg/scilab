package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.grouping.transform.rotation;

import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableModel;
import org.scilab.modules.graphic_objects.graphicEngine.maths.IVector3f;
import org.scilab.modules.graphic_objects.graphicEngine.maths.UnitaryQuaternion;

/**
 * Author: Lando Pierre
 * Date: june 29, 2010
 */
public class RotationNodeModelImpl extends AbstractDrawableModel implements IRotationNodeModel {
    private UnitaryQuaternion orientation = new UnitaryQuaternion();

    
    public void setOrientation(UnitaryQuaternion orientation) {
        this.orientation = orientation;
    }

    
    public UnitaryQuaternion getOrientation() {
        return new UnitaryQuaternion(orientation);
    }

    
    public float[] getTransformMatrix() {
        return orientation.getRotationMatrix();
    }

    
    public void setTranslation(IVector3f translation) {
        // TODO : remove
    }

    
    public float[] getInverseMatrix() {
        return orientation.getUnRotateMatrix();
    }

    
    public String getType() {
        return type;
    }
}
