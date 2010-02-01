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

package org.scilab.modules.xcos;


import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import org.scilab.modules.graph.ScilabGraph;
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
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.xcos.actions.AboutXcosAction;
import org.scilab.modules.xcos.actions.CloseAction;
import org.scilab.modules.xcos.actions.CloseViewportAction;
import org.scilab.modules.xcos.actions.CodeGenerationAction;
import org.scilab.modules.xcos.actions.CompileAction;
import org.scilab.modules.xcos.actions.DebugLevelAction;
import org.scilab.modules.xcos.actions.DiagramBackgroundAction;
import org.scilab.modules.xcos.actions.ExportAction;
import org.scilab.modules.xcos.actions.FitDiagramToViewAction;
import org.scilab.modules.xcos.actions.LinkStyleAction;
import org.scilab.modules.xcos.actions.NewDiagramAction;
import org.scilab.modules.xcos.actions.NormalViewAction;
import org.scilab.modules.xcos.actions.OpenAction;
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
import org.scilab.modules.xcos.block.AfficheBlock;
import org.scilab.modules.xcos.block.actions.AlignBlockAction;
import org.scilab.modules.xcos.block.actions.BlockDocumentationAction;
import org.scilab.modules.xcos.block.actions.BlockParametersAction;
import org.scilab.modules.xcos.block.actions.ColorAction;
import org.scilab.modules.xcos.block.actions.FlipAction;
import org.scilab.modules.xcos.block.actions.MirrorAction;
import org.scilab.modules.xcos.block.actions.RegionToSuperblockAction;
import org.scilab.modules.xcos.block.actions.RotateAction;
import org.scilab.modules.xcos.block.actions.ViewDetailsAction;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.actions.ViewPaletteBrowserAction;
import org.scilab.modules.xcos.utils.ConfigXcosManager;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.mxGraphOutline;
import com.mxgraph.util.mxConstants;

/**
 * Xcos tab operations
 * 
 * This class implement specific operation of an Xcos Tab.
 */
public class XcosTab extends ScilabTab {

    private static final long serialVersionUID = -290453474673387812L;
    private static final Size WIN_SIZE = new Size(600, 500);
    
    /*
     * Static fields
     */
    private static List<XcosDiagram> diagrams = new Vector<XcosDiagram>();
    private static Map<Integer, AfficheBlock> afficheBlocks = new HashMap<Integer, AfficheBlock>();

    private static List<Menu> recentsMenus = new ArrayList<Menu>();
    private static List<MenuItem> startMenuItems = new ArrayList<MenuItem>();
    private static List<MenuItem> stopMenuItems = new ArrayList<MenuItem>();
    private static List<PushButton> startPushButtons = new ArrayList<PushButton>();
    private static List<PushButton> stopPushButtons = new ArrayList<PushButton>();

    private static boolean startEnabled = true;
    
    /*
     * Instance fields
     */
    private XcosDiagram diagram;
    
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
    
    private boolean actionsEnabled;

    /**
     * Default constructor
     * @param diagram The associated diagram model
     */
    public XcosTab(XcosDiagram diagram) {
	super(XcosMessages.XCOS);

	this.diagram = diagram;
	this.actionsEnabled = false;

	initComponents();

	// No SimpleTab.addMember(ScilabComponent ...) so perform a raw association.
	((SwingScilabTab) getAsSimpleTab()).setContentPane(diagram.getAsComponent());
	
	setCallback(new CloseAction(diagram));
	diagram.getAsComponent().addKeyListener(new ArrowKeyListener());
    }
    
    /**
     * @param diag diagram
     */
    public static void closeDiagram(XcosDiagram diag) {

	// System.err.println("Xcos::closeDiagram : " + diagram);

	diag.closeChildren();
	diagrams.remove(diag);

	// for(int i = 0 ; i < diagrams.size(); i++){
	// System.err.println("diagrams[" + i + "] : " +
	// diagrams.get(i).getClass());
	// }
	if (diagrams.size() == 0) {
	    // System.err.println("close session");
	    Xcos.closeSession();
	}

    }

    /**
     * Try to focus to an already openned file
     * 
     * @param filename filename
     * @return True when found and focused, False otherwise
     */
    public static boolean focusOnExistingFile(String filename) {
	for (XcosDiagram diagram : diagrams) {
	    if (diagram.getSavedFile() != null) {
		if (diagram.getSavedFile().compareTo(filename) == 0) {
		    diagram.getParentTab().setCurrent();
		    return true;
		}
	    }
	}
	return false;
    }

