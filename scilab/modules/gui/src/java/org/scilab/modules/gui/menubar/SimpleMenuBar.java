/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.menubar;

import org.scilab.modules.gui.menu.Menu;

/**
 * Interface for SimpleMenuBar the associated object to Scilab GUIs MenuBar
 * @author Marouane BEN JELLOUL
 */
public interface SimpleMenuBar {

    /**
     * Add a Menu to a MenuBar
     * @param newMenu the Menu to add to the MenuBar
     */
    void add(Menu newMenu);

    /**
     * Enable/Disable a menu giving its name
     * @param menuName the name of the menu
     * @param status true to enable the menu
     */
    void setMenuEnabled(String menuName, boolean status);

    /**
     * Disable a MenuItem of a Scilab root window giving its parent name and position
     * @param parentMenuName the name of the parent menu
     * @param menuItemPosition the name of the parent menu
     * @param status true to set the menu enabled
     */
    void setSubMenuEnabled(String parentMenuName, int menuItemPosition, boolean status);

    /**
     * Remove a menu giving its name
     * @param menuName the name of the menu
     */
    void removeMenu(String menuName);
    
    /**
     * Set the visibility of the Toolbar
     * @param newVisibleState true to set the toolbar visible
     */
    void setVisible(boolean newVisibleState);
}
