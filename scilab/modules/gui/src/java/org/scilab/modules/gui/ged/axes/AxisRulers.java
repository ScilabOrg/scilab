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
package org.scilab.modules.gui.ged.axes;

import javax.swing.JPanel;
import org.scilab.modules.gui.ged.Inspector;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
* Construction and startup of all components of the section: Axis Rulers.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class AxisRulers extends Roof {
    //Header - Axis Rulers
    protected static javax.swing.JToggleButton bAxisRulers;
    protected javax.swing.JLabel lAxisRulers;
    protected javax.swing.JSeparator sAxisRulers;
    public static JPanel pAxisRulers;
    //Components of the property: Location
    protected static javax.swing.JToggleButton bLocation;
    protected javax.swing.JLabel lLocation;
    protected javax.swing.JLabel cLocation;
    protected static JPanel pLocation;
    protected javax.swing.JLabel lLocationX;
    protected javax.swing.JComboBox cLocationX;
    protected javax.swing.JLabel lLocationY;
    protected javax.swing.JComboBox cLocationY;
    //Components of the property: Reverse
    protected static javax.swing.JToggleButton bReverse;
    protected javax.swing.JLabel lReverse;
    protected javax.swing.JLabel cReverse;
    protected static JPanel pReverse;
    protected javax.swing.JLabel lReverseX;
    protected javax.swing.JComboBox cReverseX;
    protected javax.swing.JLabel lReverseY;
    protected javax.swing.JComboBox cReverseY;
    protected javax.swing.JLabel lReverseZ;
    protected javax.swing.JComboBox cReverseZ;

    protected String currentaxes = null;

    /**
    * Initializes the properties and the icons of the buttons.
    *
    * @param objectID Enters the identification of Axes.
    */
    public AxisRulers(String objectID) {
        position();
        setIconsAxisRulers();
        initPropertiesAxisRulers(objectID);
    }

    /**
    * It has all the components of the section Axis Rulers.
    */
    @Override
    public void axisRulersComponents() {
        //Header
        bAxisRulers = new javax.swing.JToggleButton();
        lAxisRulers = new javax.swing.JLabel();
        sAxisRulers = new javax.swing.JSeparator();
        pAxisRulers = new JPanel();
        //Location
        bLocation = new javax.swing.JToggleButton();
        lLocation = new javax.swing.JLabel();
        cLocation = new javax.swing.JLabel();
        pLocation = new JPanel();
        lLocationX = new javax.swing.JLabel();
        cLocationX = new javax.swing.JComboBox();
        lLocationY = new javax.swing.JLabel();
        cLocationY = new javax.swing.JComboBox();
        //Reverse
        bReverse = new javax.swing.JToggleButton();
        lReverse = new javax.swing.JLabel();
        cReverse = new javax.swing.JLabel();
        pReverse = new JPanel();
        lReverseX = new javax.swing.JLabel();
        cReverseX = new javax.swing.JComboBox();
        lReverseY = new javax.swing.JLabel();
        cReverseY = new javax.swing.JComboBox();
        lReverseZ = new javax.swing.JLabel();
        cReverseZ = new javax.swing.JComboBox();

        //Components of the header: Axis Rulers
        bAxisRulers.setBorder(null);
        bAxisRulers.setBorderPainted(false);
        bAxisRulers.setContentAreaFilled(false);
        bAxisRulers.setMaximumSize(new java.awt.Dimension(16, 16));
        bAxisRulers.setMinimumSize(new java.awt.Dimension(16, 16));
        bAxisRulers.setPreferredSize(new java.awt.Dimension(16, 16));
        bAxisRulers.setRolloverEnabled(false);
        bAxisRulers.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bAxisRulersActionPerformed(evt);
            }
        });

        lAxisRulers.setText(MessagesGED.axis_rulers);

        sAxisRulers.setPreferredSize(new java.awt.Dimension(50, 2));

        //Components of the property: Location.
        lLocation.setBackground(new java.awt.Color(255, 255, 255));
        lLocation.setText(" " + MessagesGED.location);
        lLocation.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lLocation.setOpaque(true);
        lLocation.setPreferredSize(new java.awt.Dimension(120, 16));

        cLocation.setBackground(new java.awt.Color(255, 255, 255));
        cLocation.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cLocation.setOpaque(true);

        bLocation.setSelected(true);
        bLocation.setBorder(null);
        bLocation.setBorderPainted(false);
        bLocation.setContentAreaFilled(false);
        bLocation.setMaximumSize(new java.awt.Dimension(16, 16));
        bLocation.setMinimumSize(new java.awt.Dimension(16, 16));
        bLocation.setPreferredSize(new java.awt.Dimension(16, 16));
        bLocation.setRolloverEnabled(false);
        bLocation.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bLocationActionPerformed(evt);
            }
        });

        pLocation.setVisible(false);

        lLocationX.setBackground(new java.awt.Color(255, 255, 255));
        lLocationX.setText(" X");
        lLocationX.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lLocationX.setOpaque(true);

        cLocationX.setModel(new javax.swing.DefaultComboBoxModel(new String[] {
            MessagesGED.bottom, MessagesGED.top, MessagesGED.middle, MessagesGED.origin
        }));
        cLocationX.setPreferredSize(new java.awt.Dimension(89, 20));
        cLocationX.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cLocationXActionPerformed(evt);
            }
        });

        lLocationY.setBackground(new java.awt.Color(255, 255, 255));
        lLocationY.setText(" Y");
        lLocationY.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lLocationY.setOpaque(true);

        cLocationY.setModel(new javax.swing.DefaultComboBoxModel(new String[] {
            MessagesGED.middle, MessagesGED.origin, MessagesGED.left, MessagesGED.right
        }));
        cLocationY.setPreferredSize(new java.awt.Dimension(84, 20));
        cLocationY.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cLocationYActionPerformed(evt);
            }
        });

        titleLocation();

        //Positioning the components of the property: Location.
        javax.swing.GroupLayout pLocationLayout = new javax.swing.GroupLayout(pLocation);
        pLocation.setLayout(pLocationLayout);
        pLocationLayout.setHorizontalGroup(
            pLocationLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pLocationLayout.createSequentialGroup()
                .addComponent(lLocationX, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cLocationX, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pLocationLayout.createSequentialGroup()
                .addComponent(lLocationY, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cLocationY, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        pLocationLayout.setVerticalGroup(
            pLocationLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pLocationLayout.createSequentialGroup()
                .addGroup(pLocationLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lLocationX, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cLocationX, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pLocationLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lLocationY, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cLocationY, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );


        //Components of the property: Reverse.
        lReverse.setBackground(new java.awt.Color(255, 255, 255));
        lReverse.setText(" " + MessagesGED.reverse);
        lReverse.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lReverse.setOpaque(true);
        lReverse.setPreferredSize(new java.awt.Dimension(120, 16));

        cReverse.setBackground(new java.awt.Color(255, 255, 255));
        cReverse.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cReverse.setOpaque(true);

        bReverse.setSelected(true);
        bReverse.setBorder(null);
        bReverse.setBorderPainted(false);
        bReverse.setContentAreaFilled(false);
        bReverse.setMaximumSize(new java.awt.Dimension(16, 16));
        bReverse.setMinimumSize(new java.awt.Dimension(16, 16));
        bReverse.setPreferredSize(new java.awt.Dimension(16, 16));
        bReverse.setRolloverEnabled(false);
        bReverse.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bReverseActionPerformed(evt);
            }
        });

        pReverse.setVisible(false);

        lReverseX.setBackground(new java.awt.Color(255, 255, 255));
        lReverseX.setText(" X");
        lReverseX.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lReverseX.setOpaque(true);

        cReverseX.setModel(new javax.swing.DefaultComboBoxModel(new String[] {MessagesGED.off, MessagesGED.on}));
        cReverseX.setPreferredSize(new java.awt.Dimension(58, 20));
        cReverseX.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cReverseXActionPerformed(evt);
            }
        });

        lReverseY.setBackground(new java.awt.Color(255, 255, 255));
        lReverseY.setText(" Y");
        lReverseY.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lReverseY.setOpaque(true);

        cReverseY.setModel(new javax.swing.DefaultComboBoxModel(new String[] {MessagesGED.off, MessagesGED.on}));
        cReverseY.setPreferredSize(new java.awt.Dimension(58, 20));
        cReverseY.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cReverseYActionPerformed(evt);
            }
        });

        lReverseZ.setBackground(new java.awt.Color(255, 255, 255));
        lReverseZ.setText(" Z");
        lReverseZ.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lReverseZ.setOpaque(true);

        cReverseZ.setModel(new javax.swing.DefaultComboBoxModel(new String[] {MessagesGED.off, MessagesGED.on}));
        cReverseZ.setPreferredSize(new java.awt.Dimension(58, 20));
        cReverseZ.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cReverseZActionPerformed(evt);
            }
        });
 
        titleReverse();

        //Positioning the components of the property: Reverse.
        javax.swing.GroupLayout pReverseLayout = new javax.swing.GroupLayout(pReverse);
        pReverse.setLayout(pReverseLayout);
        pReverseLayout.setHorizontalGroup(
            pReverseLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pReverseLayout.createSequentialGroup()
                .addComponent(lReverseX, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cReverseX, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pReverseLayout.createSequentialGroup()
                .addComponent(lReverseY, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cReverseY, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pReverseLayout.createSequentialGroup()
                .addComponent(lReverseZ, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cReverseZ, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        pReverseLayout.setVerticalGroup(
            pReverseLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pReverseLayout.createSequentialGroup()
                .addGroup(pReverseLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lReverseX, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cReverseX, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pReverseLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lReverseY, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cReverseY, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pReverseLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lReverseZ, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cReverseZ, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );
    }

    /**
    * Positioning all the components of the Axis Rulers.
    */
    private void position() {
        //Positioning all components.
        javax.swing.GroupLayout pAxisRulersLayout = new javax.swing.GroupLayout(pAxisRulers);
        pAxisRulers.setLayout(pAxisRulersLayout);
        pAxisRulersLayout.setHorizontalGroup(
            pAxisRulersLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pAxisRulersLayout.createSequentialGroup()
                .addComponent(bLocation, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(2, 2, 2)
                .addComponent(lLocation, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cLocation, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pAxisRulersLayout.createSequentialGroup()
                .addComponent(bReverse, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(2, 2, 2)
                .addComponent(lReverse, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cReverse, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pAxisRulersLayout.createSequentialGroup()
                .addGap(18, 18, 18)
                .addGroup(pAxisRulersLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(pLocation, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(pReverse, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );
        pAxisRulersLayout.setVerticalGroup(
            pAxisRulersLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pAxisRulersLayout.createSequentialGroup()
                .addGroup(pAxisRulersLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(pAxisRulersLayout.createSequentialGroup()
                        .addGap(3, 3, 3)
                        .addComponent(bLocation, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(lLocation, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cLocation, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addComponent(pLocation, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(5, 5, 5)
                .addGroup(pAxisRulersLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(pAxisRulersLayout.createSequentialGroup()
                        .addGap(3, 3, 3)
                        .addComponent(bReverse, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(lReverse, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cReverse, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addComponent(pReverse, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
    }

    /**
    * Loads the current properties of the section Axis Rulers.
    *
    * @param axesID Enters the identification of Axes.
    */
    private void initPropertiesAxisRulers(String axesID) {
        if (axesID != null) {
            currentaxes = axesID;
            /** Get the current status of the property: Location */
            int currentLocationX = (Integer) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_X_AXIS_LOCATION__);
            cLocationX.setSelectedIndex(currentLocationX);
            int currentLocationY = (Integer) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_Y_AXIS_LOCATION__);
            cLocationY.setSelectedIndex(currentLocationY - 2);

            /** Get the current status of the property: Reverse */
            boolean currentReverseX = (Boolean) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_X_AXIS_REVERSE__);
            if (currentReverseX) {
                cReverseX.setSelectedIndex(1);
            } else {
                cReverseX.setSelectedIndex(0);
            }
            boolean currentReverseY = (Boolean) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_Y_AXIS_REVERSE__);
            if (currentReverseX) {
                cReverseY.setSelectedIndex(1);
            } else {
                cReverseY.setSelectedIndex(0);
            }
            boolean currentReverseZ = (Boolean) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_Z_AXIS_REVERSE__);
            if (currentReverseX) {
                cReverseZ.setSelectedIndex(1);
            } else {
                cReverseZ.setSelectedIndex(0);
            }
        }
    }

    /**
    * Insert the icons on buttons.
    */
    private void setIconsAxisRulers() {
        bAxisRulers.setIcon(new javax.swing.ImageIcon(Inspector.icon_collapse));
        bAxisRulers.setSelectedIcon(new javax.swing.ImageIcon(Inspector.icon_expand));
        bLocation.setIcon(new javax.swing.ImageIcon(Inspector.icon_collapse));
        bLocation.setSelectedIcon(new javax.swing.ImageIcon(Inspector.icon_expand));
        bReverse.setIcon(new javax.swing.ImageIcon(Inspector.icon_collapse));
        bReverse.setSelectedIcon(new javax.swing.ImageIcon(Inspector.icon_expand));
    }

    /**
    * Implement the action button to show/hide.
    * 
    * @param evt
    */
    private void bAxisRulersActionPerformed(java.awt.event.ActionEvent evt) {
        if (bAxisRulers.isSelected()) {
            pAxisRulers.setVisible(false);
        } else {
            pAxisRulers.setVisible(true);
        }
    }

    /**
    * Implement the action button to show/hide.
    * 
    * @param evt
    */
    private void bLocationActionPerformed(java.awt.event.ActionEvent evt) {
        if (bLocation.isSelected()) {
            pLocation.setVisible(false);
        } else {
            pLocation.setVisible(true);
        }
    }

    /**
    * Implement the action button to show/hide.
    * 
    * @param evt
    */
    private void bReverseActionPerformed(java.awt.event.ActionEvent evt) {
        if (bReverse.isSelected()) {
            pReverse.setVisible(false);
        } else {
            pReverse.setVisible(true);
        }
    }

    /**
    * Inserts the current situation of the location of the axis on the main label.
    */
    public void titleLocation(){
        String titleLocationx = cLocationX.getSelectedItem().toString();
        String titleLocationy = cLocationY.getSelectedItem().toString();
        String titleLocation = titleLocationx + " , " + titleLocationy;
        cLocation.setText(" [" + titleLocation + "]");
    }

    /**
    * Inserts the current state of functionality: REVERSE in main label.
    */
    public void titleReverse(){
        String titleReverseX = cReverseX.getSelectedItem().toString();
        String titleReverseY = cReverseY.getSelectedItem().toString();
        String titleReverseZ = cReverseZ.getSelectedItem().toString();
        String titleReverse = titleReverseX + " , " + titleReverseY + " , " + titleReverseZ;
        cReverse.setText(" [" + titleReverse + "]");
    }

    /**
    * Updates the property: x Location.
    *
    * @param evt
    */
    private void cLocationXActionPerformed(java.awt.event.ActionEvent evt) {
        int setLocationX = cLocationX.getSelectedIndex();
        GraphicController.getController()
                .setProperty(currentaxes, GraphicObjectProperties.__GO_X_AXIS_LOCATION__, setLocationX);
        titleLocation();
    }

    /**
    * Updates the property: y Location.
    *
    * @param evt
    */
    private void cLocationYActionPerformed(java.awt.event.ActionEvent evt) {
        int setLocationY = cLocationY.getSelectedIndex();
        GraphicController.getController()
                .setProperty(currentaxes, GraphicObjectProperties.__GO_Y_AXIS_LOCATION__, setLocationY + 2);
        titleLocation();
    }

    /**
    * Updates the property: x Reverse.
    *
    * @param evt
    */
    private void cReverseXActionPerformed(java.awt.event.ActionEvent evt) {
        boolean setReverseX = true;
        if (cReverseX.getSelectedIndex() == 0) {
            setReverseX = false;
        }
        GraphicController.getController()
                .setProperty(currentaxes, GraphicObjectProperties.__GO_X_AXIS_REVERSE__, setReverseX);
        titleReverse();
    }

    /**
    * Updates the property: y Reverse.
    *
    * @param evt
    */
    private void cReverseYActionPerformed(java.awt.event.ActionEvent evt) {
        boolean setReverseY = true;
        if (cReverseY.getSelectedIndex() == 0) {
            setReverseY = false;
        }
        GraphicController.getController()
                .setProperty(currentaxes, GraphicObjectProperties.__GO_Y_AXIS_REVERSE__, setReverseY);
        titleReverse();
    }

    /**
    * Updates the property: z Reverse.
    *
    * @param evt
    */
    private void cReverseZActionPerformed(java.awt.event.ActionEvent evt) {
        boolean setReverseZ = true;
        if (cReverseZ.getSelectedIndex() == 0) {
            setReverseZ = false;
        }
        GraphicController.getController()
                .setProperty(currentaxes, GraphicObjectProperties.__GO_Z_AXIS_REVERSE__, setReverseZ);
        titleReverse();
    }
}