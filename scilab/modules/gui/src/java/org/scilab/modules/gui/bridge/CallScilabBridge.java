/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2008 - DIGITEO - Sylvestre KOUMAR
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 * Copyright (C) 2010-2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */


package org.scilab.modules.gui.bridge;

import java.awt.Color;
import java.awt.Font;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.StringSelection;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.print.PrinterJob;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

import javax.print.Doc;
import javax.print.DocFlavor;
import javax.print.DocPrintJob;
import javax.print.PrintException;
import javax.print.PrintService;
import javax.print.SimpleDoc;
import javax.print.attribute.HashPrintRequestAttributeSet;
import javax.print.attribute.PrintRequestAttribute;
import javax.print.attribute.PrintRequestAttributeSet;
import javax.print.attribute.standard.OrientationRequested;
import javax.swing.JEditorPane;
import javax.swing.JTextPane;
import javax.swing.SwingUtilities;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;

import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.console.SciConsole;
import org.scilab.modules.graphic_export.FileExporter;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.SwingView;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvasImpl;
import org.scilab.modules.gui.bridge.console.SwingScilabConsole;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.bridge.frame.SwingScilabFrame;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.checkbox.CheckBox;
import org.scilab.modules.gui.checkbox.ScilabCheckBox;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.colorchooser.ColorChooser;
import org.scilab.modules.gui.colorchooser.ScilabColorChooser;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.editbox.EditBox;
import org.scilab.modules.gui.editbox.ScilabEditBox;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.fontchooser.FontChooser;
import org.scilab.modules.gui.fontchooser.ScilabFontChooser;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.frame.ScilabFrame;
import org.scilab.modules.gui.helpbrowser.HelpBrowser;
import org.scilab.modules.gui.helpbrowser.ScilabHelpBrowser;
import org.scilab.modules.gui.label.Label;
import org.scilab.modules.gui.label.ScilabLabel;
import org.scilab.modules.gui.listbox.ListBox;
import org.scilab.modules.gui.listbox.ScilabListBox;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.messagebox.MessageBox;
import org.scilab.modules.gui.messagebox.ScilabMessageBox;
import org.scilab.modules.gui.popupmenu.PopupMenu;
import org.scilab.modules.gui.popupmenu.ScilabPopupMenu;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;
import org.scilab.modules.gui.radiobutton.RadioButton;
import org.scilab.modules.gui.radiobutton.ScilabRadioButton;
import org.scilab.modules.gui.slider.ScilabSlider;
import org.scilab.modules.gui.slider.Slider;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.uidisplaytree.ScilabUiDisplayTree;
import org.scilab.modules.gui.uidisplaytree.UiDisplayTree;
import org.scilab.modules.gui.uitable.ScilabUiTable;
import org.scilab.modules.gui.uitable.UiTable;
import org.scilab.modules.gui.utils.BarUpdater;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.ImageExporter;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PrinterHelper;
import org.scilab.modules.gui.utils.ScilabAboutBox;
import org.scilab.modules.gui.utils.ScilabPrint;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.utils.WebBrowser;
import org.scilab.modules.gui.waitbar.WaitBar;
import org.scilab.modules.gui.widget.Widget;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.localization.Messages;

/**
 * This class is used to call Scilab GUIs objects from Scilab
 * @author Vincent COUVERT
 */
public class CallScilabBridge {

    private static final int NB_COLORS = 3;

    private static final int POSITION_SIZE = 4;

    private static final int X_INDEX = 0;
    private static final int Y_INDEX = 1;
    private static final int WIDTH_INDEX = 2;
    private static final int HEIGHT_INDEX = 3;

    private static final String NORMALFONT = "normal";
    private static final String OBLIQUEFONT = "oblique";
    private static final String ITALICFONT = "italic";
    private static final String BOLDFONT = "bold";
    private static final int DEFAULTFONTSIZE = 10;
    private static final String DEFAULTFONTNAME = "helvetica";

    private static final double DEFAULT_RED_BACKGROUND = 0.8 * 255;
    private static final double DEFAULT_GREEN_BACKGROUND = 0.8 * 255;
    private static final double DEFAULT_BLUE_BACKGROUND = 0.8 * 255;

    private static final double BUTTON_RED_BACKGROUND = 0.6 * 255;
    private static final double BUTTON_GREEN_BACKGROUND = 0.6 * 255;
    private static final double BUTTON_BLUE_BACKGROUND = 0.6 * 255;

    private static final double DEFAULT_RED_FOREGROUND = 0;
    private static final double DEFAULT_GREEN_FOREGROUND = 0;
    private static final double DEFAULT_BLUE_FOREGROUND = 0;


    private static PrintRequestAttributeSet scilabPageFormat = new HashPrintRequestAttributeSet();

    private static final String FIGURE_TITLE = "Graphic window number ";

    private static final String CONSOLE = "Console";

    /**
     * Constructor
     */
    protected CallScilabBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

    /******************/
    /*                */
    /* CONSOLE BRIDGE */
    /*                */
    /******************/

    /**
     * Read a line from the Console
     * @return the line read
     */
    public static String readLine() {
        return ScilabConsole.getConsole().readLine();
    }

    /**
     * Display a line in the Console
     * @param dataToDisplay the line to display
     */
    public static void display(String dataToDisplay) {
        ScilabConsole.getConsole().display(dataToDisplay);
    }

    /**
     * Get the status of the Console
     * @return true if the console is at the prompt
     */
    public static boolean isWaitingForInput() {
        return ScilabConsole.getConsole().isWaitingForInput();
    }

    /**
     * Update the number of lines and columns that Scilab use to format data to display
     */
    public static void scilabLinesUpdate() {
        ScilabConsole.getConsole().scilabLinesUpdate();
    }

    /**
     * Clear the Console
     */
    public static void clear() {
        ScilabConsole.getConsole().clear();
    }

    /**
     * Clear some lines in the Console
     * @param nbLines the number of lines to clear
     */
    public static void clear(int nbLines) {
        ScilabConsole.getConsole().clear(nbLines);
    }

    /**
     * Get a char to know if the user wants more data to be displayed
     * @return the user answer
     */
    public static int getCharWithoutOutput() {
        return ScilabConsole.getConsole().getAsSimpleConsole().getCharWithoutOutput();
    }

    /**
     * Put the prompt on the top left corner
     */
    public static void toHome() {
        ScilabConsole.getConsole().toHome();
    }

    /**
     * Set the contents of the prompt
     * @param prompt the content to set (default is -->)
     */
    public static void setPrompt(String prompt) {
        ScilabConsole.getConsole().setPrompt(prompt);
    }

    /**
     * Clear the commands history
     */
    public static void clearHistory() {
        ScilabConsole.getConsole().clearHistory();
    }

    /**************************/
    /*                        */
    /* OBJECT CREATION BRIDGE */
    /*                        */
    /**************************/

    /**
     * Create a new Window in Scilab GUIs
     * @return the ID of the window in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int newWindow() {
        Window window = ScilabWindow.createWindow();
        return UIElementMapper.add(window);
    }

    /**
     * Create a new Menubar in Scilab GUIs
     * @return the ID of the Menubar in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int newMenuBar() {
        MenuBar menuBar = ScilabMenuBar.createMenuBar();
        return UIElementMapper.add(menuBar);
    }

    /**
     * Create a new Menu in Scilab GUIs
     * @return the ID of the menu in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int newMenu() {
        MenuItem menuItem = ScilabMenuItem.createMenuItem(false);
        return UIElementMapper.add(menuItem);
    }

    /**
     * Create a new ContextMenu in Scilab GUIs
     * @return the ID of the ContextMenu in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int newContextMenu() {
        ContextMenu contextMenu = ScilabContextMenu.createContextMenu();
        return UIElementMapper.add(contextMenu);
    }

    /**
     * Create a new Color Chooser in Scilab GUIs
     * @return the ID of the Color Chooser in the UIElementMapper
     */
    public static int newColorChooser() {
        ColorChooser colorChooser = ScilabColorChooser.createColorChooser();
        return UIElementMapper.add(colorChooser);
    }

    /**
     * Create a new Font Chooser in Scilab GUIs
     * @return the ID of the Font Chooser in the UIElementMapper
     */
    public static int newFontChooser() {
        FontChooser fontChooser = ScilabFontChooser.createFontChooser();
        return UIElementMapper.add(fontChooser);
    }

    /**
     * Create a new MessageBox in Scilab GUIs
     * @return the ID of the MessageBox in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int newMessageBox() {
        MessageBox messageBox = ScilabMessageBox.createMessageBox();
        return UIElementMapper.add(messageBox);
    }

    /**
     * Create a new PushButton in Scilab GUIs
     * @return the ID of the PushButton in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int newPushButton() {
        PushButton pushButton = ScilabPushButton.createPushButton();
        int id = UIElementMapper.add(pushButton);

        /* Default font */
        setWidgetFontName(id, DEFAULTFONTNAME);
        setWidgetFontWeight(id, NORMALFONT);
        setWidgetFontSize(id, DEFAULTFONTSIZE);

        setWidgetRelief(id, ScilabRelief.RAISED);

