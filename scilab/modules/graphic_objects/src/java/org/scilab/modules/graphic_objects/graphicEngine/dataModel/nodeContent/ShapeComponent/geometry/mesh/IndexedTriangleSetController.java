package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.mesh;

import org.scilab.modules.graphic_objects.graphicEngine.core.array.IColorArray;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.IIndexArray;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.INormalArray;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.IVertexArray;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.AbstractGraphicController;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 */
public class IndexedTriangleSetController extends AbstractGraphicController<IIndexedTriangleSetModel> implements IIndexedTriangleSetController {
    
    public IndexedTriangleSetController(IIndexedTriangleSetModel model) {
        super(model);
    }

    
    public void setVertexArray(IVertexArray vertexArray) {
        if (getVertexArray() != vertexArray) {
            getModel().setVertexArray(vertexArray);
            fireUpdate();
        }
    }

    
    public IVertexArray getVertexArray() {
        return getModel().getVertexArray();
    }

    
    public void setNormalArray(INormalArray normalArray) {
        if (getNormalArray() != normalArray) {
            getModel().setNormalArray(normalArray);
            fireUpdate();
        }
    }

    
    public INormalArray getNormalArray() {
        return getModel().getNormalArray();
    }

    
    public void setColorArray(IColorArray colorArray) {
        if (getColorArray() != colorArray) {
            getModel().setColorArray(colorArray);
            fireUpdate();
        }
    }

    
    public IColorArray getColorArray() {
        return getModel().getColorArray();
    }

    
    public void setIndexArray(IIndexArray indexArray) {
        if (getIndexArray() != indexArray) {
            getModel().setIndexArray(indexArray);
            fireUpdate();
        }
    }

    
    public IIndexArray getIndexArray() {
        return getModel().getIndexArray();
    }
}
