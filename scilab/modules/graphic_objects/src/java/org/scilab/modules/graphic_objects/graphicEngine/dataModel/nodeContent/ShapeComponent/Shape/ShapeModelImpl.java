package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.Shape;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.appearance.IAppearanceController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.IGeometryController;
import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableModel;

/**
 * Author: Lando Pierre
 * Date: june 18, 2010
 */
public class ShapeModelImpl extends AbstractDrawableModel implements IShapeModel {
    private IAppearanceController appearance = null;
    private IGeometryController geometry = null;

    public ShapeModelImpl() {
    }

    public ShapeModelImpl(IAppearanceController appearance, IGeometryController geometry) {
        this.appearance = appearance;
        this.geometry = geometry;
    }

    
    public void setAppearance(IAppearanceController appearance) {
        this.appearance = appearance; 
    }

    
    public IAppearanceController getAppearance() {
        return appearance;
    }

    
    public void setGeometry(IGeometryController geometry) {
        this.geometry = geometry;
    }

    
    public IGeometryController getGeometry() {
        return geometry;
    }

    
    public String getType() {
        return type;
    }
}
