package org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.Geometry;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.rectangle.IRectangleModel;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.IGLRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.IGLDrawer;

import javax.media.opengl.GL;

/**
 * Author: Lando Pierre
 * Date: june 25, 2010
 *
 * OpenGl drawer of IRectangleModel
 */
public class RectangleGLDrawer implements IGLDrawer<IRectangleModel> {
    /**
     * Singleton of this class.
     */
    private static final RectangleGLDrawer singleton = new RectangleGLDrawer();

    /**
     * Make constructor private for singleton.
     */
    private RectangleGLDrawer() {
    }

    public static RectangleGLDrawer getDrawer() {
        return singleton;
    }

    
    public String getType() {
        return IRectangleModel.type;
    }

    
    public void beginDraw(IRectangleModel shape, IGLRenderingInfo renderingInfo) {
        GL gl = renderingInfo.getGL();
        float w = shape.getWidth() / 2f;
        float h = shape.getHeight() / 2f;

        gl.glMatrixMode(GL.GL_MODELVIEW);
        gl.glPushMatrix();

        switch (shape.getOrigin()) {
            case LEFT:
                gl.glTranslatef(+w, 0, 0);
                break;
            case RIGHT:
                gl.glTranslatef(-w, 0, 0);
                break;
            case BOTTOM:
                gl.glTranslatef(0, +h, 0);
                break;
            case TOP:
                gl.glTranslatef(0, -h, 0);
                break;
            case CENTER:
                break;
            case BOTTOM_LEFT:
                gl.glTranslatef(+w, +h, 0);
                break;
            case BOTTOM_RIGHT:
                gl.glTranslatef(-w, +h, 0);
                break;
            case TOP_LEFT:
                gl.glTranslatef(+w, -h, 0);
                break;
            case TOP_RIGHT:
                gl.glTranslatef(-w, -h, 0);
                break;
        }

        gl.glNormal3f(0, 0, 1f);
        gl.glBegin(GL.GL_QUADS);
            gl.glVertex2f(-w, -h);
            gl.glVertex2f(+w, -h);
            gl.glVertex2f(+w, +h);
            gl.glVertex2f(-w, +h);
        gl.glEnd();

        gl.glPopMatrix();
    }

     public void endDraw(IRectangleModel shape, IGLRenderingInfo renderingInfo) {}
}
