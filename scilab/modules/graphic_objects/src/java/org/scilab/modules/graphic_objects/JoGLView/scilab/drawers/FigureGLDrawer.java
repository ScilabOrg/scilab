package org.scilab.modules.graphic_objects.JoGLView.scilab.drawers;

import org.scilab.forge.scirenderer.core.scenedrawer.IDrawerVisitor;
import org.scilab.forge.scirenderer.impl.opengl.core.drawersmanager.drawer.IGLDrawer;
import org.scilab.forge.scirenderer.impl.opengl.core.renderinginfo.IGLRenderingInfo;
import org.scilab.modules.graphic_objects.JoGLView.scilab.ScilabDrawableBridge;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;

import javax.media.opengl.GL;

/**
 * @author Pierre Lando
 */
public class FigureGLDrawer extends AbstractScilabDrawer implements IGLDrawer<ScilabDrawableBridge> {

	/** Mapping between scilab pixel drawing mode and OpenGL ones */
	private static final int[] LOGICAL_OPS = {
        GL.GL_CLEAR, GL.GL_AND, GL.GL_AND_REVERSE, GL.GL_COPY,
        GL.GL_AND_INVERTED, GL.GL_NOOP, GL.GL_XOR, GL.GL_OR,
        GL.GL_NOR, GL.GL_EQUIV, GL.GL_INVERT, GL.GL_OR_REVERSE,
        GL.GL_COPY_INVERTED, GL.GL_OR_INVERTED, GL.GL_NAND, GL.GL_SET
    };

    @Override
    public String getType() {
        // TODO use Figure.type.
        return "Figure";
    }

    @Override
    public void beginDraw(IDrawerVisitor<IGLRenderingInfo> visitor, ScilabDrawableBridge scilabDrawableBridge) {
        Object object = GraphicController.getController().getObjectFromId(scilabDrawableBridge.getId());
        if (object instanceof Figure) {
            Figure figure = (Figure) object;
            beginDraw(visitor, figure);
        }
    }

    /**
     * Real 'beginDraw' method.
     * @param visitor the drawing visitor.
     * @param figure the figure to render.
     */
    private void beginDraw(IDrawerVisitor<IGLRenderingInfo> visitor, Figure figure) {
        GL gl = visitor.getRenderingInfo().getGL();

        /**
         * Set some OpenGL status. 
         */
		gl.glHint(GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_NICEST);
		gl.glDisable(GL.GL_LINE_SMOOTH);
		gl.glShadeModel(GL.GL_SMOOTH);
		gl.glEnable(GL.GL_DEPTH_TEST);
		gl.glDepthFunc(GL.GL_LEQUAL);

        setColorMap(visitor, figure.getColorMap());

        int background = figure.getBackground();
        float[] clearColor = getColorMap(visitor).getScilabColor(background);

		gl.glClearDepth(1.0f);
        gl.glClearColor(clearColor[0], clearColor[1], clearColor[2], 1f);
        
        gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
    }

    @Override
    public void endDraw(IDrawerVisitor<IGLRenderingInfo> visitor, ScilabDrawableBridge scilabDrawableBridge) {

    }
}
