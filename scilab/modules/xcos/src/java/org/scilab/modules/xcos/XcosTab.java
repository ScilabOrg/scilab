/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.List;
import java.util.ListIterator;
import java.util.UUID;

import javax.swing.JCheckBoxMenuItem;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.graph.actions.CopyAction;
import org.scilab.modules.graph.actions.CutAction;
import org.scilab.modules.graph.actions.DeleteAction;
import org.scilab.modules.graph.actions.InvertSelectionAction;
import org.scilab.modules.graph.actions.PasteAction;
import org.scilab.modules.graph.actions.RedoAction;
import org.scilab.modules.graph.actions.SelectAllAction;
import org.scilab.modules.graph.actions.UndoAction;
import org.scilab.modules.graph.actions.ZoomInAction;
import org.scilab.modules.graph.actions.ZoomOutAction;
import org.scilab.modules.graph.event.ArrowKeyListener;
import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.BarUpdater;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.xcos.actions.AboutXcosAction;
import org.scilab.modules.xcos.actions.CloseAction;
import org.scilab.modules.xcos.actions.CompileAction;
import org.scilab.modules.xcos.actions.DebugLevelAction;
import org.scilab.modules.xcos.actions.DiagramBackgroundAction;
import org.scilab.modules.xcos.actions.ExportAction;
import org.scilab.modules.xcos.actions.ExternalAction;
import org.scilab.modules.xcos.actions.FitDiagramToViewAction;
import org.scilab.modules.xcos.actions.InitModelicaAction;
import org.scilab.modules.xcos.actions.NewDiagramAction;
import org.scilab.modules.xcos.actions.NormalViewAction;
import org.scilab.modules.xcos.actions.OpenAction;
import org.scilab.modules.xcos.actions.OpenInSciAction;
import org.scilab.modules.xcos.actions.PrintAction;
import org.scilab.modules.xcos.actions.QuitAction;
import org.scilab.modules.xcos.actions.RecentFileAction;
import org.scilab.modules.xcos.actions.SaveAction;
import org.scilab.modules.xcos.actions.SaveAsAction;
import org.scilab.modules.xcos.actions.SetContextAction;
import org.scilab.modules.xcos.actions.SetupAction;
import org.scilab.modules.xcos.actions.ShowHideShadowAction;
import org.scilab.modules.xcos.actions.StartAction;
import org.scilab.modules.xcos.actions.StopAction;
import org.scilab.modules.xcos.actions.ViewDiagramBrowserAction;
import org.scilab.modules.xcos.actions.ViewGridAction;
import org.scilab.modules.xcos.actions.ViewViewportAction;
import org.scilab.modules.xcos.actions.XcosDemonstrationsAction;
import org.scilab.modules.xcos.actions.XcosDocumentationAction;
import org.scilab.modules.xcos.block.actions.BlockDocumentationAction;
import org.scilab.modules.xcos.block.actions.BlockParametersAction;
import org.scilab.modules.xcos.block.actions.BorderColorAction;
import org.scilab.modules.xcos.block.actions.CodeGenerationAction;
import org.scilab.modules.xcos.block.actions.FilledColorAction;
import org.scilab.modules.xcos.block.actions.FlipAction;
import org.scilab.modules.xcos.block.actions.MirrorAction;
import org.scilab.modules.xcos.block.actions.RegionToSuperblockAction;
import org.scilab.modules.xcos.block.actions.RotateAction;
import org.scilab.modules.xcos.block.actions.ViewDetailsAction;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionBottom;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionCenter;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionLeft;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionMiddle;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionRight;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionTop;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.configuration.model.DocumentType;
import org.scilab.modules.xcos.configuration.utils.ConfigurationConstants;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.link.actions.StyleHorizontalAction;
import org.scilab.modules.xcos.link.actions.StyleStraightAction;
import org.scilab.modules.xcos.link.actions.StyleVerticalAction;
import org.scilab.modules.xcos.palette.actions.ViewPaletteBrowserAction;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Xcos tab operations
 * 
 * This class implement specific operation of an Xcos Tab.
 */
// CSOFF: ClassFanOutComplexity
// CSOFF: ClassDataAbstractionCoupling
public class XcosTab extends SwingScilabTab implements Tab {
    public static final String DEFAULT_WIN_UUID = "xcos-default-window";
    public static final String DEFAULT_TAB_UUID = "xcos-default-tab";

