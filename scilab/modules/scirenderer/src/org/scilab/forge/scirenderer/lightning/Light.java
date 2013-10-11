/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.lightning;

import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

/**
 * The light interface.
 *
 * @author Pierre Lando
 */
public interface Light {

    /**
     * Return the status of this light.
     * @return the status of this light.
     */
    boolean isEnable();

    /**
     * Set the status of this light.
     * @param enable the new status of this light.
     */
    void setEnable(boolean enable);

    /**
     * Return the ambient color of this light;
     * @return the ambient color of this light;
     */
    Color getAmbientColor();

    /**
     * Set the ambient color of this light.
     * @param color the new ambient color of this light.
     */
    void setAmbientColor(Color color);

    /**
     * Return the diffuse color of this light;
     * @return the diffuse color of this light;
     */
    Color getDiffuseColor();

    /**
     * Set the diffuse color of this light.
     * @param color the new diffuse color of this light.
     */
    void setDiffuseColor(Color color);

    /**
     * Return the specular color of this light;
     * @return the specular color of this light;
     */
    Color getSpecularColor();

    /**
     * Set the specular color of this light.
     * @param color the new specular color of this light.
     */
    void setSpecularColor(Color color);

    /**
     * Return the light position.
     * @return the light position.
     */
    Vector3d getPosition();

    /**
     * Set the light position.
     * @param position the new position.
     */
    void setPosition(Vector3d position);

    /**
     * Get the light direction.
     * @return the light direction.
     */
    Vector3d getDirection();

    /**
     * Set the light direction.
     * Used only for directional light.
     * @param position the new position.
     */
    void setDirection(Vector3d direction);

    /**
     * Return the spot direction.
     * @return the spot direction.
     */
    Vector3d getSpotDirection();

    /**
     * Set the spot direction.
     * @param spotDirection the new spot direction.
     */
    void setSpotDirection(Vector3d spotDirection);

    /**
     * Return the spot angle.
     * @return the spot angle.
     */
    float getSpotAngle();

    /**
     * Set the spot angle.
     * @param angle the new spot angle.
     */
    void setSpotAngle(float angle);

    /**
     * Return the light index.
     * @return the light index.
     */
    int getIndex();
}
