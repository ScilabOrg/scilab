/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.gl.glCore;

import org.scilab.modules.graphic_objects.graphicEngine.core.RenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.IGLDrawer;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.IGLDrawersManager;

import javax.media.opengl.GL;

/**
 * Author: Lando Pierre
 * Date: 20 mai 2010
 *
 * Basic implementation of IGLRenderingInfo.
 */
public class GLRenderingInfo extends RenderingInfo<IGLDrawer> implements IGLRenderingInfo {
    private GL gl = null;

    public GLRenderingInfo(IGLDrawersManager drawersManager) {
        super(drawersManager);
    }

    
    public GL getGL() {
        return gl;
    }

    
    public void setGL(GL gl) {
        this.gl = gl;
    }
}
