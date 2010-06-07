/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.palette.listener;

import java.awt.MouseInfo;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.List;

import javax.swing.SwingUtilities;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.palette.PaletteBlockCtrl;
import org.scilab.modules.xcos.palette.view.PaletteBlockView;
import org.scilab.modules.xcos.utils.XcosMessages;

/** Implement the default mouse listener for the block */
public final class PaletteBlockMouseListener implements MouseListener {
	private static final double BLOCK_DEFAULT_POSITION = 10.0;
	
	/** Default constructor */
	public PaletteBlockMouseListener() { }

	/**
	 * Load and perform display update on mouse click
	 * @param e The associated event 
	 */
	public void mouseClicked(MouseEvent e) {
		if ((e.getClickCount() == 1 && SwingUtilities.isRightMouseButton(e))
				|| e.isPopupTrigger()
				|| XcosMessages.isMacOsPopupTrigger(e)) {

			ContextMenu menu = ScilabContextMenu.createContextMenu();

			final List<XcosDiagram> allDiagrams = Xcos.getDiagrams();
			final PaletteBlockCtrl control = ((PaletteBlockView) e.getSource()).getController();
			
			// No diagram opened: should never happen as Xcos opens an empty diagram when it is launched
			assert allDiagrams.size() != 0;
			
			if (allDiagrams.size() == 1) {
				// A single diagram opened: add to this diagram
				MenuItem addTo = ScilabMenuItem.createMenuItem();

				addTo.setText(XcosMessages.ADDTO + " " + allDiagrams.get(0).getParentTab().getName());
				final XcosDiagram theDiagram = allDiagrams.get(0);
				addTo.setCallback(new CallBack(e.toString()) {
					public void callBack() {
						BasicBlock current = loadAndSetupBlock(control);
						theDiagram.addCell(current);
					}
				});

				menu.add(addTo);

			} else {
				// The user has to choose
				Menu addTo = ScilabMenu.createMenu();

				addTo.setText(XcosMessages.ADDTO);

				for (int i = 0; i < allDiagrams.size(); i++) {
					MenuItem diagram = ScilabMenuItem.createMenuItem();
					final XcosDiagram theDiagram = allDiagrams.get(i);
					diagram.setText(allDiagrams.get(i).getParentTab().getName());
					diagram.setCallback(new CallBack(e.toString()) {
						public void callBack() {
							BasicBlock current = loadAndSetupBlock(control);
							theDiagram.addCell(current);
						}
					});
					addTo.add(diagram);
				}

				menu.add(addTo);
			}


			menu.getAsSimpleContextMenu().addSeparator();

			MenuItem help = ScilabMenuItem.createMenuItem();
			help.setText("Block help");
			help.setCallback(new CallBack(e.toString()) {
				public void callBack() {
					try {
						ScilabInterpreterManagement.synchronousScilabExec("help", control.getModel().getName());
					} catch (InterpreterException e) {
						e.printStackTrace();
					}
				}
			});
			menu.add(help);

			menu.setVisible(true);

			((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(
					MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo().getLocation().y);
		}
	}
	
	/**
	 * @param control The controller
	 * @return the loaded block
	 */
	private BasicBlock loadAndSetupBlock(
			final PaletteBlockCtrl control) {
		BasicBlock current = control.loadBlock();
		current.getGeometry().setX(BLOCK_DEFAULT_POSITION);
		current.getGeometry().setY(BLOCK_DEFAULT_POSITION);
		return current;
	}

	/**
	 * Not used
	 * @param e Not used
	 */
	public void mouseEntered(MouseEvent e) { }

	/**
	 * Not used
	 * @param e Not used
	 */
	public void mouseExited(MouseEvent e) { }

	/**
	 * Select on mouse press
	 * @param e The associated event
	 */
	public void mousePressed(MouseEvent e) {
		PaletteBlockView view = (PaletteBlockView) e.getSource();
		view.getController().setSelected(true);
	}

	/**
	 * Not used
	 * @param e Not used
	 */
	public void mouseReleased(MouseEvent e) { }
}
