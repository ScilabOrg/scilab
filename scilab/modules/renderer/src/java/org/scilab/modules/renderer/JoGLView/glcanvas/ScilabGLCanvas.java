package org.scilab.modules.renderer.JoGLView.glcanvas;

import org.scilab.forge.scirenderer.canvas.glimpl.canvas.AbstractGLCanvas;
import org.scilab.modules.renderer.JoGLView.sciCanvas.ISciBuffersManager;
import org.scilab.modules.renderer.JoGLView.sciCanvas.IScilabCanvas;

import javax.media.opengl.GLAutoDrawable;

/**
 * @author Pierre Lando
 */
public abstract class ScilabGLCanvas extends AbstractGLCanvas implements IScilabCanvas {
    private final ISciBuffersManager buffersManager = new ScilabGLBuffersManager(this);
    
    public ScilabGLCanvas(GLAutoDrawable autoDrawable) {
        super(autoDrawable);
    }

    @Override
    public ISciBuffersManager getBuffersManager() {
        return buffersManager;
    }
}
