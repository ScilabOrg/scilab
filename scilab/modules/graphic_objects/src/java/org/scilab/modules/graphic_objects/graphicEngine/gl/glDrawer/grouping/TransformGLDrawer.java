/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.grouping;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.grouping.transform.ITransformNodeModel;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.IGLRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.IGLDrawer;

import javax.media.opengl.GL;

/**
 * Author: Lando Pierre
 * Date: june 11, 2010
 *
 * OpenGl drawer of ITransformNodeModel
 */
public class TransformGLDrawer implements IGLDrawer<ITransformNodeModel> {
    private static final TransformGLDrawer singleton = new TransformGLDrawer();

    /**
     * Private constructor for singleton.
     */
    private TransformGLDrawer() {}

    public static TransformGLDrawer getDrawer() {
        return singleton;
    }

    
    public String getType() {
        return ITransformNodeModel.type;
    }

    
    public void beginDraw(ITransformNodeModel transform, IGLRenderingInfo renderingInfo) {
        GL gl = renderingInfo.getGL();
        gl.glMatrixMode(GL.GL_MODELVIEW);
        gl.glPushMatrix();
        gl.glMultMatrixf(transform.getTransformMatrix(), 0);
    }

    
    public void endDraw(ITransformNodeModel transform, IGLRenderingInfo renderingInfo) {
        GL gl = renderingInfo.getGL();
        gl.glMatrixMode(GL.GL_MODELVIEW);
        gl.glPopMatrix();
    }
}
