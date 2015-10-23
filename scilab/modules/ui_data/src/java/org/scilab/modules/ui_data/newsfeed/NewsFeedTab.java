/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

package org.scilab.modules.ui_data.newsfeed;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.ui_data.utils.UiDataMessages;

import java.util.List;
import java.util.ListIterator;

/**
 * Main class to display a news feed in a tab Dockable panel (jflexdock) For
 * now, manages one tab only
 */
public class NewsFeedTab extends SwingScilabDockablePanel implements SimpleTab {
    public static final String NEWSFEEDUUID = "DC0957B3-81DA-4E39-B0B5-E93B35412162";

    private static NewsFeedTab instance = null;

    private NewsFeedWidget newsFeedWidget;
    private NewsFeedController newsFeedController;

    public static NewsFeedTab getInstance() {
        if (instance == null) {
            instance = new NewsFeedTab();
        }
        return instance;
    }

    private NewsFeedTab() {
        super(UiDataMessages.NEWS_FEED, NEWSFEEDUUID);

        newsFeedController = new NewsFeedController();
        newsFeedWidget = new NewsFeedWidget(newsFeedController);
        newsFeedController.addNewsFeedEventListener(newsFeedWidget);
        setContentPane(newsFeedWidget);

        // setAssociatedXMLIDForHelp("newsfeed");

        registerClosingOperation();
        WindowsConfigurationManager.restorationFinished(this);

        startNewsFeed();
    }

    public static void displayTab() {
        if (instance == null) {
            if (!WindowsConfigurationManager.restoreUUID(NEWSFEEDUUID)) {
                NewsFeedTabFactory.getInstance().getTab(NEWSFEEDUUID);
                instance.createTabWindow();
            }
        }
        instance.setVisible(true);
    }

    public void startNewsFeed() {
        newsFeedController.start();
    }

    private void stopNewsFeed() {
        newsFeedController.interrupt();
    }

    private void closeTab() {
        stopNewsFeed();
        instance = null;
    }

    private SwingScilabWindow createTabWindow() {
        SwingScilabWindow window = SwingScilabWindow.createWindow(true);
        window.addTab(this);
        window.setLocation(0, 0);
        window.setSize(500, 200);
        window.setVisible(true);
        return window;
    }

    private void registerClosingOperation() {
        ClosingOperationsManager.registerClosingOperation(this, new ClosingOperationsManager.ClosingOperation() {

            @Override
            public int canClose() {
                return 1;
            }

            @Override
            public void destroy() {
                closeTab();
            }

            @Override
            public String askForClosing(final List<SwingScilabDockablePanel> list) {
                return null;
            }

            @Override
            public void updateDependencies(List<SwingScilabDockablePanel> list,
                                           ListIterator<SwingScilabDockablePanel> it) {
            }
        });
    }

    /**
     * {@inheritDoc}
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        // setInfoBar(infoBarToAdd);
    }

    /**
     * {@inheritDoc}
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        setMenuBar(menuBarToAdd);
    }

    /**
     * {@inheritDoc}
     */
    public SwingScilabWindow getParentWindow() {
        return SwingScilabWindow.allScilabWindows.get(getParentWindowId());
    }

    /**
     * {@inheritDoc}
     */
    public SimpleTab getAsSimpleTab() {
        return this;
    }
}
