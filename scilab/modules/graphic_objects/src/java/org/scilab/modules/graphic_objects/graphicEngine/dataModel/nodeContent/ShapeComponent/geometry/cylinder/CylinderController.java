package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.cylinder;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.AbstractGraphicController;

/**
 * Author: Lando Pierre
 * Date: june 21, 2010
 */
public class CylinderController extends AbstractGraphicController<ICylinderModel> implements ICylinderController {

    public CylinderController() {
        super(new CylinderModelImpl());
    }

    public CylinderController(ICylinderModel model) {
        super(model);
    }

    
    public void setOrigin(Origin origin) {
        if (getOrigin() != origin) {
            getModel().setOrigin(origin);
            fireUpdate();
        }
    }

    
    public Origin getOrigin() {
        return getModel().getOrigin();
    }

    
    public void setHeight(float height) {
        if (getHeight() != height) {
            getModel().setHeight(height);
            fireUpdate();
        }
    }

    
    public float getHeight() {
        return getModel().getHeight();
    }

    
    public void setRadius(float radius) {
        if (getBottomRadius() != radius || getTopRadius() != radius) {
            getModel().setRadius(radius);
            fireUpdate();
        }
    }

    
    public float getRadius() {
        return getModel().getRadius();
    }

    
    public void setBottomRadius(float bottomRadius) {
        if (getBottomRadius() != bottomRadius) {
            getModel().setBottomRadius(bottomRadius);
            fireUpdate();
        }
    }

    
    public float getBottomRadius() {
        return getModel().getBottomRadius();
    }

    
    public void setTopRadius(float topRadius) {
        if (getTopRadius() != topRadius) {
            getModel().setTopRadius(topRadius);
            fireUpdate();
        }
    }

    
    public float getTopRadius() {
        return getModel().getTopRadius();
    }

    
    public void setBottomVisible(boolean isBottomVisible) {
        if (getBottomVisible() != isBottomVisible) {
            getModel().setBottomVisible(isBottomVisible);
            fireUpdate();
        }
    }

    
    public boolean getBottomVisible() {
        return getModel().getBottomVisible();
    }

    
    public void setSolidness(boolean solidness) {
        if (getSolidness() != solidness) {
            getModel().setSolidness(solidness);
            fireUpdate();
        }
    }

    
    public boolean getSolidness() {
        return getModel().getSolidness();
    }

    
    public void setSideVisible(boolean isSideVisible) {
        if (getSideVisible() != isSideVisible) {
            getModel().setSideVisible(isSideVisible);
            fireUpdate();
        }
    }

    
    public boolean getSideVisible() {
        return getModel().getSideVisible();
    }

    
    public void setTopVisible(boolean isTopVisible) {
        if (getTopVisible() != isTopVisible) {
            getModel().setTopVisible(isTopVisible);
            fireUpdate();
        }
    }

    
    public boolean getTopVisible() {
        return getModel().getTopVisible();
    }

    
    public boolean getVisible() {
        return getModel().getVisible();
    }
}