    /*
     * Instance fields
     */
    private MenuBar menuBar;
    private Menu fileMenu;
    private Menu recentsMenu;
    private Menu edit;
    private Menu view;
    private Menu simulate;
    private Menu format;
    private Menu alignMenu;
    private Menu linkStyle;
    private Menu tools;
    private Menu help;

    private JCheckBoxMenuItem viewport;

    private PushButton openAction;
    private PushButton saveAction;
    private PushButton saveAsAction;
    private PushButton printAction;
    private PushButton newDiagramAction;
    private PushButton deleteAction;
    private PushButton undoAction;
    private PushButton redoAction;
    private PushButton fitDiagramToViewAction;
    private PushButton startAction;
    private PushButton stopAction;
    private PushButton zoomInAction;
    private PushButton zoomOutAction;
    private PushButton xcosDemonstrationAction;
    private PushButton xcosDocumentationAction;

    private static class ClosingOperation
            implements
            org.scilab.modules.gui.utils.ClosingOperationsManager.ClosingOperation {
        private final XcosDiagram graph;

        public ClosingOperation(XcosDiagram graph) {
            this.graph = graph;
        }

        @Override
        public boolean canClose() {
            return Xcos.getInstance().canClose(graph);
        }

        @Override
        public void destroy() {
            Xcos.getInstance().destroy(graph);
            graph.setOpened(false);
        }

        @Override
        public String askForClosing(final List<SwingScilabTab> list) {
            return Xcos.getInstance().askForClosing(graph, list);
        }

        @Override
        public void updateDependencies(List<SwingScilabTab> list,
                ListIterator<SwingScilabTab> it) {
            final PaletteManagerView palette = PaletteManagerView.get();

            /*
             * if palette is not visible, then break
             */
            if (palette == null) {
                return;
            }

            /*
             * If it already contains the palette, then break.
             */
            if (list.contains(palette)) {
                return;
            }


            final boolean wasLastOpened = Xcos.getInstance()
                    .wasLastOpened(list);

            /*
             * Append the palette if all the xcos files will be closed
             */
            if (wasLastOpened) {
                it.add(palette);
            }

        }
    }

    private static class EndedRestoration implements
            WindowsConfigurationManager.EndedRestoration {
        private final XcosDiagram graph;

        public EndedRestoration(XcosDiagram graph) {
            this.graph = graph;
        }

        @Override
        public void finish() {
            graph.updateTabTitle();

            ConfigurationManager.getInstance().removeFromRecentTabs(
                    graph.getDiagramTab());
        }
    }

    /**
     * Default constructor
     * 
     * @param diagram
     *            The associated diagram
     */
    private XcosTab(XcosDiagram graph, String uuid) {
        super(XcosMessages.XCOS, uuid);
	
	setAssociatedXMLIDForHelp("xcos");

        /** tab association */
        graph.setDiagramTab(uuid);
        setWindowIcon(Xcos.ICON.getImage());

        initComponents(graph);

        graph.getAsComponent().addKeyListener(new ArrowKeyListener());
    }

    /*
     * Static API for Tabs
     */

    /**
     * Get the tab for a graph.
     * 
     * @param graph
     *            the graph
     * @return the tab (can be null)
     */
    public static XcosTab get(XcosDiagram graph) {
        final String uuid = graph.getDiagramTab();
        if (uuid == null) {
            return null;
        }

        return (XcosTab) ScilabTabFactory.getInstance().getFromCache(uuid);
    }

    /**
     * Restore or create the viewport tab for the graph
     * 
     * @param graph
     *            the graph
     */
    public static void restore(XcosDiagram graph) {
        restore(graph, true);
    }

    /**
     * Restore or create the tab for the graph
     * 
     * @param graph
     *            the graph
     * @param visible
     *            should the tab should be visible
     */
    public static void restore(final XcosDiagram graph, final boolean visible) {
        String uuid = graph.getDiagramTab();
        if (uuid == null) {
            uuid = UUID.randomUUID().toString();
        }

        final XcosTab tab = new XcosTab(graph, uuid);
        ScilabTabFactory.getInstance().addToCache(tab);

        Xcos.getInstance().addDiagram(graph.getSavedFile(), graph);
        graph.setOpened(true);

        if (visible) {
            tab.createDefaultWindow().setVisible(true);

            graph.updateTabTitle();
            BarUpdater.updateBars(tab.getParentWindowId(), tab.getMenuBar(), tab.getToolBar(), tab.getInfoBar(), tab.getName(), tab.getWindowIcon());
        }

        ClosingOperationsManager.addDependencyWithRoot((SwingScilabTab) tab);
        ClosingOperationsManager.registerClosingOperation((SwingScilabTab) tab,
                new ClosingOperation(graph));
        WindowsConfigurationManager.registerEndedRestoration(
                (SwingScilabTab) tab, new EndedRestoration(graph));
    }

