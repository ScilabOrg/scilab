/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.core.array;

import org.scilab.modules.graphic_objects.graphicEngine.core.IRenderingInfo;

import java.nio.FloatBuffer;

/**
 * Author: Lando Pierre
 * Date: june 7, 2010
 *
 * Data should be 4-aligned.
 */
public interface IVertexArray<renderingInfoType extends IRenderingInfo> {

    /**
     * @param data of this vertex array.
     */
    public void setData(FloatBuffer data);

    /**
     * Bind this vertex array has current vertex array.
     * @param renderingInfo needed to interact with graphic engine.
     * @throws UnBindableException if graphic engine refuse to bind this array.
     */
    void bind(renderingInfoType renderingInfo) throws UnBindableException;

    /**
     * @return the getSize of this index array.
     */
    int size();
}
