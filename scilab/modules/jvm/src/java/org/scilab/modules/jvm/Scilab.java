   /*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
 * Copyright (C) INRIA - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.jvm;

import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.LookAndFeelManager;
import org.scilab.modules.gui.utils.MenuBarBuilder;
import org.scilab.modules.gui.utils.ToolBarBuilder;
import org.scilab.modules.localization.Messages;

/**
 * Main Class for Scilab
 * @author Allan CORNET
 * @author Vincent COUVERT
 * @author Sylvestre Ledru INRIA 2007
 * @author Bruno JOFRET
 */
public class Scilab {

	private static final String CLASS_NOT_FOUND = "Could not find class: ";

	private static final String SCIDIR = System.getenv("SCI");

	private static final String MENUBARXMLFILE = SCIDIR + "/modules/gui/etc/main_menubar.xml";
	private static final String TOOLBARXMLFILE = SCIDIR + "/modules/gui/etc/main_toolbar.xml";
	
	private static final String SEE_DEFAULT_PATHS = "See SCI/etc/classpath.xml for default paths.";

	/** Index of windows vista version */
	private static final double VISTA_VERSION = 6.0;
	
	private static final String ENABLE_JAVA2D_OPENGL_PIPELINE = "sun.java2d.opengl";
	private static final String ENABLE = "true";
	private static final String DISABLE = "false";
	private static final String DISABLE_DDRAW = "sun.java2d.noddraw";
	private Window mainView;

	 /**
	 * Constructor Scilab Class.
	 * @param mode Mode Scilab -NW -NWNI -STD -API
	 */
	public Scilab(int mode) {
		
		// Set options for JOGL
		// they must be set before creating GUI
		setJOGLFlags();
		
		
		try {
			String gtkLookAndFeel = "com.sun.java.swing.plaf.gtk.GTKLookAndFeel";
			/* Init the LookAndFeelManager all the time since we can
			 * create windows in the NW mode */

			LookAndFeelManager lookAndFeel = new LookAndFeelManager();

			/* TODO Manages also windows */
			
			if (lookAndFeel.isSupportedLookAndFeel(gtkLookAndFeel)) {
				lookAndFeel.setLookAndFeel(gtkLookAndFeel);
			} else {
				lookAndFeel.setSystemLookAndFeel();
			}

		} catch (java.lang.NoClassDefFoundError exception) {
			System.err.println("Could not initialize graphics Environment");
			System.err.println("Scilab Graphical option may not be working correctly.");
			System.err.println("An error occurred: " + exception.getLocalizedMessage());
		}

		if (mode == 2) { /* Mode GUI */

			// Create a user config file if not already exists
			ConfigManager.createUserCopy();

			try {
				mainView = ScilabWindow.createWindow();
			} catch (NoClassDefFoundError exception) {
				System.err.println("Cannot create Scilab Window.\n"
						+ "Check if the thirdparties are available (Flexdock, JOGL...).\n" + SEE_DEFAULT_PATHS);
				System.err.println(CLASS_NOT_FOUND + exception.getLocalizedMessage());
				System.exit(-1);
			}
			
			mainView.setPosition(ConfigManager.getMainWindowPosition());
			mainView.setDims(ConfigManager.getMainWindowSize());

			/************/
			/* MENU BAR */
			/************/
			MenuBar menuBar = MenuBarBuilder.buildMenuBar(MENUBARXMLFILE);

			/************/
			/* TOOL BAR */
			/************/
			ToolBar toolBar = ToolBarBuilder.buildToolBar(TOOLBARXMLFILE);

			/* Create the console */
			Tab consoleTab = null;
			try {
				/* CONSOLE */
				/* Create a tab to put console into */
				consoleTab = ScilabTab.createTab(Messages.gettext("Console"));
				/* Exit Scilab when the console is closed */
				consoleTab.setCallback(CallBack.createCallback("exit();", CallBack.SCILAB_INSTRUCTION));

				ScilabConsole.createConsole();
			} catch (NoClassDefFoundError exception) {
				System.err.println("Cannot create Scilab Console.\nCheck if the thirdparties are available (JoGL/JRosetta...).\n"
						+ SEE_DEFAULT_PATHS);
				System.err.println(CLASS_NOT_FOUND + exception.getLocalizedMessage());
				System.exit(-1);
			}
			
			TextBox infoBar = ScilabTextBox.createTextBox();
			
			/** Adding content into container */
			toolBar.setVisible(false); // Enabled in scilab.start
			ScilabConsole.getConsole().addToolBar(toolBar);
			ScilabConsole.getConsole().addMenuBar(menuBar);
			ScilabConsole.getConsole().addInfoBar(infoBar);
			ScilabConsole.getConsole().setMaxOutputSize(ConfigManager.getMaxOutputSize());
			consoleTab.addMember(ScilabConsole.getConsole());
			mainView.addTab(consoleTab);
			mainView.draw();
		} 

	}

	/**
	 * Sets the prompt displayed in Scilab console
	 * @param prompt the prompt to be displayed as a String
	 */
	public void setPrompt(String prompt) {
		ScilabConsole.getConsole().setPrompt(prompt);
	}
	
	/**
	 * Set the command line flags to the JVM
	 */
	public static void setJOGLFlags() {
		
		// Uneash OpenGL power
		// Not yet
		//System.setProperty(ENABLE_JAVA2D_OPENGL_PIPELINE, ENABLE_WITH_DEBUG);
		System.setProperty(ENABLE_JAVA2D_OPENGL_PIPELINE, DISABLE);
		
		if (isWindowsPlateform()) {
			if (findWindowsVersion() >= VISTA_VERSION) {
				// don't enable OpenGL because of aero
				System.setProperty(ENABLE_JAVA2D_OPENGL_PIPELINE, DISABLE);
			}
			// desactivate direct3d and direct draw under windows
			System.setProperty(DISABLE_DDRAW, ENABLE);
		}

	}
	
	/**
	 * @return true if the os is windows, false otherwise
	 */
	public static boolean isWindowsPlateform() {
		// get os name
		return System.getProperty("os.name").toLowerCase().contains("windows");
	}
	
	/**
	 * Find the verion of windows used on the computer if one
	 * @return negative value if the OS is not windows, the version of windows otherwise
	 */
	public static double findWindowsVersion() {
		// default valu enot windows
		double windowsVersion = -1.0;
		
		if (isWindowsPlateform()) {
			// windows plateform
			return Double.valueOf(System.getProperty("os.version"));
		}
		
		return windowsVersion;
	}
	
}
/*--------------------------------------------------------------------------*/

