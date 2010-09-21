/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.drawer;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * Author: Lando Pierre
 * Date: 20 mai 2010
 *
 * Basic implementation of IDrawersManager
 */
public class DefaultDrawersManager<drawerType extends IDrawer> implements IDrawersManager<drawerType> {
    private final Map<String, drawerType> drawers = new HashMap<String, drawerType>();
    private final Set<String> warning = new HashSet<String>();

    
    public void registerDrawer(drawerType drawer) {
        drawers.put(drawer.getType(), drawer);
    }

    
    public drawerType getSpecificDrawer(IDrawable drawable) {
        if (drawable != null) {
            String nodeType = drawable.getType();
            drawerType drawer = drawers.get(nodeType);
            if (drawer == null && !warning.contains(nodeType)) {
                System.out.println("Warning: no IDrawer for INodeModel of type: " + nodeType);
                warning.add(nodeType);
            }
            return drawer;
        } else {
            return null;
        }
    }
}
