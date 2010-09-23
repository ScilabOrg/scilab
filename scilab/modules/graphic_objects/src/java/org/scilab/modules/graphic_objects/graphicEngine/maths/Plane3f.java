package org.scilab.modules.graphic_objects.graphicEngine.maths;

/**
 * Author: Lando Pierre
 * Date: june 29, 2010
 *
 * Basic implementation of IPlane3f.
 */
public class Plane3f implements IPlane3f {
    IVector3f normal = new Vector3f(0,0,1);
    float scalar = 0;

    public Plane3f(float x, float y, float z, float scalar) {
        setNormal(x, y, z);
        setScalar(scalar);
    }

    public Plane3f(IVector3f normal, float scale) {
        setNormal(normal);
        setScalar(scale);
    }

    
    public void setNormal(float x, float y, float z) {
        normal = new Vector3f(x, y, z);
    }

    
    public IVector3f getNormal() {
        return normal;
    }

    
    public void setNormal(IVector3f vector) {
        this.normal = vector;
    }

    
    public float getScalar() {
        return scalar;
    }

    
    public void setScalar(float scalar) {
        this.scalar = scalar;
    }
}
