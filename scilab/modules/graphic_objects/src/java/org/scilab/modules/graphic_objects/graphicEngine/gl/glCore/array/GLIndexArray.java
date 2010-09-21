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
import org.scilab.modules.graphic_objects.graphicEngine.core.array.IIndexArray;
import org.scilab.modules.graphic_objects.graphicEngine.core.array.UnBindableException;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.IGLRenderingInfo;

import javax.media.opengl.GL;
import java.nio.IntBuffer;

/**
 * Author: Lando Pierre
 * Date: june 7, 2010
 *
 * OpenGl implementation of IIndexArray
 */
public class GLIndexArray implements IIndexArray<IGLRenderingInfo> {
    private IntBuffer data = null;
    private Integer id = null;
    private int size = 0;

    /**
     * Default constructor.
     */
    public GLIndexArray() {
    }

    /**
     * Constructor with content in parameters.
     * @param intBuffer this index array content.
     */
    public GLIndexArray(IntBuffer intBuffer) {
        setData(intBuffer);
    }

    public GLIndexArray(int[] intArray) {
        this(BufferUtil.newIntBuffer(intArray.length).put(intArray));
    }

    
    public void setData(IntBuffer data) {
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
                gl.glBindBuffer(GL.GL_ELEMENT_ARRAY_BUFFER, id);
                gl.glBufferData(GL.GL_ELEMENT_ARRAY_BUFFER, size*BufferUtil.SIZEOF_INT, data, GL.GL_STATIC_DRAW);
                data = null;
            }

            gl.glBindBuffer(GL.GL_ELEMENT_ARRAY_BUFFER, id);
        } catch (Exception e) {
            throw new UnBindableException();
        }
    }

    
    public int size() {
        return size;
    }
}