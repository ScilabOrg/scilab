package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.mesh;

import org.scilab.modules.graphic_objects.graphicEngine.core.array.IColorArray;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.IIndexArray;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.INormalArray;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.IVertexArray;
import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableModel;

/**
 * Author: Lando Pierre
 * Date: june 18, 2010
 */

public class StaticIndexedTriangleSetModel extends AbstractDrawableModel implements IIndexedTriangleSetModel {
    private final IVertexArray vertexArray;
    private final INormalArray normalArray;
    private final IIndexArray indexArray;
    private final IColorArray colorArray;

    public StaticIndexedTriangleSetModel(IVertexArray vertexArray, IIndexArray indexArray, IColorArray colorArray, INormalArray normalArray) {
        this.vertexArray = vertexArray;
        this.indexArray = indexArray;
        this.colorArray = colorArray;
        this.normalArray = normalArray;
    }


     public void setVertexArray(IVertexArray vertexArray) {}
     public void setIndexArray(IIndexArray indexArray) {}
     public void setNormalArray(INormalArray normalArray) {}
     public void setColorArray(IColorArray colorArray) {}

    
    public IVertexArray getVertexArray() {
        return vertexArray;
    }

    
    public INormalArray getNormalArray() {
        return normalArray;
    }

    
    public IColorArray getColorArray() {
        return colorArray;
    }

    
    public IIndexArray getIndexArray() {
        return indexArray;
    }

    
    public String getType() {
        return IIndexedTriangleSetModel.type;
    }
}
