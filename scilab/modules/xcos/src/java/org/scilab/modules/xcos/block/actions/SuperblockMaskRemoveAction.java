/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block.actions;

import java.awt.event.ActionEvent;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;


/**
 * @author Vincent COUVERT
 *
 */
public final class SuperblockMaskRemoveAction extends DefaultAction {

	/**
	 * @param scilabGraph graph
	 */
	private SuperblockMaskRemoveAction(ScilabGraph scilabGraph) {
		super(XcosMessages.REMOVE, scilabGraph);
	}

	/**
	 * @param scilabGraph graph
	 * @return menu item
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.REMOVE, null, new SuperblockMaskRemoveAction(scilabGraph), null);
	}
	
	/**
	 * Do something
	 * @param e the parameters
	 * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
	    SuperBlock block = (SuperBlock) ((XcosDiagram) getGraph(e)).getSelectionCell();
	    block.unmask();
	}
}
