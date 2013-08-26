/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.implementation.jogl.renderer;

import org.scilab.forge.scirenderer.Drawer;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLDrawingTools;
import org.scilab.forge.scirenderer.renderer.Renderer;

import javax.media.opengl.GL2;

/**
 * @author Pierre Lando
 */
public class JoGLRenderer implements Renderer {

    /**
     * The current drawer.
     */
    private Drawer drawer;

    /**
     * The OpenGl display list name.
     */
    private Integer glName;

    /**
     * Store the display list up to date status.
     */
    private boolean upToDate;

    /**
     * Default constructor.
     * The constructor is package : only {@link JoGLRendererManager} can instantiate this object.
     */
    JoGLRenderer() {
        upToDate = false;
        drawer = null;
        glName = null;
    }

    @Override
    public void setDrawer(Drawer drawer) {
        this.drawer = drawer;
        upToDate = false;
    }

    @Override
    public Drawer getDrawer() {
        return drawer;
    }

    @Override
    public void reload() {
        glName = null;
        upToDate = false;
    }

    /**
     * Perform a draw to the given canvas.
     * @param drawingTools the given drawing tools.
     */
    public void draw(JoGLDrawingTools drawingTools) {
        if (drawingTools.getCanvas().getJoGLParameters().useDisplayList()) {
            synchronize(drawingTools);
            GL2 gl = drawingTools.getGl().getGL2();
            gl.glCallList(glName);
        } else {
            if (drawer != null) {
                drawer.draw(drawingTools);
            }
        }
    }

    /**
     * Synchronize the display list.
     * @param drawingTools drawing tools.
     */
    private void synchronize(JoGLDrawingTools drawingTools) {
        GL2 gl = drawingTools.getGl().getGL2();

        // Check glName.
        if ((glName == null) || !(gl.glIsList(glName))) {
            glName = gl.glGenLists(1);
            upToDate = false;
        }

        // Check up to date.
        if (!upToDate) {
            gl.glNewList(glName, GL2.GL_COMPILE);

            if (drawer != null) {
                drawer.draw(drawingTools);
            }

            gl.glEndList();
            upToDate = true;
        }
    }

    /**
     * Dispose resources.
     * @param gl the current OpenGl context.
     */
    void dispose(GL2 gl) {
        if ((glName != null) && gl.glIsList(glName)) {
            gl.glDeleteLists(glName, 1);
            glName = null;
            upToDate = false;
        }
    }
}
