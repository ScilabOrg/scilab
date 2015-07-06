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

package org.scilab.modules.renderer.JoGLView.util;

import org.scilab.modules.graphic_objects.DataLoader;
import org.scilab.forge.scirenderer.buffers.BuffersAllocator;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

/**
 * Interface for a buffers manager.
 * @author Calixte DENIZET
 */
public class ScilabBuffersAllocator extends BuffersAllocator {

    public ScilabBuffersAllocator() {
    }

    public IntBuffer allocateIntBuffer(int length) {
        ByteBuffer buffer = DataLoader.getDirectBuffer(length * Integer.SIZE / Byte.SIZE);
        buffer.order(ByteOrder.nativeOrder());
        return buffer.asIntBuffer();
    }

    public ByteBuffer allocateByteBuffer(int length) {
        ByteBuffer buffer = DataLoader.getDirectBuffer(length);
        buffer.order(ByteOrder.nativeOrder());
        return buffer;
    }

    public FloatBuffer allocateFloatBuffer(int length) {
        ByteBuffer buffer = DataLoader.getDirectBuffer(length * Float.SIZE / Byte.SIZE);
        buffer.order(ByteOrder.nativeOrder());
        return buffer.asFloatBuffer();
    }

    public void release(IntBuffer ib) {
        DataLoader.releaseDirectIntBuffer(ib);
    }

    public void release(ByteBuffer bb) {
        DataLoader.releaseDirectByteBuffer(bb);
    }

    public void release(FloatBuffer fb) {
        DataLoader.releaseDirectFloatBuffer(fb);
    }
}
