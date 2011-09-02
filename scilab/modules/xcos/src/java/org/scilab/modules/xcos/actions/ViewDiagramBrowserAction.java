/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.actions;

import java.awt.event.ActionEvent;
import java.io.File;
import java.io.IOException;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.OneBlockDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * View the diagram as a scilab tree
 */
public final class ViewDiagramBrowserAction extends OneBlockDependantAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.DIAGRAM_BROWSER;
	/** Icon name of the action */
	public static final String SMALL_ICON = "";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = 0;
	/** Accelerator key for the action */
	public static final int ACCELERATOR_KEY = 0;
	
	/**
	 * Constructor
	 * @param scilabGraph graph
	 */
	public ViewDiagramBrowserAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * Create the menu
	 * @param scilabGraph graph
	 * @return menu item
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, ViewDiagramBrowserAction.class);
	}

	/**
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		try {
			String temp = FileUtils.createTempFile();
			new File(temp).deleteOnExit();
			((XcosDiagram) getGraph(null)).dumpToHdf5File(temp);
			try {
				String cmd = ScilabInterpreterManagement.buildCall("import_from_hdf5", temp);
				cmd += "tree_show(scs_m); ";
				cmd += ScilabInterpreterManagement.buildCall("deletefile", temp);
				ScilabInterpreterManagement.synchronousScilabExec(cmd);
			} catch (InterpreterException e2) {
				e2.printStackTrace();
			}
		} catch (IOException e1) {
			e1.printStackTrace();
		}
	}
}
