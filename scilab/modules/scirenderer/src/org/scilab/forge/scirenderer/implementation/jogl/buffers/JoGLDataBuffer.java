/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.implementation.jogl.buffers;

import org.scilab.forge.scirenderer.buffers.DataBuffer;

import javax.media.opengl.GL;
import java.nio.Buffer;

/**
 * @author Pierre Lando
 */
public abstract class JoGLDataBuffer implements DataBuffer {

    /**
     * True if the data have been uploaded to OpenGl driver.
     */
    private boolean dataUploaded;

    /**
     * The OpenGl name for this buffer.
     *  - null : no name
     *  - an Integer : the name.
     */
    private Integer glName;

    /**
     * Default constructor.
     * The constructor is package : only {@link JoGLBuffersManager} can instantiate this object.
     */
    JoGLDataBuffer() {
        dataUploaded = false;
        glName = null;
    }

    /**
     * Called to reload a buffer.
     */
    public void reload() {
        glName = null;
        dataUploaded = false;
    }

    /**
     * Called when it's time to kill this buffer.
     * @param gl the OpenGl context.
     */
    public void dispose(GL gl) {
        if ((glName != null) && gl.glIsBuffer(glName)) {
            int[] t = new int[] {glName};
            gl.glDeleteBuffers(1, t, 0);
            glName = null;
            dataUploaded = false;
        }
    }

    /**
     * Called when it's time to kill this buffer.
     * The effective delete is done in JoGLBuffersManager::glSynchronize (to vectorize it).
     * @param gl the OpenGl context.
     */
    public int disposeWithoutDelete(GL gl) {
        if ((glName != null) && gl.glIsBuffer(glName)) {
            int n = glName;
            glName = null;
            dataUploaded = false;

            return n;
        }

        return -1;
    }

    /**
     * Synchronize this buffer.
     * @param gl the OpenGl context where synchronization is done.
     */
    protected void synchronize(GL gl) {
        // Check the glName.
        if ((glName == null) || !(gl.glIsBuffer(glName))) {
            int[] t = new int[1];
            gl.glGenBuffers(1, t, 0);
            glName = t[0];
            dataUploaded = false;
        }

        // Upload data to the OpenGl driver.
        if (!dataUploaded) {
            gl.glBindBuffer(getGLBindDestination(), glName);
            gl.glBufferData(getGLBindDestination(), getByteSize(), getByteBuffer(), GL.GL_STATIC_DRAW);
            gl.glBindBuffer(getGLBindDestination(), 0);
            dataUploaded = true;
        }
    }

    /**
     * Return the data uploaded status.
     * @return the data uploaded status.
     */
    protected boolean isDataUploaded() {
        return dataUploaded;
    }

    /**
     * Set the data uploaded status.
     * @param dataUploaded the new data uploaded status.
     */
    protected void setDataUploaded(boolean dataUploaded) {
        this.dataUploaded = dataUploaded;
    }

    /**
     * Return the OpenGl name of this buffer.
     * @param gl the OpenGl context.
     * @return the OpenGl name of this buffer.
     */
    protected Integer getGlName(GL gl) {
        synchronize(gl);
        return glName;
    }

    /**
     * Return the data as byte buffer.
     * @return the data as byte buffer.
     */
    protected abstract Buffer getByteBuffer();

    /**
     * Return the OpenGl bind destination.
     * @return the OpenGl bind destination.
     */
    protected abstract int getGLBindDestination();

    /**
     * Return the size of this buffer in byte.
     * @return the size of this buffer in byte.
     */
    public abstract int getByteSize();

    public abstract void clear();
}