    /**
     * @return All the AffichBlock and their corresponding UID.
     */
    public static Map<Integer, AfficheBlock> getAfficheBlocks() {
	return afficheBlocks;
    }

    /**
     * @return diagram list
     */
    public static List<XcosDiagram> getAllDiagrams() {
	return diagrams;
    }

    /**
     * @param status new status
     */
    public static void setStartEnabled(boolean status) {
	for (int i = 0; i < startMenuItems.size(); i++) {
	    startMenuItems.get(i).setEnabled(status);
	    startPushButtons.get(i).setEnabled(status);
	    startEnabled = status;

	    stopMenuItems.get(i).setEnabled(!status);
	    stopPushButtons.get(i).setEnabled(!status);
	}

    }

    /**
     * @param xcosDiagram diagram
     */
    public static void createTabFromDiagram(XcosDiagram xcosDiagram) {
    	diagrams.add(xcosDiagram);
    	
    	XcosTab tab = new XcosTab(xcosDiagram);
    	tab.setName(XcosMessages.UNTITLED);
    	xcosDiagram.setParentTab(tab);
    	
	// Get the palette window position and align on it.
	if (PaletteManager.isVisible()) {
		Window win = PaletteManager.getInstance().getView().getParentWindow();
	    Position palPosition = win.getPosition();
	    Size palSize = win.getDims();
	    Position mainPosition = new Position(palPosition.getX()
		    + palSize.getWidth(), palPosition.getY());
	    tab.getParentWindow().setPosition(mainPosition);
	}
	
	/*
	 * VIEW PORT
	 */
	XcosTab.createViewPort(xcosDiagram);
    }
    
    /**
     * @param xcosDiagram diagram
     */
    public static void showTabFromDiagram(XcosDiagram xcosDiagram) {
	assert xcosDiagram.isOpened();
	XcosTab tab = (XcosTab) xcosDiagram.getParentTab();
	
	xcosDiagram.setVisible(true);

	/*
	 * Superblock specific customizations
	 */
	if (xcosDiagram instanceof SuperBlockDiagram) {
	    tab.setSimulationActionEnabled(false);
	}

	/*
	 * Set buttons enabled
	 */
	tab.setActionsEnabled(true);

	// Disabling undo/redo at startup
	tab.setEnabledRedo(false);
	tab.setEnabledUndo(false);
    }
    
    /**
     * Update menu displaying recent opened files
     * @param scilabGraph graph
     */
    public static void updateRecentOpenedFilesMenu(ScilabGraph scilabGraph) {
	List<File> recentFiles = ConfigXcosManager
		.getAllRecentOpenedFiles();

	for (int j = 0; j < recentsMenus.size(); j++) {
	    ((SwingScilabMenu) recentsMenus.get(j).getAsSimpleMenu())
		    .removeAll();

	    for (int i = 0; i < recentFiles.size(); i++) {
		recentsMenus.get(j).add(
			RecentFileAction.createMenu(scilabGraph, recentFiles
				.get(i)));
	    }
	}
    }

    /**
     * Close the current diagram 
     */
    public void closeDiagram() {

	diagram.closeChildren();
	diagrams.remove(diagram);

	if (diagrams.size() == 0) {
	    Xcos.closeSession();
	}

    }

    /**
     * Instantiate all the subcomponents of this Tab.
     */
    private void initComponents() {
    	Window window = ScilabWindow.createWindow();
		window.setDims(WIN_SIZE);
		
		/* Create the menu bar */
		menuBar = createMenuBar();
		addMenuBar(menuBar);
		
		/* Create the toolbar */
		ToolBar toolBar = createToolBar();
		addToolBar(toolBar);
		
		/* Create the infoBar */
		getAsSimpleTab().setInfoBar(ScilabTextBox.createTextBox());
		
		window.addTab(this);
    }
    
