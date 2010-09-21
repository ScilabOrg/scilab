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

/**
 * Author: Lando Pierre
 * Date: 23 mars 2010
 *
 * Drawer manager interface, the drawer manager contain a collection of INodeModel drawer and
 * return the specific drawer (if any) for given node type.
 *
 */
public interface IDrawersManager<drawerType extends IDrawer> {

    /**
     * Register a drawer for a given node type.
     * @param drawer the drawer.
     */
    public void registerDrawer(drawerType drawer);

    /**
     * @param drawable the given drawable node.
     * @return a drawer for the given node type. 
     */
    public drawerType getSpecificDrawer(IDrawable drawable);
}
