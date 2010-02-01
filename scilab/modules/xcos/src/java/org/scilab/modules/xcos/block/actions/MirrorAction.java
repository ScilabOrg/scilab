/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.xcos.block.actions;

import java.awt.Toolkit;
import java.awt.event.KeyEvent;

import javax.swing.KeyStroke;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * @author Antoine ELIAS
 *
 */
public class MirrorAction extends DefaultAction {

    private static final long serialVersionUID = -6228416497044111945L;

    /**
     * Constructor
     * @param scilabGraph associated diagram
     */
    public MirrorAction(ScilabGraph scilabGraph) {
	super(XcosMessages.MIRROR, scilabGraph);
    }

    /**
     * Menu added to the menubar
     * @param scilabGraph associated diagram
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
	return createMenu(XcosMessages.MIRROR, null, new MirrorAction(scilabGraph),
		KeyStroke.getKeyStroke(KeyEvent.VK_M, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask(), true));
    }

    /**
     * Action !!
     * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
     */
    public void doAction() {
	if (((XcosDiagram) getGraph(null)).getSelectionCells().length != 0) {

	    Object[] allCells = ((XcosDiagram) getGraph(null)).getSelectionCells();

	    getGraph(null).getModel().beginUpdate();
	    for (int i = 0; i < allCells.length; ++i) {
		if (allCells[i] instanceof BasicBlock) {
		    ((BasicBlock) allCells[i]).toggleMirror();
		}
	    }
	    getGraph(null).getModel().endUpdate();
	}
    }

}
