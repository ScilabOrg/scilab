/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.util;

import com.sun.opengl.util.BufferUtil;

import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

/**
 * @author Pierre Lando
 */
public class BufferAllocation {

    public static ByteBuffer newByteBuffer(int length) throws OutOfMemory {
        try {
            ByteBuffer buffer = BufferUtil.newByteBuffer(length);
            if (buffer.limit() != length) {
                throw new OutOfMemory();
            }
            return buffer;
        } catch (OutOfMemoryError error) {
            throw new OutOfMemory();
        }
    }

    public static IntBuffer newIntBuffer(int length) throws OutOfMemory {
        try {
            IntBuffer buffer = BufferUtil.newIntBuffer(length);
            if (buffer.limit() != length) {
                throw new OutOfMemory();
            }
            return buffer;
        } catch (OutOfMemoryError error) {
            throw new OutOfMemory();
        }
    }

    public static FloatBuffer newFloatBuffer(int length) throws OutOfMemory {
        try {
            FloatBuffer buffer = BufferUtil.newFloatBuffer(length);
            if (buffer.limit() != length) {
                throw new OutOfMemory();
            }
            return buffer;
        } catch (OutOfMemoryError error) {
            throw new OutOfMemory();
        }
    }
}
