/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
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
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosInterpreterManagement;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.scilab.modules.xcos.utils.XcosInterpreterManagement.InterpreterException;

/**
 * Diagram compilation management
 * @author Allan SIMON
 */
public class CompileAction extends DefaultAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 * @param scilabGraph associated diagram
	 */
	public CompileAction(ScilabGraph scilabGraph) {
		super(XcosMessages.COMPILE, scilabGraph);
	}

	/**
	 * Create associated menu
	 * @param scilabGraph associated diagram
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.COMPILE, null, new CompileAction(scilabGraph), null);
	}

	/**
	 * Action !!
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		((XcosDiagram) getGraph(null)).info(XcosMessages.COMPILATION_IN_PROGRESS);
			Thread launchMe = new Thread() {
				public void run() {
					File temp;
					try {
						temp = File.createTempFile("xcos", ".h5");
						temp.deleteOnExit();
						((XcosDiagram) getGraph(null)).dumpToHdf5File(temp.getAbsolutePath());
						
						String command = "import_from_hdf5(\"" + temp.getAbsolutePath() + "\");"
						               + "xcos_compile(scs_m);";
						try {
							XcosInterpreterManagement.asynchronousScilabExec(command, new ActionListener() {
								public void actionPerformed(ActionEvent arg0) {
									((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);	
								}
							});
						} catch (InterpreterException e) {
							e.printStackTrace();
						}
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			};
			launchMe.start();
	}
}