        /* Default colors */
        setWidgetBackgroundColor(id, (int) BUTTON_RED_BACKGROUND, (int) BUTTON_GREEN_BACKGROUND, (int) BUTTON_BLUE_BACKGROUND);
        setWidgetForegroundColor(id, (int) DEFAULT_RED_FOREGROUND, (int) DEFAULT_GREEN_FOREGROUND, (int) DEFAULT_BLUE_FOREGROUND);
        return id;
    }

    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void newPushButton(int UID) {
        PushButton pushButton = ScilabPushButton.createPushButton();
        ((ScilabPushButton) pushButton).setIdentifier(UID);
        int id = UIElementMapper.add(pushButton);

        /* Default font */
        setWidgetFontName(id, DEFAULTFONTNAME);
        setWidgetFontWeight(id, NORMALFONT);
        setWidgetFontSize(id, DEFAULTFONTSIZE);

        setWidgetRelief(id, ScilabRelief.RAISED);

        /* Default colors */
        setWidgetBackgroundColor(id, (int) BUTTON_RED_BACKGROUND, (int) BUTTON_GREEN_BACKGROUND, (int) BUTTON_BLUE_BACKGROUND);
        setWidgetForegroundColor(id, (int) DEFAULT_RED_FOREGROUND, (int) DEFAULT_GREEN_FOREGROUND, (int) DEFAULT_BLUE_FOREGROUND);
    }
    /**
     * Create a new EditBox in Scilab GUIs
     * @return the ID of the Edit in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int newEditBox() {
        EditBox editBox = ScilabEditBox.createEditBox();
        int id = UIElementMapper.add(editBox);

        /* Default font */
        setWidgetFontName(id, DEFAULTFONTNAME);
        setWidgetFontWeight(id, NORMALFONT);
        setWidgetFontSize(id, DEFAULTFONTSIZE);

        setWidgetRelief(id, ScilabRelief.SUNKEN);

        /* Default colors */
        setWidgetBackgroundColor(id, (int) DEFAULT_RED_BACKGROUND, (int) DEFAULT_GREEN_BACKGROUND, (int) DEFAULT_BLUE_BACKGROUND);
        setWidgetForegroundColor(id, (int) DEFAULT_RED_FOREGROUND, (int) DEFAULT_GREEN_FOREGROUND, (int) DEFAULT_BLUE_FOREGROUND);
        return id;
    }

    /**
     * Create a new Label in Scilab GUIs
     * @return the ID of the Label in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int newLabel() {
        Label label = ScilabLabel.createLabel();
        int id = UIElementMapper.add(label);

        /* Default font */
        setWidgetFontName(id, DEFAULTFONTNAME);
        setWidgetFontWeight(id, NORMALFONT);
        setWidgetFontSize(id, DEFAULTFONTSIZE);

        setWidgetRelief(id, ScilabRelief.FLAT);

        /* Default colors */
        setWidgetBackgroundColor(id, (int) DEFAULT_RED_BACKGROUND, (int) DEFAULT_GREEN_BACKGROUND, (int) DEFAULT_BLUE_BACKGROUND);
        setWidgetForegroundColor(id, (int) DEFAULT_RED_FOREGROUND, (int) DEFAULT_GREEN_FOREGROUND, (int) DEFAULT_BLUE_FOREGROUND);
        return id;
    }

    /**
     * Create a new CheckBox in Scilab GUIs
     * @return the ID of the CheckBox in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int newCheckBox() {
        CheckBox checkBox = ScilabCheckBox.createCheckBox();
        int id = UIElementMapper.add(checkBox);

        /* Default font */
        setWidgetFontName(id, DEFAULTFONTNAME);
        setWidgetFontWeight(id, NORMALFONT);
        setWidgetFontSize(id, DEFAULTFONTSIZE);

        setWidgetRelief(id, ScilabRelief.FLAT);

        /* Default colors */
        setWidgetBackgroundColor(id, (int) DEFAULT_RED_BACKGROUND, (int) DEFAULT_GREEN_BACKGROUND, (int) DEFAULT_BLUE_BACKGROUND);
        setWidgetForegroundColor(id, (int) DEFAULT_RED_FOREGROUND, (int) DEFAULT_GREEN_FOREGROUND, (int) DEFAULT_BLUE_FOREGROUND);
        return id;
    }

    /**
     * Create a new RadioButton in Scilab GUIs
     * @return the ID of the RadioButton in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int newRadioButton() {
        RadioButton radioButton = ScilabRadioButton.createRadioButton();
        int id = UIElementMapper.add(radioButton);

        /* Default font */
        setWidgetFontName(id, DEFAULTFONTNAME);
        setWidgetFontWeight(id, NORMALFONT);
        setWidgetFontSize(id, DEFAULTFONTSIZE);

        setWidgetRelief(id, ScilabRelief.FLAT);

        /* Default colors */
        setWidgetBackgroundColor(id, (int) DEFAULT_RED_BACKGROUND, (int) DEFAULT_GREEN_BACKGROUND, (int) DEFAULT_BLUE_BACKGROUND);
        setWidgetForegroundColor(id, (int) DEFAULT_RED_FOREGROUND, (int) DEFAULT_GREEN_FOREGROUND, (int) DEFAULT_BLUE_FOREGROUND);
        return id;
    }

    /**
     * Create a new UiTable in Scilab GUIs
     * @return the ID of the UiTable in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int newUiTable() {
        UiTable uiTable = ScilabUiTable.createUiTable();
        int id = UIElementMapper.add(uiTable);

        /* Default font */
        setWidgetFontName(id, DEFAULTFONTNAME);
        setWidgetFontWeight(id, NORMALFONT);
        setWidgetFontSize(id, DEFAULTFONTSIZE);

        setWidgetRelief(id, ScilabRelief.FLAT);

        /* Default colors */
        setWidgetBackgroundColor(id, (int) DEFAULT_RED_BACKGROUND, (int) DEFAULT_GREEN_BACKGROUND, (int) DEFAULT_BLUE_BACKGROUND);
        setWidgetForegroundColor(id, (int) DEFAULT_RED_FOREGROUND, (int) DEFAULT_GREEN_FOREGROUND, (int) DEFAULT_BLUE_FOREGROUND);
        return id;
    }

    /**
     * Create a new UiDisplayTree in Scilab GUIs
     * @return the ID of the UiDisplayTree in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int newUiDisplayTree() {
        UiDisplayTree uiTree = ScilabUiDisplayTree.createUiDisplayTree();
        int id = UIElementMapper.add(uiTree);

        /* Default font */
        setWidgetFontName(id, DEFAULTFONTNAME);
        setWidgetFontWeight(id, NORMALFONT);
        setWidgetFontSize(id, DEFAULTFONTSIZE);

        setWidgetRelief(id, ScilabRelief.FLAT);

        /* Default colors */
        setWidgetBackgroundColor(id, (int) DEFAULT_RED_BACKGROUND, (int) DEFAULT_GREEN_BACKGROUND, (int) DEFAULT_BLUE_BACKGROUND);
        setWidgetForegroundColor(id, (int) DEFAULT_RED_FOREGROUND, (int) DEFAULT_GREEN_FOREGROUND, (int) DEFAULT_BLUE_FOREGROUND);
        return id;
    }

    /**
     * Create a new Slider in Scilab GUIs
     * @return the ID of the Slider in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int newSlider() {
        Slider slider = ScilabSlider.createSlider();
        int id = UIElementMapper.add(slider);

        /* Default font */
        /* Set a default font because is null when JScrollBar is created */
        slider.setFont(new Font(DEFAULTFONTNAME, Font.PLAIN, DEFAULTFONTSIZE));
        setWidgetFontName(id, DEFAULTFONTNAME);
        setWidgetFontWeight(id, NORMALFONT);
        setWidgetFontSize(id, DEFAULTFONTSIZE);

        setWidgetRelief(id, ScilabRelief.FLAT);

        /* Default colors */
        setWidgetBackgroundColor(id, (int) DEFAULT_RED_BACKGROUND, (int) DEFAULT_GREEN_BACKGROUND, (int) DEFAULT_BLUE_BACKGROUND);
        setWidgetForegroundColor(id, (int) DEFAULT_RED_FOREGROUND, (int) DEFAULT_GREEN_FOREGROUND, (int) DEFAULT_BLUE_FOREGROUND);

        slider.setText(""); /* Because Name property is null at creation */
        slider.setMinimumValue(0);
        slider.setMaximumValue(1);
        slider.setUserValue(0);

        return id;
    }

    /**
     * Create a new ListBox in Scilab GUIs
     * @return the ID of the ListBox in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int newListBox() {
        ListBox listBox = ScilabListBox.createListBox();
        int id = UIElementMapper.add(listBox);

        /* Default font */
        setWidgetFontName(id, DEFAULTFONTNAME);
        setWidgetFontWeight(id, NORMALFONT);
        setWidgetFontSize(id, DEFAULTFONTSIZE);

        setWidgetRelief(id, ScilabRelief.FLAT);

        /* Default colors */
        setWidgetBackgroundColor(id, (int) DEFAULT_RED_BACKGROUND, (int) DEFAULT_GREEN_BACKGROUND, (int) DEFAULT_BLUE_BACKGROUND);
        setWidgetForegroundColor(id, (int) DEFAULT_RED_FOREGROUND, (int) DEFAULT_GREEN_FOREGROUND, (int) DEFAULT_BLUE_FOREGROUND);
        return id;
    }

    /**
     * Create a new PopupMenu in Scilab GUIs
     * @return the ID of the PopupMenu in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int newPopupMenu() {
        PopupMenu popupMenu = ScilabPopupMenu.createPopupMenu();
        int id = UIElementMapper.add(popupMenu);

        /* Default font */
        setWidgetFontName(id, DEFAULTFONTNAME);
        setWidgetFontWeight(id, NORMALFONT);
        setWidgetFontSize(id, DEFAULTFONTSIZE);

        setWidgetRelief(id, ScilabRelief.FLAT);

        /* Default colors */
        setWidgetBackgroundColor(id, (int) DEFAULT_RED_BACKGROUND, (int) DEFAULT_GREEN_BACKGROUND, (int) DEFAULT_BLUE_BACKGROUND);
        setWidgetForegroundColor(id, (int) DEFAULT_RED_FOREGROUND, (int) DEFAULT_GREEN_FOREGROUND, (int) DEFAULT_BLUE_FOREGROUND);
        return id;
    }

    /**
     * Create a new Frame in Scilab GUIs
     * @return the ID of the Frame in the UIElementMapper
     */
    public static int newFrame() {
        Frame frame = ScilabFrame.createFrame();
        int id = UIElementMapper.add(frame);

        /* Default font */
        setFrameFontName(id, DEFAULTFONTNAME);
        setFrameFontWeight(id, NORMALFONT);
        setFrameFontSize(id, DEFAULTFONTSIZE);

        setFrameRelief(id, ScilabRelief.RIDGE);

        /* Default colors */
        setFrameBackgroundColor(id, (int) DEFAULT_RED_BACKGROUND, (int) DEFAULT_GREEN_BACKGROUND, (int) DEFAULT_BLUE_BACKGROUND);
        setFrameForegroundColor(id, (int) DEFAULT_RED_FOREGROUND, (int) DEFAULT_GREEN_FOREGROUND, (int) DEFAULT_BLUE_FOREGROUND);

        frame.setText(""); /* Because Name property is null at creation */
        return id;
    }

    /**
     * Create a new ContextMenu in Scilab GUIs
     * and wait for a user answer
     * @param uicontextmenuUID the id of the Context Menu
     * @return the item of the menu selected
     */
    public static String displayAndWaitContextMenu(int uicontextmenuUID) {
        SwingViewObject uicontextmenu = SwingView.getFromId(uicontextmenuUID);
        return ((SwingScilabContextMenu) uicontextmenu).displayAndWait();
    }

    /**
     * Destroy a Widget
     * @param id the id of the Widget to destroy
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void destroyWidget(int id) {
        ((Widget) UIElementMapper.getCorrespondingUIElement(id)).destroy();
        UIElementMapper.removeMapping(id);
    }

    /**
     * Destroy a Frame
     * @param id the id of the Frame to destroy
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void destroyFrame(int id) {
        ((Frame) UIElementMapper.getCorrespondingUIElement(id)).destroy();
        UIElementMapper.removeMapping(id);
    }

    /**
     * Create a new window with id figureIndex.
     * The created window contains an empty tab.
     * @param figureIndex index of the figure to create
     * @return id of the window
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int newWindow(int figureIndex) {
        Window newWindow = ScilabWindow.createWindow();

        newWindow.setTitle(FIGURE_TITLE + figureIndex);
        /* MENUBAR */
        MenuBar menuBar = null; //MenuBarBuilder.buildMenuBar(MENUBARXMLFILE, figureIndex);
        /* TOOLBAR */
        ToolBar toolBar = null; //ToolBarBuilder.buildToolBar(TOOLBARXMLFILE, figureIndex);

        TextBox infoBar = ScilabTextBox.createTextBox();

        // create a tab able to display a figure handle
        Tab graphicTab = ScilabTab.createTab(FIGURE_TITLE + figureIndex, figureIndex);
        /* Destroy the graphic figure when the tab is closed */
        // // check if figure is already closed
        // if (get_figure_handle(fid) <> []) then
        //   if (get(get_figure_handle(fid), 'event_handler_enable') == 'on') then
        //     // execute closing call back
        //     execstr(get(get_figure_handle(fid), 'event_handler') + '(fid, -1, -1, -1000)', 'errcatch', 'm');
        //   end
        //   // destroy the figure
        //   delete(get_figure_handle(fid));
        // end
        String closingCommand =
            "if (get_figure_handle(" + figureIndex + ") <> []) then"
            +      "  if (get(get_figure_handle(" + figureIndex + "), 'event_handler_enable') == 'on') then"
            +      "    execstr(get(get_figure_handle(" + figureIndex + "), 'event_handler')+'(" + figureIndex + ", -1, -1, -1000)', 'errcatch', 'm');"
            +      "  end;"
            +      "  delete(get_figure_handle(" + figureIndex + "));"
            +      "end;";
        //graphicTab.setCallback(ScilabCloseCallBack.create(figureIndex, closingCommand));
        graphicTab.addMenuBar(menuBar);
        graphicTab.addToolBar(toolBar);
        graphicTab.addInfoBar(infoBar);
        ((SwingScilabTab) graphicTab.getAsSimpleTab()).setWindowIcon("graphic-window");
        newWindow.addTab(graphicTab);

        // link the tab and canvas with their figure
        //        DrawableFigureGL associatedFigure = FigureMapper.getCorrespondingFigure(figureIndex);
        //associatedFigure.setRendererProperties(new ScilabRendererProperties(graphicTab, graphicCanvas));
        //        associatedFigure.setRendererProperties(new ScilabRendererProperties(graphicTab, null, figureIndex));
        // don't draw now, figure will show itself when all its parameters will be set

        return 0;
    }

    /****************************/
    /*                          */
    /* OBJECT DIMENSIONS BRIDGE */
    /*                          */
    /****************************/

    /**
     * Set the dimensions of an object in Scilab GUIs
     * @param objID the ID of the object in the UIElementMapper
     * @param width the width of the object
     * @param height the height of the object
     */
    public static void setDims(int objID, int width, int height) {
        UIElementMapper.getCorrespondingUIElement(objID).setDims(new Size(width, height));
    }

    /**********************/
    /*                    */
    /* OBJECT TEXT BRIDGE */
    /*                    */
    /**********************/

    /**
     * Set the text of a widget in Scilab GUIs
     * @param objID the ID of the Widget in the UIElementMapper
     * @param text the text to set to the widget
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setWidgetText(int objID, String text) {
        ((Widget) UIElementMapper.getCorrespondingUIElement(objID)).setText(text);
    }

    /**
     * Get the text of a widget in Scilab GUIs
     * @param objID the ID of the Widget in the UIElementMapper
     * @return the text of the widget
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static String getWidgetText(int objID) {
        return ((Widget) UIElementMapper.getCorrespondingUIElement(objID)).getText();
    }

    /**
     * Set the text of a Frame in Scilab GUIs
     * @param objID the ID of the Widget in the UIElementMapper
     * @param text the text to set to the Frame
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setFrameText(int objID, String text) {
        ((Frame) UIElementMapper.getCorrespondingUIElement(objID)).setText(text);
    }

    /**
     * Get the text of a Frame in Scilab GUIs
     * @param objID the ID of the Frame in the UIElementMapper
     * @return the text of the Frame
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static String getFrameText(int objID) {
        return ((Frame) UIElementMapper.getCorrespondingUIElement(objID)).getText();
    }

    /******************/
    /*                */
    /* PARENT SETTING */
    /*                */
    /******************/

    /**
     * Set a figure as parent for an UIElement
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the object in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setFigureAsParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();

          if (!(UIElementMapper.getCorrespondingUIElement(objID) instanceof MenuItem)) {
          // Add the menu to the tab
          parentTab.getMenuBar().add((Menu) UIElementMapper.getCorrespondingUIElement(objID));
          } else {
          // obj is a MenuItem that has to be converted to a Menu
          MenuItem menuItem = (MenuItem) UIElementMapper.getCorrespondingUIElement(objID);

          Menu menuToAdd = ScilabMenu.createMenu();
          UIElementMapper.removeMapping(objID);
          UIElementMapper.addMapping(objID, menuToAdd);

          // Copy all properties from MenuItem to Menu
          menuToAdd.setText(menuItem.getText());
          menuToAdd.setCallback(menuItem.getCallback());
          menuToAdd.setForeground(menuItem.getForeground());
          menuToAdd.setVisible(menuItem.isVisible());
          menuToAdd.setEnabled(menuItem.isEnabled());
          menuToAdd.setChecked(menuItem.isChecked());
          // End of properties copy

          // Add the menu to the tab
          parentTab.getMenuBar().add(menuToAdd);
          }*/
    }

    /**
     * Set a figure as parent for a PushButton
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the PushButton in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setPushButtonParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          PushButton pushButton = (PushButton) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.addMember(parentTab, pushButton);*/
    }

    /**
     * Remove a PushButton from its parent figure
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the PushButton in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void removePushButtonFromParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          PushButton pushButton = (PushButton) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.removeMember(parentTab, pushButton);*/
    }

    /**
     * Set a figure as parent for a EditBox
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the PushButton in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setEditBoxParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          EditBox editBox = (EditBox) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.addMember(parentTab, editBox);*/
    }

    /**
     * Remove an EditBox from its parent figure
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the PushButton in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void removeEditBoxFromParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          EditBox editBox = (EditBox) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.removeMember(parentTab, editBox);*/
    }

    /**
     * Set a figure as parent for a Label
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the PushButton in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setLabelParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          Label label = (Label) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.addMember(parentTab, label);*/
    }

    /**
     * Remove a Label from its parent figure
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the PushButton in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void removeLabelFromParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          Label label = (Label) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.removeMember(parentTab, label);*/
    }

    /**
     * Set a figure as parent for a CheckBox
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the PushButton in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setCheckBoxParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          CheckBox checkBox = (CheckBox) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.addMember(parentTab, checkBox);*/
    }

    /**
     * Remove a CheckBox from its parent figure
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the PushButton in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void removeCheckBoxFromParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          CheckBox checkBox = (CheckBox) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.removeMember(parentTab, checkBox);*/
    }

    /**
     * Set a figure as parent for a RadioButton
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the PushButton in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setRadioButtonParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          RadioButton radioButton = (RadioButton) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.addMember(parentTab, radioButton);*/
    }

    /**
     * Remove a RadioButton from its parent figure
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the PushButton in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void removeRadioButtonFromParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          RadioButton radioButton = (RadioButton) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.removeMember(parentTab, radioButton);*/
    }

    /**
     * Set a figure as parent for a UiTable
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the PushButton in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setUiTableParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          UiTable uiTable = (UiTable) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.addMember(parentTab, uiTable);*/
    }

    /**
     * Remove a UiTable from its parent figure
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the PushButton in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void removeUiTableFromParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          UiTable uiTable = (UiTable) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.removeMember(parentTab, uiTable);*/
    }

    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setUiDisplayTreeParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          UiDisplayTree uiTree = (UiDisplayTree) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.addMember(parentTab, uiTree);*/
    }
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void removeUiDisplayTreeFromParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          UiDisplayTree uiTree = (UiDisplayTree) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.removeMember(parentTab, uiTree);*/
    }

    /**
     * Set a figure as parent for a Slider
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the PushButton in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setSliderParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          Slider slider = (Slider) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.addMember(parentTab, slider);*/
    }

    /**
     * Remove a Slider from its parent figure
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the Slider in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void removeSliderFromParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          Slider slider = (Slider) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.removeMember(parentTab, slider);*/
    }

    /**
     * Set a figure as parent for a ListBox
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the ListBox in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setListBoxParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          ListBox listBox = (ListBox) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.addMember(parentTab, listBox);*/
    }

    /**
     * Remove a ListBox from its parent figure
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the ListBox in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void removeListBoxFromParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          ListBox listBox = (ListBox) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.removeMember(parentTab, listBox);*/
    }

    /**
     * Set a figure as parent for a PopupMenu
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the PopupMenu in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setPopupMenuParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          PopupMenu popupMenu = (PopupMenu) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.addMember(parentTab, popupMenu);*/
    }

    /**
     * Remove a PopupMenu from its parent figure
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the PopupMenu in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void removePopupMenuFromParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          PopupMenu popupMenu = (PopupMenu) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.removeMember(parentTab, popupMenu);*/
    }

    /**
     * Set a figure as parent for a Frame
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the PopupMenu in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setFrameParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          Frame frame = (Frame) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.addMember(parentTab, frame);*/
    }

    /**
     * Remove a Frame from its parent figure
     * @param figureID the ID of the figure in the FigureMapper
     * @param objID the ID of the PopupMenu in the UIElementMapper
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void removeFrameFromParent(int figureID, int objID) {
        /*Tab parentTab = ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(figureID).getRendererProperties()).getParentTab();
          Frame frame = (Frame) UIElementMapper.getCorrespondingUIElement(objID);
          ScilabBridge.removeMember(parentTab, frame);*/
    }

    /**
     * Set root Scilab object (the console tab) as the parent of the menu
     * @param objID the id of the menu
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setRootAsParent(int objID) {
        if (!(UIElementMapper.getCorrespondingUIElement(objID) instanceof MenuItem)) {
            // Add the menu to the tab
            ScilabConsole.getConsole().getMenuBar().add((Menu) UIElementMapper.getCorrespondingUIElement(objID));
        } else {
            // obj is a MenuItem that has top be converted to a Menu
            MenuItem menuItem = (MenuItem) UIElementMapper.getCorrespondingUIElement(objID);

            Menu menuToAdd = ScilabMenu.createMenu();
            UIElementMapper.removeMapping(objID);
            UIElementMapper.addMapping(objID, menuToAdd);
            // Copy all properties from MenuItem to Menu
            menuToAdd.setText(menuItem.getText());
            menuToAdd.setCallback(menuItem.getCallback());
            menuToAdd.setForeground(menuItem.getForeground());
            menuToAdd.setVisible(menuItem.isVisible());
            // End of properties copy

            ScilabConsole.getConsole().getMenuBar().add(menuToAdd);
        }
    }

    /**
     * Set an other menu as the parent of the menu
     * @param menuID the id of the parent menu
     * @param objID the id of the menu
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setMenuAsParent(int menuID, int objID) {

        if (UIElementMapper.getCorrespondingUIElement(menuID) instanceof CheckBoxMenuItem) {
            CheckBoxMenuItem parentMenu = (CheckBoxMenuItem) UIElementMapper.getCorrespondingUIElement(menuID);

            if (!(UIElementMapper.getCorrespondingUIElement(objID) instanceof MenuItem)) {
                parentMenu.add((Menu) UIElementMapper.getCorrespondingUIElement(objID));
            } else if (UIElementMapper.getCorrespondingUIElement(objID) instanceof MenuItem) {
                parentMenu.add((MenuItem) UIElementMapper.getCorrespondingUIElement(objID));
            }
        } else if (UIElementMapper.getCorrespondingUIElement(menuID) instanceof MenuItem) {
            MenuItem parentMenu = (MenuItem) UIElementMapper.getCorrespondingUIElement(menuID);

            if (!(UIElementMapper.getCorrespondingUIElement(objID) instanceof MenuItem)) {
                parentMenu.add((Menu) UIElementMapper.getCorrespondingUIElement(objID));
            } else if (UIElementMapper.getCorrespondingUIElement(objID) instanceof MenuItem) {
                parentMenu.add((MenuItem) UIElementMapper.getCorrespondingUIElement(objID));
            }
        } else if (UIElementMapper.getCorrespondingUIElement(menuID) instanceof ContextMenu) {
            ContextMenu parentMenu = (ContextMenu) UIElementMapper.getCorrespondingUIElement(menuID);

            if (!(UIElementMapper.getCorrespondingUIElement(objID) instanceof MenuItem)) {
                parentMenu.add((Menu) UIElementMapper.getCorrespondingUIElement(objID));
            } else if (UIElementMapper.getCorrespondingUIElement(objID) instanceof MenuItem) {
                parentMenu.add((MenuItem) UIElementMapper.getCorrespondingUIElement(objID));
            }
        } else if (UIElementMapper.getCorrespondingUIElement(menuID) instanceof Menu) {
            Menu parentMenu = (Menu) UIElementMapper.getCorrespondingUIElement(menuID);

            if (!(UIElementMapper.getCorrespondingUIElement(objID) instanceof MenuItem)) {
                parentMenu.add((Menu) UIElementMapper.getCorrespondingUIElement(objID));
            } else if (UIElementMapper.getCorrespondingUIElement(objID) instanceof MenuItem) {
                parentMenu.add((MenuItem) UIElementMapper.getCorrespondingUIElement(objID));
            }
        }
    }

    /*******************/
    /*                 */
    /* OBJECT CALLBACK */
    /*                 */
    /*******************/

    /**
     * Set a callback for a Widget
     * @param objID the ID of the object in the UIElementMapper
     * @param callbackString the text of the callback
     * @param callbackType the type of the callback
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setWidgetCallback(int objID, String callbackString, int callbackType) {
        //((Widget) UIElementMapper.getCorrespondingUIElement(objID))
        //.setCallback(CommonCallBack.createCallback(callbackString, callbackType, objID));
    }

    /**
     * Set a callback for a Frame
     * @param objID the ID of the object in the UIElementMapper
     * @param callbackString the text of the callback
     * @param callbackType the type of the callback
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setFrameCallback(int objID, String callbackString, int callbackType) {
        //((Frame) UIElementMapper.getCorrespondingUIElement(objID)).setCallback(CommonCallBack.createCallback(callbackString, callbackType, objID));
    }

    /************************/
    /*                      */
    /* ENABLE/DISABLE MENUS */
    /*                      */
    /************************/

    /**
     * Enable/Disable a menu of a Scilab figure or console giving its name
     * @param parentUID the UID of the figure or console
     * @param menuName the name of the menu
     * @param status true to set the menu enabled
     */
    public static void setMenuEnabled(int parentUID, String menuName, boolean status) {
        SwingScilabTab parentTab = (SwingScilabTab) SwingView.getFromId(parentUID);
        if (parentTab != null) { /** Parent must exist */
            parentTab.getMenuBar().getAsSimpleMenuBar().setMenuEnabled(menuName, status);
        }
    }

    /**
     * Disable a MenuItem of a Scilab figure or console giving its parent name and position
     * @param parentUID the UID of the figure or console
     * @param parentMenuName the name of the parent menu
     * @param menuItemPosition the name of the parent menu
     * @param status true to set the menu enabled
     */
    public static void setSubMenuEnabled(int parentUID, String parentMenuName, int menuItemPosition, boolean status) {
        SwingScilabTab parentTab = (SwingScilabTab) SwingView.getFromId(parentUID);
        if (parentTab != null) { /** Parent must exist */
            parentTab.getMenuBar().getAsSimpleMenuBar().setSubMenuEnabled(parentMenuName, menuItemPosition, status);
        }
    }

    /****************/
    /*              */
    /* REMOVE MENUS */
    /*              */
    /****************/

    /**
     * Delete a menu of a Scilab figure or console giving its name
     * @param parentUID the UID of the figure or console
     * @param menuName the name of the menu
     */
    public static void removeMenu(int parentUID, String menuName) {
        SwingScilabTab parentTab = (SwingScilabTab) SwingView.getFromId(parentUID);
        if (parentTab != null) { /** Parent must exist */
            parentTab.getMenuBar().getAsSimpleMenuBar().removeMenu(menuName);
        }
    }

    /***********************/
    /*                     */
    /* FILE CHOOSER BRIDGE */
    /*                     */
    /***********************/



    /**
     * Create a new Graphic Export File Chooser in Scilab GUIs
     * @param figureId id of the figure to export
     * @return the ID of the File Chooser in the UIElementMapper
     */

    public static int newExportFileChooser(int figureId) {
        FileChooser fileChooser = ScilabFileChooser.createExportFileChooser(figureId);
        return 0;
    }



    /**********************/
    /*                    */
    /* MESSAGE BOX BRIDGE */
    /*                    */
    /**********************/

    /**
     * Set the MessageBox title
     * @param id the id of the messageBox
     * @param title the title of the messageBox
     */
    public static void setMessageBoxTitle(int id, String title) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setTitle(title);
    }

    /**
     * Set the MessageBox message
     * @param id the id of the messageBox
     * @param message the message of the messageBox
     */
    public static void setMessageBoxMessage(int id, String message) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setMessage(message);
    }

    /**
     * Set the MessageBox message (multi-line)
     * @param id the id of the messageBox
     * @param message the message of the messageBox
     */
    public static void setMessageBoxMessage(int id, String[] message) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setMessage(message);
    }

    /**
     * Display this chooser and wait for user selection
     * @param id the id of the messageBox
     */
    public static void messageBoxDisplayAndWait(int id) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).displayAndWait();
    }

    /**
     * Get the index of the selected button
     * @param id the id of the messageBox
     * @return the index of the selected button
     */
    public static int getMessageBoxSelectedButton(int id) {
        return ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).getSelectedButton();
    }

    /**
     * Set the indices of the default selected buttons (x_choices)
     * @param id the id of the MessageBox
     * @param indices the indices of the default selected buttons
     */
    public static void setMessageBoxDefaultSelectedButtons(int id, int[] indices) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setDefaultSelectedButtons(indices);
    }

    /**
     * Get the indices of the user selected buttons (x_choices)
     * @param id the id of the MessageBox
     * @return the indices of the user selected buttons
     */
    public static int[] getMessageBoxUserSelectedButtons(int id) {
        return ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).getUserSelectedButtons();
    }

    /**
     * Set the labels of the buttons in the MessageBox
     * @param id the id of the MessageBox
     * @param labels the labels of the buttons
     */
    public static void setMessageBoxButtonsLabels(int id, String[] labels) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setButtonsLabels(labels);
    }

    /**
     * Set the initial value for the editable part of the MessageBox
     * @param id the id of the MessageBox
     * @param value the initial value
     */
    public static void setMessageBoxInitialValue(int id, String[] value) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setInitialValue(value);
    }

    /**
     * Get the value of the editable zone in the MessageBox
     * @param id the id of the MessageBox
     * @return the value
     */
    public static String[] getMessageBoxValue(int id) {
        return ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).getValue();
    }

    /**
     * Get the size of the value of the editable zone in the MessageBox
     * @param id the id of the MessageBox
     * @return the value size
     */
    public static int getMessageBoxValueSize(int id) {
        return ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).getValueSize();
    }

    /**
     * Set the items of the listbox in the MessageBox
     * @param id the id of the MessageBox
     * @param items the items to set
     */
    public static void setMessageBoxListBoxItems(int id, String[] items) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setListBoxItems(items);
    }

    /**
     * Get the index of the selected item in the listbox in the MessageBox
     * @param id the id of the MessageBox
     * @return the index
     */
    public static int getMessageBoxSelectedItem(int id) {
        return ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).getSelectedItem();
    }

    /**
     * Set the name of the lines labels in the editable zone in the MessageBox
     * @param id the id of the MessageBox
     * @param labels the labels
     */
    public static void setMessageBoxLineLabels(int id, String[] labels) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setLineLabels(labels);
    }

    /**
     * Set the name of the columns labels in the editable zone in the MessageBox
     * @param id the id of the MessageBox
     * @param labels the labels
     */
    public static void setMessageBoxColumnLabels(int id, String[] labels) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setColumnLabels(labels);
    }

    /**
     * Set the default values of a multi-value editable zone in the MessageBox
     * @param id the id of the MessageBox
     * @param values the values
     */
    public static void setMessageBoxDefaultInput(int id, String[] values) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setDefaultInput(values);
    }

    /**
     * Set a MessageBox modal or not
     * @param id the id of the MessageBox
     * @param status true to set the MessageBox modal and false else
     */
    public static void setMessageBoxModal(int id, boolean status) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setModal(status);
    }

    /**
     * Set the MessageBox icon
     * @param id the id of the MessageBox
     * @param name the name of the icon
     */
    public static void setMessageBoxIcon(int id, String name) {
        ((MessageBox) UIElementMapper.getCorrespondingUIElement(id)).setIcon(name);
    }

    /************************/
    /*                      */
    /* OBJECT COLORS BRIDGE */
    /*                      */
    /************************/

    /**
     * Builds a color with composants between 0 and 255
     * @param red the red value for the color
     * @param green the green value for the color
     * @param blue the blue value for the color
     */
    public static Color getGoodColor(int red, int green, int blue) {
        int r = Math.max(0, Math.min(red, 255));
        int g = Math.max(0, Math.min(green, 255));
        int b = Math.max(0, Math.min(blue, 255));

        return new Color(r, g, b);
    }

    /**
     * Set the background color of a Widget
     * @param id the id of the Widget
     * @param red the red value for the color
     * @param green the green value for the color
     * @param blue the blue value for the color
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setWidgetBackgroundColor(int id, int red, int green, int blue) {
        ((Widget) UIElementMapper.getCorrespondingUIElement(id)).setBackground(getGoodColor(red, green, blue));
    }

    /**
     * Get the background color of a widget
     * @param id the id of the widget
     * @return the color [R, G, B]
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int[] getWidgetBackgroundColor(int id) {
        Color tmpColor = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getBackground();
        int[] color = new int[NB_COLORS];
        color[0] = tmpColor.getRed();
        color[1] = tmpColor.getGreen();
        color[2] = tmpColor.getBlue();
        return color;
    }

    /**
     * Set the foreground color of a Widget
     * @param id the id of the Widget
     * @param red the red value for the color
     * @param green the green value for the color
     * @param blue the blue value for the color
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setWidgetForegroundColor(int id, int red, int green, int blue) {
        ((Widget) UIElementMapper.getCorrespondingUIElement(id)).setForeground(getGoodColor(red, green, blue));
    }

    /**
     * Get the foreground color of a Widget
     * @param id the id of the Widget
     * @return the color [R, G, B]
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int[] getWidgetForegroundColor(int id) {
        Color tmpColor = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getForeground();
        int[] color = new int[NB_COLORS];
        color[0] = tmpColor.getRed();
        color[1] = tmpColor.getGreen();
        color[2] = tmpColor.getBlue();
        return color;
    }

    /**
     * Set the background color of a Frame
     * @param id the id of the Frame
     * @param red the red value for the color
     * @param green the green value for the color
     * @param blue the blue value for the color
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setFrameBackgroundColor(int id, int red, int green, int blue) {
        ((Frame) UIElementMapper.getCorrespondingUIElement(id)).setBackground(getGoodColor(red, green, blue));
    }

    /**
     * Get the background color of a Frame
     * @param id the id of the Frame
     * @return the color [R, G, B]
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int[] getFrameBackgroundColor(int id) {
        Color tmpColor = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getBackground();
        int[] color = new int[NB_COLORS];
        color[0] = tmpColor.getRed();
        color[1] = tmpColor.getGreen();
        color[2] = tmpColor.getBlue();
        return color;
    }

    /**
     * Set the foreground color of a Frame
     * @param id the id of the Frame
     * @param red the red value for the color
     * @param green the green value for the color
     * @param blue the blue value for the color
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setFrameForegroundColor(int id, int red, int green, int blue) {
        ((Frame) UIElementMapper.getCorrespondingUIElement(id)).setForeground(getGoodColor(red, green, blue));
    }

    /**
     * Get the foreground color of a Frame
     * @param id the id of the Frame
     * @return the color [R, G, B]
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int[] getFrameForegroundColor(int id) {
        Color tmpColor = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getForeground();
        int[] color = new int[NB_COLORS];
        color[0] = tmpColor.getRed();
        color[1] = tmpColor.getGreen();
        color[2] = tmpColor.getBlue();
        return color;
    }

    /**********************/
    /*                    */
    /* OBJECT FONT BRIDGE */
    /*                    */
    /**********************/

    /**
     * Set the name of a Widget font
     * @param id the id of the Widget
     * @param name the name of the Widget font
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setWidgetFontName(int id, String name) {
        Font font = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getFont();
        font = new Font(name, font.getStyle(), font.getSize());
        ((Widget) UIElementMapper.getCorrespondingUIElement(id)).setFont(font);
    }

    /**
     * Get the name of a Widget font
     * @param id the id of the Widget
     * @return the name of the Widget font
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static String getWidgetFontName(int id) {
        return ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getFont().getName();
    }

    /**
     * Set the weight of a Widget font
     * @param id the id of the Widget
     * @param weight the weight of the Widget font
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setWidgetFontWeight(int id, String weight) {
        Font font = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getFont();

        if (weight.equals(BOLDFONT)) {
            if (font.isItalic()) {
                font = new Font(font.getName(), Font.ITALIC + Font.BOLD, font.getSize());
            } else {
                font = new Font(font.getName(), Font.BOLD, font.getSize());
            }
        } else {
            if (font.isItalic()) {
                font = new Font(font.getName(), Font.ITALIC, font.getSize());
            } else {
                font = new Font(font.getName(), Font.PLAIN, font.getSize());
            }
        }

        ((Widget) UIElementMapper.getCorrespondingUIElement(id)).setFont(font);
    }

    /**
     * Set the angle of a Widget font
     * @param id the id of the Widget
     * @param angle the angle of the Widget font
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setWidgetFontAngle(int id, String angle) {
        Font font = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getFont();

        if (angle.equals(ITALICFONT) || angle.equals(OBLIQUEFONT)) {
            if (font.isBold()) {
                font = new Font(font.getName(), Font.ITALIC + Font.BOLD, font.getSize());
            } else {
                font = new Font(font.getName(), Font.ITALIC, font.getSize());
            }
        } else {
            if (font.isBold()) {
                font = new Font(font.getName(), Font.BOLD, font.getSize());
            } else {
                font = new Font(font.getName(), Font.PLAIN, font.getSize());
            }
        }

        ((Widget) UIElementMapper.getCorrespondingUIElement(id)).setFont(font);
    }

    /**
     * Set the size of a Widget font
     * @param id the id of the Widget
     * @param size the size of the Widget font
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setWidgetFontSize(int id, int size) {
        Font font = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getFont();
        font = new Font(font.getName(), font.getStyle(), size);
        ((Widget) UIElementMapper.getCorrespondingUIElement(id)).setFont(font);
    }

    /**
     * Get the size of a Widget font
     * @param id the id of the Widget
     * @return the size of the Widget font
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int getWidgetFontSize(int id) {
        return ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getFont().getSize();
    }

    /**
     * Set the weight of a Frame font
     * @param id the id of the Frame
     * @param weight the weight of the Frame font
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setFrameFontWeight(int id, String weight) {
        Font font = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getFont();

        if (weight.equals(BOLDFONT)) {
            if (font.isItalic()) {
                font = new Font(font.getName(), Font.ITALIC + Font.BOLD, font.getSize());
            } else {
                font = new Font(font.getName(), Font.BOLD, font.getSize());
            }
        } else {
            if (font.isItalic()) {
                font = new Font(font.getName(), Font.ITALIC, font.getSize());
            } else {
                font = new Font(font.getName(), Font.PLAIN, font.getSize());
            }
        }

        ((Frame) UIElementMapper.getCorrespondingUIElement(id)).setFont(font);
    }

    /**
     * Set the angle of a Frame font
     * @param id the id of the Frame
     * @param angle the angle of the Frame font
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setFrameFontAngle(int id, String angle) {
        Font font = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getFont();

        if (angle.equals(ITALICFONT) || angle.equals(OBLIQUEFONT)) {
            if (font.isBold()) {
                font = new Font(font.getName(), Font.ITALIC + Font.BOLD, font.getSize());
            } else {
                font = new Font(font.getName(), Font.ITALIC, font.getSize());
            }
        } else {
            if (font.isBold()) {
                font = new Font(font.getName(), Font.BOLD, font.getSize());
            } else {
                font = new Font(font.getName(), Font.PLAIN, font.getSize());
            }
        }

        ((Frame) UIElementMapper.getCorrespondingUIElement(id)).setFont(font);
    }

    /**
     * Set the size of a Frame font
     * @param id the id of the Frame
     * @param size the size of the Frame font
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setFrameFontSize(int id, int size) {
        Font font = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getFont();
        font = new Font(font.getName(), font.getStyle(), size);
        ((Frame) UIElementMapper.getCorrespondingUIElement(id)).setFont(font);
    }

    /**
     * Get the size of a Frame font
     * @param id the id of the Frame
     * @return the size of the Frame font
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int getFrameFontSize(int id) {
        return ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getFont().getSize();
    }

    /**
     * Set the name of a Frame font
     * @param id the id of the Frame
     * @param name the name of the Frame font
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setFrameFontName(int id, String name) {
        Font font = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getFont();
        font = new Font(name, font.getStyle(), font.getSize());
        ((Frame) UIElementMapper.getCorrespondingUIElement(id)).setFont(font);
    }

    /**
     * Get the name of a Frame font
     * @param id the id of the Frame
     * @return the name of the Frame font
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static String getFrameFontName(int id) {
        return ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getFont().getName();
    }

    /**************************/
    /*                        */
    /* OBJECT POSITION BRIDGE */
    /*                        */
    /**************************/

    /**
     * Set the position (in Scilab terms) of a Widget
     * @param id the id of the Widget
     * @param x the X-coordinate for the Widget
     * @param y the Y-coordinate for the Widget
     * @param width the width of the Widget
     * @param height the height of the Widget
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setWidgetPosition(int id, int x, int y, int width, int height) {
        /* Dimensions must be set before position beacause of PositionConverter */
        UIElementMapper.getCorrespondingUIElement(id).setDims(new Size(width, height));
        UIElementMapper.getCorrespondingUIElement(id).setPosition(new Position(x, y));
    }

    /**
     * Get the position (in Scilab terms) of a widget
     * @param id the id of the widget
     * @return the position (X-coordinate, Y-coordinate, width, height) of the button
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int[] getWidgetPosition(int id) {
        int[] position = new int[POSITION_SIZE];

        position[X_INDEX] = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getPosition().getX();
        position[Y_INDEX] = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getPosition().getY();
        position[WIDTH_INDEX] = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getDims().getWidth();
        position[HEIGHT_INDEX] = ((Widget) UIElementMapper.getCorrespondingUIElement(id)).getDims().getHeight();

        return position;
    }

    /**
     * Set the position (in Scilab terms) of a Frame
     * @param id the id of the Frame
     * @param x the X-coordinate for the Frame
     * @param y the Y-coordinate for the Frame
     * @param width the width of the Frame
     * @param height the height of the Frame
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setFramePosition(int id, int x, int y, int width, int height) {
        /* Dimensions must be set before position beacause of PositionConverter */
        UIElementMapper.getCorrespondingUIElement(id).setDims(new Size(width, height));
        UIElementMapper.getCorrespondingUIElement(id).setPosition(new Position(x, y));
    }

    /**
     * Get the position (in Scilab terms) of a Frame
     * @param id the id of the Frame
     * @return the position (X-coordinate, Y-coordinate, width, height) of the button
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int[] getFramePosition(int id) {
        int[] position = new int[POSITION_SIZE];

        position[X_INDEX] = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getPosition().getX();
        position[Y_INDEX] = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getPosition().getY();
        position[WIDTH_INDEX] = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getDims().getWidth();
        position[HEIGHT_INDEX] = ((Frame) UIElementMapper.getCorrespondingUIElement(id)).getDims().getHeight();

        return position;
    }

    /***************************/
    /*                         */
    /* OBJECT ALIGNMENT BRIDGE */
    /*                         */
    /***************************/

    /**
     * Set the horizontal alignment for the Widget text
     * @param id the id of the Widget
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setWidgetHorizontalAlignment(int id, String alignment) {
        ((Widget) UIElementMapper.getCorrespondingUIElement(id)).setHorizontalAlignment(alignment);
    }

    /**
     * Set the vertical alignment for the Widget text
     * @param id the id of the Widget
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setWidgetVerticalAlignment(int id, String alignment) {
        ((Widget) UIElementMapper.getCorrespondingUIElement(id)).setVerticalAlignment(alignment);
    }

    /**********************/
    /*                    */
    /* SLIDER STEP BRIDGE */
    /*                    */
    /**********************/

    /**
     * Set the minor tick spacing for a Slider
     * @param id the id of the Slider
     * @param space the increment value
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setSliderMinorTickSpacing(int id, int space) {
        ((Slider) UIElementMapper.getCorrespondingUIElement(id)).setMinorTickSpacing(space);
    }

    /**
     * Set the major tick spacing for a Slider
     * @param id the id of the Slider
     * @param space the increment value
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setSliderMajorTickSpacing(int id, int space) {
        ((Slider) UIElementMapper.getCorrespondingUIElement(id)).setMajorTickSpacing(space);
    }

    /*************************/
    /*                       */
    /* SLIDER MIN/MAX BRIDGE */
    /*                       */
    /*************************/

    /**
     * Set the minimum value of a Slider
     * @param id the id of the Slider
     * @param value the minimum value
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setSliderMinValue(int id, int value) {
        ((Slider) UIElementMapper.getCorrespondingUIElement(id)).setMinimumValue(value);
    }

    /**
     * Set the maximum value of a Slider
     * @param id the id of the Widget
     * @param value the maximum value
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setSliderMaxValue(int id, int value) {
        ((Slider) UIElementMapper.getCorrespondingUIElement(id)).setMaximumValue(value);
    }

    /*****************************/
    /*                           */
    /* SLIDER ORIENTATION BRIDGE */
    /*                           */
    /*****************************/

    /**
     * Set the slider orientation to vertical
     * @param id the id of the slider
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setSliderVertical(int id) {
        ((Slider) UIElementMapper.getCorrespondingUIElement(id)).setVertical();
    }

    /**
     * Set the slider orientation to horizontal
     * @param id the id of the slider
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setSliderHorizontal(int id) {
        ((Slider) UIElementMapper.getCorrespondingUIElement(id)).setHorizontal();
    }

    /******************/
    /*                */
    /* LISTBOX BRIDGE */
    /*                */
    /******************/

    /**
     * Set if more than one item can be selected in a ListBox
     * @param id the id of the ListBox
     * @param status true if multiple selection is enabled
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setListBoxMultipleSelectionEnabled(int id, boolean status) {
        ((ListBox) UIElementMapper.getCorrespondingUIElement(id)).setMultipleSelectionEnabled(status);
    }

    /**
     * Get the text of all the list items
     * @param id the id of the ListBox
     * @return the text items
     * @see org.scilab.modules.gui.listbox.ListBox#getAllItemsText()
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static String[] getListBoxAllItemsText(int id) {
        return ((ListBox) UIElementMapper.getCorrespondingUIElement(id)).getAllItemsText();
    }

    /**
     * Get the number of items in the list
     * @param id the id of the ListBox
     * @return the number of items
     * @see org.scilab.modules.gui.listbox.ListBox#getNumberOfItems()
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int getListBoxNumberOfItems(int id) {
        return ((ListBox) UIElementMapper.getCorrespondingUIElement(id)).getNumberOfItems();
    }

    /**
     * Set the text of the list items
     * @param id the id of the ListBox
     * @param text the text of the items
     * @see org.scilab.modules.gui.listbox.ListBox#setText(java.lang.String[])
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setListBoxText(int id, String[] text) {
        ((ListBox) UIElementMapper.getCorrespondingUIElement(id)).setText(text);
    }

    /**
     * Adjusts the view so that the element given by index is displayed at the top of the ListBox.
     * @param id the id of the ListBox
     * @param index the index of the element to be displayed at the top of the ListBox.
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setListBoxListBoxTop(int id, int index) {
        ((ListBox) UIElementMapper.getCorrespondingUIElement(id)).setListBoxTop(index);
    }

    /**
     * Gets the index of the element displayed at the top of the ListBox
     * @param id the id of the ListBox
     * @return the index of the element displayed at the top of the ListBox
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int getListBoxListBoxTop(int id) {
        return ((ListBox) UIElementMapper.getCorrespondingUIElement(id)).getListBoxTop();
    }

    /********************/
    /*                  */
    /* POPUPMENU BRIDGE */
    /*                  */
    /********************/

    /**
     * Get the text of all the PopupMenu items
     * @param id the id of the PopupMenu
     * @return the text items
     * @see org.scilab.modules.gui.popupmenu.PopupMenu#getAllItemsText()
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static String[] getPopupMenuAllItemsText(int id) {
        return ((PopupMenu) UIElementMapper.getCorrespondingUIElement(id)).getAllItemsText();
    }

    /**
     * Get the number of items in the PopupMenu
     * @param id the id of the PopupMenu
     * @return the number of items
     * @see org.scilab.modules.gui.popupmenu.PopupMenu#getNumberOfItems()
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int getPopupMenuNumberOfItems(int id) {
        return ((PopupMenu) UIElementMapper.getCorrespondingUIElement(id)).getNumberOfItems();
    }

    /**
     * Set the text of the PopupMenu items
     * @param id the id of the PopupMenu
     * @param text the text of the items
     * @see org.scilab.modules.gui.popupmenu.PopupMenu#setText(java.lang.String[])
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setPopupMenuText(int id, String[] text) {
        ((PopupMenu) UIElementMapper.getCorrespondingUIElement(id)).setText(text);
    }

    /****************/
    /*              */
    /* VALUE BRIDGE */
    /*              */
    /****************/

    /**
     * Set the selected indices of the ListBox
     * @param id the id of the ListBox
     * @param indices the indices of the items to be selected
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setListBoxSelectedIndices(int id, int[] indices) {
        ((ListBox) UIElementMapper.getCorrespondingUIElement(id)).setSelectedIndices(indices);
    }

    /**
     * Get the selected indices of the ListBox
     * @param id the id of the ListBox
     * @return the indices of the items selected
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int[] getListBoxSelectedIndices(int id) {
        return ((ListBox) UIElementMapper.getCorrespondingUIElement(id)).getSelectedIndices();
    }

    /**
     * Get the number of items selected in the ListBox
     * @param id the id of the ListBox
     * @return the number of items selected
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int getListBoxSelectionSize(int id) {
        return ((ListBox) UIElementMapper.getCorrespondingUIElement(id)).getSelectionSize();
    }

    /**
     * Set the selected index of the PopupMenu
     * @param id the id of the PopupMenu
     * @param index the index of the item to be selected
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setPopupMenuSelectedIndex(int id, int index) {
        ((PopupMenu) UIElementMapper.getCorrespondingUIElement(id)).setUserSelectedIndex(index);
    }

    /**
     * Get the selected index of the PopupMenu
     * @param id the id of the PopupMenu
     * @return the index of the item selected
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int getPopupMenuSelectedIndex(int id) {
        return ((PopupMenu) UIElementMapper.getCorrespondingUIElement(id)).getUserSelectedIndex();
    }

    /**
     * Get the current value of the Slider
     * @param id the id of the Slider
     * @return the current value of the Slider
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static int getSliderValue(int id) {
        return ((Slider) UIElementMapper.getCorrespondingUIElement(id)).getValue();
    }

    /**
     * Sets the current value of the Slider
     * @param id the id of the Slider
     * @param value the new value
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setSliderValue(int id, int value) {
        ((Slider) UIElementMapper.getCorrespondingUIElement(id)).setUserValue(value);
    }

    /**
     * Set if the RadioButton is checked or not
     * @param id the id of the RadioButton
     * @param status true to set the RadioButton checked
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setRadioButtonChecked(int id, boolean status) {
        ((RadioButton) UIElementMapper.getCorrespondingUIElement(id)).setChecked(status);
    }

    /**
     * Get the status of the RadioButton
     * @param id the id of the RadioButton
     * @return true if the RadioButton is checked
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static boolean isRadioButtonChecked(int id) {
        return ((RadioButton) UIElementMapper.getCorrespondingUIElement(id)).isChecked();
    }

    /**
     * Set if the CheckBox is checked or not
     * @param id the id of the CheckBox
     * @param status true to set the CheckBox checked
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setCheckBoxChecked(int id, boolean status) {
        ((CheckBox) UIElementMapper.getCorrespondingUIElement(id)).setChecked(status);
    }

    /**
     * Get the status of the CheckBox
     * @param id the id of the CheckBox
     * @return true if the CheckBox is checked
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static boolean isCheckBoxChecked(int id) {
        return ((CheckBox) UIElementMapper.getCorrespondingUIElement(id)).isChecked();
    }

    /*****************/
    /*               */
    /* RELIEF BRIDGE */
    /*               */
    /*****************/

    /**
     * Set the Relief of the Widget
     * @param id the id of the Widget
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setWidgetRelief(int id, String reliefType) {
        ((Widget) UIElementMapper.getCorrespondingUIElement(id)).setRelief(reliefType);
    }

    /**
     * Set the Relief of the Frame
     * @param id the id of the Frame
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setFrameRelief(int id, String reliefType) {
        ((Frame) UIElementMapper.getCorrespondingUIElement(id)).setRelief(reliefType);
    }

    /******************/
    /*                */
    /* TOOLBAR BRIDGE */
    /*                */
    /******************/

    /**
     * Set the visibility of a Toolbar
     * @param parentUID the parent (figure or console) UID
     * @param status true to set the Toolbar visible
     */
    public static void setToolbarVisible(int parentUID, boolean status) {
        SwingScilabTab parentTab = (SwingScilabTab) SwingView.getFromId(parentUID);
        if (parentTab != null) {
            boolean oldStatus = parentTab.getToolBar().getAsSimpleToolBar().isVisible();
            if (oldStatus != status) {
                parentTab.getToolBar().getAsSimpleToolBar().setVisible(status);
                BarUpdater.updateBars(parentTab.getParentWindowId(), parentTab.getMenuBar(),
                                      parentTab.getToolBar(), parentTab.getInfoBar(), parentTab.getName(), parentTab.getWindowIcon());
            }
        }
    }

    /**
     * Get the visibility of a Toolbar
     * @param parentUID the parent (figure or console) UID
     * @return true to set the Toolbar visible
     */
    public static boolean isToolbarVisible(int parentUID) {
        SwingScilabTab parentTab = (SwingScilabTab) SwingView.getFromId(parentUID);
        if (parentTab != null) {
            return parentTab.getToolBar().getAsSimpleToolBar().isVisible();
        } else {
            return false;
        }
    }

    /***********************/
    /*                     */
    /* EVENTHANDLER BRIDGE */
    /*                     */
    /***********************/

    /**
     * Set the event handler of the figure
     * @param figNum the figure
     * @param command the name of the Scilab function to call
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setEventHandler(int figNum, String command) {
        //FigureMapper.getCorrespondingFigure(figNum).getRendererProperties().setEventHandler(command);
    }

    /**
     * Set the status of the event handler of the figure
     * @param figNum the figure
     * @param status is true to set the event handler active
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setEventHandlerEnabled(int figNum, boolean status) {
        //FigureMapper.getCorrespondingFigure(figNum).getRendererProperties().setEventHandlerEnabled(status);
    }

    /******************/
    /*                */
    /* WAITBAR BRIDGE */
    /*                */
    /******************/

    /**
     * Set the WaitBar message (multi-line)
     * @param id the id of the waitBar
     * @param message the message of the waitBar
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setWaitBarMessage(int id, String[] message) {
        ((WaitBar) UIElementMapper.getCorrespondingUIElement(id)).setMessage(message);
    }

    /**
     * Set the WaitBar value
     * @param id the id of the waitBar
     * @param value the value of the waitBar
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setWaitBarValue(int id, int value) {
        ((WaitBar) UIElementMapper.getCorrespondingUIElement(id)).setValue(value);
    }

    /**
     * Destroy a WaitBar
     * @param id the id of the WaitBar to destroy
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void destroyWaitBar(int id) {
        ((WaitBar) UIElementMapper.getCorrespondingUIElement(id)).close();
    }

    /**
     * Indicates if the total execution time is known
     * @param id the id of the WaitBar to destroy
     * @param status true if the total progress time in unknown
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setWaitBarIndeterminateMode(int id, boolean status) {
        ((WaitBar) UIElementMapper.getCorrespondingUIElement(id)).setIndeterminateMode(status);
    }

    /**********************/
    /*                    */
    /* HELPBROWSER BRIDGE */
    /*                    */
    /**********************/

    /**
     * Launch Scilab help browser if not already created
     * @param helps help chapters and directories
     * @param language Scilab current language
     */
    public static void launchHelpBrowser(final String[] helps, final String language) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                try {
                    ScilabHelpBrowser.createHelpBrowser(helps, language);
                    ScilabHelpBrowser.startHomePage();
                } catch (Exception e) {
                    System.err.println(e);
                }
            }
        });
    }

    /**
     * Search a keyword in Scilab Help Browser
     * @param keyword the keyword to search
     * @param helps help chapters and directories
     * @param language Scilab current language
     * @param fullText true for a full-text search
     */
    public static void searchKeyword(final String[] helps, final String keyword, final String language, final boolean fullText) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                try {
                    if (fullText) {
                        HelpBrowser helpBrowser = ScilabHelpBrowser.createHelpBrowser(helps, language);
                        if (helpBrowser != null) {
                            helpBrowser.fullTextSearch(keyword);
                        }
                    } else {
                        HelpBrowser helpBrowser = ScilabHelpBrowser.createHelpBrowser(helps, language);
                        if (helpBrowser != null) {
                            helpBrowser.searchKeywork(keyword);
                        }
                    }
                } catch (Exception e) {
                    System.err.println(e);
                }
            }
        });
    }

    /**
     * Open HelpBrowser on the page with the given xmlID
     * @param xmlID the xml id
     */
    public static void openHelp(final String xmlID) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                try {
                    HelpBrowser helpBrowser = ScilabHelpBrowser.createHelpBrowser(null, ScilabCommons.getlanguage());
                    if (helpBrowser != null) {
                        helpBrowser.searchKeywork(xmlID);
                    }
                } catch (Exception e) {
                    System.err.println(e);
                }
            }
        });
    }

    /**
     * Close Scilab Help Browser
     */
    public static void closeHelpBrowser() {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                try {
                    ScilabHelpBrowser.getHelpBrowser().close();
                } catch (Exception e) {
                    System.err.println(e);
                }
            }
        });
    }

    /**
     * Show search field in Scilab Help Browser
     */
    public static void showSearchFieldInHelp() {
        ScilabHelpBrowser.getHelpBrowser().showSearchField();
    }

    /**
     * Increase the font in the help viewer
     */
    public static void increaseFontInHelpViewer() {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                ScilabHelpBrowser.getHelpBrowser().increaseFont();
            }
        });
    }

    /**
     * Decrease the font in the help viewer
     */
    public static void decreaseFontInHelpViewer() {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                ScilabHelpBrowser.getHelpBrowser().decreaseFont();
            }
        });
    }

    /************/
    /*          */
    /* WEBLINKS */
    /*          */
    /************/

    /**
     * Open a Browser on Scilab Web Site
     */
    public static void openScilabWebSite() {
        WebBrowser.openUrl("http://www.scilab.org/");
    }

    /**
     * Open a Browser on Wiki Web Site
     */
    public static void openWiki() {
        WebBrowser.openUrl("http://wiki.scilab.org/");
    }

    /**
     * Open a Browser on ATOMS Web Site
     */
    public static void openAtomsScilab() {
        WebBrowser.openUrl("http://atoms.scilab.org/");
    }

    /**
     * Open a Browser on File Exchange Web Site
     */
    public static void openFileExchange() {
        WebBrowser.openUrl("http://fileexchange.scilab.org/");
    }

    /**
     * Open a Browser on Bugzilla Web Site
     */
    public static void openBugzilla() {
        WebBrowser.openUrl("http://bugzilla.scilab.org/");
    }

    /**
     * Open a Browser on Forge Web Site
     */
    public static void openForge() {
        WebBrowser.openUrl("http://forge.scilab.org/");
    }

    /**
     * Open a Browser on Scilab Online Help
     */
    public static void openOnlineHelp() {
        WebBrowser.openUrl("http://help.scilab.org/");
    }

    /**
     * Open a Browser on Mailing List info
     */
    public static void openMailingList() {
        WebBrowser.openUrl("http://www.scilab.org/communities/developer_zone/tools/mailing_list");
    }

    /**
     * Open a Browser on Mailing List Archives
     */
    public static void openMailingListArchives() {
        WebBrowser.openUrl("http://mailinglists.scilab.org/");
    }

    /**
     * Open a Browser on S/E
     */
    public static void openSE() {
        WebBrowser.openUrl("http://www.scilab-enterprises.com/");
    }

    /***************************/
    /*                         */
    /* JAVA CALLBACKS FOR GUIS */
    /*                         */
    /***************************/

    /**
     * Select all the console contents
     */
    public static void selectAllConsoleContents() {
        ScilabConsole.getConsole().selectAll();
    }

    /**
     * Select all the console contents
     */
    public static void helpOnTheKeyword() {
        ScilabConsole.getConsole().helpOnTheKeyword();
    }

    /**
     * Put the console selected text in the clipboard
     */
    public static void copyConsoleSelection() {
        ScilabConsole.getConsole().copyToClipboard();
    }

    /**
     * Cut the console selected text in the clipboard
     */
    public static void cutConsoleSelection() {
        ScilabConsole.getConsole().cutSelection();
    }

    /**
     * Paste clipboard contents in Console input line
     */
    public static void pasteClipboardIntoConsole() {
        ScilabConsole.getConsole().pasteClipboard();
    }

    /**
     * Make the clipboard contents empty
     */
    public static void emptyClipboard() {
        Transferable contents = new StringSelection("");
        Toolkit.getDefaultToolkit().getSystemClipboard().setContents(contents, null);
    }

    /**
     * Evaluate the selection with echo
     */
    public static void evaluateSelectionWithEcho() {
        ScilabConsole.getConsole().evaluateSelectionWithEcho();
    }

    /**
     * Evaluate the selection with no echo
     */
    public static void evaluateSelectionWithNoEcho() {
        ScilabConsole.getConsole().evaluateSelectionWithNoEcho();
    }

    /**
     * Opens a dialog to selected a new font for the console
     */
    public static void changeConsoleFont() {
        FontChooser fontChooser = ScilabFontChooser.createFontChooser(ScilabConsole.getConsole().getFont());
        fontChooser.displayAndWait();

        Font selectedFont = fontChooser.getSelectedFont();

        if (selectedFont != null) {
            /* Change console font */
            ScilabConsole.getConsole().setFont(selectedFont);

            /* Save new settings */
            ConfigManager.saveFont(selectedFont);
        }
    }

    /**
     * Unblock the console if it is in "Continue display..." mode
     */
    public static void unblockConsole() {
        SwingScilabConsole sciConsole = ((SwingScilabConsole) ScilabConsole.getConsole().getAsSimpleConsole());
        sciConsole.unblock();
    }

    /**
     * Opens a dialog to selected a new Foreground Color for the console
     */
    public static void changeConsoleForeground() {
        ColorChooser colorChooser = ScilabColorChooser.createColorChooser(ScilabConsole.getConsole().getForeground());
        colorChooser.setTitle(Messages.gettext("Console Font..."));
        colorChooser.displayAndWait();

        Color selectedColor = colorChooser.getSelectedColor();

        if (selectedColor != null) {
            /* Change console foreground */
            ScilabConsole.getConsole().setForeground(selectedColor);

            /* Save new settings */
            ConfigManager.saveConsoleForeground(selectedColor);
        }
    }

    /**
     * Opens a dialog to selected a new Background Color for the console
     */
    public static void changeConsoleBackground() {
        ColorChooser colorChooser = ScilabColorChooser.createColorChooser(ScilabConsole.getConsole().getBackground());
        colorChooser.setTitle(Messages.gettext("Console Background..."));
        colorChooser.displayAndWait();

        Color selectedColor = colorChooser.getSelectedColor();

        if (selectedColor != null) {
            /* Change console background */
            ScilabConsole.getConsole().setBackground(selectedColor);

            /* Save new settings */
            ConfigManager.saveConsoleBackground(selectedColor);
        }
    }

    /**
     * Display a dialog to print the console text contents
     */
    public static void printConsoleContents() {

        SciConsole scilabConsole = ((SciConsole) ScilabConsole.getConsole().getAsSimpleConsole());
        Document doc = ((JEditorPane) scilabConsole.getConfiguration().getOutputView()).getDocument();
        String textToPrint = null;

        /* Text selected in the input */
        String strInputSelected = ((JTextPane) scilabConsole.getConfiguration().getInputCommandView()).getSelectedText();
        /* Text selected in the output */
        String strOutputSelected = ((JEditorPane) scilabConsole.getConfiguration().getOutputView()).getSelectedText();

        try {
            textToPrint = doc.getText(0, doc.getLength());
        } catch (BadLocationException e) {
            e.printStackTrace();
        }
        if (strInputSelected != null) {
            printString(strInputSelected, new String(CONSOLE));
        } else if (strOutputSelected != null) {
            printString(strOutputSelected, new String(CONSOLE));
        } else {
            printString(textToPrint, new String(CONSOLE));
        }
    }

    /**
     * Print a character string
     * @param theString the string to print
     * @param pageHeader header for printed pages
     * @return execution status
     */
    public static boolean printString(String theString, String pageHeader) {
        return PrinterHelper.printString(theString, pageHeader);
    }

    /**
     * Display a dialog to print a file
     * @param fileName the name of the file
     * @return execution status
     */
    public static boolean printFile(String fileName) {
        return PrinterHelper.printFile(fileName);
    }

    /**
     * Display a dialog to print a figure (Called from graphics figures menus)
     * @param figID the ID of the figure to print
     * @return execution status
     */
    public static boolean printFigure(int figID) {
        return printFigure(figID, true, true);
    }

    /**
     * Display a dialog to print a figure
     * @param figID the ID of the figure to print
     * @param postScript true to print in postscript mode
     * @param displayDialog true to display a print setup dialog
     * @return execution status
     */
    public static boolean printFigure(int figID, boolean postScript, boolean displayDialog) {
        // Get the PrinterJob object
        PrinterJob printerJob = PrinterJob.getPrinterJob();

        boolean userOK = true;
        if (displayDialog) {
            userOK = printerJob.printDialog(scilabPageFormat);
        }

        if (userOK) {
            //If the OS is Windows
            if (isWindowsPlateform()) {
                Figure figure = (Figure) GraphicController.getController().getObjectFromId(figID);
                int figureID = figure.getId();
                SwingScilabCanvas canvas;
                canvas = ((SwingScilabTab) SwingView.getFromId(figID)).getContentCanvas();
                ScilabPrint scilabPrint = new ScilabPrint(canvas.dumpAsBufferedImage(), printerJob, scilabPageFormat);
                if (scilabPrint != null) {
                    return true;
                } else {
                    return false;
                }

                //If the OS is Linux
            } else {
                DocFlavor printDocFlavor = DocFlavor.INPUT_STREAM.POSTSCRIPT;
                String fileExtension = ".ps";

                try {
                    String tmpPrinterFile = File.createTempFile("scilabfigure", "").getAbsolutePath();
                    // Export image to PostScript
                    if (((PrintRequestAttribute) scilabPageFormat.get(OrientationRequested.class)) == OrientationRequested.PORTRAIT) {
                        FileExporter.fileExport(figID,
                                                tmpPrinterFile + fileExtension,
                                                "PS", 1, 0); // 1 is the quality. Useless in this context
                    } else {
                        FileExporter.fileExport(figID,
                                                tmpPrinterFile + fileExtension,
                                                "PS", 1, 1); // 1 is the quality. Useless in this context
                    }

                    // Read file
                    FileInputStream psStream = null;

                    try {
                        psStream = new FileInputStream(tmpPrinterFile + fileExtension);
                    } catch (FileNotFoundException ffne) {
                        ffne.printStackTrace();
                        return false;
                    }

                    Doc myDoc = new SimpleDoc(psStream, printDocFlavor, null);
                    PrintService printService = printerJob.getPrintService();

                    if (printService == null) {
                        // Could not find the print service
                        MessageBox messageBox = ScilabMessageBox.createMessageBox();
                        messageBox.setMessage(Messages.gettext("No print service found."));
                        messageBox.setModal(true);
                        messageBox.setIcon("error");
                        messageBox.displayAndWait();
                        return false;
                    }
                    DocPrintJob job = printService.createPrintJob();

                    // Remove Orientation option from page setup because already managed in FileExporter
                    PrintRequestAttributeSet aset = new HashPrintRequestAttributeSet(scilabPageFormat);
                    aset.add(OrientationRequested.PORTRAIT);
                    job.print(myDoc, aset);
                    return true;
                } catch (IOException e) {
                    System.err.println("Could not create temporary file " + e.getLocalizedMessage());
                    return false;
                } catch (PrintException e) {
                    e.printStackTrace();
                    return false;
                }
            }
        }
        return false;
    }

    /**
     * Display a page setup dialog for printing
     * @return true if the user clicked the OK button
     */
    public static boolean pageSetup() {
        // Get the PrinterJob object
        PrinterJob job = PrinterJob.getPrinterJob();

        // Get the default page format, then allow the user to modify it
        return (job.pageDialog(scilabPageFormat) != null);
    }

    /***********************/
    /*                     */
    /* FONT CHOOSER BRIDGE */
    /*                     */
    /***********************/

    /**
     * Set the default font name for a FileChooser
     * @param id the id of the FileChooser
     * @param fontName the name of the font
     */
    public static void setFontChooserFontName(int id, String fontName) {
        ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).setDefaultFontName(fontName);
    }

    /**
     * Set the default font size for a FileChooser
     * @param id the id of the FileChooser
     * @param fontSize the size of the font
     */
    public static void setFontChooserFontSize(int id, int fontSize) {
        ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).setDefaultFontSize(fontSize);
    }

    /**
     * Set the default bold attribute for a FileChooser
     * @param id the id of the FileChooser
     * @param bold the bold attribute of the font
     */
    public static void setFontChooserBold(int id, boolean bold) {
        ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).setDefaultBold(bold);
    }

    /**
     * Set the default italic attribute for a FileChooser
     * @param id the id of the FileChooser
     * @param italic the italic attribute of the font
     */
    public static void setFontChooserItalic(int id, boolean italic) {
        ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).setDefaultItalic(italic);
    }

    /**
     * Get the selected font name for a FileChooser
     * @param id the id of the FileChooser
     * @return the name of the font
     */
    public static String getFontChooserFontName(int id) {
        Font selectedFont = ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).getSelectedFont();
        if (selectedFont != null) {
            return selectedFont.getFamily();
        } else {
            return "";
        }
    }

    /**
     * Get the selected font size for a FileChooser
     * @param id the id of the FileChooser
     * @return the size of the font
     */
    public static int getFontChooserFontSize(int id) {
        return ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).getSelectedFont().getSize();
    }

    /**
     * Get the selected bold attribute for a FileChooser
     * @param id the id of the FileChooser
     * @return the bold attribute of the font
     */
    public static boolean getFontChooserBold(int id) {
        return ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).getSelectedFont().isBold();
    }

    /**
     * Get the selected italic attribute for a FileChooser
     * @param id the id of the FileChooser
     * @return the italic attribute of the font
     */
    public static boolean getFontChooserItalic(int id) {
        return ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).getSelectedFont().isItalic();
    }

    /**
     * Display this chooser and wait for user selection
     * @param id the id of the FontChooser
     */
    public static void fontChooserDisplayAndWait(int id) {
        ((FontChooser) UIElementMapper.getCorrespondingUIElement(id)).displayAndWait();
    }

    /************************/
    /*                      */
    /* COLOR CHOOSER BRIDGE */
    /*                      */
    /************************/

    /**
     * Set the title of a ColorChooser
     * @param id the id of the ColorChooser
     * @param title the title
     */
    public static void setColorChooserTitle(int id, String title) {
        ((ColorChooser) UIElementMapper.getCorrespondingUIElement(id)).setTitle(title);
    }

    /**
     * Set the default Color for a ColorChooser
     * @param id the id of the ColorChooser
     * @param rgb the default color
     */
    public static void setColorChooserDefaultColor(int id, int[] rgb) {
        ((ColorChooser) UIElementMapper.getCorrespondingUIElement(id)).setDefaultColor(new Color(rgb[0], rgb[1], rgb[2]));
    }

    /**
     * Get the selected Color for a ColorChooser
     * @param id the id of the ColorChooser
     * @return the selected color
     */
    public static int[] getColorChooserSelectedColor(int id) {
        Color selectedColor = ((ColorChooser) UIElementMapper.getCorrespondingUIElement(id)).getSelectedColor();
        int[] returnedValues = new int[NB_COLORS];
        if (selectedColor != null) {
            returnedValues[0] = selectedColor.getRed();
            returnedValues[1] = selectedColor.getGreen();
            returnedValues[2] = selectedColor.getBlue();
        } else {
            returnedValues[0] = -1;
            returnedValues[1] = -1;
            returnedValues[2] = -1;
        }
        return returnedValues;

    }

    /**
     * Display this chooser and wait for user selection
     * @param id the id of the ColorChooser
     */
    public static void colorChooserDisplayAndWait(int id) {
        ((ColorChooser) UIElementMapper.getCorrespondingUIElement(id)).displayAndWait();
    }

    /******************/
    /*                */
    /* WIDGET VISIBLE */
    /*                */
    /******************/

    /**
     * Set the visibility of the Widget
     * @param id the id of the Widget
     * @param status the new status
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setWidgetVisible(int id, boolean status) {
        ((Widget) UIElementMapper.getCorrespondingUIElement(id)).setVisible(status);
    }

    /**
     * Set the visibility of the Frame
     * @param id the id of the Frame
     * @param status the new status
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setFrameVisible(int id, boolean status) {
        ((Frame) UIElementMapper.getCorrespondingUIElement(id)).setVisible(status);
    }

    /**
     * Get the visibility of the Widget
     * @param id the id of the Widget
     * @return the status
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static boolean isWidgetVisible(int id) {
        return ((Widget) UIElementMapper.getCorrespondingUIElement(id)).isVisible();
    }

    /**
     * Get the visibility of the Frame
     * @param id the id of the Frame
     * @return the status
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static boolean isFrameVisible(int id) {
        return ((Frame) UIElementMapper.getCorrespondingUIElement(id)).isVisible();
    }

    /*****************/
    /*               */
    /* WIDGET ENABLE */
    /*               */
    /*****************/

    /**
     * Set the enable of the Widget
     * @param id the id of the Widget
     * @param status the new status
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setWidgetEnable(int id, boolean status) {
        ((Widget) UIElementMapper.getCorrespondingUIElement(id)).setEnabled(status);
    }

    /**
     * Set the enable of the Frame
     * @param id the id of the Frame
     * @param status the new status
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setFrameEnable(int id, boolean status) {
        ((Frame) UIElementMapper.getCorrespondingUIElement(id)).setEnabled(status);
    }

    /**
     * Get the enable of the Widget
     * @param id the id of the Widget
     * @return the status
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static boolean isWidgetEnable(int id) {
        return ((Widget) UIElementMapper.getCorrespondingUIElement(id)).isEnabled();
    }

    /**
     * Get the visibility of the Frame
     * @param id the id of the Frame
     * @return the status
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static boolean isFrameEnable(int id) {
        return ((Frame) UIElementMapper.getCorrespondingUIElement(id)).isEnabled();
    }

    /****************/
    /*              */
    /* MENU CHECKED */
    /*              */
    /****************/

    /**
     * Set the Checked status of the Menu
     * @param id the id of the Menu
     * @param status the new status
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setMenuChecked(int id, boolean status) {
        if (UIElementMapper.getCorrespondingUIElement(id) instanceof CheckBoxMenuItem) {
            ((CheckBoxMenuItem) UIElementMapper.getCorrespondingUIElement(id)).setChecked(status);
        } else if (UIElementMapper.getCorrespondingUIElement(id) instanceof MenuItem) {
            ((MenuItem) UIElementMapper.getCorrespondingUIElement(id)).setChecked(status);
        } else {
            ((Menu) UIElementMapper.getCorrespondingUIElement(id)).setChecked(status);
        }
    }

    /**
     * Get the Checked status of the Menu
     * @param id the id of the Menu
     * @return the status
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static boolean isMenuChecked(int id) {
        return ((Menu) UIElementMapper.getCorrespondingUIElement(id)).isChecked();
    }

    /************************/
    /*                      */
    /* CLIPBOARD MANAGEMENT */
    /*                      */
    /************************/

    /**
     * Get the contents of the clipboard
     * @return the string contained in the clipboard or null
     */
    public static String getClipboardContents() {
        // Gets the contents of the clipboard
        Toolkit toolkit = Toolkit.getDefaultToolkit();
        Clipboard systemClipboard = toolkit.getSystemClipboard();
        String clipboardContents = "";

        // Verify that clibpboard data is of text type
        boolean dataAvailable;
        try {
            dataAvailable = systemClipboard.isDataFlavorAvailable(DataFlavor.stringFlavor);
        } catch (IllegalStateException exception) {
            return clipboardContents;
        }

        // Exit if text data not available
        if (!dataAvailable) {
            return clipboardContents;
        }

        // Read data
        try {
            clipboardContents = (String) systemClipboard.getData(DataFlavor.stringFlavor);
        } catch (UnsupportedFlavorException e1) {
            // Should never be here
            e1.printStackTrace();
        } catch (IOException e1) {
            // Should never be here
            e1.printStackTrace();
        }

        return clipboardContents;
    }

    /**
     * Set the contents of the clipboard
     * @param text the string to put in the clipboard
     */
    public static void setClipboardContents(String text) {
        Transferable contents = new StringSelection(text);
        Toolkit.getDefaultToolkit().getSystemClipboard().setContents(contents, null);
    }

    /**
     * Copy figure to clipboard
     * @param figID the ID of the figure
     */
    public static void copyFigureToClipBoard(int figID) {
        Image figureImage = ImageExporter.imageExport(figID);
        if (figureImage != null) {
            Transferable clipboardImage = new ClipboardImage(figureImage);
            Toolkit.getDefaultToolkit().getSystemClipboard().setContents(clipboardImage, null);
        }
    }

    /**
     * Class used to store Images in the clipboard
     */
    public static class ClipboardImage implements Transferable {
        private final DataFlavor[] flavors = new DataFlavor[] {DataFlavor.imageFlavor};
        private final Image image;

        /**
         * Default constructor
         * @param image the image
         */
        public ClipboardImage(Image image) {
            this.image = image;
        }

        /**
         * DataFlavors of this transferable
         * @return the DataFlavors accepeted
         */
        @Override
        public DataFlavor[] getTransferDataFlavors() {
            return flavors;
        }

        /**
         * Test supported DataFlavors
         * @param flavor the flavor to test
         * @return true if the flavor is supported
         */
        @Override
        public boolean isDataFlavorSupported(DataFlavor flavor) {
            return flavors[0].equals(flavor);
        }

        /**
         * Get the contents of this transferable
         * @param flavor the flavor to test
         * @return the contents
         * @throws UnsupportedFlavorException if the flavor is not supported by this transferable
         */
        @Override
        public Object getTransferData(DataFlavor flavor) throws UnsupportedFlavorException {
            if (!flavors[0].equals(flavor)) {
                throw new UnsupportedFlavorException(flavor);
            }

            return image;
        }
    }

    /********/
    /*      */
    /* MISC */
    /*      */
    /********/

    /**
     * Get the screen resolution
     * @return the number of dot per inch
     */
    public static int getScreenResolution() {
        return Toolkit.getDefaultToolkit().getScreenResolution();
    }

    /**
     * Get the screen Width in pixels
     * @return the screen Width
     */
    public static double getScreenWidth() {
        return Toolkit.getDefaultToolkit().getScreenSize().getWidth();
    }

    /**
     * Get the screen Height in pixels
     * @return the screen Height
     */
    public static double getScreenHeight() {
        return Toolkit.getDefaultToolkit().getScreenSize().getHeight();
    }

    /**
     * Get the screen Depth
     * @return the screen Depth
     */
    public static int getScreenDepth() {
        return Toolkit.getDefaultToolkit().getColorModel().getPixelSize();
    }

    /**
     * Give the focus to a uicontrol
     * @param uicontrolUID the uicontrolUID of the Widget
     */
    public static void requestFocus(int uicontrolUID) {
        SwingViewObject uicontrol = SwingView.getFromId(uicontrolUID);
        if (uicontrol instanceof SwingScilabFrame) {
            ((SwingScilabFrame) uicontrol).requestFocus();
        } else {
            ((Widget) uicontrol).requestFocus();
        }

    }

    /**
     * Raise the window specified by the user
     * @param id the id of the figure
     */
    public static void raiseWindow(int id) {
        /*((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(id).getRendererProperties()).getParentTab().getParentWindow().raise();
          ((ScilabRendererProperties) FigureMapper.getCorrespondingFigure(id).getRendererProperties()).getParentTab().setCurrent();*/
    }

    /**
     * @return true if the os is windows, false otherwise
     */
    public static boolean isWindowsPlateform() {
        // get os name
        return System.getProperty("os.name").toLowerCase().contains("windows");
    }

    /**
     * Change object used for graphic figure (GLCanvas or GLJPanel)
     * @param onOrOff true to set GLCanvas display
     */
    public static void useCanvasForDisplay(boolean onOrOff) {
        SwingScilabCanvasImpl.switchToGLCanvas(onOrOff);
    }

    /**
     * Get object used for graphic figure (GLCanvas or GLJPanel)
     * @return true if GLCanvas display is set
     */
    public static boolean useCanvasForDisplay() {
        return SwingScilabCanvasImpl.isGLCanvasEnabled();
    }

    /**
     * Display Scilab about box
     */
    public static void scilabAboutBox() {
        ScilabAboutBox.displayAndWait();
    }

    /******************/
    /*                */
    /* UITABLE BRIDGE */
    /*                */
    /******************/

    /**
     * Sets the column names for the uitable
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setUiTableColnames(int id, String text) {
        //((UiTable) UIElementMapper.getCorrespondingUIElement(id)).setColnames(text);
    }

    /**
     * Sets the row names for the uitable
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setUiTableRownames(int id, String text) {
        //((UiTable) UIElementMapper.getCorrespondingUIElement(id)).setRownames(text);
    }

    /**
     * Sets the data for the uitable
     */
    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setUiTableData(int id, String text) {
        ((UiTable) UIElementMapper.getCorrespondingUIElement(id)).setData(text);
    }

    /******************/
    /*                */
    /* UITABLE BRIDGE */
    /*                */
    /******************/

    // TODO REMOVE ME (NO MORE USED IN JNI)
    public static void setUiTreeData(int id, String[] text) {
        ((UiDisplayTree) UIElementMapper.getCorrespondingUIElement(id)).setData(text);
    }

    public static void fireClosingFinished(int figUID) {
        SwingScilabTab parentTab = (SwingScilabTab) SwingView.getFromId(figUID);
        ClosingOperationsManager.removeFromDunnoList(parentTab);
    }
}
