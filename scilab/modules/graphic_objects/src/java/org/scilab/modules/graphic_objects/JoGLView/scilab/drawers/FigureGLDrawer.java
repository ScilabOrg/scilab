package org.scilab.modules.graphic_objects.JoGLView.scilab.drawers;

import org.scilab.forge.scirenderer.core.scenedrawer.IDrawerVisitor;
import org.scilab.forge.scirenderer.impl.opengl.core.drawersmanager.drawer.IGLDrawer;
import org.scilab.forge.scirenderer.impl.opengl.core.renderinginfo.IGLRenderingInfo;
import org.scilab.modules.graphic_objects.JoGLView.scilab.ScilabDrawableBridge;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;

import javax.media.opengl.GL;

/**
 * @author Pierre Lando
 */
public class FigureGLDrawer implements IGLDrawer<ScilabDrawableBridge> {

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
        int background = figure.getBackground();
        float[] clearColor = figure.getColorMap().getScilabColor(background);

        gl.glClearColor(clearColor[0], clearColor[1], clearColor[2], 1f);

        gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);

    }

    @Override
    public void endDraw(IDrawerVisitor<IGLRenderingInfo> visitor, ScilabDrawableBridge scilabDrawableBridge) {

    }
}
