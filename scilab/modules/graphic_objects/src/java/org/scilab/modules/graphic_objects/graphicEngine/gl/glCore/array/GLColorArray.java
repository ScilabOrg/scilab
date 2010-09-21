package org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.array;

import com.sun.opengl.util.BufferUtil;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.IColorArray;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.UnBindableException;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.IGLRenderingInfo;

import javax.media.opengl.GL;
import java.nio.FloatBuffer;

/**
 * Author: Lando Pierre
 * Date: june 21, 2010
 *
 * OpenGl implementation of IColorArray
 */
public class GLColorArray implements IColorArray<IGLRenderingInfo> {
    private FloatBuffer data = null;
    private Integer id = null;
    private int size = 0;

    /**
     * Default constructor.
     */
    public GLColorArray() {
    }

    /**
     * Constructor with content in parameters.
     * @param floatBuffer this vertex array content.
     */
    public GLColorArray(FloatBuffer floatBuffer) {
        setData(floatBuffer);
    }

    public GLColorArray(float[] floatArray) {
        this(BufferUtil.newFloatBuffer(floatArray.length).put(floatArray));
    }

    
    public void setData(FloatBuffer data) {
        this.data = data;
    }

    
    public void bind(IGLRenderingInfo renderingInfo) throws UnBindableException {
        try {
            GL gl = renderingInfo.getGL();

            if (id == null) {
                int[] t = new int[1];
                gl.glGenBuffers(1, t, 0);
                id = t[0];
            }

            if (!gl.glIsBuffer(id)) {
                throw new UnBindableException();
            }

            if (data != null) {
                data.rewind();
                size = data.limit();
                gl.glBindBuffer(GL.GL_ARRAY_BUFFER, id);
                gl.glBufferData(GL.GL_ARRAY_BUFFER, size*BufferUtil.SIZEOF_FLOAT, data, GL.GL_STATIC_DRAW);
                data = null;
            }

            if (size != 0) {
                gl.glEnable(GL.GL_COLOR_ARRAY);
                gl.glBindBuffer(GL.GL_ARRAY_BUFFER, id);
                gl.glColorPointer(4, GL.GL_FLOAT, 0, 0);
            }
        } catch (Exception e) {
            throw new UnBindableException();
        }
    }

    
    public int getSize() {
        return size;
    }
}
