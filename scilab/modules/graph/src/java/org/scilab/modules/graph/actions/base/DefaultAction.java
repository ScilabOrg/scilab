/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009-2010 - DIGITEO - Clement DAVID
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph.actions.base;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.io.File;
import java.io.IOException;
import java.util.HashSet;
import java.util.Set;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.KeyStroke;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.bridge.checkboxmenuitem.SwingScilabCheckBoxMenuItem;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.checkboxmenuitem.ScilabCheckBoxMenuItem;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;

import com.mxgraph.swing.mxGraphComponent;

/**
 * Default action for a Scilab Graph
 */
public abstract class DefaultAction extends CallBack {
	private static final Set<String> ICON_PATH = new HashSet<String>();

	static {
		addIconPath(System.getenv("SCI") + "/modules/gui/images/icons/");
	}
	
	private final ScilabGraph scilabGraph;

	/**
	 * Default constructor.
	 * 
	 *The {@link AbstractAction} object is configured using the reflection API.
	 * So you have to be sure that the following fields are declared as static
	 * final fields of each subclasses.
	 * <ul>
	 * <li>String NAME : The name of the action</li>
	 * <li>String SMALL_ICON : The associated icon name (located on
	 * $SCI/modules/gui/images/icons)</li>
	 * <li>int MNEMONIC_KEY : The key associated with the action (see
	 * {@link KeyEvent})</li>
	 * <li>int ACCELERATOR_KEY : The key mask to apply to the mnemonic</li>
	 * </ul>
	 * 
	 * @param scilabGraph
	 *            corresponding Scilab Graph
	 */
	public DefaultAction(ScilabGraph scilabGraph) {
		super("");
		this.scilabGraph = scilabGraph;

		installProperties();
	}
	
	/**
	 * Add an icon path to the default icon path.
	 * 
	 * @param path the icon path (with the trailing /)
	 */
	public static void addIconPath(String path) {
		ICON_PATH.add(path);
	}

	/**
	 * Add an icon path to the default icon path.
	 * 
	 * @param path the icon path (with the trailing /)
	 */
	public static void addIconPath(File path) {
		if (path.isDirectory()) {
			try {
				ICON_PATH.add(path.getCanonicalPath() + File.separatorChar);
			} catch (IOException e) { }
		}
	}
	
