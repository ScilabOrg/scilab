package org.scilab.modules.renderer.JoGLView.glcanvas;

import com.sun.opengl.util.BufferUtil;
import org.scilab.forge.scirenderer.canvas.glimpl.canvas.IGLCanvas;
import org.scilab.forge.scirenderer.canvas.interfaces.geometry.buffers.IVertexBuffer;
import org.scilab.modules.graphic_objects.NativeGL;
import org.scilab.modules.graphic_objects.polyline.Polyline;
import org.scilab.modules.renderer.JoGLView.sciCanvas.ISciBuffersManager;

import javax.media.opengl.GL;
import java.nio.DoubleBuffer;

/**
 * @author Pierre Lando
 */
public class ScilabGLBuffersManager implements ISciBuffersManager {
    private final IGLCanvas canvas;
    private int maxIndex = -1;

    public ScilabGLBuffersManager(IGLCanvas canvas) {
        this.canvas = canvas;
    }

    @Override
    public IVertexBuffer getVertexBuffer(final Polyline polyline) {

        return new IVertexBuffer() {
            DoubleBuffer data = BufferUtil.newDoubleBuffer(1000);
            @Override
            public DoubleBuffer getData4d() {
                NativeGL.loadGLData(data, polyline.getIdentifier());
                return data;
            }

            @Override
            public int getVertexNumber() {
                return 10;
            }
        };
    }

    @Override
    public void setCurrentVertexBuffer(IVertexBuffer vertexBuffer) {
        GL gl = canvas.getGL();
        gl.glBindBuffer(GL.GL_ARRAY_BUFFER, 0);
        gl.glEnable(GL.GL_VERTEX_ARRAY);

        DoubleBuffer buffer = vertexBuffer.getData4d();
        if (!buffer.isDirect()) {
            buffer = BufferUtil.copyDoubleBuffer(buffer);
            System.out.println("Warning : not a direct buffer.");
        }
        gl.glVertexPointer(4, GL.GL_DOUBLE, 0, buffer);

        if (maxIndex == -1) {
            maxIndex = vertexBuffer.getVertexNumber();
        } else {
            maxIndex = Math.min(maxIndex, vertexBuffer.getVertexNumber());
        }
    }

    @Override
    public void unbindAllBuffer() {
        GL gl = canvas.getGL();

        gl.glDisable(GL.GL_VERTEX_ARRAY);
        gl.glBindBuffer(GL.GL_ARRAY_BUFFER, 0);
        maxIndex = -1;

        /*
        gl.glBindBuffer(GL.GL_ELEMENT_ARRAY_BUFFER, 0);
        gl.glDisable(GL.GL_COLOR_ARRAY);
        gl.glDisable(GL.GL_INDEX_ARRAY);
        */
    }

    @Override
    public int getAvailableIndexes() {
        return maxIndex;
    }
}
