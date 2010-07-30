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

import javax.swing.JTree;
import javax.swing.SwingUtilities;
import javax.swing.tree.TreePath;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.model.Category;
import org.scilab.modules.xcos.palette.model.Palette;
import org.scilab.modules.xcos.palette.model.PaletteNode;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Manage the click on the category tree
 */
public class PaletteManagerMouseListener implements MouseListener {

	/** Default constructor */
	public PaletteManagerMouseListener() { }
	
	/**
	 * Manage category selection
	 * @param e Not used
	 * @see java.awt.event.MouseListener#mouseClicked(java.awt.event.MouseEvent)
	 */
	@Override
	public void mouseClicked(final MouseEvent e) {
		// Right click
		if ((e.getClickCount() == 1 && SwingUtilities.isRightMouseButton(e))
				|| e.isPopupTrigger() || XcosMessages.isMacOsPopupTrigger(e)) {

			final PaletteManager manager = PaletteManager.getInstance();
			final JTree paletteTree = manager.getView().getTree();
			final TreePath path = paletteTree.getPathForLocation(e.getX(), e
					.getY());
			paletteTree.setSelectionPath(path);

			final ContextMenu menu = ScilabContextMenu.createContextMenu();

			final MenuItem create = ScilabMenuItem.createMenuItem();
			setupCreateOrAdd(paletteTree, path, create);
			menu.add(create);
			
			final MenuItem remove = ScilabMenuItem.createMenuItem();
			setupRemove(paletteTree, path, remove);
			menu.add(remove);
			
			menu.setVisible(true);
			((SwingScilabContextMenu) menu.getAsSimpleContextMenu())
					.setLocation(MouseInfo.getPointerInfo().getLocation().x,
							MouseInfo.getPointerInfo().getLocation().y);

		}
	}

	/**
	 * Setup the create item
	 * @param paletteTree the current tree
	 * @param path the current path
	 * @param create the menu item
	 */
	private void setupCreateOrAdd(final JTree paletteTree, final TreePath path,
			final MenuItem create) {
		PaletteNode node;
		if (path != null) {
			node = (PaletteNode) (path.getLastPathComponent());
		} else {
			node = (PaletteNode) paletteTree.getModel().getRoot();
		}
		final PaletteNode currentNode = node; 
		
		if (currentNode instanceof Category) {
			create.setText(XcosMessages.CREATE);
		} else if (currentNode instanceof Palette) {
			create.setText(XcosMessages.ADDTO_CATEGORY);
		} else {
			throw new IllegalArgumentException("Invalid node selected");
		}
		
		create.setCallback(new CallBack(XcosMessages.CREATE) {
			@Override
			public void callBack() {
				Category nonModifiedRoot = currentNode.getParent();
				final Category c = new Category();
				c.setEnable(true);
				c.setName(XcosMessages.DEFAULT_CATEGORY_NAME);
				
				if (currentNode instanceof Category) {
					((Category) currentNode).getNode().add(c);
					c.setParent((Category) currentNode);
					if (path != null) {
						path.pathByAddingChild(c);
					} else {
						nonModifiedRoot = (Category) currentNode;
					}
				} else if (currentNode instanceof Palette) {
					final int index = nonModifiedRoot.getIndex(currentNode);
					nonModifiedRoot.getNode().set(index, c);
					c.getNode().add(currentNode);
					currentNode.setParent(c);
					c.setParent(nonModifiedRoot);
					path.getParentPath().pathByAddingChild(c);
				}
				
				org.scilab.modules.xcos.palette.Palette.refreshView(c);
			}
		});

		create.setEnabled(true);
	}
	
	/**
	 * Setup the remove item
	 * @param paletteTree the current tree
	 * @param path the current path
	 * @param remove the menu item
	 */
	private void setupRemove(final JTree paletteTree, final TreePath path,
			final MenuItem remove) {
		remove.setText(XcosMessages.REMOVE);
		remove.setCallback(new CallBack(XcosMessages.REMOVE) {
			@Override
			public void callBack() {
				if (path == null) {
					return;
				}
				
				try {
					final PaletteNode currentNode = (PaletteNode) path.getLastPathComponent();
					org.scilab.modules.xcos.palette.Palette.remove(currentNode);
				} catch (final Exception exception) {
					LogFactory.getLog(PaletteManagerMouseListener.class).error(exception);
				}
			}
		});

		boolean canBeRemoved = true;
		try {
			org.scilab.modules.xcos.palette.Palette
					.checkRemoving((PaletteNode) path.getLastPathComponent());
		} catch (final Exception exception) {
			canBeRemoved = false;
		}
		
		remove.setEnabled(canBeRemoved);
	}
	
	/**
	 * Not used
	 * @param e Not used
	 * @see java.awt.event.MouseListener#mouseEntered(java.awt.event.MouseEvent)
	 */
	@Override
	public void mouseEntered(final MouseEvent e) {
	}

	/**
	 * Not used
	 * @param e Not used
	 * @see java.awt.event.MouseListener#mouseExited(java.awt.event.MouseEvent)
	 */
	@Override
	public void mouseExited(final MouseEvent e) {	
	}

	/**
	 * Not used
	 * @param e Not used
	 * @see java.awt.event.MouseListener#mousePressed(java.awt.event.MouseEvent)
	 */
	@Override
	public void mousePressed(final MouseEvent e) {
	}

	/**
	 * Not used
	 * @param e Not used
	 * @see java.awt.event.MouseListener#mouseReleased(java.awt.event.MouseEvent)
	 */
	@Override
	public void mouseReleased(final MouseEvent e) {
	}

}
