/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.grouping.transform;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.IGraphicModel;
import org.scilab.modules.graphic_objects.graphicEngine.maths.IVector3f;

/**
 * Author: Lando Pierre.
 * Date: june 11, 2010
 */
public interface ITransformNodeModel extends IGraphicModel {
    static final String type = "Transform";

    /**
     * @return the transform matrix of this transform.
     */
    float[] getTransformMatrix();

    void setTranslation(IVector3f translation);

    /**
     * @return the inverse matrix of this transform.
     */
    float[] getInverseMatrix();
}
