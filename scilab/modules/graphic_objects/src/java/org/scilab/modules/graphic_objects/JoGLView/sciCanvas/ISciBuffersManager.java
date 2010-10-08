package org.scilab.modules.graphic_objects.JoGLView.sciCanvas;

import org.scilab.forge.scirenderer.canvas.interfaces.buffers.IBuffersManager;
import org.scilab.modules.graphic_objects.polyline.Polyline;

/**
 * @author Pierre Lando
 */
public interface ISciBuffersManager extends IBuffersManager {

    /**
     * This method set the current vertex buffer from the given scilab polyline.
     * @param polyline the given polyline.
     */
    void setVertexBuffer(Polyline polyline);
}
