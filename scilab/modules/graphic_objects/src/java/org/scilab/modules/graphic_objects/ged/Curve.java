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
* Properties of the curve.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Curve extends javax.swing.JPanel {
    private javax.swing.JButton bBackColor;
    public static javax.swing.JToggleButton bBaseProperties;
    private javax.swing.JButton bForeColor;
    public static javax.swing.JToggleButton bStyle;
    private javax.swing.JDialog backcolorDialog;
    private javax.swing.JLabel cBackColor;
    private javax.swing.JLabel cForeColor;
    private javax.swing.JComboBox cLine;
    private javax.swing.JComboBox cPolyline;
    private javax.swing.JComboBox cVisible;
    private javax.swing.JColorChooser chooserBack;
    private javax.swing.JColorChooser chooserFore;
    private javax.swing.JDialog forecolorDialog;
    private javax.swing.JLabel lBackColor;
    private javax.swing.JLabel lBaseProperties;
    private javax.swing.JLabel lForeColor;
    private javax.swing.JLabel lLine;
    private javax.swing.JLabel lPolyline;
    private javax.swing.JLabel lStyle;
    private javax.swing.JLabel lVisible;
    private javax.swing.JButton okBack;
    private javax.swing.JButton okFore;
    private javax.swing.JPanel pBackColor;
    public static javax.swing.JPanel pBaseProperties;
    private javax.swing.JPanel pForeColor;
    public static javax.swing.JPanel pStyle;
    private javax.swing.JSeparator sBaseProperties;
    private javax.swing.JSeparator sStyle;
    private Color choiceBack;
    private Color choiceFore;
    private String currentcurve = null;

    /**
    * Initializes the components of the JPanel.
    * 
    * @param objectID Enters the identification of Figure.
    */
    public Curve(String objectID) {
        guiCurveComponents();
        initProperties(objectID);
    }

    /**
    * Loads the current properties of the Figure.
    *
    * @param objectID Enters the identification of curve.
    */
    private void initProperties(String objectID) {
        if (objectID != null) {
            currentcurve = objectID;
            /** Get the current status of the property: Visible */
            boolean isVisible = (Boolean) GraphicController.getController()
                    .getProperty(currentcurve, GraphicObjectProperties.__GO_VISIBLE__);
            if (isVisible) {
                cVisible.setSelectedIndex(1);
            } else {
                cVisible.setSelectedIndex(0);
            }
            /** Get the current status of the property: Line Style */
            int currentLineStyle = (Integer) GraphicController.getController()
                    .getProperty(currentcurve, GraphicObjectProperties.__GO_LINE_STYLE__);
            cLine.setSelectedIndex(currentLineStyle);
            /** Get the current status of the property: Polyline Style */
            int currentPolylineStyle = (Integer) GraphicController.getController()
                    .getProperty(currentcurve, GraphicObjectProperties.__GO_POLYLINE_STYLE__);
            cPolyline.setSelectedIndex(currentPolylineStyle);
        }
    }

    /**
    * It has all the components of the Curve JPanel.
    */
    private void guiCurveComponents() {
        backcolorDialog = new javax.swing.JDialog();
        chooserBack = new javax.swing.JColorChooser();
        okBack = new javax.swing.JButton();
        forecolorDialog = new javax.swing.JDialog();
        chooserFore = new javax.swing.JColorChooser();
        okFore = new javax.swing.JButton();
        bBaseProperties = new javax.swing.JToggleButton();
        lBaseProperties = new javax.swing.JLabel();
        sBaseProperties = new javax.swing.JSeparator();
        pBaseProperties = new javax.swing.JPanel();
        lVisible = new javax.swing.JLabel();
        cVisible = new javax.swing.JComboBox();
        bStyle = new javax.swing.JToggleButton();
        lStyle = new javax.swing.JLabel();
        sStyle = new javax.swing.JSeparator();
        pStyle = new javax.swing.JPanel();
        lBackColor = new javax.swing.JLabel();
        pBackColor = new javax.swing.JPanel();
        bBackColor = new javax.swing.JButton();
        cBackColor = new javax.swing.JLabel();
        lForeColor = new javax.swing.JLabel();
        pForeColor = new javax.swing.JPanel();
        bForeColor = new javax.swing.JButton();
        cForeColor = new javax.swing.JLabel();
        lLine = new javax.swing.JLabel();
        cLine = new javax.swing.JComboBox();
        lPolyline = new javax.swing.JLabel();
        cPolyline = new javax.swing.JComboBox();

        backcolorDialog.setTitle("Choose");
        backcolorDialog.setMinimumSize(new java.awt.Dimension(670, 450));
        backcolorDialog.setModal(true);
        backcolorDialog.setResizable(false);

        okBack.setText("OK");
        okBack.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                okBackActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout backcolorDialogLayout = new javax.swing.GroupLayout(backcolorDialog.getContentPane());
        backcolorDialog.getContentPane().setLayout(backcolorDialogLayout);
        backcolorDialogLayout.setHorizontalGroup(
            backcolorDialogLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(backcolorDialogLayout.createSequentialGroup()
                .addComponent(chooserBack, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(0, 0, Short.MAX_VALUE))
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, backcolorDialogLayout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(okBack)
                .addContainerGap())
        );
        backcolorDialogLayout.setVerticalGroup(
            backcolorDialogLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(backcolorDialogLayout.createSequentialGroup()
                .addComponent(chooserBack, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(okBack)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        forecolorDialog.setTitle("Choose");
        forecolorDialog.setMinimumSize(new java.awt.Dimension(670, 450));
        forecolorDialog.setModal(true);
        forecolorDialog.setResizable(false);

        okFore.setText("OK");
        okFore.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                okForeActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout forecolorDialogLayout = new javax.swing.GroupLayout(forecolorDialog.getContentPane());
        forecolorDialog.getContentPane().setLayout(forecolorDialogLayout);
        forecolorDialogLayout.setHorizontalGroup(
            forecolorDialogLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(forecolorDialogLayout.createSequentialGroup()
                .addComponent(chooserFore, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(0, 0, Short.MAX_VALUE))
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, forecolorDialogLayout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(okFore)
                .addContainerGap())
        );
        forecolorDialogLayout.setVerticalGroup(
            forecolorDialogLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(forecolorDialogLayout.createSequentialGroup()
                .addComponent(chooserFore, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(okFore)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
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
            .addComponent(lVisible, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
            .addComponent(cVisible, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
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

        pStyle.setAlignmentX(0.0F);
        pStyle.setAlignmentY(0.0F);

        lBackColor.setBackground(new java.awt.Color(255, 255, 255));
        lBackColor.setText(" Background Color");
        lBackColor.setAlignmentX(0.5F);
        lBackColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lBackColor.setOpaque(true);
        lBackColor.setPreferredSize(new java.awt.Dimension(153, 20));

        pBackColor.setBackground(new java.awt.Color(255, 255, 255));
        pBackColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));

        bBackColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        bBackColor.setContentAreaFilled(false);
        bBackColor.setPreferredSize(new java.awt.Dimension(16, 16));
        bBackColor.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bBackColorActionPerformed(evt);
            }
        });

        cBackColor.setBackground(new java.awt.Color(239, 77, 31));
        cBackColor.setText(" ");
        cBackColor.setToolTipText("");
        cBackColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cBackColor.setOpaque(true);

        javax.swing.GroupLayout pBackColorLayout = new javax.swing.GroupLayout(pBackColor);
        pBackColor.setLayout(pBackColorLayout);
        pBackColorLayout.setHorizontalGroup(
            pBackColorLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pBackColorLayout.createSequentialGroup()
                .addGap(36, 36, 36)
                .addComponent(bBackColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(11, 11, 11)
                .addComponent(cBackColor, javax.swing.GroupLayout.PREFERRED_SIZE, 27, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        pBackColorLayout.setVerticalGroup(
            pBackColorLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pBackColorLayout.createSequentialGroup()
                .addGap(1, 1, 1)
                .addComponent(bBackColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pBackColorLayout.createSequentialGroup()
                .addGap(3, 3, 3)
                .addComponent(cBackColor, javax.swing.GroupLayout.PREFERRED_SIZE, 11, javax.swing.GroupLayout.PREFERRED_SIZE))
        );

        lForeColor.setBackground(new java.awt.Color(255, 255, 255));
        lForeColor.setText(" Foreground Color");
        lForeColor.setAlignmentX(0.5F);
        lForeColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lForeColor.setOpaque(true);
        lForeColor.setPreferredSize(new java.awt.Dimension(153, 20));

        pForeColor.setBackground(new java.awt.Color(255, 255, 255));
        pForeColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));

        bForeColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        bForeColor.setContentAreaFilled(false);
        bForeColor.setPreferredSize(new java.awt.Dimension(16, 16));
        bForeColor.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bForeColorActionPerformed(evt);
            }
        });

        cForeColor.setBackground(new java.awt.Color(239, 77, 31));
        cForeColor.setText(" ");
        cForeColor.setToolTipText("");
        cForeColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cForeColor.setOpaque(true);

        javax.swing.GroupLayout pForeColorLayout = new javax.swing.GroupLayout(pForeColor);
        pForeColor.setLayout(pForeColorLayout);
        pForeColorLayout.setHorizontalGroup(
            pForeColorLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pForeColorLayout.createSequentialGroup()
                .addGap(36, 36, 36)
                .addComponent(bForeColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(11, 11, 11)
                .addComponent(cForeColor, javax.swing.GroupLayout.PREFERRED_SIZE, 27, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        pForeColorLayout.setVerticalGroup(
            pForeColorLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pForeColorLayout.createSequentialGroup()
                .addGap(1, 1, 1)
                .addComponent(bForeColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pForeColorLayout.createSequentialGroup()
                .addGap(3, 3, 3)
                .addComponent(cForeColor, javax.swing.GroupLayout.PREFERRED_SIZE, 11, javax.swing.GroupLayout.PREFERRED_SIZE))
        );

        lLine.setBackground(new java.awt.Color(255, 255, 255));
        lLine.setText(" Line");
        lLine.setAlignmentX(0.5F);
        lLine.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lLine.setOpaque(true);
        lLine.setPreferredSize(new java.awt.Dimension(153, 20));

        cLine.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "solid", "dash", "dash dot", "longdash dot", "bigdash dot", "bigdash longdash", "dot", "double dot" }));
        cLine.setBorder(null);
        cLine.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        cLine.setEditor(null);
        cLine.setPreferredSize(new java.awt.Dimension(150, 20));
        cLine.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cLineActionPerformed(evt);
            }
        });

        lPolyline.setBackground(new java.awt.Color(255, 255, 255));
        lPolyline.setText(" Polyline Style");
        lPolyline.setAlignmentX(0.5F);
        lPolyline.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lPolyline.setOpaque(true);
        lPolyline.setPreferredSize(new java.awt.Dimension(153, 20));

        cPolyline.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "interpolated", "staircase", "barplot", "bar", "arrowed", "filled" }));
        cPolyline.setBorder(null);
        cPolyline.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        cPolyline.setEditor(null);
        cPolyline.setPreferredSize(new java.awt.Dimension(150, 20));
        cPolyline.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cPolylineActionPerformed(evt);
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
                        .addComponent(lBackColor, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(pBackColor, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lForeColor, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(pForeColor, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lLine, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cLine, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lPolyline, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cPolyline, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))))
        );
        pStyleLayout.setVerticalGroup(
            pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pStyleLayout.createSequentialGroup()
                .addGroup(pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lBackColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(pBackColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lForeColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(pForeColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lLine, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cLine, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lPolyline, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cPolyline, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addComponent(pBaseProperties, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(layout.createSequentialGroup()
                .addGap(21, 21, 21)
                .addComponent(lBaseProperties))
            .addGroup(layout.createSequentialGroup()
                .addGap(12, 12, 12)
                .addComponent(sBaseProperties, javax.swing.GroupLayout.PREFERRED_SIZE, 338, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addComponent(bBaseProperties, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
            .addGroup(layout.createSequentialGroup()
                .addGap(21, 21, 21)
                .addComponent(lStyle))
            .addComponent(bStyle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
            .addGroup(layout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addComponent(sStyle, javax.swing.GroupLayout.PREFERRED_SIZE, 338, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(layout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addComponent(pStyle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(22, 22, 22)
                        .addComponent(pBaseProperties, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(2, 2, 2)
                        .addComponent(lBaseProperties))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(15, 15, 15)
                        .addComponent(sBaseProperties, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(bBaseProperties, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(8, 8, 8)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lStyle)
                    .addComponent(bStyle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(13, 13, 13)
                        .addComponent(sStyle, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(20, 20, 20)
                        .addComponent(pStyle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))))
        );
        /**
        * Insert the icons on buttons.
        */
        bBaseProperties.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_minus.gif"));
        bBaseProperties.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_plus.gif"));
        bStyle.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_minus.gif"));
        bStyle.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_plus.gif"));
        bBackColor.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "color_fill.png"));
        bForeColor.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "color_fill.png"));
    }

    /**
    * Implement the action button to show/hide.
    */
    private void bBasePropertiesActionPerformed(java.awt.event.ActionEvent evt) {
        if (bBaseProperties.isSelected() == true) {
            pBaseProperties.setVisible(false);
        } else {
            pBaseProperties.setVisible(true);
        }
    }
    /**
    * Implement the action button to show/hide.
    */
    private void bStyleActionPerformed(java.awt.event.ActionEvent evt) {
        if (bStyle.isSelected() == true) {
            pStyle.setVisible(false);
        } else {
            pStyle.setVisible(true);
        }
    }

    /**
    * Implement the action button to open a JDialog with a JColorChoose.
    */
    private void bBackColorActionPerformed(java.awt.event.ActionEvent evt) {
        backcolorDialog.setVisible(true);
    }
    /**
    * Implement the action button to open a JDialog with a JColorChoose.
    */
    private void bForeColorActionPerformed(java.awt.event.ActionEvent evt) {
        forecolorDialog.setVisible(true);
    }

    /**
    * Implement the action on the OK button to save the color chosen by the user.
    */
    private void okBackActionPerformed(java.awt.event.ActionEvent evt) {
        choiceBack = chooserBack.getColor();
        cBackColor.setBackground(choiceBack);
        backcolorDialog.dispose();
    }
    /**
    * Implement the action on the OK button to save the color chosen by the user.
    */
    private void okForeActionPerformed(java.awt.event.ActionEvent evt) {
        choiceFore = chooserFore.getColor();
        cForeColor.setBackground(choiceFore);
        forecolorDialog.dispose();
    }

    private void cVisibleActionPerformed(java.awt.event.ActionEvent evt) {
        boolean setVisible = true;
        if (cVisible.getSelectedIndex() == 0) {
            setVisible = false;
        }
        GraphicController.getController()
                .setProperty(currentcurve, GraphicObjectProperties.__GO_VISIBLE__, setVisible);
    }

    private void cLineActionPerformed(java.awt.event.ActionEvent evt) {
        int setLine = cLine.getSelectedIndex();
        GraphicController.getController().setProperty(currentcurve, GraphicObjectProperties.__GO_LINE_STYLE__, setLine);

    }

    private void cPolylineActionPerformed(java.awt.event.ActionEvent evt) {
        int setPolyline = cPolyline.getSelectedIndex();
        GraphicController.getController()
                .setProperty(currentcurve, GraphicObjectProperties.__GO_POLYLINE_STYLE__, setPolyline);
    }
}