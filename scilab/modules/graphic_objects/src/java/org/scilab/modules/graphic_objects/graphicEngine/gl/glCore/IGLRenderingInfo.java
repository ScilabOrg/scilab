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

import org.scilab.modules.graphic_objects.graphicEngine.core.IRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.IGLDrawer;

import javax.media.opengl.GL;

/**
 * Author: Lando Pierre
 * Date: 19 mai 2010
 *
 * The IGLRenderingInfo extends the IRenderingInfo and add gl interface as an information.
 */
public interface IGLRenderingInfo extends IRenderingInfo<IGLDrawer> {

    /**
     * @return OpenGl interface.
     */
    GL getGL();

    /**
     * @param gl will be the new OpenGl interface.
     */
    void setGL(GL gl);
}
