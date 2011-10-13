/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.root;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ROOT__;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.IVisitor;

/**
 * @author Vincent COUVERT
 */
public final class Root extends GraphicObject {

    private static Root me;

    /**
     * Constructor
     */
    private Root() {
        super();
    }

    /**
     * Returns the root object
     * @return the root object
     */
    public static Root getRoot() {
        if (me == null) {
            me = new Root();
        }

        return me;
        
    }

    /**
     * Get this object type
     * @return uicontrol
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#getType()
     */
    public String getType() {
        return __GO_ROOT__;
    }

    /**
     * Accept method
     * @param visitor the visitor
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#accept(org.scilab.modules.graphic_objects.graphicObject.IVisitor)
     */
    public void accept(IVisitor visitor) {

    }

}
