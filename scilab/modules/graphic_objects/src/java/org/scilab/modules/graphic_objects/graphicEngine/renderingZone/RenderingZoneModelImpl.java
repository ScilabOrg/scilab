package org.scilab.modules.graphic_objects.graphicEngine.renderingZone;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.core.IGraphicTreeController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.navigation.ICameraController;
import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableModel;
import org.scilab.modules.graphic_objects.graphicEngine.renderingZone.layout.ILayoutPolicy;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 */
public class RenderingZoneModelImpl extends AbstractDrawableModel implements IRenderingZoneModel {
    private ICameraController camera;
    private IGraphicTreeController graphicDataController;
    private ILayoutPolicy layoutPolicy;

    
    public void setCamera(ICameraController camera) {
        this.camera = camera;
    }

    
    public ICameraController getCamera() {
        return camera;
    }

    
    public void setDataController(IGraphicTreeController graphicDataController) {
        this.graphicDataController = graphicDataController;
    }

    
    public IGraphicTreeController getDataController() {
        return graphicDataController;
    }

    
    public void setLayoutPolicy(ILayoutPolicy layoutPolicy) {
        this.layoutPolicy = layoutPolicy;
    }

    
    public ILayoutPolicy getLayoutPolicy() {
        return layoutPolicy;
    }

    
    public String getType() {
        return type;
    }
}
