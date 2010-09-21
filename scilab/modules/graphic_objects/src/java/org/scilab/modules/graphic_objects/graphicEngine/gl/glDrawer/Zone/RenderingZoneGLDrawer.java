/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.Zone;

import org.scilab.modules.graphic_objects.graphicEngine.core.IRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.IGLDrawer;
import org.scilab.modules.graphic_objects.graphicEngine.renderingZone.IRenderingZoneController;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.IGLRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.maths.MathUtil;
import org.scilab.modules.graphic_objects.graphicEngine.renderingZone.IRenderingZoneModel;

import javax.media.opengl.GL;

/**
 * Author: Lando Pierre
 * Date: 20 mai 2010
 * // TODO : why not a IRenderingZoneModel
 * OpenGl drawer of IRenderingZoneController
 */
public class RenderingZoneGLDrawer implements IGLDrawer<IRenderingZoneController> {
    /**
     * Singleton of this class.
     */
    private static final RenderingZoneGLDrawer singleton = new RenderingZoneGLDrawer();

    /**
     * Make constructor private for singleton.
     */
    private RenderingZoneGLDrawer() {
    }

    public static RenderingZoneGLDrawer getDrawer() {
        return singleton;
    }

    
    public String getType() {
        return IRenderingZoneModel.type;
    }

    
    public void beginDraw(IRenderingZoneController renderingZone, IGLRenderingInfo renderingInfo) {
        GL gl = renderingInfo.getGL();

        if (renderingInfo.getRenderingMode() == IRenderingInfo.RenderingMode.NORMAL_MODE) {
            gl.glClearColor(1f, 1f, 1f, 1f);
        }

        gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
        gl.glEnable(GL.GL_DEPTH_TEST);
        gl.glViewport(0, 0, renderingInfo.getFrameWidth(), renderingInfo.getFrameHeight());

        gl.glMatrixMode(GL.GL_MODELVIEW);
        gl.glPushMatrix();
        gl.glLoadIdentity();

        gl.glMatrixMode(GL.GL_PROJECTION);
        gl.glPushMatrix();

        renderingZone.getLayoutPolicy().setRenderingInfo(renderingInfo);

        float[] projectionMatrix = MathUtil.getProjectionMatrix(renderingZone);
        if (projectionMatrix != null) {
            gl.glLoadMatrixf(projectionMatrix, 0);
        }

        if (renderingInfo.getRenderingMode() == IRenderingInfo.RenderingMode.NORMAL_MODE) {
            gl.glEnable(GL.GL_LIGHTING);
            gl.glEnable(GL.GL_LIGHT0);

            // Create light components
            float ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
            float diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
            float position[] = new float[] {
                renderingZone.getCamera().getPosition().getX(),
                renderingZone.getCamera().getPosition().getY(),
                renderingZone.getCamera().getPosition().getZ(),
                1f
            };

            // Assign created components to GL_LIGHT0
            gl.glLightfv(GL.GL_LIGHT0, GL.GL_AMBIENT, ambientLight, 0);
            gl.glLightfv(GL.GL_LIGHT0, GL.GL_DIFFUSE, diffuseLight, 0);
            gl.glLightfv(GL.GL_LIGHT0, GL.GL_POSITION, position, 0);

            gl.glLightModeli(GL.GL_LIGHT_MODEL_TWO_SIDE, GL.GL_TRUE);

            // enable color tracking
            gl.glEnable(GL.GL_COLOR_MATERIAL);
            // set material properties which will be assigned by glColor
            gl.glColorMaterial(GL.GL_FRONT_AND_BACK, GL.GL_AMBIENT_AND_DIFFUSE);
        } else {
            gl.glDisable(GL.GL_LIGHTING);
        }
    }

    
    public void endDraw(IRenderingZoneController renderingZone, IGLRenderingInfo renderingInfo) {
        GL gl = renderingInfo.getGL();

        if (renderingInfo.getRenderingMode() == IRenderingInfo.RenderingMode.NORMAL_MODE) {
            gl.glDisable(GL.GL_LIGHTING);
            drawAspectRatio(renderingZone, renderingInfo);
        }

        gl.glMatrixMode(GL.GL_PROJECTION);
        gl.glPopMatrix();

        gl.glMatrixMode(GL.GL_MODELVIEW);
        gl.glPopMatrix();
    }

    /**
     * Draw the camera aspect ratio.
     * If the rendering zone aspect ratio differs from the camera aspect ratio, the extra part of the rendering zone is
     * grey tone.
     *
     * If the camera aspect ratio is null or negative, nothing is drawn.
     *
     * @param renderingZone the rendering zone.
     * @param renderingInfo the rendering info.
     */
    private void drawAspectRatio(IRenderingZoneController renderingZone, IGLRenderingInfo renderingInfo) {
        if (renderingZone != null && renderingZone.getCamera() != null) {
            float realAspectRation = renderingZone.getLayoutPolicy().getAspectRatio();
            float preferredAspectRatio = renderingZone.getCamera().getAspectRatio();

            if (preferredAspectRatio > 0) {
                GL gl = renderingInfo.getGL();
                gl.glPushAttrib(GL.GL_DEPTH_BUFFER_BIT | GL.GL_COLOR_BUFFER_BIT | GL.GL_POLYGON_BIT);
                
                gl.glDisable(GL.GL_DEPTH_TEST);
                gl.glEnable(GL.GL_BLEND);
                gl.glBlendFunc(GL.GL_SRC_ALPHA,GL.GL_ONE_MINUS_SRC_ALPHA);
                gl.glPolygonMode(GL.GL_FRONT_AND_BACK, GL.GL_FILL);

                gl.glMatrixMode(GL.GL_PROJECTION);
                gl.glLoadIdentity();
                gl.glMatrixMode(GL.GL_MODELVIEW);
                gl.glLoadIdentity();


                gl.glColor4d(.7, .7, .7, .3);

                if(realAspectRation < preferredAspectRatio) {
                    gl.glRectd(-1,  realAspectRation/ preferredAspectRatio, 1, 1);
                    gl.glRectd(-1, -realAspectRation/ preferredAspectRatio, 1, -1);
                } else {
                    gl.glRectd( preferredAspectRatio /realAspectRation,-1,  1, 1);
                    gl.glRectd(-preferredAspectRatio /realAspectRation,-1, -1, 1);
                }

                gl.glPopAttrib();
            }
        } else {
            System.out.println("Warning: renderingZone == null or renderingZone.getCamera() == null");
        }
    }
}
