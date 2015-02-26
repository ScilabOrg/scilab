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

package org.scilab.modules.gui.bridge.toolbar;

import javax.swing.JToolBar;

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.toolbar.SimpleToolBar;

/**
 * Swing implementation for Scilab ToolBar in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabToolBar extends JToolBar implements SimpleToolBar {

    private static final long serialVersionUID = 1L;

    /**
     * Constructor
     */
    public SwingScilabToolBar() {
        super();
        setFloatable(false);
        setRollover(true); /* Bug 2752 */
    }

    /**
     * Add a PushButton to the ToolBar.
     * @param pushButton the PushButton to add .
     */
    public void add(PushButton pushButton) {
        super.add((SwingScilabPushButton) pushButton.getAsSimplePushButton());
    }

    /**
     * Add a Separator to a Toolbar
     * @see org.scilab.modules.gui.toolbar.ToolBar#addSeparator(org.scilab.modules.gui..)
     */
    public void addSeparator() {
        super.addSeparator();
    }
}
