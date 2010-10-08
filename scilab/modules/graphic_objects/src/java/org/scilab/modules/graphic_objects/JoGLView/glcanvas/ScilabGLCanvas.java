package org.scilab.modules.graphic_objects.JoGLView.glcanvas;

import org.scilab.forge.scirenderer.canvas.glimpl.canvas.AbstractGLCanvas;
import org.scilab.modules.graphic_objects.JoGLView.sciCanvas.IScilabCanvas;

import javax.media.opengl.GLAutoDrawable;

/**
 * @author Pierre Lando
 */
public abstract class ScilabGLCanvas extends AbstractGLCanvas implements IScilabCanvas {
    
    public ScilabGLCanvas(GLAutoDrawable autoDrawable) {
        super(autoDrawable);
    }

    @Override
    public ScilabGLBuffersManager getBuffersManager() {
        return ScilabGLBuffersManager.getSingleton();
    }
}
