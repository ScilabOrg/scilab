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
package org.scilab.modules.graphic_objects.ged;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import java.awt.Color;

/**
* Properties of the figure.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
* */
public class Figure extends javax.swing.JPanel {
    public static javax.swing.JToggleButton bBaseProperties;
    public static javax.swing.JButton bColor;
    public static javax.swing.JToggleButton bControl;
    public static javax.swing.JToggleButton bData;
    public static javax.swing.JToggleButton bStyle;
    private javax.swing.JComboBox cAutoResize;
    private javax.swing.JLabel cColor;
    private javax.swing.JTextField cFigureID;
    private javax.swing.JTextField cFigureName;
    private javax.swing.JComboBox cPixelDrawing;
    private javax.swing.JComboBox cPixmap;
    private javax.swing.JComboBox cRotationStyle;
    private javax.swing.JComboBox cUnits;
    private javax.swing.JComboBox cVisible;
    private javax.swing.JColorChooser chooser;
    private javax.swing.JDialog colorDialog;
    private javax.swing.JTextField cxposition;
    private javax.swing.JTextField cxsize;
    private javax.swing.JTextField cyposition;
    private javax.swing.JTextField cysize;
    private javax.swing.JLabel lAutoResize;
    private javax.swing.JLabel lBaseProperties;
    private javax.swing.JLabel lColor;
    private javax.swing.JLabel lControl;
    private javax.swing.JLabel lData;
    private javax.swing.JLabel lFigureID;
    private javax.swing.JLabel lFigureName;
    private javax.swing.JLabel lPixelDrawing;
    private javax.swing.JLabel lPixmap;
    private javax.swing.JLabel lRotationStyle;
    private javax.swing.JLabel lStyle;
    private javax.swing.JLabel lUnits;
    private javax.swing.JLabel lVisible;
    private javax.swing.JLabel lxposition;
    private javax.swing.JLabel lxsize;
    private javax.swing.JLabel lyposition;
    private javax.swing.JLabel lysize;
    private javax.swing.JButton ok;
    public static javax.swing.JPanel pBaseProperties;
    public static javax.swing.JPanel pColor;
    public static javax.swing.JPanel pControl;
    public static javax.swing.JPanel pData;
    public static javax.swing.JPanel pStyle;
    private javax.swing.JSeparator sBaseProperties;
    private javax.swing.JSeparator sControl;
    private javax.swing.JSeparator sData;
    private javax.swing.JSeparator sStyle;
    private Color choice;
    private String currentfigure = null;


    /**
    * Initializes the components of the JPanel.
    *
    * @param figureUid Enters the identification of curve.
    */
    public Figure(String figureUid) {
        guiFigureComponents();
        initProperties(figureUid);
    }


