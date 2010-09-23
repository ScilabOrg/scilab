/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.gl.synchronization;

import javax.media.opengl.DebugGL;
import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLEventListener;

/**
 * Author: Lando Pierre
 * Date: june 7, 2010
 */
public abstract class GLSynchronizedAction implements GLEventListener {

    public GLSynchronizedAction() {
        GLSynchronization.register(this);
    }

    public abstract void perform(GL gl);


    
    public void display(GLAutoDrawable glAutoDrawable) {
        perform(new DebugGL(glAutoDrawable.getGL()));
        GLSynchronization.unRegister(this);
        notify();
    }


     public void init(GLAutoDrawable glAutoDrawable) {}
     public void reshape(GLAutoDrawable glAutoDrawable, int i, int i1, int i2, int i3) {}
     public void displayChanged(GLAutoDrawable glAutoDrawable, boolean b, boolean b1) {}

    synchronized public void waitEnd() {
        try {
            wait();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
