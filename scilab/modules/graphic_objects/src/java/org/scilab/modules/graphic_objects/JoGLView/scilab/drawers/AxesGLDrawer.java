package org.scilab.modules.graphic_objects.JoGLView.scilab.drawers;

import org.scilab.forge.scirenderer.core.scenedrawer.IDrawerVisitor;
import org.scilab.forge.scirenderer.impl.opengl.core.drawersmanager.drawer.IGLDrawer;
import org.scilab.forge.scirenderer.impl.opengl.core.renderinginfo.IGLRenderingInfo;
import org.scilab.modules.graphic_objects.JoGLView.scilab.ScilabDrawableBridge;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;

import javax.media.opengl.GL;

/**
 * @author Pierre Lando
 */
public class AxesGLDrawer extends AbstractScilabDrawer implements IGLDrawer<ScilabDrawableBridge> {
    @Override
    public String getType() {
        // TODO : use Axes.type
        return "Axes";
    }

    @Override
    public void beginDraw(IDrawerVisitor<IGLRenderingInfo> visitor, ScilabDrawableBridge scilabDrawableBridge) {
        GL gl = visitor.getRenderingInfo().getGL();

        /**
         * Push some attributes :
         *  - GL_FRONT_FACE 
         */
        gl.glPushAttrib(GL.GL_POLYGON_BIT);

        Object object = GraphicController.getController().getObjectFromId(scilabDrawableBridge.getId());
        if (object instanceof Axes) {
            Axes axes = (Axes) object;
            beginDraw(visitor, axes);
        }
    }

    /**
     * Real 'beginDraw' method.
     * @param visitor the drawing visitor.
     * @param axes axes to render.
     */
    private void beginDraw(IDrawerVisitor<IGLRenderingInfo> visitor, Axes axes) {
        GL gl = visitor.getRenderingInfo().getGL();
        setFrontFace(gl, axes);

        ColorMap colorMap = getColorMap(visitor);

        float[] backColor = colorMap.getScilabColor(axes.getBackground());

        gl.glColor3fv(backColor, 0);
        gl.glBegin(GL.GL_QUADS);
            gl.glVertex2i(0, 0);
            gl.glVertex2i(0, 1);
            gl.glVertex2i(1, 1);
            gl.glVertex2i(1, 0);
        gl.glEnd();
    }

    /**
     * This method define front-facing and back-facing polygons. 
     * @param gl the current OpenGL context.
     * @param axes the current axes.
     */
    private static void setFrontFace(GL gl, Axes axes) {
        /**
         * There was a global revert if the number of revert axis is odd.
         */
        if (axes.getXAxisReverse() ^ axes.getYAxisReverse() ^ axes.getZAxisReverse()) {
            gl.glFrontFace(GL.GL_CW);
		} else {
			gl.glFrontFace(GL.GL_CCW);
        }
    }

    @Override
    public void endDraw(IDrawerVisitor<IGLRenderingInfo> visitor, ScilabDrawableBridge scilabDrawableBridge) {
        GL gl = visitor.getRenderingInfo().getGL();
        gl.glPopAttrib();
    }
}