	/**
	 * Install the static actions properties on the instance
	 */
	private void installProperties() {
		String name = "";
		String icon = "";
		int mnemonic = 0;
		int accelerator = 0;
		try {
			name = (String) getClass().getField("NAME").get(null);
			
			/*
			 * Getting icon from the registered icon path
			 */
			String iconName = (String) getClass().getField("SMALL_ICON").get(null);
			if (iconName != null && !iconName.isEmpty()) {
				// TODO: manage icons according to http://live.gnome.org/ThemableAppSpecificIcons
				
				// add default extension of no-one set
				if (iconName.lastIndexOf('.') == -1) {
					iconName = iconName + ".png";
				}
				
				// get the existing icon path
				for (String path : ICON_PATH) {
					if (new File(path + iconName).isFile()) {
						icon = path + iconName;
						break;
					}
				}
			}
			
			mnemonic = getClass().getField("MNEMONIC_KEY").getInt(null);
			accelerator = getClass().getField("ACCELERATOR_KEY").getInt(null);
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
		} catch (SecurityException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (NoSuchFieldException e) {
			e.printStackTrace();
		}

		assert !"".equals(name);
		putValue(Action.NAME, name);
		putValue(Action.SHORT_DESCRIPTION, name);
		putValue(Action.LONG_DESCRIPTION, name);
		if (!icon.isEmpty()) {
			putValue(Action.SMALL_ICON, new ImageIcon(icon));
		}

		/*
		 * Set up the accelerator instead of the mnemonic as the menu is the
		 * preferred way on keyboard control. We are using Action.MNEMONIC_KEY
		 * as keyboard key and Action.ACCELERATOR_KEY as a mask.
		 * 
		 * Install it only when there is a real shortcut (with a mnemonic).
		 */
		if (mnemonic != 0) {
			putValue(Action.MNEMONIC_KEY, mnemonic);
			putValue(Action.ACCELERATOR_KEY, KeyStroke.getKeyStroke(mnemonic,
					accelerator));
		}
	}

	/**
	 * Create a menu item associated with the graph
	 * 
	 * @param graph
	 *            the graph to work on
	 * @param klass
	 *            the associated klass
	 * @return the menu item
	 */
	protected static MenuItem createMenu(ScilabGraph graph,
			final Class< ? extends DefaultAction> klass) {
		DefaultAction action = GraphActionManager.getInstance(graph, klass);
		MenuItem item = ScilabMenuItem.createMenuItem();

		SwingScilabMenuItem swingItem = (SwingScilabMenuItem) item
				.getAsSimpleMenuItem();
		swingItem.setAction(action);

		return item;
	}

	/**
	 * Create a menu item associated with the graph
	 * 
	 * @param graph
	 *            the graph to work on
	 * @param klass
	 *            the associated klass
	 * @return the push button
	 */
	protected static PushButton createButton(ScilabGraph graph,
			final Class< ? extends DefaultAction> klass) {
		DefaultAction action = GraphActionManager.getInstance(graph, klass);
		PushButton item = ScilabPushButton.createPushButton();

		SwingScilabPushButton swingItem = (SwingScilabPushButton) item
				.getAsSimplePushButton();
		swingItem.setAction(action);

		// Hide the text on buttons
		swingItem.setHideActionText(true);

		return item;
	}

	/**
	 * Create a menu item associated with the graph
	 * 
	 * @param graph
	 *            the graph to work on
	 * @param klass
	 *            the associated klass
	 * @return the checkbox item
	 */
	protected static CheckBoxMenuItem createCheckBoxMenu(ScilabGraph graph,
			Class< ? extends DefaultAction> klass) {
		DefaultAction action = GraphActionManager.getInstance(graph, klass);
		CheckBoxMenuItem item = ScilabCheckBoxMenuItem.createCheckBoxMenuItem();

		SwingScilabCheckBoxMenuItem swingItem = (SwingScilabCheckBoxMenuItem) item
				.getAsSimpleCheckBoxMenuItem();
		swingItem.setAction(action);

		return item;
	}

	/**
	 * Constructor
	 * @param label action descriptor
	 * @param scilabGraph associated Scilab Graph
	 */
	@Deprecated
	protected DefaultAction(String label, ScilabGraph scilabGraph) {
		super(label);
		this.scilabGraph = scilabGraph;
	}

	/**
	 * Get associated graph
	 * @param e event
	 * @return Returns the graph for the given action event.
	 */
	protected final ScilabGraph getGraph(ActionEvent e) {
		if (this.scilabGraph != null) {
			return this.scilabGraph;
		}
		
		if (e == null) {
			return null;
		}

		if (e.getSource() instanceof Component) {
			Component component = (Component) e.getSource();

			while (component != null && !(component instanceof mxGraphComponent)) {
				component = component.getParent();
			}

			return (ScilabGraph) ((mxGraphComponent) component).getGraph();
		}

		return null;
	}


	/**
	 * Create a button for a graph toolbar
	 * @param title label of the menu
	 * @param icon the path the an icon file
	 * @param listener action listener associated
	 * @param keyStroke menu shortcut
	 * @return the button
	 */
	@Deprecated
	protected static MenuItem createMenu(String title, String icon, DefaultAction listener, KeyStroke keyStroke) {
		MenuItem menu = ScilabMenuItem.createMenuItem();
		menu.setCallback(listener);
		menu.setText(title);

		if (keyStroke != null) {
			((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(keyStroke);
		}

		return menu;
	}
	
	/**
	 * Action
	 * @param e parameters
	 * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public abstract void actionPerformed(ActionEvent e);
	
	/**
	 * Not used
	 * @see org.scilab.modules.gui.events.callback.CallBack#callBack()
	 */
	@Override
	public void callBack() {
		assert "Must never be called as we bypass Callback.java".equals("");
	}
}
