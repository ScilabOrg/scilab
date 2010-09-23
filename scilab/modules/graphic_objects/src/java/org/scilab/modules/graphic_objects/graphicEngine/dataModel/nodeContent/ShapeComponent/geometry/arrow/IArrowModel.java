package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.arrow;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.IGeometryModel;


/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 *
 * Arrow data model.
 */
public interface IArrowModel extends IGeometryModel {
    static final String type = "Arrow";
    static final float defaultBodySize = 1f;
    static final float defaultHeadRadius = .1f;

    /**
     * @param size The new getSize of this arrow.
     */
    public void setSize(float size);

    /**
     * @return the getSize of this arrow.
     */
    public float getSize();


    /**
     * @param radius The new radius of the arrow.
     */
    public void setHeadRadius(float radius);

    /**
     * @return the radius of the arrow.
     */
    public float getHeadRadius();
}
