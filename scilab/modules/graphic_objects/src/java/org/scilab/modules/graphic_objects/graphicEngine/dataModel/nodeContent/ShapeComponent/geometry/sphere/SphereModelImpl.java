package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.sphere;

import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableModel;

/**
 * Author: Lando Pierre
 * Date: june 25, 2010
 */
public class SphereModelImpl extends AbstractDrawableModel implements ISphereModel {
    private float radius = defaultRadius;

    public SphereModelImpl() {
    }

    public SphereModelImpl(float radius) {
        setRadius(radius);
    }

    
    public void setRadius(float radius) {
        this.radius = radius;
    }

    
    public float getRadius() {
        return radius;
    }

    
    public String getType() {
        return type;
    }
}
