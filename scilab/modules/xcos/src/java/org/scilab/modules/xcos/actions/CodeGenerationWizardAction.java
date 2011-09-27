/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
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

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.actions.dialog.CodeGenerationDialog;
import org.scilab.modules.xcos.block.actions.SuperBlockSelectedAction;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Codegeneration method selector dialog
 */
public class CodeGenerationWizardAction extends SuperBlockSelectedAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.CODE_GENERATION;;
	/** Icon name of the action */
	public static final String SMALL_ICON = "";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = 0;
	/** Accelerator key for the action */
	public static final int ACCELERATOR_KEY = 0;
	private ScilabGraph scilabGraph;

	/**
	 * Constructor
	 * @param scilabGraph Associated Scilab Graph
	 */
	public CodeGenerationWizardAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
		
		this.scilabGraph = scilabGraph;
	}
	
	/**
	 * Create corresponding menu
	 * @param scilabGraph Associated Scilab Graph
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, CodeGenerationWizardAction.class);
	}

	/**
	 * Action !
	 * @param e the event
	 * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {

		
		final XcosDiagram diag = (XcosDiagram) getGraph(e);
		
		final CodeGenerationDialog dialog = new CodeGenerationDialog(diag.getAsComponent(), scilabGraph );
		
		dialog.pack();
		dialog.setVisible(true);
	}
}