    /**
     * Create the windows menu bar
     */
    private MenuBar createMenuBar() {
	List<File> recentFiles = ConfigXcosManager
		.getAllRecentOpenedFiles();

	menuBar = ScilabMenuBar.createMenuBar();

	/** FILE MENU */
	fileMenu = ScilabMenu.createMenu();
	fileMenu.setText(XcosMessages.FILE);
	fileMenu.setMnemonic('F');

	fileMenu.add(NewDiagramAction.createMenu(diagram));

	fileMenu.add(OpenAction.createMenu(diagram));
	fileMenu.addSeparator();
	fileMenu.add(SaveAction.createMenu(diagram));
	fileMenu.add(SaveAsAction.createMenu(diagram));
	fileMenu.add(ExportAction.createMenu(diagram));

	recentsMenu = ScilabMenu.createMenu();
	recentsMenu.setText(XcosMessages.RECENT_FILES);
	for (int i = 0; i < recentFiles.size(); i++) {
	    recentsMenu.add(RecentFileAction.createMenu(diagram,
		    recentFiles.get(i)));
	}
	recentsMenus.add(recentsMenu);
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
	CheckBoxMenuItem menu = ViewViewportAction
		.createCheckBoxMenu(diagram);
	view.add(menu);
	((XcosDiagram) diagram).setViewPortMenuItem(menu);
	view.add(ViewDetailsAction.createMenu(diagram));

	/** Simulation menu */
	simulate = ScilabMenu.createMenu();
	simulate.setText(XcosMessages.SIMULATION);
	simulate.setMnemonic('S');
	menuBar.add(simulate);

	MenuItem startMenu = StartAction.createMenu(diagram);
	startMenuItems.add(startMenu);
	startMenu.setEnabled(startEnabled);
	MenuItem stopMenu = StopAction.createMenu(diagram);
	stopMenuItems.add(stopMenu);
	stopMenu.setEnabled(startEnabled);

	simulate.add(SetupAction.createMenu(diagram));
	simulate.add(DebugLevelAction.createMenu(diagram));
	simulate.add(SetContextAction.createMenu(diagram));
	simulate.add(CompileAction.createMenu(diagram));
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
	alignMenu.add(AlignBlockAction.createMenu(diagram,
		XcosMessages.ALIGN_LEFT, mxConstants.ALIGN_LEFT));
	alignMenu.add(AlignBlockAction.createMenu(diagram,
		XcosMessages.ALIGN_CENTER, mxConstants.ALIGN_CENTER));
	alignMenu.add(AlignBlockAction.createMenu(diagram,
		XcosMessages.ALIGN_RIGHT, mxConstants.ALIGN_RIGHT));
	alignMenu.addSeparator();
	alignMenu.add(AlignBlockAction.createMenu(diagram,
		XcosMessages.ALIGN_TOP, mxConstants.ALIGN_TOP));
	alignMenu.add(AlignBlockAction.createMenu(diagram,
		XcosMessages.ALIGN_MIDDLE, mxConstants.ALIGN_MIDDLE));
	alignMenu.add(AlignBlockAction.createMenu(diagram,
		XcosMessages.ALIGN_BOTTOM, mxConstants.ALIGN_BOTTOM));
	format.add(alignMenu);
	format.addSeparator();

	format.add(ColorAction.createMenu(diagram,
		XcosMessages.BORDER_COLOR, mxConstants.STYLE_STROKECOLOR));
	format.add(ColorAction.createMenu(diagram, XcosMessages.FILL_COLOR,
		mxConstants.STYLE_FILLCOLOR));
	format.addSeparator();

	linkStyle = ScilabMenu.createMenu();
	linkStyle.setText(XcosMessages.LINK_STYLE);
	linkStyle.add(LinkStyleAction.createMenu(diagram,
		XcosMessages.LINK_STYLE_HORIZONTAL,
		mxConstants.ELBOW_HORIZONTAL));
	linkStyle.add(LinkStyleAction.createMenu(diagram,
		XcosMessages.LINK_STYLE_STRAIGHT, mxConstants.SHAPE_CONNECTOR));
	linkStyle.add(LinkStyleAction.createMenu(diagram,
		XcosMessages.LINK_STYLE_VERTICAL, mxConstants.ELBOW_VERTICAL));
	format.add(linkStyle);
	format.addSeparator();

	format.add(DiagramBackgroundAction.createMenu(diagram));
	CheckBoxMenuItem gridMenu = ViewGridAction
		.createCheckBoxMenu(diagram);
	format.add(gridMenu);
	((XcosDiagram) diagram).setGridMenuItem(menu);

	/** Tools menu */
	tools = ScilabMenu.createMenu();
	tools.setText(XcosMessages.TOOLS);
	tools.setMnemonic('T');
	menuBar.add(tools);

	tools.add(CodeGenerationAction.createMenu(diagram));

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

    /**
     * @return tool bar
     */
    private ToolBar createToolBar() {
	ToolBar toolBar = ScilabToolBar.createToolBar();

	newDiagramAction = NewDiagramAction.createButton(diagram);
	toolBar.add(newDiagramAction);

	openAction = OpenAction.createButton(diagram);
	toolBar.add(openAction);
	
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
	startAction.setEnabled(startEnabled);
	stopAction.setEnabled(!startEnabled);
	startPushButtons.add(startAction);
	stopPushButtons.add(stopAction);

	toolBar.add(startAction);
	toolBar.add(stopAction);

	toolBar.addSeparator();

	// ZOOMIN / ZOOMOUT
	zoomInAction = ZoomInAction.zoominButton(diagram);
	toolBar.add(zoomInAction);
	zoomOutAction = ZoomOutAction.zoomoutButton(diagram);
	toolBar.add(zoomOutAction);

	toolBar.addSeparator();

	xcosDemonstrationAction = XcosDemonstrationsAction.createButton(diagram);
	toolBar.add(xcosDemonstrationAction);
	xcosDocumentationAction = XcosDocumentationAction.createButton(diagram);
	toolBar.add(xcosDocumentationAction);

	return toolBar;
    }

    /**
     * @param xcosDiagramm diagram
     */
    private static void createViewPort(ScilabGraph xcosDiagramm) {
	Window outline = ScilabWindow.createWindow();
	Tab outlineTab = ScilabTab.createTab(XcosMessages.VIEWPORT);

	outlineTab.setCallback(new CloseViewportAction(xcosDiagramm));

	MenuBar vpMenuBar = ScilabMenuBar.createMenuBar();
	outlineTab.addMenuBar(vpMenuBar);

	Menu vpMenu = ScilabMenu.createMenu();
	vpMenu.setText(XcosMessages.VIEWPORT);
	vpMenu.setMnemonic('V');
	vpMenuBar.add(vpMenu);

	vpMenu.add(CloseViewportAction.createMenu(xcosDiagramm));

	outlineTab.getAsSimpleTab().setInfoBar(ScilabTextBox.createTextBox());

	((XcosDiagram) xcosDiagramm).setViewPort(outlineTab);

	// Creates the graph outline component
	mxGraphOutline graphOutline = new mxGraphOutline(xcosDiagramm
		.getAsComponent());

	graphOutline.setDrawLabels(true);

	((SwingScilabTab) outlineTab.getAsSimpleTab())
		.setContentPane(graphOutline);
	outline.addTab(outlineTab);
	outline.setVisible(false);
	outlineTab.setVisible(false);
    }

    /**
     * @param status new status
     */
    public void setEnabledRedo(boolean status) {
	if (actionsEnabled) {
	redoAction.setEnabled(status);
	}
    }

    /**
     * @param status new status
     */
    public void setEnabledUndo(boolean status) {
	if (actionsEnabled) {
	undoAction.setEnabled(status);
	}
    }
    
    /**
     * Enable or Disable simulation related actions
     * @param state True if it have to be enabled, flase otherwise
     */
    public void setSimulationActionEnabled(boolean state) {
	if (actionsEnabled) {
    	simulate.setEnabled(state);
    	startAction.setEnabled(state);
    	stopAction.setEnabled(state);
	}
    }
    
    /**
     * Enable or disable all actions
     * @param status True if they have to be enabled, false otherwise
     */
    public void setActionsEnabled(boolean status) {
	fileMenu.setEnabled(status);
	edit.setEnabled(status);
	view.setEnabled(status);
	simulate.setEnabled(status);
	format.setEnabled(status);
	alignMenu.setEnabled(status);
	linkStyle.setEnabled(status);
	tools.setEnabled(status);
	help.setEnabled(status);
	openAction.setEnabled(status);
	saveAction.setEnabled(status);
	saveAsAction.setEnabled(status);
	printAction.setEnabled(status);
	newDiagramAction.setEnabled(status);
	deleteAction.setEnabled(status);
	undoAction.setEnabled(status);
	redoAction.setEnabled(status);
	fitDiagramToViewAction.setEnabled(status);
	startAction.setEnabled(status);
	stopAction.setEnabled(false);
	zoomInAction.setEnabled(status);
	zoomOutAction.setEnabled(status);
	xcosDemonstrationAction.setEnabled(status);
	xcosDocumentationAction.setEnabled(status);
	
	actionsEnabled = status;
    }
}
