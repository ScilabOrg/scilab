/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
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
 * Open dialog to set block parameters
 * @author Vincent COUVERT
 */
public class BlockParametersAction extends DefaultAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 * @param scilabGraph associated diagram
	 */
	public BlockParametersAction(ScilabGraph scilabGraph) {
		super(XcosMessages.BLOCK_PARAMETERS, scilabGraph);
	}

	/**
	 * Menu for diagram menubar
	 * @param scilabGraph associated diagram
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.BLOCK_PARAMETERS, null, new BlockParametersAction(scilabGraph),
				KeyStroke.getKeyStroke(KeyEvent.VK_B, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

	/**
	 * Action !!
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
	    if (((XcosDiagram) getGraph(null)).getSelectionCell() != null) {
		XcosDiagram diagram = (XcosDiagram) getGraph(null); 
		((BasicBlock) diagram.getSelectionCell()).openBlockSettings(diagram.buildEntireContext());
	    }
	}

}
