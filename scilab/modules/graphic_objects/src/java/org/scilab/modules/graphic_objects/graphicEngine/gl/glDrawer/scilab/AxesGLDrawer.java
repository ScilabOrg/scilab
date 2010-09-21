package org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.scilab;

import org.scilab.modules.graphic_objects.JoGLView.scilab.ISciObjModel;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axes.Camera;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.IGLRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.IGLDrawer;

import javax.media.opengl.GL;

/**
 * Author: lando
 * Date: 22 sept. 2010
 */
public class AxesGLDrawer implements IGLDrawer<ISciObjModel> {

    /*
     * Singleton of this class.
     */
    private static final AxesGLDrawer singleton = new AxesGLDrawer();

    /**
     * Make constructor private for singleton.
     */
    private AxesGLDrawer() {
    }

    public static AxesGLDrawer getDrawer() {
        return singleton;
    }

    public String getType() {
        // TODO : Axes.getType() en static.
        return "Axes";
    }

    public void beginDraw(ISciObjModel shape, IGLRenderingInfo renderingInfo) {
        GL gl = renderingInfo.getGL();
        gl.glMatrixMode(GL.GL_MODELVIEW);
        gl.glPushMatrix();
        
        Object object = GraphicController.getController().getObjectFromId(shape.getId());
        if (object instanceof Axes) {
            Axes axes = (Axes) object;
            drawCamera(gl, axes.getCamera());
        }
    }

    public void endDraw(ISciObjModel shape, IGLRenderingInfo renderingInfo) {
        GL gl = renderingInfo.getGL();
        gl.glMatrixMode(GL.GL_MODELVIEW);
        gl.glPopMatrix();
    }

    private void drawCamera(GL gl, Camera camera) {
        Double[] ra = camera.getRotationAngles();

        gl.glRotated(ra[0], 1, 0, 0);
        gl.glRotated(ra[1], 0, 0, 1);

        if (camera.getIsoview()) {

        }

        if (camera.getCubeScaling()) {
            
        }
    }
}
