package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.grouping.transform.rotation;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.grouping.transform.ITransformNodeModel;
import org.scilab.modules.graphic_objects.graphicEngine.maths.UnitaryQuaternion;

/**
 * Author: Lando Pierre
 * Date: june 29, 2010
 */
public interface IRotationNodeModel extends ITransformNodeModel {

    void setOrientation(UnitaryQuaternion orientation);

    UnitaryQuaternion getOrientation();
}
