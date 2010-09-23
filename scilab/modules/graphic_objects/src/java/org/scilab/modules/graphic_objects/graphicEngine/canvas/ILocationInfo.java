/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.canvas;

import org.scilab.modules.graphic_objects.graphicEngine.drawer.IDrawable;
import org.scilab.modules.graphic_objects.graphicEngine.maths.ISegment3f;
import org.scilab.modules.graphic_objects.graphicEngine.renderingZone.IRenderingZoneController;
import org.scilab.modules.graphic_objects.graphicEngine.maths.IVector3f;

/**
 * Author: Lando Pierre
 * Date: 1 mai 2010
 *
 * The location info contains information about the a screen position and the world segment under it.
 */
public interface ILocationInfo {

    /**
     * @return the drawable under this location if exists, null otherwise.
     */
    IDrawable getDrawable();

    /**
     * @return the rendering zone relative to this location.
     */
    IRenderingZoneController getRenderingZone();

    /**
     * @return the window position of the fragment under this location if exists, null otherwise.
     * The returned vector is in the [0, 1]x[0, 1]x[0, 1] box.
     */
    IVector3f getNormalizedWindowPosition();

    /**
     * @return the window position of the fragment under this location if exists, null otherwise.
     * The returned vector is in the [0, w]x[0, h]x[0, 1] box.
     */
    IVector3f getWindowPosition();

    /**
     * @return the world position of the fragment under this location if exists, null otherwise.
     */
    IVector3f getWorldPosition();

    /**
     * @return the segment of underlying point in world coordinate.
     */
    ISegment3f getWorldSegment();

    /**
     * Force evaluation.
     */
    void forceEvaluation(); 
}