    /**
    * Loads the current properties of the Figure.
    *
    * @param figureUid Enters the identification of Figure.
    */
    private void initProperties(String figureUid) {
        if (figureUid != null) {
            currentfigure = figureUid;
            /** Get the current status of the property: Visible */
            boolean isVisible = (Boolean) GraphicController.getController()
                    .getProperty(currentfigure, GraphicObjectProperties.__GO_VISIBLE__);
            if (isVisible) {
                cVisible.setSelectedIndex(1);
            } else {
                cVisible.setSelectedIndex(0);
            }
            /** Get the current status of the property: Auto Resize */
            boolean isAutoResize = (Boolean) GraphicController.getController()
                    .getProperty(currentfigure, GraphicObjectProperties.__GO_AUTORESIZE__);
            if (isAutoResize) {
                cAutoResize.setSelectedIndex(1);
            } else {
		cAutoResize.setSelectedIndex(0);
            }
            /** Get the current status of the property: Position */
            Integer[] currentPosition = (Integer[]) GraphicController.getController()
                        .getProperty(currentfigure, GraphicObjectProperties.__GO_POSITION__);
            String currentXposition;
            currentXposition = currentPosition[0].toString();
            cxposition.setText(currentXposition);
            String currentYposition;
            currentYposition = currentPosition[1].toString();
            cyposition.setText(currentXposition);
            /** Get the current status of the property: Size */
            Integer[] currentSize = (Integer[]) GraphicController.getController()
                    .getProperty(currentfigure, GraphicObjectProperties.__GO_SIZE__);
            String currentXsize;
            currentXsize = currentSize[0].toString();
            cxsize.setText(currentXsize);
            String currentYsize;
            currentYsize = currentSize[1].toString();
            cysize.setText(currentXsize);
            /** Get the current status of the property: Figure ID */
            Integer currentFigureID = (Integer) GraphicController.getController()
                    .getProperty(currentfigure, GraphicObjectProperties.__GO_ID__);
            String figureID;
            figureID = currentFigureID.toString();
            cFigureID.setText(figureID);
            /** Get the current status of the property: Figure Name */
            String figureName = (String) GraphicController.getController()
                    .getProperty(currentfigure, GraphicObjectProperties.__GO_NAME__);
            cFigureName.setText(figureName);
            /** Get the current status of the property: Pixmap */
            boolean currentPixmap = (Boolean) GraphicController.getController()
                    .getProperty(currentfigure, GraphicObjectProperties.__GO_PIXMAP__);
            if (currentPixmap) {
                cPixmap.setSelectedIndex(1);
            } else {
		cPixmap.setSelectedIndex(0);
            }
            /** Get the current status of the property: Pixel Drawing Mode */
            Integer currentPixelDrawingMode = (Integer) GraphicController.getController()
                    .getProperty(currentfigure, GraphicObjectProperties.__GO_PIXEL_DRAWING_MODE__);
            cPixelDrawing.setSelectedIndex(currentPixelDrawingMode);
            /** Get the current status of the property: Rotation Style */
            Integer currentRotationStyle = (Integer) GraphicController.getController()
                    .getProperty(currentfigure, GraphicObjectProperties.__GO_ROTATION_TYPE__);
            cRotationStyle.setSelectedIndex(currentRotationStyle);
        }
    }

