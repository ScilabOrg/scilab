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

import javax.media.opengl.GLCanvas;
import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLContext;

/**
 * Author: Lando Pierre
 * Date: june 7, 2010
 */
public class GLSynchronization {
	private static GLCapabilities caps = new GLCapabilities();
	private static GLCanvas masterCanvas;
	static {
        // TODO : enable / disable anti aliasing
	    caps.setSampleBuffers(true);
		caps.setNumSamples(4);
		masterCanvas = new GLCanvas(caps);
	}

    public static GLContext getContext() {
        return masterCanvas.getContext();
    }

    public static GLCapabilities getCapabilities() {
        return caps;
    }

    public static void register(GLSynchronizedAction action) {
        masterCanvas.addGLEventListener(action);
        masterCanvas.display();
    }

    public static void unRegister(GLSynchronizedAction action) {
        masterCanvas.removeGLEventListener(action);
    }
}