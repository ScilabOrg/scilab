/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
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
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosInterpreterManagement;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.scilab.modules.xcos.utils.XcosInterpreterManagement.InterpreterException;

/**
 * @author Bruno JOFRET
 *
 */
public class StartAction  extends DefaultAction {

    private static final long serialVersionUID = -7548486977403506053L;

    /**
     * @param scilabGraph graph
     */
    public StartAction(ScilabGraph scilabGraph) {
	super(XcosMessages.START, scilabGraph);
    }

    /**
     * @param scilabGraph graph
     * @return push button
     */
    public static PushButton createButton(ScilabGraph scilabGraph) {
	return createButton(XcosMessages.START, "media-playback-start.png", new StartAction(scilabGraph));
    }

    /**
     * @param scilabGraph graph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
	return createMenu(XcosMessages.START, null, new StartAction(scilabGraph), null);
    }

    public void actionPerformed(ActionEvent e) {
	File temp;
	((XcosDiagram) getGraph(null)).info(XcosMessages.SIMULATION_IN_PROGRESS);
	XcosTab.setStartEnabled(false);
	((XcosDiagram) getGraph(null)).setReadOnly(true);
	
	try {
	    temp = File.createTempFile("xcos",".h5");
	    ((XcosDiagram) getGraph(e)).getRootDiagram().dumpToHdf5File(temp.getAbsolutePath());

	    String command = "import_from_hdf5(\"" + temp.getAbsolutePath() + "\");"
	    				+ "scicos_debug(" + ((XcosDiagram) getGraph(e)).getDebugLevel() + ");"
	    				+ "xcos_simulate(scs_m);"
	    				+ "deletefile(\"" + temp.getAbsolutePath() + "\");";
	    try {
			XcosInterpreterManagement.asynchronousScilabExec(command, new ActionListener() {
				public void actionPerformed(ActionEvent arg0) {
					((XcosDiagram) getGraph(null)).info(XcosMessages.EMPTY_INFO);
				    XcosTab.setStartEnabled(true);
				    ((XcosDiagram) getGraph(null)).setReadOnly(false);
				}
			});
		} catch (InterpreterException e1) {
			e1.printStackTrace();
		}
	} catch (IOException e1) {
	    e1.printStackTrace();
	    XcosTab.setStartEnabled(true);
	}
    }
}
