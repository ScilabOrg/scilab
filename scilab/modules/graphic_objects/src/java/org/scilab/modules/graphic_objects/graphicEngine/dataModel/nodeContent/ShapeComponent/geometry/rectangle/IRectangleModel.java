package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.rectangle;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.IGeometryModel;

/**
 * Author: Lando Pierre
 * Date: june 25, 2010
 *
 * Rectangle data model.
 */
public interface IRectangleModel extends IGeometryModel {
    static final String type = "Rectangle";

    /**
     * Enum of possible origin location.
     */
    static enum Origin {CENTER, BOTTOM, TOP, LEFT, RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, TOP_LEFT, TOP_RIGHT}

    static final Origin defaultOrigin = Origin.CENTER;

    static final float defaultWidth = 2f;
    static final float defaultHeight = 2f;

    /**
     * Set the origin of this rectangle.
     * @param origin the new origin of this rectangle.
     */
    void setOrigin(Origin origin);

    /**
     * @return the origin of this rectangle.
     */
    Origin getOrigin();

    /**
     * Set the height of this rectangle.
     * @param height the new height of this rectangle.
     */
    void setHeight(float height);

    /**
     * @return the height of this rectangle.
     */
    float getHeight();

    /**
     * Set the width of this rectangle.
     * @param width the new width of this rectangle.
     */
    void setWidth(float width);

    /**
     * @return the width of this rectangle.
     */
    float getWidth();
}
