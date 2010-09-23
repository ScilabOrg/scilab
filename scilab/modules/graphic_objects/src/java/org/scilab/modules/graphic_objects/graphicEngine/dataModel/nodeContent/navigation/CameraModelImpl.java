package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.navigation;

import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableModel;
import org.scilab.modules.graphic_objects.graphicEngine.maths.IVector3f;
import org.scilab.modules.graphic_objects.graphicEngine.maths.UnitaryQuaternion;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 */
public class CameraModelImpl extends AbstractDrawableModel implements ICameraModel {
    private IVector3f position = ICameraModel.defaultPosition;
    private UnitaryQuaternion orientation = ICameraModel.defaultOrientation;
    private float nearPlane = ICameraModel.defaultNearPlane;
    private float farPlane = ICameraModel.defaultFarPlane;
    private float fieldOfView = ICameraModel.defaultFieldOfView;
    private float aspectRatio = ICameraModel.defaultAspectRatio;

    
    public void setPosition(IVector3f position) {
        this.position = position;
    }

    
    public IVector3f getPosition() {
        return position;
    }

    
    public void setOrientation(UnitaryQuaternion orientation) {
        this.orientation = orientation;
    }

    
    public UnitaryQuaternion getOrientation() {
        return orientation;
    }

    
    public void setNearPlane(float nearPlane) {
        this.nearPlane = nearPlane;
    }

    
    public float getNearPlane() {
        return nearPlane;
    }

    
    public void setFarPlane(float farPlane) {
        this.farPlane = farPlane;
    }

    
    public float getFarPlane() {
        return farPlane;
    }

    
    public void setFieldOfView(float fieldOfView) {
        this.fieldOfView = fieldOfView;
    }

    
    public float getFieldOfView() {
        return fieldOfView;
    }

    
    public void setAspectRatio(float aspectRatio) {
        this.aspectRatio = aspectRatio;
    }

    
    public float getAspectRatio() {
        return aspectRatio;
    }

    
    public String getType() {
        return type;
    }
}
