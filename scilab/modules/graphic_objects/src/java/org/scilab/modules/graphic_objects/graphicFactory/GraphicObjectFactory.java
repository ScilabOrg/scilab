/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.graphicFactory;

import java.util.List;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.types.ScilabType;

public interface GraphicObjectFactory {

    GraphicObject createObject(List<ScilabType> args); 

    void deleteObject(int id);
}
