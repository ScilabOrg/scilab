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
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.box.IBoxModel;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.IGLRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.IGLDrawer;
import org.scilab.modules.graphic_objects.graphicEngine.maths.IVector3f;

import javax.media.opengl.GL;

/**
 * Author: Lando Pierre
 * Date: 20 mai 2010
 *
 * OpenGl drawer of IBoxModel
 */
public class BoxGLDrawer implements IGLDrawer<IBoxModel> {
    /**
     * Singleton of this class.
     */
    private static final BoxGLDrawer singleton = new BoxGLDrawer();

    /**
     * Make constructor private for singleton.
     */
    private BoxGLDrawer() {
    }

    public static BoxGLDrawer getDrawer() {
        return singleton;
    }

    
    public String getType() {
        return IBoxModel.type;
    }

    
    public void beginDraw(IBoxModel box, IGLRenderingInfo renderingInfo) {
        GL gl = renderingInfo.getGL();
        gl.glMatrixMode(GL.GL_MODELVIEW);
        gl.glPushMatrix();

        IVector3f size = box.getSize();
        gl.glScalef(size.getX()/2f, size.getY()/2f, size.getZ()/2f);

        if (renderingInfo.getRenderingMode() == IRenderingInfo.RenderingMode.NORMAL_MODE) {
            gl.glColor3f(.2f, .3f, .4f);
        }

        gl.glBegin(GL.GL_QUADS);
            gl.glVertex3f(+1, +1, +1); gl.glVertex3f(+1, -1, +1); gl.glVertex3f(-1, -1, +1); gl.glVertex3f(-1, +1, +1);
            gl.glVertex3f(+1, +1, -1); gl.glVertex3f(+1, -1, -1); gl.glVertex3f(-1, -1, -1); gl.glVertex3f(-1, +1, -1);

            gl.glVertex3f(+1, +1, +1); gl.glVertex3f(+1, +1, -1); gl.glVertex3f(+1, -1, -1); gl.glVertex3f(+1, -1, +1);
            gl.glVertex3f(-1, +1, +1); gl.glVertex3f(-1, +1, -1); gl.glVertex3f(-1, -1, -1); gl.glVertex3f(-1, -1, +1);

            gl.glVertex3f(+1, +1, +1); gl.glVertex3f(+1, +1, -1); gl.glVertex3f(-1, +1, -1); gl.glVertex3f(-1, +1, +1);
            gl.glVertex3f(+1, -1, +1); gl.glVertex3f(+1, -1, -1); gl.glVertex3f(-1, -1, -1); gl.glVertex3f(-1, -1, +1);
        gl.glEnd();

        gl.glPopMatrix();
    }

    
    public void endDraw(IBoxModel shape, IGLRenderingInfo renderingInfo) {

    }
}
