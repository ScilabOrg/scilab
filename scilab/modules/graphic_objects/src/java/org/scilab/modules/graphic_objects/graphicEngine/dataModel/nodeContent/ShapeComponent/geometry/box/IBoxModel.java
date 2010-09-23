package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.box;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.IGraphicModel;
import org.scilab.modules.graphic_objects.graphicEngine.maths.IVector3f;
import org.scilab.modules.graphic_objects.graphicEngine.maths.Vector3f;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 *
 * Box data model.
 */
public interface IBoxModel extends IGraphicModel {
    static final String type = "Box";
    static final IVector3f defaultSize = new Vector3f(2, 2, 2);
    static final Boolean defaultSolidness = true;

    /**
     * @param size is the new getSize of this box.
     */
    void setSize(IVector3f size);

    /**
     * @return the getSize of this box.
     */
    IVector3f getSize();

    /**
     * @param solidness is the new solidness of this box.
     */
    void setSolid(boolean solidness);

    /**
     * @return solidness of this box.
     */
    boolean getSolid();
}
