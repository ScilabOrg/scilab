/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.plotbrowser;

import java.awt.CardLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Vector;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ListSelectionModel;
import javax.swing.ScrollPaneConstants;

import org.scilab.modules.gui.plotbrowser.actions.CloseAction;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.editor.ObjectSearcher;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
 * Swing implementation of Plot Browser.
 *
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class SwingPlotBrowser extends SwingScilabTab {
    private JScrollPane desktop;
    public static JPanel pReceive;
    private JList list;
    private MenuBar menuBar;
    private Menu fileMenu;
    private String currentfigure;
    private final ArrayList<String> uids = new ArrayList<String>();
    public static final String PLOTBROWSERUUID = "4m249547-6a71-4998-r8c-00o367s47932-B";

    /**
    * Constructor
    *
    * @param objectID Enters the identification of object.
    */
    public SwingPlotBrowser(String objectID) {
        super(MessagesPlotBrowser.plot_browser, PLOTBROWSERUUID);

        currentfigure = objectID;

        setAssociatedXMLIDForHelp("plotbrowser");

        buildMenuBar();
        addMenuBar(menuBar);

        guiComponents();

        setContentPane(desktop);
        WindowsConfigurationManager.restorationFinished(this);
    }

    /**
    * It has all the components of the plot browser panel.
    */
    private void guiComponents() {
        desktop = new JScrollPane();
        pReceive = new JPanel();
        list = new JList();

        desktop.setBorder(null);
        desktop.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        desktop.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
        desktop.setAlignmentX(0.0F);
        desktop.setAlignmentY(0.0F);

        pReceive.setAlignmentX(0.0F);
        pReceive.setAlignmentY(0.0F);
        pReceive.setLayout(new CardLayout());

        desktop.setViewportView(pReceive);

        list.setCellRenderer(new CheckBoxCellRenderer());
        list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

        list.addKeyListener(new KeyAdapter() {
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_SPACE) {
                    int index = list.getSelectedIndex();
                    if (index != -1) {
                        JCheckBox checkbox = (JCheckBox) list.getModel().getElementAt(index);
                        checkbox.setSelected(!checkbox.isSelected());
                        setVisible(!checkbox.isSelected(), uids.get(index));
                        pReceive.repaint();
                    }
                }
            }
        });

        list.addMouseListener(new MouseAdapter(){
            public void mousePressed(MouseEvent e){
                if (e.getX()<20) {
                    int index = list.locationToIndex(e.getPoint());
                    if(index != -1){
                        JCheckBox checkbox = (JCheckBox) list.getModel().getElementAt(index);
                        checkbox.setSelected(!checkbox.isSelected());
                        setVisible(!checkbox.isSelected(), uids.get(index));
                        pReceive.repaint();
                    }
                }
            }
        });

        ObjectSearcher instanceObject = new ObjectSearcher();

        String[] axes = searchAxes(currentfigure);

        ArrayList<Object> cbArray = new ArrayList<Object>();

        for (int i=0; i<axes.length; i++) {
            cbArray.add(new JCheckBox("Axis"));
            uids.add(axes[i]);
            String[] line = instanceObject.search(axes[i], "Polyline");
            for (int j=0; j<line.length; j++) {
                cbArray.add(new JCheckBox("Polyline"));
                uids.add(line[j]);
            }
        }

        list.setListData(new Vector(cbArray));

        pReceive.add(new JScrollPane(list));
    }

    /**
    * Changes the visibility of an object.
    */
    public void setVisible(boolean status, String id) {
        GraphicController.getController()
                .setProperty(id, GraphicObjectProperties.__GO_VISIBLE__, status);
    }

    /**
    * Locates all axes are plotted in Figure.
    * @param figureID Enters the identification of figure.
    * @return the existing axes.
    */
    private static String[] searchAxes(String figureID) {
        return (new ObjectSearcher()).search(figureID, GraphicObjectProperties.__GO_AXES__);
    }

    /**
    * Creates the menuBar.
    */
    public void buildMenuBar() {
        menuBar = ScilabMenuBar.createMenuBar();

        fileMenu = ScilabMenu.createMenu();
        fileMenu.setText(MessagesPlotBrowser.file);
        fileMenu.setMnemonic('F');
        fileMenu.add(CloseAction.createMenu());

        menuBar.add(fileMenu);
    }

    /**
     * {@inheritDoc}
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        setInfoBar(infoBarToAdd);
    }

    /**
     * {@inheritDoc}
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        setMenuBar(menuBarToAdd);
    }
}
