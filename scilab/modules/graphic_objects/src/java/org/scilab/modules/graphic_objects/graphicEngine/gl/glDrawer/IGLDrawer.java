/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer;

import org.scilab.modules.graphic_objects.graphicEngine.drawer.IDrawable;
import org.scilab.modules.graphic_objects.graphicEngine.drawer.IDrawer;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.IGLRenderingInfo;

/**
 * Author: Lando Pierre
 * Date: 20 mai 2010
 */
public interface IGLDrawer <nodeType extends IDrawable> extends IDrawer<nodeType, IGLRenderingInfo> {
}
