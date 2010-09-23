package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.cylinder;

import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableModel;

/**
 * Author: Lando Pierre
 * Date: june 21, 2010
 */
public class CylinderModelImpl extends AbstractDrawableModel implements ICylinderModel {
    private float height = ICylinderModel.defaultHeight;
    private float bottomRadius = ICylinderModel.defaultRadius;
    private float topRadius = ICylinderModel.defaultRadius;
    private boolean solidness = ICylinderModel.defaultSolidness;
    private boolean isBottomVisible = ICylinderModel.defaultBottomVisible;
    private boolean isSideVisible = ICylinderModel.defaultSideVisible;
    private boolean isTopVisible = ICylinderModel.defaultTopVisible;
    private Origin origin = ICylinderModel.defaultOrigin;

    
    public void setOrigin(Origin origin) {
        this.origin = origin;
    }

    
    public Origin getOrigin() {
        return origin;
    }

    
    public void setHeight(float height) {
        this.height = height;
    }

    
    public float getHeight() {
        return height;
    }

    
    public void setRadius(float radius) {
        setTopRadius(radius);
        setBottomRadius(radius);
    }

    
    public float getRadius() {
        return (getTopRadius() + getBottomRadius()) / 2f;
    }

    
    public void setBottomRadius(float bottomRadius) {
        this.bottomRadius = bottomRadius;
    }

    
    public float getBottomRadius() {
        return bottomRadius;
    }

    
    public void setTopRadius(float topRadius) {
        this.topRadius = topRadius;
    }

    
    public float getTopRadius() {
        return topRadius;
    }

    
    public void setBottomVisible(boolean isBottomVisible) {
        this.isBottomVisible = isBottomVisible;
    }

    
    public boolean getBottomVisible() {
        return isBottomVisible;
    }

    
    public void setSolidness(boolean solidness) {
        this.solidness = solidness;
    }

    
    public boolean getSolidness() {
        return solidness;
    }

    
    public void setSideVisible(boolean isSideVisible) {
        this.isSideVisible = isSideVisible;
    }

    
    public boolean getSideVisible() {
        return isSideVisible;
    }

    
    public void setTopVisible(boolean isTopVisible) {
        this.isTopVisible = isTopVisible;
    }

    
    public boolean getTopVisible() {
        return isTopVisible;
    }

    
    public boolean getVisible() {
        return isTopVisible || isBottomVisible || isSideVisible;
    }

    
    public String getType() {
        return type;
    }
}
