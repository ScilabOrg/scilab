package org.scilab.modules.graphic_objects.graphicEngine.renderingZone;

import org.scilab.modules.graphic_objects.util.MVC.*;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.core.IGraphicTreeController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.navigation.ICameraController;
import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableController;
import org.scilab.modules.graphic_objects.graphicEngine.renderingZone.layout.ILayoutPolicy;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 */
public class RenderingZoneController extends AbstractDrawableController<IRenderingZoneModel> implements IRenderingZoneController {
    private final LocalListener listener = new LocalListener(); 

    /**
     * Default constructor.
     * @param model handled bu this controller.
     */
    public RenderingZoneController(IRenderingZoneModel model) {
        super(model);
    }

    
    public void setCamera(ICameraController camera) {
        ICameraController oldCamera = getCamera();
        if (oldCamera != camera) {
            getModel().setCamera(camera);
            fireUpdate();
        }
    }

    
    public ICameraController getCamera() {
        return getModel().getCamera();
    }

    
    public void setDataController(IGraphicTreeController graphicDataController) {
        IGraphicTreeController oldGraphicTreeController = getDataController();
        if (oldGraphicTreeController != graphicDataController) {
            if (oldGraphicTreeController != null) {
                oldGraphicTreeController.removeListener(listener);
            }

            getModel().setDataController(graphicDataController);

            if (graphicDataController != null) {
                graphicDataController.addListener(listener);
            }

            fireUpdate();
        }
    }

    
    public IGraphicTreeController getDataController() {
        return getModel().getDataController();
    }

    
    public void setLayoutPolicy(ILayoutPolicy layoutPolicy) {
        getModel().setLayoutPolicy(layoutPolicy);
        fireUpdate();
    }

    
    public ILayoutPolicy getLayoutPolicy() {
        return getModel().getLayoutPolicy();
    }

    
    public String getType() {
        return getModel().getType();
    }

    private class LocalListener implements IDataListener {

        
        public void update(IDataController sources, IDataEvent event) {
            fireUpdate(event);
        }
    }
}
