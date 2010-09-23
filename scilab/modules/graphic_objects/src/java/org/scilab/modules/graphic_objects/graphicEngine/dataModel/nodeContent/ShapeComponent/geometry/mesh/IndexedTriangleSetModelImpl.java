package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.mesh;

import org.scilab.modules.graphic_objects.graphicEngine.core.array.IColorArray;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.IIndexArray;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.INormalArray;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.IVertexArray;
import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableModel;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 */
public class IndexedTriangleSetModelImpl extends AbstractDrawableModel implements IIndexedTriangleSetModel {
    private IVertexArray vertexArray;
    private IIndexArray indexArray;
    private INormalArray normalArray;
    private IColorArray colorArray;


    
    public void setVertexArray(IVertexArray vertexArray) {
        this.vertexArray = vertexArray;
    }

    
    public IVertexArray getVertexArray() {
        return vertexArray;
    }

    
    public void setNormalArray(INormalArray normalArray) {
        this.normalArray = normalArray;
    }

    
    public INormalArray getNormalArray() {
        return normalArray;
    }

    
    public void setColorArray(IColorArray colorArray) {
        this.colorArray = colorArray;
    }

    
    public IColorArray getColorArray() {
        return colorArray;
    }

    
    public void setIndexArray(IIndexArray indexArray) {
        this.indexArray = indexArray;
    }

    
    public IIndexArray getIndexArray() {
        return indexArray;
    }

    
    public String getType() {
        return type;
    }
}
