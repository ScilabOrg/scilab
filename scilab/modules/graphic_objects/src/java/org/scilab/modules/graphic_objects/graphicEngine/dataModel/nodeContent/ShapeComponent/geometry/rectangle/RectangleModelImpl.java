package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.rectangle;

import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableModel;

/**
 * Author: Lando Pierre
 * Date: june 25, 2010
 */
public class RectangleModelImpl extends AbstractDrawableModel implements IRectangleModel {

    private Origin origin = defaultOrigin;
    private float height = defaultHeight;
    private float width = defaultWidth;

    public RectangleModelImpl() {
    }

    public RectangleModelImpl(float width, float height) {
        setWidth(width);
        setHeight(height);
    }

    public RectangleModelImpl(float width, float height, Origin origin) {
        setWidth(width);
        setHeight(height);
        setOrigin(origin);
    }

    
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

    
    public void setWidth(float width) {
        this.width = width;
    }

    
    public float getWidth() {
        return width;
    }

    
    public String getType() {
        return type;
    }
}
