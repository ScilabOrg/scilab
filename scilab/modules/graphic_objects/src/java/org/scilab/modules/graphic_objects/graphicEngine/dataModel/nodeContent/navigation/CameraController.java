package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.navigation;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.AbstractGraphicController;
import org.scilab.modules.graphic_objects.graphicEngine.maths.IVector3f;
import org.scilab.modules.graphic_objects.graphicEngine.maths.UnitaryQuaternion;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 */
public class CameraController extends AbstractGraphicController<ICameraModel> implements ICameraController {

    public CameraController() {
        super(new CameraModelImpl());
    }

    public CameraController(ICameraModel model) {
        super(model);
    }
    
    public void setPosition(IVector3f position) {
        getModel().setPosition(position);
        fireUpdate();
    }

    
    public IVector3f getPosition() {
        return getModel().getPosition();
    }

    
    public void setOrientation(UnitaryQuaternion orientation) {
        getModel().setOrientation(orientation);
        fireUpdate();
    }

    
    public UnitaryQuaternion getOrientation() {
        return getModel().getOrientation();
    }

    
    public void setNearPlane(float nearPlane) {
        getModel().setNearPlane(nearPlane);
        fireUpdate();
    }

    
    public float getNearPlane() {
        return getModel().getNearPlane();
    }

    
    public void setFarPlane(float farPlane) {
        getModel().setFarPlane(farPlane);
        fireUpdate();
    }

    
    public float getFarPlane() {
        return getModel().getFarPlane();
    }

    
    public void setFieldOfView(float fieldOfView) {
        getModel().setFieldOfView(fieldOfView);
        fireUpdate();
    }

    
    public float getFieldOfView() {
        return getModel().getFieldOfView();
    }

    
    public void setAspectRatio(float aspectRatio) {
        getModel().setAspectRatio(aspectRatio);
        fireUpdate();
    }

    
    public float getAspectRatio() {
        return getModel().getAspectRatio();
    }
}