    /*
     * Specific implementation
     */

    /**
     * Instantiate all the subcomponents of this Tab.
     * 
     * @param diagram
     *            the diagram
     */
    private void initComponents(final XcosDiagram diagram) {
        /* Create the menu bar */
        menuBar = createMenuBar(diagram);
        setMenuBar(menuBar);

        /* Create the toolbar */
        final ToolBar toolBar = createToolBar(diagram);
        setToolBar(toolBar);

        // No SimpleTab.addMember(ScilabComponent ...) so perform a raw
        // association.
        setContentPane(diagram.getAsComponent());

        /* Create the infoBar */
        setInfoBar(ScilabTextBox.createTextBox());
    }

    /**
     * Create the windows menu bar
     * 
     * @param diagram
     *            the diagram
     * @return the Xcos diagram menu bar
     */
    // CSOFF: JavaNCSS
    private MenuBar createMenuBar(final XcosDiagram diagram) {

        menuBar = ScilabMenuBar.createMenuBar();

        /** FILE MENU */
        fileMenu = ScilabMenu.createMenu();
        fileMenu.setText(XcosMessages.FILE);
        fileMenu.setMnemonic('F');

        fileMenu.add(NewDiagramAction.createMenu(diagram));
        fileMenu.add(OpenAction.createMenu(diagram));
        fileMenu.add(OpenInSciAction.createMenu(diagram));
        fileMenu.addSeparator();
        fileMenu.add(SaveAction.createMenu(diagram));
        fileMenu.add(SaveAsAction.createMenu(diagram));
        fileMenu.add(ExportAction.createMenu(diagram));

        recentsMenu = createRecentMenu();

        fileMenu.add(recentsMenu);

        fileMenu.add(PrintAction.createMenu(diagram));
        fileMenu.addSeparator();
        fileMenu.add(CloseAction.createMenu(diagram));
        fileMenu.addSeparator();
        fileMenu.add(QuitAction.createMenu(diagram));

        menuBar.add(fileMenu);

        /** Edit menu */
        edit = ScilabMenu.createMenu();
        edit.setText(XcosMessages.EDIT);
        edit.setMnemonic('E');
        menuBar.add(edit);

        edit.add(UndoAction.undoMenu(diagram));
        edit.add(RedoAction.redoMenu(diagram));
        edit.addSeparator();
        edit.add(CutAction.cutMenu(diagram));
        edit.add(CopyAction.copyMenu(diagram));
        edit.add(PasteAction.pasteMenu(diagram));
        edit.add(DeleteAction.createMenu(diagram));
        edit.addSeparator();
        edit.add(SelectAllAction.createMenu(diagram));
        edit.add(InvertSelectionAction.createMenu(diagram));
        edit.addSeparator();
        edit.add(BlockParametersAction.createMenu(diagram));
        edit.addSeparator();
        edit.add(RegionToSuperblockAction.createMenu(diagram));

        /** View menu */
        view = ScilabMenu.createMenu();
        view.setText(XcosMessages.VIEW);
        view.setMnemonic('V');
        menuBar.add(view);

        view.add(ZoomInAction.zoominMenu(diagram));
        view.add(ZoomOutAction.zoomoutMenu(diagram));
        view.add(FitDiagramToViewAction.createMenu(diagram));
        view.add(NormalViewAction.createMenu(diagram));
        view.addSeparator();
        view.add(ViewPaletteBrowserAction.createCheckBoxMenu(diagram));
        view.add(ViewDiagramBrowserAction.createMenu(diagram));
        final CheckBoxMenuItem menuItem = ViewViewportAction
                .createCheckBoxMenu(diagram);
        viewport = (JCheckBoxMenuItem) menuItem.getAsSimpleCheckBoxMenuItem();
        view.add(menuItem);
        view.add(ViewDetailsAction.createMenu(diagram));

        /** Simulation menu */
        simulate = ScilabMenu.createMenu();
        simulate.setText(XcosMessages.SIMULATION);
        simulate.setMnemonic('S');
        menuBar.add(simulate);

        final MenuItem startMenu = StartAction.createMenu(diagram);
        final MenuItem stopMenu = StopAction.createMenu(diagram);

        simulate.add(SetupAction.createMenu(diagram));
        simulate.add(DebugLevelAction.createMenu(diagram));
        simulate.add(SetContextAction.createMenu(diagram));
        simulate.add(CompileAction.createMenu(diagram));
        simulate.add(InitModelicaAction.createMenu(diagram));
        simulate.add(startMenu);
        simulate.add(stopMenu);

        /** Format menu */
        format = ScilabMenu.createMenu();
        format.setText(XcosMessages.FORMAT);
        format.setMnemonic('O');
        menuBar.add(format);

        format.add(RotateAction.createMenu(diagram));
        format.add(FlipAction.createMenu(diagram));
        format.add(MirrorAction.createMenu(diagram));
        format.add(ShowHideShadowAction.createMenu(diagram));

        format.addSeparator();
        alignMenu = ScilabMenu.createMenu();
        alignMenu.setText(XcosMessages.ALIGN_BLOCKS);
        alignMenu.add(AlignBlockActionLeft.createMenu(diagram));
        alignMenu.add(AlignBlockActionCenter.createMenu(diagram));
        alignMenu.add(AlignBlockActionRight.createMenu(diagram));
        alignMenu.addSeparator();
        alignMenu.add(AlignBlockActionTop.createMenu(diagram));
        alignMenu.add(AlignBlockActionMiddle.createMenu(diagram));
        alignMenu.add(AlignBlockActionBottom.createMenu(diagram));
        format.add(alignMenu);
        format.addSeparator();

        format.add(BorderColorAction.createMenu(diagram));
        format.add(FilledColorAction.createMenu(diagram));
        format.addSeparator();

        linkStyle = ScilabMenu.createMenu();
        linkStyle.setText(XcosMessages.LINK_STYLE);
        linkStyle.add(StyleHorizontalAction.createMenu(diagram));
        linkStyle.add(StyleStraightAction.createMenu(diagram));
        linkStyle.add(StyleVerticalAction.createMenu(diagram));
        format.add(linkStyle);
        format.addSeparator();

        format.add(DiagramBackgroundAction.createMenu(diagram));
        final CheckBoxMenuItem gridMenu = ViewGridAction
                .createCheckBoxMenu(diagram);
        format.add(gridMenu);

        /** Tools menu */
        tools = ScilabMenu.createMenu();
        tools.setText(XcosMessages.TOOLS);
        tools.setMnemonic('T');
        menuBar.add(tools);

        tools.add(CodeGenerationAction.createMenu(diagram));

        // add external action to the tools menu
        final List<ExternalAction> externalActions = Xcos.getInstance().getExternalActions();
        for (ExternalAction action : externalActions) {
            final MenuItem item = ScilabMenuItem.createMenuItem();

            final SwingScilabMenuItem swingItem = (SwingScilabMenuItem) item.getAsSimpleMenuItem();
            swingItem.setAction(new ExternalAction(action, diagram));

            tools.add(item);
        }

        /** Help menu */
        help = ScilabMenu.createMenu();
        help.setText(XcosMessages.HELP);
        help.setMnemonic('?');
        menuBar.add(help);

        help.add(XcosDocumentationAction.createMenu(diagram));
        help.add(BlockDocumentationAction.createMenu(diagram));
        help.addSeparator();
        help.add(XcosDemonstrationsAction.createMenu(diagram));
        help.addSeparator();
        help.add(AboutXcosAction.createMenu(diagram));

        return menuBar;
    }

