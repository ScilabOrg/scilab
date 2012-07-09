/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos Cardinot
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged;

import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import org.scilab.modules.gui.ged.axes.HideAxis;
import org.scilab.modules.gui.ged.curve.HideCurve;
import org.scilab.modules.gui.ged.figure.HideFigure;

/**
* Main properties window - Light GED.
* Receive the classes of graphic objects (JPanels).
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Inspector extends javax.swing.JFrame {
    public static String selectaux;
    public static Inspector window;
    public static javax.swing.JPanel pReceive;
    private javax.swing.JToggleButton bHideShow;
    private javax.swing.JScrollPane desktop;
    private javax.swing.JPanel pMenu;
    private static String scidir = System.getenv("SCI");
    private static String imagepath = scidir + "/modules/gui/images/icons/";
    public static String icon_collapse = imagepath + "11x11/actions/tree-collapse.png";
    public static String icon_expand = imagepath + "11x11/actions/tree-expand.png";
    public static String icon_collapse_all = imagepath + "22x22/actions/tree-diagramm.png";
    public static String icon_expand_all = imagepath + "22x22/actions/tree-diagramm-delete.png";
    public static String icon_color_fill = imagepath + "16x16/actions/color-fill.png";

    /**
    * Constructor - Initializes the inspection window.
    * @param select Indicates which property window will open.
    */
    public Inspector(String select , String figureID) {
        guiComponents();
        new Swap(select , figureID , 1 , 1);
        selectaux = select;
    }

    /**
    * If lightGED not yet started it creates a JFrame.
    * @param select Indicates which property window will open initially.
    * @return window Shows the lightGED was generated.
    */
    public static Inspector createGuiInspector(String select , String figureID) {
        if (window == null) {
            window = new Inspector(select , figureID);
        }
        return window;
    }

    /**
    * Loads the JFrame with the JPanel corresponding to the property selected by the user.
    * @param select Indicates which property window will open.
    * @return null
    */
    public static Inspector startProperty(String select , String figureID) {
        if (window != null) {
            new Swap(select , figureID , 1 , 1);
            selectaux = select;
        }
        return null;
    }

    /**
    * Manages the operation of the button show/hide for the current object.
    * @param evt
    */
    private void bHideShowActionPerformed(java.awt.event.ActionEvent evt) {
        if (selectaux.equals("axes")) {
            if (bHideShow.isSelected()) {
                new HideAxis(true);
            } else {
                new HideAxis(false);
            }
        } else {
            if (selectaux.equals("curve")) {
                if (bHideShow.isSelected()) {
                    new HideCurve(true);
                } else {
                    new HideCurve(false);
                }
            } else {
                if (bHideShow.isSelected() == true) {
                    new HideFigure(true);
                } else {
                    new HideFigure(false);
                }
            }
        }
    }

    /**
    * It has all the components of the main window.
    */
    private void guiComponents() {
        setVisible(true);
        pMenu = new javax.swing.JPanel();
        bHideShow = new javax.swing.JToggleButton();
        desktop = new javax.swing.JScrollPane();
        pReceive = new javax.swing.JPanel();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        /**
        * Implements an event button to close the window.
        * Resets the variable responsible for identifying if the window is open.
        */
        addWindowListener(new WindowListener() {
        public void windowClosing(WindowEvent evt) {
            window = null;
        }
        public void windowClosed(WindowEvent evt) {
            window = null;
        }
        public void windowOpened(WindowEvent evt) {}  
        public void windowIconified(WindowEvent evt) {}  
        public void windowDeiconified(WindowEvent evt) {}  
        public void windowActivated(WindowEvent evt) {}  
        public void windowDeactivated(WindowEvent evt) {}  
        });  


        setResizable(false);

        pMenu.setBorder(javax.swing.BorderFactory.createEtchedBorder());
        pMenu.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        pMenu.setMinimumSize(new java.awt.Dimension(335, 30));
        pMenu.setPreferredSize(new java.awt.Dimension(370, 30));

        
        bHideShow.setToolTipText(MessagesGED.collapse_expand);
        bHideShow.setAlignmentY(0.0F);
        bHideShow.setContentAreaFilled(false);
        bHideShow.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        bHideShow.setMaximumSize(new java.awt.Dimension(20, 20));
        bHideShow.setMinimumSize(new java.awt.Dimension(20, 20));
        bHideShow.setPreferredSize(new java.awt.Dimension(20, 20));
        bHideShow.setRolloverEnabled(false);
        bHideShow.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bHideShowActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout pMenuLayout = new javax.swing.GroupLayout(pMenu);
        pMenu.setLayout(pMenuLayout);
        pMenuLayout.setHorizontalGroup(
            pMenuLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pMenuLayout.createSequentialGroup()
                .addGap(4, 4, 4)
                .addComponent(bHideShow, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        pMenuLayout.setVerticalGroup(
            pMenuLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pMenuLayout.createSequentialGroup()
                .addGap(2, 2, 2)
                .addComponent(bHideShow, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
        );

        desktop.setBorder(null);
        desktop.setHorizontalScrollBarPolicy(javax.swing.ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        desktop.setVerticalScrollBarPolicy(javax.swing.ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
        desktop.setAlignmentX(0.0F);
        desktop.setAlignmentY(0.0F);

        pReceive.setAlignmentX(0.0F);
        pReceive.setAlignmentY(0.0F);
        pReceive.setPreferredSize(new java.awt.Dimension(370, 780));

        javax.swing.GroupLayout pReceiveLayout = new javax.swing.GroupLayout(pReceive);
        pReceive.setLayout(pReceiveLayout);
        pReceiveLayout.setHorizontalGroup(
            pReceiveLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 370, Short.MAX_VALUE)
        );
        pReceiveLayout.setVerticalGroup(
            pReceiveLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 780, Short.MAX_VALUE)
        );

        desktop.setViewportView(pReceive);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(pMenu, javax.swing.GroupLayout.DEFAULT_SIZE, 370, Short.MAX_VALUE)
            .addComponent(desktop, javax.swing.GroupLayout.DEFAULT_SIZE, 370, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(pMenu, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(desktop, javax.swing.GroupLayout.DEFAULT_SIZE, 584, Short.MAX_VALUE))
        );

        pack();
        setIcons();
    }

    /**
    * Insert the icons on buttons.
    */
    private void setIcons() {
        bHideShow.setIcon(new javax.swing.ImageIcon(icon_collapse_all));
        bHideShow.setSelectedIcon(new javax.swing.ImageIcon(icon_expand_all));
    }
}
