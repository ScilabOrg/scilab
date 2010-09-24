package org.scilab.modules.graphic_objects.JoGLView.renderer;

import org.scilab.forge.scirenderer.impl.opengl.core.drawersmanager.IGLDrawersManager;
import org.scilab.forge.scirenderer.impl.opengl.core.scenedrawer.GLSceneDrawer;
import org.scilab.forge.scirenderer.impl.opengl.renderer.simple.SimpleGLRenderer;
import org.scilab.modules.graphic_objects.JoGLView.scilab.drawers.AxesGLDrawer;
import org.scilab.modules.graphic_objects.JoGLView.scilab.drawers.FigureGLDrawer;

import javax.media.opengl.GLAutoDrawable;

/**
 * @author Pierre Lando
 */
public class RendererFactory {
    private static boolean init = false;

    private static void init() {
        if (init) {
            return;
        }

        init = true;

        IGLDrawersManager drawerManager = GLSceneDrawer.getSingleton().getDrawersManager();
        drawerManager.registerDrawer(new FigureGLDrawer());
        drawerManager.registerDrawer(new AxesGLDrawer());
    }

    static public SimpleGLRenderer getRenderer(GLAutoDrawable autoDrawable) {
        init();
        return new SimpleGLRenderer(autoDrawable);
    }
}
