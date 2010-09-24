package org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.shape;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.Shape.IShapeModel;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.appearance.IAppearanceModel;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.IGeometryModel;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.IGLRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.IGLDrawer;

/**
 * Author: Lando Pierre
 * Date: june 18, 2010
 *
 * OpenGl drawer of IShapeModel 
 */
public class ShapeGLDrawer implements IGLDrawer<IShapeModel> {
    private static final ShapeGLDrawer drawer = new ShapeGLDrawer();

    public static IGLDrawer getDrawer() {
        return drawer;
    }

    private ShapeGLDrawer() {}

    
    public void beginDraw(IShapeModel shape, IGLRenderingInfo renderingInfo) {
        IAppearanceModel appearance = shape.getAppearance();
        IGeometryModel geometry = shape.getGeometry();

        IGLDrawer<IAppearanceModel> appearanceDrawer = renderingInfo.getDrawersManager().getSpecificDrawer(appearance);
        IGLDrawer<IGeometryModel> geometryDrawer = renderingInfo.getDrawersManager().getSpecificDrawer(geometry);

        if (appearanceDrawer != null) {
            appearanceDrawer.beginDraw(appearance, renderingInfo);
        }

        if (geometryDrawer != null) {
            geometryDrawer.beginDraw(geometry, renderingInfo);
            geometryDrawer.endDraw(geometry, renderingInfo);
        }

        if (appearanceDrawer != null) {
            appearanceDrawer.endDraw(appearance, renderingInfo);
        }
    }

    
    public void endDraw(IShapeModel shape, IGLRenderingInfo renderingInfo) {
    }

    
    public String getType() {
        return IShapeModel.type;
    }
}
