package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.arrow;

import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableModel;

/**
 * Author: Lando Pierre
 * Date: june 18, 2010
 */
public class ArrowModelImpl extends AbstractDrawableModel implements IArrowModel {
    private float size = IArrowModel.defaultBodySize;
    private float radius = IArrowModel.defaultHeadRadius;

    
    public void setSize(float size) {
        this.size = size;
    }

    
    public float getSize() {
        return size;
    }

    
    public void setHeadRadius(float radius) {
        this.radius = radius;
    }

    
    public float getHeadRadius() {
        return radius;
    }

    
    public String getType() {
        return type;
    }
}
