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
 *
 * Basic implementation of IVector3f.
 */
public class Vector3f implements IVector3f {
    private final float[] values = new float[]{0,0,0};

    public Vector3f(float x, float y, float z) {
        setX(x);
        setY(y);
        setZ(z);
    }

    public Vector3f(double x, double y, double z) {
        setX((float) x);
        setY((float) y);
        setZ((float) z);
    }

    public Vector3f(IVector3f v) {
        setX(v.getX());
        setY(v.getY());
        setZ(v.getZ());
    }

    public Vector3f(float x, float y, float z, float w) {
        this(x/w, y/w, z/w);
    }

    public Vector3f(float[] v) {
        if (v!=null && v.length==3) {
            System.arraycopy(v, 0, values, 0, 3);
        }
    }

    
    public void setX(float x) {
        values[0] = x;
    }

    
    public float getX() {
        return values[0];
    }

    
    public void setY(float y) {
        values[1] = y;
    }

    
    public float getY() {
        return values[1];
    }

    
    public void setZ(float z) {
        values[2] = z;
    }

    
    public float getZ() {
        return values[2];
    }

    
    public IVector3f normalize() {
        return scale(1f/getNorm());
    }

    
    public IVector3f getNormalized() {
        return new Vector3f(this).normalize();
    }

    
    public IVector3f scale(float factor) {
        values[0] *= factor;
        values[1] *= factor;
        values[2] *= factor;
        return this;
    }

    
    public IVector3f times(float factor) {
        return new Vector3f(this).scale(factor);
    }

    
    public float getNorm() {
        float x = values[0];
        float y = values[1];
        float z = values[2];
        return (float) Math.sqrt(x*x + y*y + z*z);
    }

    
    public void set(IVector3f vector) {
        values[0] = vector.getX();
        values[1] = vector.getY();
        values[2] = vector.getZ();
    }

    
    public IVector3f minus(IVector3f vector) {
        return new Vector3f(getX() - vector.getX(), getY() - vector.getY(), getZ() - vector.getZ());
    }

    
    public IVector3f add(IVector3f vector) {
        values[0] += vector.getX();
        values[1] += vector.getY();
        values[2] += vector.getZ();
        return this;
    }

    
    public IVector3f subtract(IVector3f vector) {
        values[0] -= vector.getX();
        values[1] -= vector.getY();
        values[2] -= vector.getZ();
        return this;
    }

    
    public IVector3f plus(IVector3f vector) {
        return new Vector3f(getX() + vector.getX(), getY() + vector.getY(), getZ() + vector.getZ());
    }

    
    public float[] data() {
        float[] r = new float[3];
        System.arraycopy(values, 0, r, 0, 3);
        return r;
    }

    
    public IVector3f crossProduct(IVector3f vector) {
        return new Vector3f(
                getY()*vector.getZ() - getZ()*vector.getY(),
                getZ()*vector.getX() - getX()*vector.getZ(),
                getX()*vector.getY() - getY()*vector.getX()
        );
    }

    
    public float dot(IVector3f vector) {
        return getX()*vector.getX() + getY()*vector.getY() + getZ()*vector.getZ();
    }

    
    public String toString() {
        return "(" + values[0] + ", " + values[1] + ", " + values[2] + ")";
    }
}
