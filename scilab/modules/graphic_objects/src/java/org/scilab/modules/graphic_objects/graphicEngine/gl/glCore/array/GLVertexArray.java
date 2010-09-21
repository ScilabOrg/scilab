/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.array;

import com.sun.opengl.util.BufferUtil;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.IVertexArray;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.UnBindableException;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.IGLRenderingInfo;

import javax.media.opengl.GL;
import java.nio.FloatBuffer;

/**
 * Author: Lando Pierre
 * Date: june 7, 2010
 *
 * OpenGl implementation of IVertexArray
 */
public class GLVertexArray implements IVertexArray<IGLRenderingInfo> {
    private FloatBuffer data = null;
    private Integer id = null;
    private int size = 0;

    /**
     * Default constructor.
     */
    public GLVertexArray() {
    }

    /**
     * Constructor with content in parameters.
     * @param floatBuffer this vertex array content.
     */
    public GLVertexArray(FloatBuffer floatBuffer) {
        setData(floatBuffer);
    }

    public GLVertexArray(float[] floatArray) {
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

            gl.glEnable(GL.GL_VERTEX_ARRAY);
            gl.glBindBuffer(GL.GL_ARRAY_BUFFER, id);
            gl.glVertexPointer(4, GL.GL_FLOAT, 0, 0);
        } catch (Exception e) {
            throw new UnBindableException();
        }
    }

    
    public int size() {
        return size;
    }
}
