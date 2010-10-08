package org.scilab.modules.graphic_objects.JoGLView.sciCanvas;

import org.scilab.forge.scirenderer.canvas.interfaces.canvas.ICanvas;

/**
 * @author Pierre Lando
 */
public interface IScilabCanvas extends ICanvas {

    @Override
    ISciBuffersManager getBuffersManager();
}
