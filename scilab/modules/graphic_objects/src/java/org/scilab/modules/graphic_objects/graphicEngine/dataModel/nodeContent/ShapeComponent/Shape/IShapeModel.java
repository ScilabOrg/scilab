package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.Shape;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.IGraphicModel;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.appearance.IAppearanceController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.IGeometryController;

/**
 * Author: Lando Pierre
 * Date: june 18, 2010
 */
public interface IShapeModel extends IGraphicModel {
    final String type = "Shape";

    /**
     * Set the appearance of this shape.
     * @param appearance the new appearance of this shape.
     */
    void setAppearance(IAppearanceController appearance);

    /**
     * @return the appearance of this shape.
     */
    IAppearanceController getAppearance();

    /**
     * Set the geometry of this shape.
     * @param geometry the new geometry of this shape.
     */
    void setGeometry(IGeometryController geometry);

    /**
     * @return the geometry of this shape.
     */
    IGeometryController getGeometry();
}