    // CSON: JavaNCSS

    /**
     * Create the recent menu from the previously opened files
     * 
     * @return the recent menu
     */
    private Menu createRecentMenu() {
        final Menu recent;

        recent = ScilabMenu.createMenu();
        recent.setText(XcosMessages.RECENT_FILES);

        final ConfigurationManager manager = ConfigurationManager.getInstance();
        final List<DocumentType> recentFiles = manager.getSettings()
                .getRecent();
        for (int i = 0; i < recentFiles.size(); i++) {
            URL url;
            try {
                url = new URL(recentFiles.get(i).getUrl());
            } catch (final MalformedURLException e) {
                LogFactory.getLog(XcosTab.class).error(e);
                break;
            }
            recent.add(RecentFileAction.createMenu(url));
        }

        ConfigurationManager.getInstance().addPropertyChangeListener(
                ConfigurationConstants.RECENT_FILES_CHANGED,
                new PropertyChangeListener() {
                    @Override
                    public void propertyChange(final PropertyChangeEvent evt) {
                        assert evt.getPropertyName().equals(
                                ConfigurationConstants.RECENT_FILES_CHANGED);

                        /*
                         * We only handle menu creation there. Return when this
                         * is not the case.
                         */
                        if (evt.getOldValue() != null) {
                            return;
                        }

                        URL url;
                        try {
                            url = new URL(((DocumentType) evt.getNewValue())
                                    .getUrl());
                        } catch (final MalformedURLException e) {
                            LogFactory.getLog(XcosTab.class).error(e);
                            return;
                        }

                        ((SwingScilabMenu) recent.getAsSimpleMenu()).add(
                                (SwingScilabMenu) RecentFileAction.createMenu(
                                        url).getAsSimpleMenu(), 0);
                    }
                });

        return recent;
    }

