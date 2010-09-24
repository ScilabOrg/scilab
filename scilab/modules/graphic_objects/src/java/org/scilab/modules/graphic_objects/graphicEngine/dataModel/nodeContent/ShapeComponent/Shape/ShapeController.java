package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.Shape;

import org.scilab.modules.graphic_objects.util.MVC.IDataController;
import org.scilab.modules.graphic_objects.util.MVC.IDataEvent;
import org.scilab.modules.graphic_objects.util.MVC.IDataListener;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.AbstractGraphicController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.appearance.IAppearanceController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.IGeometryController;

/**
 * Author: Lando Pierre
 * Date: june 18, 2010
 */
public class ShapeController extends AbstractGraphicController<IShapeModel> implements IShapeController, IDataListener {

    public ShapeController() {
        this(new ShapeModelImpl());
    }

    public ShapeController(IShapeModel model) {
        super(model);
    }

    public ShapeController(IAppearanceController appearance, IGeometryController geometry) {
        this(new ShapeModelImpl());
        setAppearance(appearance);
        setGeometry(geometry);
    }

    
    public void setAppearance(IAppearanceController appearance) {
        if (getAppearance() != appearance) {

            if (getAppearance() != null) {
                getAppearance().removeListener(this);
            }

            getModel().setAppearance(appearance);

            if (getAppearance() != null) {
                getAppearance().addListener(this);
            }

            fireUpdate();
        }
    }

    
    public IAppearanceController getAppearance() {
        return getModel().getAppearance();
    }

    
    public void setGeometry(IGeometryController geometry) {
        if (getGeometry() != geometry) {
            getModel().setGeometry(geometry);
            fireUpdate();
        }
    }

    
    public IGeometryController getGeometry() {
        return getModel().getGeometry();
    }

    
    public void update(IDataController sources, IDataEvent event) {
        fireUpdate(event);
    }
}
