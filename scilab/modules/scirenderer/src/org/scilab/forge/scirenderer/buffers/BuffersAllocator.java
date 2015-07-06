/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 20015 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.buffers;

import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

/**
 * Interface for a buffers manager.
 * @author Calixte DENIZET
 */
public class BuffersAllocator {

    public BuffersAllocator() {
    }

    public IntBuffer allocateIntBuffer(int length) {
        return IntBuffer.allocate(length);
    }

    public ByteBuffer allocateByteBuffer(int length) {
        return ByteBuffer.allocate(length);
    }

    public FloatBuffer allocateFloatBuffer(int length) {
        return FloatBuffer.allocate(length);
    }

    public void release(IntBuffer ib) {
    }

    public void release(ByteBuffer ib) {
    }

    public void release(FloatBuffer ib) {
    }
}
