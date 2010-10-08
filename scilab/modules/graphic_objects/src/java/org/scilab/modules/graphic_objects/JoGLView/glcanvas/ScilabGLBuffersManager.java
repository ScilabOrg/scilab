package org.scilab.modules.graphic_objects.JoGLView.glcanvas;

import org.scilab.modules.graphic_objects.JoGLView.nativegl.NativeGL;
import org.scilab.modules.graphic_objects.JoGLView.sciCanvas.ISciBuffersManager;
import org.scilab.modules.graphic_objects.polyline.Polyline;

/**
 * @author Pierre Lando
 */
public class ScilabGLBuffersManager implements ISciBuffersManager {
    private final static ScilabGLBuffersManager SINGLETON = new ScilabGLBuffersManager();

    private ScilabGLBuffersManager() {
    }

    public static ScilabGLBuffersManager getSingleton() {
        return SINGLETON;
    }

    @Override
    public void setVertexBuffer(Polyline polyline) {
        NativeGL.loadGLData(polyline.getIdentifier());
    }
}
