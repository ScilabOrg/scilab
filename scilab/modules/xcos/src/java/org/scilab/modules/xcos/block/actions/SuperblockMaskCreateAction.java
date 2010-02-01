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

import java.awt.event.ActionEvent;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * @author Vincent COUVERT
 *
 */
public final class SuperblockMaskCreateAction extends DefaultAction {

	/**
	 * @param scilabGraph graph
	 */
	private SuperblockMaskCreateAction(ScilabGraph scilabGraph) {
		super(XcosMessages.CREATE, scilabGraph);
	}

	/**
	 * @param scilabGraph graph
	 * @return menu item
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.CREATE, null,
				new SuperblockMaskCreateAction(scilabGraph), null);
	}

	/**
	 * Callback to be done
	 * @param e parameters
	 * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		SuperBlock block = (SuperBlock) ((XcosDiagram) getGraph(e))
				.getSelectionCell();
		/*
		 * FIXME: this action doesn't handle variable settings
		 */
		block.mask();

		/* Set default values */
		ScilabList exprs = new ScilabList() {
			{
				add(new ScilabDouble());
				add(new ScilabList() {
					{
						add(new ScilabDouble());
						add(new ScilabString(
								XcosMessages.MASK_DEFAULTWINDOWNAME));
						add(new ScilabList() {
							{
								add(new ScilabDouble());
							}
						});
					}
				});
			}
		};
		block.setExprs(exprs);
	}
}