    /**
     * Create the Tab toolbar
     * 
     * @param diagram
     *            the associated diagram
     * @return tool bar
     */
    private ToolBar createToolBar(final XcosDiagram diagram) {
        final ToolBar toolBar = ScilabToolBar.createToolBar();

        newDiagramAction = NewDiagramAction.createButton(diagram);
        toolBar.add(newDiagramAction);

        openAction = OpenAction.createButton(diagram);
        toolBar.add(openAction);

        toolBar.add(OpenInSciAction.createButton(diagram));

        toolBar.addSeparator();

        saveAction = SaveAction.createButton(diagram);
        toolBar.add(saveAction);
        saveAsAction = SaveAsAction.createButton(diagram);
        toolBar.add(saveAsAction);

        toolBar.addSeparator();

        printAction = PrintAction.createButton(diagram);
        toolBar.add(printAction);

        toolBar.addSeparator();

        deleteAction = DeleteAction.createButton(diagram);
        toolBar.add(deleteAction);

        toolBar.addSeparator();

        // UNDO / REDO
        undoAction = UndoAction.undoButton(diagram);
        redoAction = RedoAction.redoButton(diagram);
        toolBar.add(undoAction);
        toolBar.add(redoAction);

        toolBar.addSeparator();

        fitDiagramToViewAction = FitDiagramToViewAction.createButton(diagram);
        toolBar.add(fitDiagramToViewAction);

        toolBar.addSeparator();

        // START / STOP
        startAction = StartAction.createButton(diagram);
        stopAction = StopAction.createButton(diagram);

        toolBar.add(startAction);
        toolBar.add(stopAction);

        toolBar.addSeparator();

        // ZOOMIN / ZOOMOUT
        zoomInAction = ZoomInAction.zoominButton(diagram);
        toolBar.add(zoomInAction);
        zoomOutAction = ZoomOutAction.zoomoutButton(diagram);
        toolBar.add(zoomOutAction);

        toolBar.addSeparator();

        xcosDemonstrationAction = XcosDemonstrationsAction
                .createButton(diagram);
        toolBar.add(xcosDemonstrationAction);
        xcosDocumentationAction = XcosDocumentationAction.createButton(diagram);
        toolBar.add(xcosDocumentationAction);

        return toolBar;
    }

    /**
     * Check/uncheck the viewport check box menu item
     * 
     * @param status
     *            the checked status
     */
    public void setViewportChecked(boolean status) {
        viewport.setSelected(status);
    }

    /**
     * Get the check/uncheck status of the check box menu item
     * 
     * @param status
     *            the checked status
     */
    public boolean isViewportChecked() {
        return viewport.isSelected();
    }

    private Window createDefaultWindow() {
        final Window win;

        final Window configuration = WindowsConfigurationManager.createWindow(
                DEFAULT_WIN_UUID, false);
        if (configuration != null) {
            win = configuration;
        } else {
            win = ScilabWindow.createWindow();
        }

        win.addTab(this);
        return win;
    }

    /*
     * Tab implementation
     */

    @Deprecated
    @Override
    public void addToolBar(ToolBar toolBarToAdd) {
        setToolBar(toolBarToAdd);
    }

    @Deprecated
    @Override
    public void addMenuBar(MenuBar menuBarToAdd) {
        setMenuBar(menuBarToAdd);
    }

    @Deprecated
    @Override
    public void addInfoBar(TextBox infoBarToAdd) {
        setInfoBar(infoBarToAdd);
    }

    @Deprecated
    @Override
    public SimpleTab getAsSimpleTab() {
        return this;
    }

    @Deprecated
    @Override
    public Window getParentWindow() {
        return createDefaultWindow();
    }
}

// CSON: ClassDataAbstractionCoupling
// CSON: ClassFanOutComplexity
