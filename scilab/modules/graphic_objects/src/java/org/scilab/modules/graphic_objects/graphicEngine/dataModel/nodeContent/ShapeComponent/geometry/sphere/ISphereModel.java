package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.sphere;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.IGeometryModel;

/**
 * Author: Lando Pierre
 * Date: june 25, 2010
 *
 * Sphere data model.
 */
public interface ISphereModel extends IGeometryModel {
    static final String type = "Sphere";
    static final float defaultRadius = 1f;

    /**
     * @param radius The new radius of the arrow.
     */
    public void setRadius(float radius);

    /**
     * @return the radius of the arrow.
     */
    public float getRadius();
}