    /**
    * It has all the components of the Figure JPanel.
    */
    private void guiFigureComponents() {
        colorDialog = new javax.swing.JDialog();
        chooser = new javax.swing.JColorChooser();
        ok = new javax.swing.JButton();
        pBaseProperties = new javax.swing.JPanel();
        lVisible = new javax.swing.JLabel();
        cVisible = new javax.swing.JComboBox();
        bBaseProperties = new javax.swing.JToggleButton();
        lBaseProperties = new javax.swing.JLabel();
        sBaseProperties = new javax.swing.JSeparator();
        pControl = new javax.swing.JPanel();
        lAutoResize = new javax.swing.JLabel();
        cAutoResize = new javax.swing.JComboBox();
        lxposition = new javax.swing.JLabel();
        cxposition = new javax.swing.JTextField();
        lyposition = new javax.swing.JLabel();
        cyposition = new javax.swing.JTextField();
        lxsize = new javax.swing.JLabel();
        cxsize = new javax.swing.JTextField();
        lysize = new javax.swing.JLabel();
        cysize = new javax.swing.JTextField();
        bControl = new javax.swing.JToggleButton();
        lControl = new javax.swing.JLabel();
        sControl = new javax.swing.JSeparator();
        pData = new javax.swing.JPanel();
        lUnits = new javax.swing.JLabel();
        cUnits = new javax.swing.JComboBox();
        lFigureName = new javax.swing.JLabel();
        cFigureName = new javax.swing.JTextField();
        lFigureID = new javax.swing.JLabel();
        cFigureID = new javax.swing.JTextField();
        bData = new javax.swing.JToggleButton();
        lData = new javax.swing.JLabel();
        sData = new javax.swing.JSeparator();
        pStyle = new javax.swing.JPanel();
        pColor = new javax.swing.JPanel();
        bColor = new javax.swing.JButton();
        cColor = new javax.swing.JLabel();
        lColor = new javax.swing.JLabel();
        lPixmap = new javax.swing.JLabel();
        cPixmap = new javax.swing.JComboBox();
        lPixelDrawing = new javax.swing.JLabel();
        cPixelDrawing = new javax.swing.JComboBox();
        lRotationStyle = new javax.swing.JLabel();
        cRotationStyle = new javax.swing.JComboBox();
        bStyle = new javax.swing.JToggleButton();
        lStyle = new javax.swing.JLabel();
        sStyle = new javax.swing.JSeparator();

        colorDialog.setTitle("Choose Color");
        colorDialog.setMinimumSize(new java.awt.Dimension(670, 450));
        colorDialog.setModal(true);
        colorDialog.setResizable(false);

        ok.setText("OK");
        ok.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                okActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout colorDialogLayout = new javax.swing.GroupLayout(colorDialog.getContentPane());
        colorDialog.getContentPane().setLayout(colorDialogLayout);
        colorDialogLayout.setHorizontalGroup(
            colorDialogLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(colorDialogLayout.createSequentialGroup()
                .addComponent(chooser, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(0, 0, Short.MAX_VALUE))
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, colorDialogLayout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(ok)
                .addContainerGap())
        );
        colorDialogLayout.setVerticalGroup(
            colorDialogLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(colorDialogLayout.createSequentialGroup()
                .addComponent(chooser, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(ok)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        pBaseProperties.setAlignmentX(0.0F);
        pBaseProperties.setAlignmentY(0.0F);

        lVisible.setBackground(new java.awt.Color(255, 255, 255));
        lVisible.setText(" Visible");
        lVisible.setAlignmentX(0.5F);
        lVisible.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lVisible.setOpaque(true);
        lVisible.setPreferredSize(new java.awt.Dimension(153, 20));

        cVisible.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Off", "On" }));
        cVisible.setBorder(null);
        cVisible.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        cVisible.setEditor(null);
        cVisible.setPreferredSize(new java.awt.Dimension(150, 20));
        cVisible.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cVisibleActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout pBasePropertiesLayout = new javax.swing.GroupLayout(pBaseProperties);
        pBaseProperties.setLayout(pBasePropertiesLayout);
        pBasePropertiesLayout.setHorizontalGroup(
            pBasePropertiesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pBasePropertiesLayout.createSequentialGroup()
                .addGap(18, 18, 18)
                .addComponent(lVisible, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cVisible, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        pBasePropertiesLayout.setVerticalGroup(
            pBasePropertiesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pBasePropertiesLayout.createSequentialGroup()
                .addGap(1, 1, 1)
                .addComponent(lVisible, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addComponent(cVisible, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
        );

        bBaseProperties.setBorder(null);
        bBaseProperties.setBorderPainted(false);
        bBaseProperties.setContentAreaFilled(false);
        bBaseProperties.setMaximumSize(new java.awt.Dimension(16, 16));
        bBaseProperties.setMinimumSize(new java.awt.Dimension(16, 16));
        bBaseProperties.setPreferredSize(new java.awt.Dimension(16, 16));
        bBaseProperties.setRolloverEnabled(false);
        bBaseProperties.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bBasePropertiesActionPerformed(evt);
            }
        });

        lBaseProperties.setFont(new java.awt.Font("Tahoma", 1, 11));
        lBaseProperties.setText("Base Properties");

        sBaseProperties.setPreferredSize(new java.awt.Dimension(50, 2));

        pControl.setAlignmentX(0.0F);
        pControl.setAlignmentY(0.0F);

        lAutoResize.setBackground(new java.awt.Color(255, 255, 255));
        lAutoResize.setText(" Auto Resize");
        lAutoResize.setAlignmentX(0.5F);
        lAutoResize.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lAutoResize.setOpaque(true);
        lAutoResize.setPreferredSize(new java.awt.Dimension(153, 20));

        cAutoResize.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Off", "On" }));
        cAutoResize.setBorder(null);
        cAutoResize.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        cAutoResize.setEditor(null);
        cAutoResize.setPreferredSize(new java.awt.Dimension(150, 20));
        cAutoResize.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cAutoResizeActionPerformed(evt);
            }
        });

        lxposition.setBackground(new java.awt.Color(255, 255, 255));
        lxposition.setText(" X Position");
        lxposition.setAlignmentX(0.5F);
        lxposition.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lxposition.setOpaque(true);
        lxposition.setPreferredSize(new java.awt.Dimension(153, 20));

        cxposition.setToolTipText("Enter a x position.");
        cxposition.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cxposition.setPreferredSize(new java.awt.Dimension(150, 20));
        cxposition.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cxpositionActionPerformed(evt);
            }
        });

        lyposition.setBackground(new java.awt.Color(255, 255, 255));
        lyposition.setText(" Y Position");
        lyposition.setAlignmentX(0.5F);
        lyposition.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lyposition.setOpaque(true);
        lyposition.setPreferredSize(new java.awt.Dimension(153, 20));

        cyposition.setToolTipText("Enter a y position.");
        cyposition.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cyposition.setPreferredSize(new java.awt.Dimension(150, 20));
        cyposition.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cypositionActionPerformed(evt);
            }
        });

        lxsize.setBackground(new java.awt.Color(255, 255, 255));
        lxsize.setText(" X Size");
        lxsize.setAlignmentX(0.5F);
        lxsize.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lxsize.setOpaque(true);
        lxsize.setPreferredSize(new java.awt.Dimension(153, 20));

        cxsize.setToolTipText("Enter a x size.");
        cxsize.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cxsize.setPreferredSize(new java.awt.Dimension(150, 20));
        cxsize.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cxsizeActionPerformed(evt);
            }
        });

        lysize.setBackground(new java.awt.Color(255, 255, 255));
        lysize.setText(" Y Size");
        lysize.setAlignmentX(0.5F);
        lysize.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lysize.setOpaque(true);
        lysize.setPreferredSize(new java.awt.Dimension(153, 20));

        cysize.setToolTipText("Enter a y size.");
        cysize.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cysize.setPreferredSize(new java.awt.Dimension(150, 20));
        cysize.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cysizeActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout pControlLayout = new javax.swing.GroupLayout(pControl);
        pControl.setLayout(pControlLayout);
        pControlLayout.setHorizontalGroup(
            pControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pControlLayout.createSequentialGroup()
                .addGap(18, 18, 18)
                .addGroup(pControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(pControlLayout.createSequentialGroup()
                        .addComponent(lAutoResize, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cAutoResize, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(pControlLayout.createSequentialGroup()
                        .addComponent(lxposition, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cxposition, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(pControlLayout.createSequentialGroup()
                        .addComponent(lyposition, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cyposition, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(pControlLayout.createSequentialGroup()
                        .addComponent(lxsize, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cxsize, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(pControlLayout.createSequentialGroup()
                        .addComponent(lysize, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cysize, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))))
        );
        pControlLayout.setVerticalGroup(
            pControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pControlLayout.createSequentialGroup()
                .addGroup(pControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lAutoResize, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cAutoResize, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lxposition, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cxposition, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lyposition, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cyposition, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lxsize, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cxsize, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lysize, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cysize, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );

        bControl.setBorder(null);
        bControl.setBorderPainted(false);
        bControl.setContentAreaFilled(false);
        bControl.setMaximumSize(new java.awt.Dimension(16, 16));
        bControl.setMinimumSize(new java.awt.Dimension(16, 16));
        bControl.setPreferredSize(new java.awt.Dimension(16, 16));
        bControl.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bControlActionPerformed(evt);
            }
        });

        lControl.setFont(new java.awt.Font("Tahoma", 1, 11));
        lControl.setText("Control");

        sControl.setPreferredSize(new java.awt.Dimension(50, 2));

        pData.setAlignmentX(0.0F);
        pData.setAlignmentY(0.0F);

        lUnits.setBackground(new java.awt.Color(255, 255, 255));
        lUnits.setText(" Units");
        lUnits.setAlignmentX(0.5F);
        lUnits.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lUnits.setOpaque(true);
        lUnits.setPreferredSize(new java.awt.Dimension(153, 20));

        cUnits.setModel(new javax.swing.DefaultComboBoxModel(new String[] { 
            "Inches", "Centimeters", "Normalized",
            "Points", "Pixels", "Characters"}));
        cUnits.setSelectedIndex(4);
        cUnits.setBorder(null);
        cUnits.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        cUnits.setEditor(null);
        cUnits.setPreferredSize(new java.awt.Dimension(150, 20));

        lFigureName.setBackground(new java.awt.Color(255, 255, 255));
        lFigureName.setText(" Figure Name");
        lFigureName.setAlignmentX(0.5F);
        lFigureName.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lFigureName.setOpaque(true);
        lFigureName.setPreferredSize(new java.awt.Dimension(153, 20));

        cFigureName.setToolTipText("Enter a figure name.");
        cFigureName.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cFigureName.setPreferredSize(new java.awt.Dimension(150, 20));
        cFigureName.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cFigureNameActionPerformed(evt);
            }
        });

        lFigureID.setBackground(new java.awt.Color(255, 255, 255));
        lFigureID.setText(" Figure ID");
        lFigureID.setAlignmentX(0.5F);
        lFigureID.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lFigureID.setOpaque(true);
        lFigureID.setPreferredSize(new java.awt.Dimension(153, 20));

        cFigureID.setBackground(new java.awt.Color(238, 238, 238));
        cFigureID.setEditable(true);
        cFigureID.setToolTipText("Number of current figure.");
        cFigureID.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cFigureID.setPreferredSize(new java.awt.Dimension(150, 20));
        cFigureID.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cFigureIDActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout pDataLayout = new javax.swing.GroupLayout(pData);
        pData.setLayout(pDataLayout);
        pDataLayout.setHorizontalGroup(
            pDataLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pDataLayout.createSequentialGroup()
                .addGap(18, 18, 18)
                .addGroup(pDataLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(pDataLayout.createSequentialGroup()
                        .addComponent(lFigureID, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cFigureID, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(pDataLayout.createSequentialGroup()
                        .addComponent(lFigureName, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cFigureName, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(pDataLayout.createSequentialGroup()
                        .addComponent(lUnits, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cUnits, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))))
        );
        pDataLayout.setVerticalGroup(
            pDataLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pDataLayout.createSequentialGroup()
                .addGroup(pDataLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lFigureID, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(pDataLayout.createSequentialGroup()
                        .addGap(1, 1, 1)
                        .addComponent(cFigureID, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addGap(6, 6, 6)
                .addGroup(pDataLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lFigureName, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(pDataLayout.createSequentialGroup()
                        .addGap(1, 1, 1)
                        .addComponent(cFigureName, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addGap(6, 6, 6)
                .addGroup(pDataLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(pDataLayout.createSequentialGroup()
                        .addGap(1, 1, 1)
                        .addComponent(lUnits, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(cUnits, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );

        bData.setBorder(null);
        bData.setBorderPainted(false);
        bData.setContentAreaFilled(false);
        bData.setMaximumSize(new java.awt.Dimension(16, 16));
        bData.setMinimumSize(new java.awt.Dimension(16, 16));
        bData.setPreferredSize(new java.awt.Dimension(16, 16));
        bData.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bDataActionPerformed(evt);
            }
        });

        lData.setFont(new java.awt.Font("Tahoma", 1, 11));
        lData.setText("Data");

        sData.setPreferredSize(new java.awt.Dimension(50, 2));

        pStyle.setAlignmentX(0.0F);
        pStyle.setAlignmentY(0.0F);

        pColor.setBackground(new java.awt.Color(255, 255, 255));
        pColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        pColor.setPreferredSize(new java.awt.Dimension(150, 20));


        bColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        bColor.setContentAreaFilled(false);
        bColor.setPreferredSize(new java.awt.Dimension(16, 16));
        bColor.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bColorActionPerformed(evt);
            }
        });

        cColor.setBackground(new java.awt.Color(239, 77, 31));
        cColor.setText(" ");
        cColor.setToolTipText("");
        cColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cColor.setOpaque(true);

        javax.swing.GroupLayout pColorLayout = new javax.swing.GroupLayout(pColor);
        pColor.setLayout(pColorLayout);
        pColorLayout.setHorizontalGroup(
            pColorLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pColorLayout.createSequentialGroup()
                .addGap(35, 35, 35)
                .addComponent(bColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(11, 11, 11)
                .addComponent(cColor, javax.swing.GroupLayout.PREFERRED_SIZE, 30, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        pColorLayout.setVerticalGroup(
            pColorLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pColorLayout.createSequentialGroup()
                .addGap(1, 1, 1)
                .addComponent(bColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pColorLayout.createSequentialGroup()
                .addGap(3, 3, 3)
                .addComponent(cColor, javax.swing.GroupLayout.PREFERRED_SIZE, 11, javax.swing.GroupLayout.PREFERRED_SIZE))
        );

        lColor.setBackground(new java.awt.Color(255, 255, 255));
        lColor.setText(" Color");
        lColor.setAlignmentX(0.5F);
        lColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lColor.setOpaque(true);
        lColor.setPreferredSize(new java.awt.Dimension(153, 20));

        lPixmap.setBackground(new java.awt.Color(255, 255, 255));
        lPixmap.setText(" Pixmap");
        lPixmap.setAlignmentX(0.5F);
        lPixmap.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lPixmap.setOpaque(true);
        lPixmap.setPreferredSize(new java.awt.Dimension(153, 20));

        cPixmap.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Off", "On" }));
        cPixmap.setBorder(null);
        cPixmap.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        cPixmap.setEditor(null);
        cPixmap.setPreferredSize(new java.awt.Dimension(150, 20));
        cPixmap.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cPixmapActionPerformed(evt);
            }
        });

        lPixelDrawing.setBackground(new java.awt.Color(255, 255, 255));
        lPixelDrawing.setText(" Pixel Drawing Mode");
        lPixelDrawing.setAlignmentX(0.5F);
        lPixelDrawing.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lPixelDrawing.setOpaque(true);
        lPixelDrawing.setPreferredSize(new java.awt.Dimension(153, 20));

        cPixelDrawing.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "clear", "and", "andReverse", "copy", "andInverted", "noop", "xor", "or", "nor", "equiv", "invert", "orReverse", "copyInverted", "orInverted", "nand", "set" }));
        cPixelDrawing.setSelectedIndex(3);
        cPixelDrawing.setBorder(null);
        cPixelDrawing.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        cPixelDrawing.setEditor(null);
        cPixelDrawing.setPreferredSize(new java.awt.Dimension(150, 20));
        cPixelDrawing.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cPixelDrawingActionPerformed(evt);
            }
        });


        lRotationStyle.setBackground(new java.awt.Color(255, 255, 255));
        lRotationStyle.setText(" Rotation Style");
        lRotationStyle.setAlignmentX(0.5F);
        lRotationStyle.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lRotationStyle.setOpaque(true);
        lRotationStyle.setPreferredSize(new java.awt.Dimension(153, 20));

        cRotationStyle.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "unary", "multiple" }));
        cRotationStyle.setBorder(null);
        cRotationStyle.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        cRotationStyle.setEditor(null);
        cRotationStyle.setPreferredSize(new java.awt.Dimension(150, 20));
        cRotationStyle.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cRotationStyleActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout pStyleLayout = new javax.swing.GroupLayout(pStyle);
        pStyle.setLayout(pStyleLayout);
        pStyleLayout.setHorizontalGroup(
            pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pStyleLayout.createSequentialGroup()
                .addGap(18, 18, 18)
                .addGroup(pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lColor, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(pColor, javax.swing.GroupLayout.DEFAULT_SIZE, 157, Short.MAX_VALUE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lPixmap, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cPixmap, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lPixelDrawing, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cPixelDrawing, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lRotationStyle, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cRotationStyle, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))))
        );
        pStyleLayout.setVerticalGroup(
            pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pStyleLayout.createSequentialGroup()
                .addGroup(pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(pColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(6, 6, 6)
                .addGroup(pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addGap(1, 1, 1)
                        .addComponent(lPixmap, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(cPixmap, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(7, 7, 7)
                .addGroup(pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addGap(1, 1, 1)
                        .addComponent(lPixelDrawing, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(cPixelDrawing, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(7, 7, 7)
                .addGroup(pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addGap(1, 1, 1)
                        .addComponent(lRotationStyle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(cRotationStyle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );

        bStyle.setBorder(null);
        bStyle.setBorderPainted(false);
        bStyle.setContentAreaFilled(false);
        bStyle.setMaximumSize(new java.awt.Dimension(16, 16));
        bStyle.setMinimumSize(new java.awt.Dimension(16, 16));
        bStyle.setPreferredSize(new java.awt.Dimension(16, 16));
        bStyle.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bStyleActionPerformed(evt);
            }
        });

        lStyle.setFont(new java.awt.Font("Tahoma", 1, 11));
        lStyle.setText("Style/Appearance");

        sStyle.setPreferredSize(new java.awt.Dimension(50, 2));

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(bBaseProperties, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
            .addGroup(layout.createSequentialGroup()
                .addGap(21, 21, 21)
                .addComponent(lBaseProperties))
            .addGroup(layout.createSequentialGroup()
                .addGap(12, 12, 12)
                .addComponent(sBaseProperties, javax.swing.GroupLayout.PREFERRED_SIZE, 338, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(layout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addComponent(pBaseProperties, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(layout.createSequentialGroup()
                .addGap(20, 20, 20)
                .addComponent(lControl))
            .addGroup(layout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addComponent(sControl, javax.swing.GroupLayout.PREFERRED_SIZE, 338, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addComponent(bControl, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
            .addGroup(layout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addComponent(pControl, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(layout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addComponent(pData, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addComponent(bData, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
            .addGroup(layout.createSequentialGroup()
                .addGap(20, 20, 20)
                .addComponent(lData))
            .addGroup(layout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addComponent(sData, javax.swing.GroupLayout.PREFERRED_SIZE, 338, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(layout.createSequentialGroup()
                .addComponent(bStyle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(5, 5, 5)
                .addComponent(lStyle))
            .addGroup(layout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addComponent(pStyle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(layout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addComponent(sStyle, javax.swing.GroupLayout.PREFERRED_SIZE, 338, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(bBaseProperties, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(2, 2, 2)
                        .addComponent(lBaseProperties))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(15, 15, 15)
                        .addComponent(sBaseProperties, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(22, 22, 22)
                        .addComponent(pBaseProperties, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addGap(5, 5, 5)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(2, 2, 2)
                        .addComponent(lControl))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(15, 15, 15)
                        .addComponent(sControl, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(bControl, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(22, 22, 22)
                        .addComponent(pControl, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addGap(5, 5, 5)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(22, 22, 22)
                        .addComponent(pData, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(bData, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(2, 2, 2)
                        .addComponent(lData))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(15, 15, 15)
                        .addComponent(sData, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addGap(5, 5, 5)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(bStyle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(2, 2, 2)
                        .addComponent(lStyle))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(22, 22, 22)
                        .addComponent(pStyle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(15, 15, 15)
                        .addComponent(sStyle, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE))))
        );
        /**
        * It has all the components of the Curve JPanel.
        */
        bBaseProperties.setSelectedIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_plus.gif"));
        bBaseProperties.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_minus.gif"));
        bControl.setSelectedIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_plus.gif"));
        bControl.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_minus.gif"));
	bData.setSelectedIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_plus.gif"));
        bData.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_minus.gif"));
        bStyle.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_minus.gif"));
        bStyle.setSelectedIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_plus.gif"));
        bColor.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "color_fill.png"));
    }

   /**
   * Implement the action button to show/hide.
   * @param evt
   */
   private void bBasePropertiesActionPerformed(java.awt.event.ActionEvent evt){
        if (bBaseProperties.isSelected()) {
            pBaseProperties.setVisible(false);
        } else {
            pBaseProperties.setVisible(true);
        }
    }
   /**
   * Implement the action button to show/hide.
   * @param evt
   */
    private void bControlActionPerformed(java.awt.event.ActionEvent evt) {
        if (bControl.isSelected()) {
            pControl.setVisible(false);
        } else {
            pControl.setVisible(true);
        }
    }
   /**
   * Implement the action button to show/hide.
   * @param evt
   */
    private void bDataActionPerformed(java.awt.event.ActionEvent evt) {
        if (bData.isSelected()) {
            pData.setVisible(false);
        } else {
            pData.setVisible(true);
        }
    }
   /**
   * Implement the action button to show/hide.
   * @param evt
   */
    private void bStyleActionPerformed(java.awt.event.ActionEvent evt) {
        if (bStyle.isSelected()) {
            pStyle.setVisible(false);
        } else {
            pStyle.setVisible(true);
        }
    }
   /**
   * Implement the action button to show/hide.
   * @param evt
   */
    private void bColorActionPerformed(java.awt.event.ActionEvent evt) {
        colorDialog.setVisible(true);
    }

    /**
    * Implement the action on the OK button to save the color chosen by the user.
    * @param evt
    */
    private void okActionPerformed(java.awt.event.ActionEvent evt) {
        choice = chooser.getColor();
        cColor.setBackground(choice);
        colorDialog.dispose();
    }

    /**
    * Updates the property: Visible.
    * @param evt
    */
    private void cVisibleActionPerformed(java.awt.event.ActionEvent evt) {        
        boolean setVisible = true;
        if (cVisible.getSelectedIndex() == 0) {
            setVisible = false;
        }
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_VISIBLE__, setVisible);
    }
    /**
    * Updates the property: Auto Resize.
    * @param evt
    */
    private void cAutoResizeActionPerformed(java.awt.event.ActionEvent evt) {
        boolean setAutoResize = true;
        if (cAutoResize.getSelectedIndex() == 0) {
            setAutoResize = false;
        }
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_AUTORESIZE__, setAutoResize);
    }
    /**
    * Updates the property: xPosition.
    * @param evt
    */
    private void cxpositionActionPerformed(java.awt.event.ActionEvent evt) {
        int setXposition = Integer.parseInt(cxposition.getText());
        int setYposition = Integer.parseInt(cyposition.getText());
        Integer[] setPosition = new Integer[2];
        setPosition[0] = setXposition;
        setPosition[1] = setYposition;
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_POSITION__, setPosition);
    }
    /**
    * Updates the property: yPosition.
    * @param evt
    */
    private void cypositionActionPerformed(java.awt.event.ActionEvent evt) {
        cxpositionActionPerformed(evt);
    }
    /**
    * Updates the property: xSize.
    * @param evt
    */
    private void cxsizeActionPerformed(java.awt.event.ActionEvent evt) {
        int setXsize = Integer.parseInt(cxsize.getText());
        int setYsize = Integer.parseInt(cysize.getText());
        Integer[] setSize = new Integer[2];
        setSize[0] = setXsize;
        setSize[1] = setYsize;
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_SIZE__, setSize);
    }
    /**
    * Updates the property: ySize.
    * @param evt
    */
    private void cysizeActionPerformed(java.awt.event.ActionEvent evt) {
        cxsizeActionPerformed(evt);
    }
    /**
    * Updates the property: Figure ID.
    * @param evt
    */
    private void cFigureIDActionPerformed(java.awt.event.ActionEvent evt) {
        int setfigureID = Integer.parseInt(cFigureID.getText());
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_ID__, setfigureID);
    }
    /**
    * Updates the property: Figure Name.
    * @param evt
    */
    private void cFigureNameActionPerformed(java.awt.event.ActionEvent evt) {
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_NAME__, cFigureName.getText());
    }
    /**
    * Updates the property: Pixmap.
    * @param evt
    */
    private void cPixmapActionPerformed(java.awt.event.ActionEvent evt) {
        boolean setPixmap = true;
        if (cPixmap.getSelectedIndex() == 0) {
            setPixmap = false;
        }
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_PIXMAP__, setPixmap);
    }
    /**
    * Updates the property: Pixel Drawing Mode.
    * @param evt
    */
    private void cPixelDrawingActionPerformed(java.awt.event.ActionEvent evt) {
        int setPixelDrawing = cPixelDrawing.getSelectedIndex();
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_PIXEL_DRAWING_MODE__, setPixelDrawing);
    }
    /**
    * Updates the property: Rotation Style.
    * @param evt
    */
    private void cRotationStyleActionPerformed(java.awt.event.ActionEvent evt) {
        int setRotationStyle = cRotationStyle.getSelectedIndex();
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_ROTATION_TYPE__, setRotationStyle);
    }
}