/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.actions;

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
 * @author Vincent COUVERT
 *
 */
public final class ViewDiagramBrowserAction extends DefaultAction {

	/**
	 * @param scilabGraph graph
	 */
	private ViewDiagramBrowserAction(ScilabGraph scilabGraph) {
		super(XcosMessages.DIAGRAM_BROWSER, scilabGraph);
	}

	/**
	 * @param scilabGraph graph
	 * @return menu item
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.DIAGRAM_BROWSER, null, new ViewDiagramBrowserAction(scilabGraph), null);
	}
	
	   public void doAction() {
		try {
		    File temp = File.createTempFile("xcos",".h5");
		    temp.deleteOnExit();
		    ((XcosDiagram) getGraph(null)).dumpToHdf5File(temp.getAbsolutePath());
		    try {
				XcosInterpreterManagement.synchronousScilabExec("import_from_hdf5(\"" + temp.getAbsolutePath() + "\");"
				    + "tree_show(scs_m);"
				    + "deletefile(\"" + temp.getAbsolutePath() + "\");");
			} catch (InterpreterException e) {
				e.printStackTrace();
			}
		} catch (IOException e1) {
		    e1.printStackTrace();
		}
	    }
}
