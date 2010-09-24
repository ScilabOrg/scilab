package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.cylinder;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.IGeometryModel;

/**
 * Author: Lando Pierre
 * Date: june 21, 2010
 *
 * Cylinder data model.
 */
public interface ICylinderModel extends IGeometryModel {
    static final String type = "Cylinder";

    /**
     * Enum of possible origin location.
     */
    static enum Origin {CENTER, BOTTOM, TOP}

    static final Origin defaultOrigin = Origin.CENTER;

    static final float defaultHeight = 2f;
    static final float defaultRadius = 1f;

    static final boolean defaultBottomVisible = true;
    static final boolean defaultSolidness = true;
    static final boolean defaultSideVisible = true;
    static final boolean defaultTopVisible = true;

    /**
     * Set the origin of this cylinder.
     * @param origin the new origin of this cylinder.
     */
    void setOrigin(Origin origin);

    /**
     * @return the origin of this cylinder.
     */
    Origin getOrigin();

    /**
     * Set the height of this cylinder.
     * @param height the new height of this cylinder.
     */
    void setHeight(float height);

    /**
     * @return the height of this cylinder.
     */
    float getHeight();

    /**
     * Set the radius (bottom and top) of this cylinder.
     * @param radius the new radius of this cylinder.
     */
    void setRadius(float radius);

    /**
     * @return the average radius of this cylinder.
     */
    float getRadius();

    /**
     * Set the bottom radius of this cylinder.
     * @param bottomRadius the new bottomRadius radius of this cylinder.
     */
    void setBottomRadius(float bottomRadius);

    /**
     * @return the bottom radius of this cylinder.
     */
    float getBottomRadius();

    /**
     * Set the top radius of this cylinder.
     * @param topRadius the new topRadius radius of this cylinder.
     */
    void setTopRadius(float topRadius);

    /**
     * @return the top radius of this cylinder.
     */
    float getTopRadius();

    /**
     * Set the visibility of the bottom of this cylinder.
     * @param isBottomVisible the new visibility of the bottom of this cylinder.
     */
    void setBottomVisible(boolean isBottomVisible);

    /**
     * @return the visibility of the bottom of this cylinder.
     */
    boolean getBottomVisible();

    /**
     * Set the solidness of this cylinder.
     * @param solidness the new solidness of this cylinder.
     */
    void setSolidness(boolean solidness);

    /**
     * @return the solidness of this cylinder.
     */
    boolean getSolidness();

    /**
     * Set the visibility of the side of this cylinder.
     * @param isSideVisible the new visibility of the side of this cylinder.
     */
    void setSideVisible(boolean isSideVisible);

    /**
     * @return the visibility of the side of this cylinder.
     */
    boolean getSideVisible();


    /**
     * Set the visibility of the top of this cylinder.
     * @param isTopVisible the new visibility of the top of this cylinder.
     */
    void setTopVisible(boolean isTopVisible);

    /**
     * @return the visibility of the top of this cylinder.
     */
    boolean getTopVisible();

    /**
     * @return the visibility of this cylinder.
     * This cylinder is visible only if one of is components (top, bottom, side) is visible.
     */
    boolean getVisible();
}
