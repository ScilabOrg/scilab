package org.scilab.modules.graphic_objects.graphicEngine.maths.boundingBox;

/**
 * Author: Lando Pierre
 * Date: june 30, 2010
 */
public class BoundingBox implements IBoundingBoxModel {
    float xMin;
    float xMax;
    float yMin;
    float yMax;
    float zMin;
    float zMax;

    public BoundingBox() {
        
    }

    public BoundingBox(float min, float max) {
        xMin = yMin = zMin = min;
        xMax = yMax = zMax = max;
    }

    
    public float getXMin() {
        return xMin;
    }

    
    public float getXMax() {
        return xMax;
    }

    
    public float getYMin() {
        return yMin;
    }

    
    public float getYMax() {
        return yMax;
    }

    
    public float getZMin() {
        return zMin;
    }

    
    public float getZMax() {
        return zMax;
    }
}
