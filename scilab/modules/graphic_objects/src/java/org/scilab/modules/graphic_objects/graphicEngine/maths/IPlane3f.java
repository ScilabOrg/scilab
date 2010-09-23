package org.scilab.modules.graphic_objects.graphicEngine.maths;

/**
 * Author: Lando Pierre
 * Date: june 29, 2010
 *
 * This interface specify a plane.
 * The plane is defined by all (x, y, z) points that satisfy the equation :
 * x*nx + y*ny + z*nz + s = 0
 * Where (nx, ny, nz) is the normal vector.
 * s is the scalar value.
 */
public interface IPlane3f {

    /**
     * return the normal of this plane.
     * @return the normal of this plane.
     */
    IVector3f getNormal();

    /**
     * Set the normal of this plane.
     * @param vector the new normal of this plane.
     */
    void setNormal(IVector3f vector); // TODO : throw null vector.

    /**
     * Set the normal of this plane. 
     * @param nx the first coordinate of the new normal of this plane.
     * @param ny the second coordinate of the new normal of this plane.
     * @param nz the third coordinate of the new normal of this plane.
     */
    void setNormal(float nx, float ny, float nz); // TODO : throw null vector.

    /**
     * return the scalar of this plane.
     * @return the scalar of this plane.
     */
    float getScalar();

    /**
     * Set the scalar of this plane.
     * @param scalar the new scalar of this plane.
     */
    void setScalar(float scalar);
}
