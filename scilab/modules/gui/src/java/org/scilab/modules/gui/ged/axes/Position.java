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

import org.scilab.modules.gui.ged.Inspector;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
* Construction and startup of all components of the section: Position.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Position extends Label {
    //Header - Label
    protected static javax.swing.JToggleButton bPosition;
    protected javax.swing.JLabel lPosition;
    protected javax.swing.JSeparator sPosition;
    protected static javax.swing.JPanel pPosition;
    //Components of the property: Axes Bounds
    protected static javax.swing.JToggleButton bAxesBounds;
    protected javax.swing.JLabel lAxesBounds;
    protected javax.swing.JLabel cAxesBounds;
    protected static javax.swing.JPanel pAxesBounds;
    protected javax.swing.JLabel lBoundsLeft;
    protected javax.swing.JTextField cBoundsLeft;
    protected javax.swing.JLabel lBoundsUp;
    protected javax.swing.JTextField cBoundsUp;
    protected javax.swing.JLabel lBoundsWidth;
    protected javax.swing.JTextField cBoundsWidth;
    protected javax.swing.JLabel lBoundsHeight;
    protected javax.swing.JTextField cBoundsHeight;
    //Components of the property: Margins
    protected static javax.swing.JToggleButton bMargins;
    protected javax.swing.JLabel lMargins;
    protected javax.swing.JLabel cMargins;
    protected static javax.swing.JPanel pMargins;
    protected javax.swing.JLabel lMarginsLeft;
    protected javax.swing.JTextField cMarginsLeft;
    protected javax.swing.JLabel lMarginsRight;
    protected javax.swing.JTextField cMarginsRight;
    protected javax.swing.JLabel lMarginsTop;
    protected javax.swing.JTextField cMarginsTop;
    protected javax.swing.JLabel lMarginsBottom;
    protected javax.swing.JTextField cMarginsBottom;

    /**
    * Initializes the properties and the icons of the buttons.
    *
    * @param objectID Enters the identification of Axes.
    */
    public Position(String objectID) {
        super(objectID);
        position();
        setIconsPosition();
        initPropertiesPosition(objectID);
    }

    /**
    * It has all the components of the section Position.
    */
    @Override
    public void positionComponents() {
        //Header
        bPosition = new javax.swing.JToggleButton();
        lPosition = new javax.swing.JLabel();
        sPosition = new javax.swing.JSeparator();
        pPosition = new javax.swing.JPanel();
        //Axes bounds
        bAxesBounds = new javax.swing.JToggleButton();
        lAxesBounds = new javax.swing.JLabel();
        cAxesBounds = new javax.swing.JLabel();
        pAxesBounds = new javax.swing.JPanel();
        lBoundsLeft = new javax.swing.JLabel();
        cBoundsLeft = new javax.swing.JTextField();
        lBoundsUp = new javax.swing.JLabel();
        cBoundsUp = new javax.swing.JTextField();
        lBoundsWidth = new javax.swing.JLabel();
        cBoundsWidth = new javax.swing.JTextField();
        lBoundsHeight = new javax.swing.JLabel();
        cBoundsHeight = new javax.swing.JTextField();
        //Margins
        bMargins = new javax.swing.JToggleButton();
        lMargins = new javax.swing.JLabel();
        cMargins = new javax.swing.JLabel();
        pMargins = new javax.swing.JPanel();
        lMarginsLeft = new javax.swing.JLabel();
        cMarginsLeft = new javax.swing.JTextField();
        lMarginsRight = new javax.swing.JLabel();
        cMarginsRight = new javax.swing.JTextField();
        lMarginsTop = new javax.swing.JLabel();
        cMarginsTop = new javax.swing.JTextField();
        lMarginsBottom = new javax.swing.JLabel();
        cMarginsBottom = new javax.swing.JTextField();

        //Components of the header: Position
        bPosition.setBorder(null);
        bPosition.setBorderPainted(false);
        bPosition.setContentAreaFilled(false);
        bPosition.setMaximumSize(new java.awt.Dimension(16, 16));
        bPosition.setMinimumSize(new java.awt.Dimension(16, 16));
        bPosition.setPreferredSize(new java.awt.Dimension(16, 16));
        bPosition.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bPositionActionPerformed(evt);
            }
        });

        lPosition.setFont(new java.awt.Font("Tahoma", 1, 11));
        lPosition.setText(MessagesGED.position);

        sPosition.setPreferredSize(new java.awt.Dimension(50, 2));

        //Components of the property: Axes Bounds.
        pAxesBounds.setVisible(false);
        bAxesBounds.setSelected(true);
        bAxesBounds.setBorder(null);
        bAxesBounds.setBorderPainted(false);
        bAxesBounds.setContentAreaFilled(false);
        bAxesBounds.setMaximumSize(new java.awt.Dimension(16, 16));
        bAxesBounds.setMinimumSize(new java.awt.Dimension(16, 16));
        bAxesBounds.setPreferredSize(new java.awt.Dimension(16, 16));
        bAxesBounds.setRolloverEnabled(false);
        bAxesBounds.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bAxesBoundsActionPerformed(evt);
            }
        });

        lAxesBounds.setBackground(new java.awt.Color(255, 255, 255));
        lAxesBounds.setText(" " + MessagesGED.axes_bounds);
        lAxesBounds.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lAxesBounds.setOpaque(true);
        lAxesBounds.setPreferredSize(new java.awt.Dimension(120, 16));

        cAxesBounds.setBackground(new java.awt.Color(255, 255, 255));
        cAxesBounds.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cAxesBounds.setOpaque(true);

        lBoundsLeft.setBackground(new java.awt.Color(255, 255, 255));
        lBoundsLeft.setText(" " + MessagesGED.left);
        lBoundsLeft.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lBoundsLeft.setOpaque(true);

        cBoundsLeft.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cBoundsLeft.setPreferredSize(new java.awt.Dimension(55, 20));
        cBoundsLeft.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cBoundsLeftActionPerformed(evt);
            }
        });

        lBoundsUp.setBackground(new java.awt.Color(255, 255, 255));
        lBoundsUp.setText(" " + MessagesGED.up);
        lBoundsUp.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lBoundsUp.setOpaque(true);

        cBoundsUp.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cBoundsUp.setPreferredSize(new java.awt.Dimension(55, 20));
        cBoundsUp.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cBoundsUpActionPerformed(evt);
            }
        });

        lBoundsWidth.setBackground(new java.awt.Color(255, 255, 255));
        lBoundsWidth.setText(" " + MessagesGED.width);
        lBoundsWidth.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lBoundsWidth.setOpaque(true);

        cBoundsWidth.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cBoundsWidth.setPreferredSize(new java.awt.Dimension(55, 20));
        cBoundsWidth.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cBoundsWidthActionPerformed(evt);
            }
        });

        lBoundsHeight.setBackground(new java.awt.Color(255, 255, 255));
        lBoundsHeight.setText(" " + MessagesGED.height);
        lBoundsHeight.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lBoundsHeight.setOpaque(true);

        cBoundsHeight.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cBoundsHeight.setPreferredSize(new java.awt.Dimension(55, 20));
        cBoundsHeight.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cBoundsHeightActionPerformed(evt);
            }
        });

        titleBounds();

        //Positioning the components of the property: Axes Bounds.
        javax.swing.GroupLayout pAxesBoundsLayout = new javax.swing.GroupLayout(pAxesBounds);
        pAxesBounds.setLayout(pAxesBoundsLayout);
        pAxesBoundsLayout.setHorizontalGroup(
            pAxesBoundsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pAxesBoundsLayout.createSequentialGroup()
                .addComponent(lBoundsLeft, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cBoundsLeft, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pAxesBoundsLayout.createSequentialGroup()
                .addComponent(lBoundsUp, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cBoundsUp, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pAxesBoundsLayout.createSequentialGroup()
                .addComponent(lBoundsWidth, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cBoundsWidth, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pAxesBoundsLayout.createSequentialGroup()
                .addComponent(lBoundsHeight, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cBoundsHeight, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        pAxesBoundsLayout.setVerticalGroup(
            pAxesBoundsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pAxesBoundsLayout.createSequentialGroup()
                .addGroup(pAxesBoundsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lBoundsLeft, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cBoundsLeft, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pAxesBoundsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lBoundsUp, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cBoundsUp, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pAxesBoundsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lBoundsWidth, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cBoundsWidth, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pAxesBoundsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lBoundsHeight, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cBoundsHeight, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );
        
        //Components of the property: Margins.
        bMargins.setSelected(true);
        bMargins.setBorder(null);
        bMargins.setBorderPainted(false);
        bMargins.setContentAreaFilled(false);
        bMargins.setMaximumSize(new java.awt.Dimension(16, 16));
        bMargins.setMinimumSize(new java.awt.Dimension(16, 16));
        bMargins.setPreferredSize(new java.awt.Dimension(16, 16));
        bMargins.setRolloverEnabled(false);
        bMargins.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bMarginsActionPerformed(evt);
            }
        });

        pMargins.setVisible(false);

        lMargins.setBackground(new java.awt.Color(255, 255, 255));
        lMargins.setText(" " + MessagesGED.margins);
        lMargins.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lMargins.setOpaque(true);
        lMargins.setPreferredSize(new java.awt.Dimension(120, 16));

        cMargins.setBackground(new java.awt.Color(255, 255, 255));
        cMargins.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cMargins.setOpaque(true);

        lMarginsLeft.setBackground(new java.awt.Color(255, 255, 255));
        lMarginsLeft.setText(" " + MessagesGED.left);
        lMarginsLeft.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lMarginsLeft.setOpaque(true);

        cMarginsLeft.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cMarginsLeft.setPreferredSize(new java.awt.Dimension(55, 20));
        cMarginsLeft.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cMarginsLeftActionPerformed(evt);
            }
        });

        lMarginsRight.setBackground(new java.awt.Color(255, 255, 255));
        lMarginsRight.setText(" " + MessagesGED.right);
        lMarginsRight.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lMarginsRight.setOpaque(true);

        cMarginsRight.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cMarginsRight.setPreferredSize(new java.awt.Dimension(55, 20));
        cMarginsRight.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cMarginsRightActionPerformed(evt);
            }
        });

        lMarginsTop.setBackground(new java.awt.Color(255, 255, 255));
        lMarginsTop.setText(" " + MessagesGED.top);
        lMarginsTop.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lMarginsTop.setOpaque(true);

        cMarginsTop.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cMarginsTop.setPreferredSize(new java.awt.Dimension(55, 20));
        cMarginsTop.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cMarginsTopActionPerformed(evt);
            }
        });

        lMarginsBottom.setBackground(new java.awt.Color(255, 255, 255));
        lMarginsBottom.setText(" " + MessagesGED.bottom);
        lMarginsBottom.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lMarginsBottom.setOpaque(true);

        cMarginsBottom.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cMarginsBottom.setPreferredSize(new java.awt.Dimension(55, 20));
        cMarginsBottom.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cMarginsBottomActionPerformed(evt);
            }
        });

        titleMargins();

        //Positioning the components of the property: Margins.
        javax.swing.GroupLayout pMarginsLayout = new javax.swing.GroupLayout(pMargins);
        pMargins.setLayout(pMarginsLayout);
        pMarginsLayout.setHorizontalGroup(
            pMarginsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pMarginsLayout.createSequentialGroup()
                .addComponent(lMarginsLeft, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cMarginsLeft, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pMarginsLayout.createSequentialGroup()
                .addComponent(lMarginsRight, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cMarginsRight, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pMarginsLayout.createSequentialGroup()
                .addComponent(lMarginsTop, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cMarginsTop, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pMarginsLayout.createSequentialGroup()
                .addComponent(lMarginsBottom, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cMarginsBottom, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        pMarginsLayout.setVerticalGroup(
            pMarginsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pMarginsLayout.createSequentialGroup()
                .addGroup(pMarginsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lMarginsLeft, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cMarginsLeft, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pMarginsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lMarginsRight, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cMarginsRight, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pMarginsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lMarginsTop, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cMarginsTop, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pMarginsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lMarginsBottom, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cMarginsBottom, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );
    }

    /**
    * Positioning all the components of the Position.
    */
    private void position() {
        //Positioning all components.
        javax.swing.GroupLayout pPositionLayout = new javax.swing.GroupLayout(pPosition);
        pPosition.setLayout(pPositionLayout);
        pPositionLayout.setHorizontalGroup(
            pPositionLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pPositionLayout.createSequentialGroup()
                .addComponent(bAxesBounds, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(2, 2, 2)
                .addComponent(lAxesBounds, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cAxesBounds, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pPositionLayout.createSequentialGroup()
                .addComponent(bMargins, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(2, 2, 2)
                .addComponent(lMargins, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cMargins, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pPositionLayout.createSequentialGroup()
                .addGap(18, 18, 18)
                .addGroup(pPositionLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(pAxesBounds, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(pMargins, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );
        pPositionLayout.setVerticalGroup(
            pPositionLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pPositionLayout.createSequentialGroup()
                .addGroup(pPositionLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(bAxesBounds, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lAxesBounds, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cAxesBounds, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addComponent(pAxesBounds, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addGroup(pPositionLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(bMargins, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(pPositionLayout.createSequentialGroup()
                        .addGap(1, 1, 1)
                        .addGroup(pPositionLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(lMargins, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(cMargins, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE))))
                .addGap(5, 5, 5)
                .addComponent(pMargins, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
    }

    /**
    * Loads the current properties of the section Position.
    *
    * @param axesID Enters the identification of Axes.
    */
    private void initPropertiesPosition(String axesID) {
        if (axesID != null) {
            currentaxes = axesID;
            /** Get the current status of the property: Axes Bounds */
            Double[] currentBounds = (Double []) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_AXES_BOUNDS__);
            cBoundsLeft.setText(currentBounds[0].toString());
            cBoundsUp.setText(currentBounds[1].toString());
            cBoundsWidth.setText(currentBounds[2].toString());
            cBoundsHeight.setText(currentBounds[3].toString());
            titleBounds();

            /** Get the current status of the property: Margins */
            Double[] currentMargins = (Double []) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_MARGINS__);
            cMarginsLeft.setText(currentMargins[0].toString());
            cMarginsRight.setText(currentMargins[1].toString());
            cMarginsTop.setText(currentMargins[2].toString());
            cMarginsBottom.setText(currentMargins[3].toString());
            titleMargins();
        }
    }

    /**
    * Insert the icons on buttons.
    */
    private final void setIconsPosition() {
        bPosition.setIcon(new javax.swing.ImageIcon(Inspector.icon_collapse));
        bPosition.setSelectedIcon(new javax.swing.ImageIcon(Inspector.icon_expand));
        bMargins.setIcon(new javax.swing.ImageIcon(Inspector.icon_collapse));
        bMargins.setSelectedIcon(new javax.swing.ImageIcon(Inspector.icon_expand));
        bAxesBounds.setIcon(new javax.swing.ImageIcon(Inspector.icon_collapse));
        bAxesBounds.setSelectedIcon(new javax.swing.ImageIcon(Inspector.icon_expand));
    }

    /**
    * Implement the action button to show/hide.
    * 
    * @param evt
    */
    private void bLabelActionPerformed(java.awt.event.ActionEvent evt) {
        if (bLabel.isSelected()) {
            pLabel.setVisible(false);
        } else {
            pLabel.setVisible(true);
        }
    }

    /**
    * Implement the action button to show/hide.
    * 
    * @param evt
    */
    private void bPositionActionPerformed(java.awt.event.ActionEvent evt) {
        if (bPosition.isSelected()) {
            pPosition.setVisible(false);
        } else {
            pPosition.setVisible(true);
        }
    }

    /**
    * Implement the action button to show/hide.
    * 
    * @param evt
    */
    private void bAxesBoundsActionPerformed(java.awt.event.ActionEvent evt) {
        if (bAxesBounds.isSelected()) {
            pAxesBounds.setVisible(false);
        } else {
            pAxesBounds.setVisible(true);
        }
    }

    /**
    * Implement the action button to show/hide.
    * 
    * @param evt
    */
    private void bMarginsActionPerformed(java.awt.event.ActionEvent evt) {
        if (bMargins.isSelected()) {
            pMargins.setVisible(false);
        } else {
            pMargins.setVisible(true);
        }
    }

    /**
    * Inserts the current state of functionality: AXES BOUNDS in main label.
    */
    public void titleBounds() {
        String titleBoundsLeft = cBoundsLeft.getText();
        String titleBoundsUp = cBoundsUp.getText();
        String titleBoundsWidth = cBoundsWidth.getText();
        String titleBoundsHeight = cBoundsHeight.getText();
        String titleBounds = titleBoundsLeft + " , " + titleBoundsUp + " , "
                             + titleBoundsWidth + " , " + titleBoundsHeight;
        cAxesBounds.setText(" [" + titleBounds + "]");
    }

    /**
    * Inserts the current state of functionality: MARGINS in main label.
    */
    public void titleMargins() {
        String titleMarginsLeft = cMarginsLeft.getText();
        String titleMarginsRight = cMarginsRight.getText();
        String titleMarginsTop = cMarginsTop.getText();
        String titleMarginsBottom = cMarginsBottom.getText();
        String titleMargins = titleMarginsLeft + " , " + titleMarginsRight + " , "
                             + titleMarginsTop + " , " + titleMarginsBottom;
        cMargins.setText(" [" + titleMargins + "]");
    }

    /**
    * Updates the property: Axes Bounds - Left.
    *
    * @param evt
    */
    private void cBoundsLeftActionPerformed(java.awt.event.ActionEvent evt) {
        Double[] value = new Double[4];
        value[0] = Double.parseDouble(cBoundsLeft.getText());
        value[1] = Double.parseDouble(cBoundsUp.getText());
        value[2] = Double.parseDouble(cBoundsWidth.getText());
        value[3] = Double.parseDouble(cBoundsHeight.getText());
        GraphicController.getController()
                .setProperty(currentaxes, GraphicObjectProperties.__GO_AXES_BOUNDS__, value);
        titleBounds();
    }

    /**
    * Updates the property: Axes Bounds - Up.
    *
    * @param evt
    */
    private void cBoundsUpActionPerformed(java.awt.event.ActionEvent evt) {
        cBoundsLeftActionPerformed(evt);
    }

    /**
    * Updates the property: Axes Bounds - Width.
    *
    * @param evt
    */
    private void cBoundsWidthActionPerformed(java.awt.event.ActionEvent evt) {
        cBoundsLeftActionPerformed(evt);
    }

    /**
    * Updates the property: Axes Bounds - Height.
    *
    * @param evt
    */
    private void cBoundsHeightActionPerformed(java.awt.event.ActionEvent evt) {
        cBoundsLeftActionPerformed(evt);
    }

    /**
    * Updates the property: Margins - Left.
    *
    * @param evt
    */
    private void cMarginsLeftActionPerformed(java.awt.event.ActionEvent evt) {
        Double[] value = new Double[4];
        value[0] = Double.parseDouble(cMarginsLeft.getText());
        value[1] = Double.parseDouble(cMarginsRight.getText());
        value[2] = Double.parseDouble(cMarginsTop.getText());
        value[3] = Double.parseDouble(cMarginsBottom.getText());
        GraphicController.getController()
                .setProperty(currentaxes, GraphicObjectProperties.__GO_MARGINS__, value);

        titleMargins();
    }

    /**
    * Updates the property: Margins - Right.
    *
    * @param evt
    */
    private void cMarginsRightActionPerformed(java.awt.event.ActionEvent evt) {
        cMarginsLeftActionPerformed(evt);
    }

    /**
    * Updates the property: Margins - Top.
    *
    * @param evt
    */
    private void cMarginsTopActionPerformed(java.awt.event.ActionEvent evt) {
        cMarginsLeftActionPerformed(evt);
    }

    /**
    * Updates the property: Margins - Bottom.
    *
    * @param evt
    */
    private void cMarginsBottomActionPerformed(java.awt.event.ActionEvent evt) {
        cMarginsLeftActionPerformed(evt);
    }
}