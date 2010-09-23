package org.scilab.modules.graphic_objects.graphicEngine.gl.glUtil;

import org.scilab.modules.graphic_objects.graphicEngine.maths.IVector3f;
import org.scilab.modules.graphic_objects.graphicEngine.maths.UnitaryQuaternion;

import javax.media.opengl.GL;

/**
 * Author: Lando Pierre
 * Date: june 17, 2010
 */
public class GLUtil {
    public static void translate(GL gl, IVector3f t) {
        gl.glMatrixMode(GL.GL_MODELVIEW);
        gl.glTranslatef(t.getX(), t.getY(), t.getZ());
    }

    public static void unRotate(GL gl, UnitaryQuaternion orientation) {
        gl.glMatrixMode(GL.GL_MODELVIEW);
        gl.glMultMatrixf(orientation.getUnRotateMatrix(), 0);
    }
}
