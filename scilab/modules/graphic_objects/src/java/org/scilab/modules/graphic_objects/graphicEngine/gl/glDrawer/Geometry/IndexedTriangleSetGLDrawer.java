/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.Geometry;

import org.scilab.modules.graphic_objects.graphicEngine.core.IRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.UnBindableException;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.mesh.IIndexedTriangleSetModel;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.IGLRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.IGLDrawer;

import javax.media.opengl.GL;

/**
 * Author: Lando Pierre
 * Date: june 7, 2010
 *
 * OpenGl drawer of IIndexedTriangleSetModel
 */
public class IndexedTriangleSetGLDrawer implements IGLDrawer<IIndexedTriangleSetModel> {
    private final static IndexedTriangleSetGLDrawer singleton = new IndexedTriangleSetGLDrawer();

    private IndexedTriangleSetGLDrawer() {
    }

    public static IndexedTriangleSetGLDrawer getDrawer() {
        return singleton;
    }

    
    public String getType() {
        return IIndexedTriangleSetModel.type;
    }

    
    public void beginDraw(IIndexedTriangleSetModel shape, IGLRenderingInfo renderingInfo) {

        try {
            GL gl = renderingInfo.getGL();

            gl.glShadeModel(GL.GL_SMOOTH);

            if (shape.getIndexArray() != null) {
                shape.getIndexArray().bind(renderingInfo);
                if (shape.getVertexArray() != null) {
                    shape.getVertexArray().bind(renderingInfo);

                    if (renderingInfo.getRenderingMode() == IRenderingInfo.RenderingMode.NORMAL_MODE) {                    
                        if (shape.getNormalArray() != null) {
                            shape.getNormalArray().bind(renderingInfo);
                        }
                        if (shape.getColorArray() != null) {
                            shape.getColorArray().bind(renderingInfo);
                        }
                    }
                    gl.glDrawElements(GL.GL_TRIANGLES, shape.getIndexArray().size(), GL.GL_UNSIGNED_INT, 0);
                }
            }

            gl.glDisable(GL.GL_VERTEX_ARRAY);
            gl.glDisable(GL.GL_COLOR_ARRAY);
            gl.glDisable(GL.GL_NORMAL_ARRAY);
            
        } catch (UnBindableException e) {
            e.printStackTrace();
        }
    }

    
    public void endDraw(IIndexedTriangleSetModel shape, IGLRenderingInfo renderingInfo) {
    }
}
