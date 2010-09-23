/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.maths;

/**
 * Author: Lando Pierre
 * Date: 20 mai 2010
 */
public interface IVector3f {

    /**
     * Set the x value.
     * @param x the new x value.
     */
    void setX(float x);

    /**
     * return the x value.
     * @return the x value.
     */
    float getX();

    /**
     * Set the y value.
     * @param y the new y value.
     */
    void setY(float y);

    /**
     * return the y value.
     * @return the y value.
     */
    float getY();

    /**
     * Set the z value.
     * @param z the new z value.
     */
    void setZ(float z);

    /**
     * return the z value.
     * @return the z value.
     */
    float getZ();

    /**
     * Normalize this vector and return it.
     * If the vector was initially (0,0,0) it's set to (1,0,0).
     * @return this.
     */
    public IVector3f normalize();

    /** 
     * @return a normalized copy of this vector.
     */
    public IVector3f getNormalized();


    /**
     * Scale this vector by the given factor and return it.
     * @param factor by witch this vector is scaled.
     * @return this.
     */
    public IVector3f scale(float factor);

    /**
     * @param factor by witch the vector is scaled.
     * @return a scaled copy of this vector.
     */
    public IVector3f times(float factor);

    /**
     * @return the norm of this vector.
     */
    public float getNorm();

    /**
     * Copy value from given vector.
     * @param vector The vector copied.
     */
    void set(IVector3f vector);

    /**
     * @param vector given vector.
     * @return a new IVector3f equals to this minus given vector.
     */
    IVector3f minus(IVector3f vector);

    /**
     * @param vector given vector.
     * @return this with vector added.
     */
    IVector3f add(IVector3f vector);

    /**
     * @param vector given vector.
     * @return this with vector subtracted.
     */
    IVector3f subtract(IVector3f vector);

    /**
     * @param vector given vector.
     * @return a new IVector3f equals to this plus given vector. 
     */
    IVector3f plus(IVector3f vector);

    /**
     * @return data of this vector.
     */
    float[] data();

    /**
     * @param vector given vector.
     * @return a new IVector3f equals to this cross product given vector.
     */
    IVector3f crossProduct(IVector3f vector);

    /**
     * @param vector given vector.
     * @return the dot product of this vector by the given one.
     */
    float dot(IVector3f vector);
}
