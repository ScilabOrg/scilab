/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.core;

import java.awt.event.ActionEvent;
import java.util.List;
import java.util.UUID;

import javax.swing.JTextArea;

import org.scilab.modules.jvm.LoadClassPath;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.MenuBarBuilder;
import org.scilab.modules.gui.utils.ToolBarBuilder;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.localization.Messages;

/**
 *
 * @author Calixte DENIZET
 */
public class ConsoleTab {

    private static final String CLASS_NOT_FOUND = "Could not find class: ";
    private static final String SEE_DEFAULT_PATHS = "See SCI/etc/classpath.xml for default paths.";
    private static final String SCIDIR = System.getenv("SCI");
    private static final String MENUBARXMLFILE = SCIDIR + "/modules/gui/etc/main_menubar.xml";
    private static final String TOOLBARXMLFILE = SCIDIR + "/modules/gui/etc/main_toolbar.xml";
    private static final String NOCONSOLE = Messages.gettext("No available console !\nPlease use STD mode.");
    private static final String EMPTYTAB = Messages.gettext("Empty tab");

    public static Tab getConsoleTab(String uuid) {
	if (Scilab.getMode() != 2) {
	    Tab tab = ScilabTab.createTab(EMPTYTAB, uuid);
	    JTextArea textarea = new JTextArea(NOCONSOLE);
	    textarea.setEditable(false);
	    ((SwingScilabTab) tab.getAsSimpleTab()).setContentPane(textarea);
	    
            ClosingOperationsManager.registerClosingOperation(tab, new ClosingOperationsManager.ClosingOperation() {

                    public boolean canClose() {
			return true;
		    }

                    public void destroy() { }

                    public String askForClosing() {
			return null;
		    }
                });

	    ClosingOperationsManager.addDependencyWithRoot(tab);
	    
	    return tab;
	}

        MenuBar menuBar = MenuBarBuilder.buildMenuBar(MENUBARXMLFILE);
        ToolBar toolBar = ToolBarBuilder.buildToolBar(TOOLBARXMLFILE);

        /* Create the console */
        Tab consoleTab = null;
        try {
            /* CONSOLE */
            /* Create a tab to put console into */
            LoadClassPath.loadOnUse("Console");
            if (uuid == null) {
                consoleTab = ScilabTab.createTab(Messages.gettext("Scilab Console"), UUID.randomUUID().toString());
            } else {
                consoleTab = ScilabTab.createTab(Messages.gettext("Scilab Console"), uuid);
            }

            ClosingOperationsManager.registerClosingOperation(consoleTab, new ClosingOperationsManager.ClosingOperation() {

                    public boolean canClose() {
			return true;
		    }

                    public void destroy() {
			CallBack.createCallback("exit(0);", CallBack.SCILAB_INSTRUCTION).actionPerformed(new ActionEvent(new Object(), 0, ""));
		    }

                    public String askForClosing() {
			return "Console";
		    }
                });

	    ClosingOperationsManager.setRoot(consoleTab);
	    
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

        return consoleTab;
    }
}
