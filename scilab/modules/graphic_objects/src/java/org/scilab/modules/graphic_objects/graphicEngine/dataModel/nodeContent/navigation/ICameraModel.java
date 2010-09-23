/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.navigation;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.IGraphicModel;
import org.scilab.modules.graphic_objects.graphicEngine.maths.IVector3f;
import org.scilab.modules.graphic_objects.graphicEngine.maths.UnitaryQuaternion;
import org.scilab.modules.graphic_objects.graphicEngine.maths.Vector3f;

/**
 * Author: Lando Pierre
 * Date: 20 mai 2010
 */
public interface ICameraModel extends IGraphicModel {
    static final String type = "Camera";
    static final IVector3f defaultPosition = new Vector3f(0f, 0f,10f);
    static final UnitaryQuaternion defaultOrientation = new UnitaryQuaternion();
    static final float defaultNearPlane = 1/8f;
    static final float defaultFarPlane = 512f;
    static final float defaultFieldOfView = 45f;
    static final float defaultAspectRatio = 16f/9f;

    /**
     * @param position is the new position of the camera.
     */
    void setPosition(IVector3f position);

    /**
     * @return the position of the camera.
     */
    IVector3f getPosition();

    /**
     * @param orientation is the new orientation of the camera.
     */
    void setOrientation(UnitaryQuaternion orientation);

    /**
     * @return the orientation 
     */
    UnitaryQuaternion getOrientation();

    /**
     * @param nearPlane is the new value for the near plane distance.
     */
    void setNearPlane(float nearPlane);

    /**
     * @return the near plane distance.
     */
    float getNearPlane();

    /**
     * @param farPlane is the new value for the far plane distance.
     */
    void setFarPlane(float farPlane);

    /** 
     * @return the far plane distance.
     */
    float getFarPlane();

    /**
     * @param fieldOfView is the new value for the field of view in degree.
     */
    void setFieldOfView(float fieldOfView);

    /**
     * @return the field of view in degree.
     */
    float getFieldOfView();

    /**
     * @param aspectRatio is the new value for the aspect ratio. width / height.
     */
    void setAspectRatio(float aspectRatio);

    /**
     * A zero or negative aspect ratio indicate that should not be taken into account. 
     * @return the aspect ratio, width / height.
     */
    float getAspectRatio();
}
