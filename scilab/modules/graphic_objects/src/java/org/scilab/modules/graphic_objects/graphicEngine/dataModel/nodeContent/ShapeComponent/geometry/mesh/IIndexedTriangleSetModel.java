/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.mesh;

import org.scilab.modules.graphic_objects.graphicEngine.core.array.IColorArray;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.IIndexArray;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.INormalArray;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.IVertexArray;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.IGeometryModel;

/**
 * Author: Lando Pierre
 * Date: june 7, 2010
 */
public interface IIndexedTriangleSetModel extends IGeometryModel {
    static final String type = "IndexedTriangleSet";

    /**
     * @param vertexArray the vertex array of this shape.
     */
    public void setVertexArray(IVertexArray vertexArray);

    /**
     * @return the vertex array of this shape.
     */
    public IVertexArray getVertexArray();

    /**
     * @param normalArray the new normal array of this shape.
     */
    public void setNormalArray(INormalArray normalArray);

    /**
     * @return the normal array of this shape.
     */
    public INormalArray getNormalArray();

    /**
     * @param colorArray the new colorArray array of this shape.
     */
    public void setColorArray(IColorArray colorArray);

    /**
     * @return the color array of this shape.
     */
    public IColorArray getColorArray();

    /**
     * @param indexArray the index array of this shape.
     */
    public void setIndexArray(IIndexArray indexArray);

    /**
     * @return the index array of this shape.
     */
    public IIndexArray getIndexArray();
}
