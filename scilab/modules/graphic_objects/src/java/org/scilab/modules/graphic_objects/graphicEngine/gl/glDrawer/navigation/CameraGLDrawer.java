package org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.navigation;

import org.scilab.modules.graphic_objects.graphicEngine.core.IRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.navigation.ICameraModel;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.IGLRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.IGLDrawer;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glUtil.GLUtil;

import javax.media.opengl.GL;

/**
 * Author: Lando Pierre
 * Date: june 17, 2010
 *
 * OpenGl drawer of ICameraModel
 */
public class CameraGLDrawer implements IGLDrawer<ICameraModel> {
    private static final CameraGLDrawer singleton = new CameraGLDrawer();

    /**
     * Private constructor for singleton.
     */
    private CameraGLDrawer() {}

    public static CameraGLDrawer getDrawer() {
        return singleton;
    }

    
    public String getType() {
        return ICameraModel.type;
    }

    
    public void beginDraw(ICameraModel camera, IGLRenderingInfo renderingInfo) {
        GL gl = renderingInfo.getGL();

        gl.glPushAttrib(GL.GL_LIGHTING_BIT | GL.GL_POLYGON_BIT);
        gl.glDisable(GL.GL_LIGHTING);
		gl.glPolygonMode(GL.GL_FRONT, GL.GL_LINE);

		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glPushMatrix();

        GLUtil.translate(gl, camera.getPosition());
        GLUtil.unRotate(gl, camera.getOrientation());

		double dy = .4;
		double dx = dy * camera.getAspectRatio();
		gl.glLineWidth(1.0f);

        // TODO : find good user color & remove color here.
        if (renderingInfo.getRenderingMode() == IRenderingInfo.RenderingMode.NORMAL_MODE) {
            gl.glColor3f(.8f, .7f, .6f);
        }

		gl.glBegin(GL.GL_TRIANGLE_FAN);
			gl.glVertex3d(0, 0, 1);
			gl.glVertex3d(+dx, +dy, 0);
			gl.glVertex3d(+dx, -dy, 0);
			gl.glVertex3d(-dx, -dy, 0);
			gl.glVertex3d(-dx, +dy, 0);
			gl.glVertex3d(+dx, +dy, 0);
		gl.glEnd();

		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glPopMatrix();
        
        gl.glPopAttrib();
    }

     public void endDraw(ICameraModel shape, IGLRenderingInfo renderingInfo) {}
}
