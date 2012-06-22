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

import java.awt.Color;

/**
* Properties of the axes.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Axes extends javax.swing.JPanel {
    public static javax.swing.JToggleButton bAxesBounds;
    public static javax.swing.JToggleButton bMargins;
    public static javax.swing.JToggleButton bAxisRulers;
    public static javax.swing.JToggleButton bAxisTitle;
    public static javax.swing.JButton bBackColor;
    public static javax.swing.JButton bForeColor;
    public static javax.swing.JButton bFontColor;
    public static javax.swing.JToggleButton bLabel;
    public static javax.swing.JToggleButton bLocation;
    public static javax.swing.JToggleButton bPosition;
    public static javax.swing.JToggleButton bReverse;
    public static javax.swing.JToggleButton bStyle;
    private javax.swing.JLabel cAxesBounds;
    private javax.swing.JLabel cMargins;
    private javax.swing.JLabel cAxisTitle;
    private javax.swing.JTextField cBoundsHeight;
    private javax.swing.JTextField cMarginsBottom;
    private javax.swing.JTextField cBoundsLeft;
    private javax.swing.JTextField cMarginsLeft;
    private javax.swing.JTextField cBoundsUp;
    private javax.swing.JTextField cMarginsRight;
    private javax.swing.JTextField cBoundsWidth;
    private javax.swing.JTextField cMarginsTop;
    private javax.swing.JLabel cBackColor;
    private javax.swing.JLabel cForeColor;
    private javax.swing.JLabel cFontColor;
    private javax.swing.JComboBox cFontStyle;
    private javax.swing.JLabel cLocation;
    private javax.swing.JComboBox cLocationX;
    private javax.swing.JComboBox cLocationY;
    private javax.swing.JLabel cReverse;
    private javax.swing.JComboBox cReverseX;
    private javax.swing.JComboBox cReverseY;
    private javax.swing.JComboBox cReverseZ;
    private javax.swing.JTextField cTitlePage;
    private javax.swing.JTextField cTitleX;
    private javax.swing.JTextField cTitleY;
    private javax.swing.JTextField cTitleZ;
    private javax.swing.JComboBox cVisible;
    private javax.swing.JColorChooser chooser;
    private javax.swing.JColorChooser chooser1;
    private javax.swing.JColorChooser chooser2;
    private javax.swing.JDialog backColorDialog;
    private javax.swing.JDialog foreColorDialog;
    private javax.swing.JDialog fontColorDialog;
    private javax.swing.JLabel lAxesBounds;
    private javax.swing.JLabel lMargins;
    private javax.swing.JLabel lAxisRulers;
    private javax.swing.JLabel lAxisTitle;
    private javax.swing.JLabel lBackColor;
    private javax.swing.JLabel lBoundsHeight;
    private javax.swing.JLabel lMarginsBottom;
    private javax.swing.JLabel lBoundsLeft;
    private javax.swing.JLabel lMarginsLeft;
    private javax.swing.JLabel lBoundsUp;
    private javax.swing.JLabel lMarginsRight;
    private javax.swing.JLabel lBoundsWidth;
    private javax.swing.JLabel lMarginsTop;
    private javax.swing.JLabel lFontColor;
    private javax.swing.JLabel lFontStyle;
    private javax.swing.JLabel lForeColor;
    private javax.swing.JLabel lLabel;
    private javax.swing.JLabel lLocation;
    private javax.swing.JLabel lLocationX;
    private javax.swing.JLabel lLocationY;
    private javax.swing.JLabel lPosition;
    private javax.swing.JLabel lReverse;
    private javax.swing.JLabel lReverseX;
    private javax.swing.JLabel lReverseY;
    private javax.swing.JLabel lReverseZ;
    private javax.swing.JLabel lStyle;
    private javax.swing.JLabel lTitlePage;
    private javax.swing.JLabel lTitleX;
    private javax.swing.JLabel lTitleY;
    private javax.swing.JLabel lTitleZ;
    private javax.swing.JLabel lVisible;
    private javax.swing.JButton okBack;
    private javax.swing.JButton okFore;
    private javax.swing.JButton okFont;
    public static javax.swing.JPanel pAxesBounds;
    public static javax.swing.JPanel pMargins;
    public static javax.swing.JPanel pAxisRulers;
    public static javax.swing.JPanel pAxisTitle;
    public static javax.swing.JPanel pBackColor;
    public static javax.swing.JPanel pFontColor;
    public static javax.swing.JPanel pForeColor;
    public static javax.swing.JPanel pLabel;
    public static javax.swing.JPanel pLocation;
    public static javax.swing.JPanel pPosition;
    public static javax.swing.JPanel pReverse;
    public static javax.swing.JPanel pStyle;
    private javax.swing.JSeparator sAxisRulers;
    private javax.swing.JSeparator sLabel;
    private javax.swing.JSeparator sPosition;
    private javax.swing.JSeparator sStyle;
    private Color choice;
    private Color choice1;
    private Color choice2;

    /**
    * Initializes the components of the JPanel.
    */
    public Axes() {
        guiAxesComponents();
    }

    /**
    * It has all the components of the Axes JPanel.
    */
    private void guiAxesComponents() {
        backColorDialog = new javax.swing.JDialog();
        chooser = new javax.swing.JColorChooser();
        okBack = new javax.swing.JButton();
        foreColorDialog = new javax.swing.JDialog();
        chooser1 = new javax.swing.JColorChooser();
        okFore = new javax.swing.JButton();
        fontColorDialog = new javax.swing.JDialog();
        chooser2 = new javax.swing.JColorChooser();
        okFont = new javax.swing.JButton();
        bAxisRulers = new javax.swing.JToggleButton();
        lAxisRulers = new javax.swing.JLabel();
        sAxisRulers = new javax.swing.JSeparator();
        pAxisRulers = new javax.swing.JPanel();
        lLocation = new javax.swing.JLabel();
        cLocation = new javax.swing.JLabel();
        bLocation = new javax.swing.JToggleButton();
        pLocation = new javax.swing.JPanel();
        lLocationX = new javax.swing.JLabel();
        cLocationX = new javax.swing.JComboBox();
        lLocationY = new javax.swing.JLabel();
        cLocationY = new javax.swing.JComboBox();
        lReverse = new javax.swing.JLabel();
        cReverse = new javax.swing.JLabel();
        bReverse = new javax.swing.JToggleButton();
        pReverse = new javax.swing.JPanel();
        lReverseX = new javax.swing.JLabel();
        cReverseX = new javax.swing.JComboBox();
        lReverseY = new javax.swing.JLabel();
        cReverseY = new javax.swing.JComboBox();
        lReverseZ = new javax.swing.JLabel();
        cReverseZ = new javax.swing.JComboBox();
        bLabel = new javax.swing.JToggleButton();
        lLabel = new javax.swing.JLabel();
        sLabel = new javax.swing.JSeparator();
        pLabel = new javax.swing.JPanel();
        lTitlePage = new javax.swing.JLabel();
        cTitlePage = new javax.swing.JTextField();
        bAxisTitle = new javax.swing.JToggleButton();
        lAxisTitle = new javax.swing.JLabel();
        cAxisTitle = new javax.swing.JLabel();
        pAxisTitle = new javax.swing.JPanel();
        lTitleX = new javax.swing.JLabel();
        cTitleX = new javax.swing.JTextField();
        lTitleY = new javax.swing.JLabel();
        cTitleY = new javax.swing.JTextField();
        lTitleZ = new javax.swing.JLabel();
        cTitleZ = new javax.swing.JTextField();
        bPosition = new javax.swing.JToggleButton();
        lPosition = new javax.swing.JLabel();
        sPosition = new javax.swing.JSeparator();
        pPosition = new javax.swing.JPanel();
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
        lFontColor = new javax.swing.JLabel();
        pFontColor = new javax.swing.JPanel();
        bFontColor = new javax.swing.JButton();
        cFontColor = new javax.swing.JLabel();
        lFontStyle = new javax.swing.JLabel();
        cFontStyle = new javax.swing.JComboBox();
        lVisible = new javax.swing.JLabel();
        cVisible = new javax.swing.JComboBox();

        backColorDialog.setTitle("Choose Background Color");
        backColorDialog.setMinimumSize(new java.awt.Dimension(670, 450));
        backColorDialog.setModal(true);
        backColorDialog.setResizable(false);

        okBack.setText("OK");
        okBack.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                okBackActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout backColorDialogLayout = new javax.swing.GroupLayout(backColorDialog.getContentPane());
        backColorDialog.getContentPane().setLayout(backColorDialogLayout);
        backColorDialogLayout.setHorizontalGroup(
            backColorDialogLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(backColorDialogLayout.createSequentialGroup()
                .addComponent(chooser, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(0, 10, Short.MAX_VALUE))
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, backColorDialogLayout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(okBack)
                .addContainerGap())
        );
        backColorDialogLayout.setVerticalGroup(
            backColorDialogLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(backColorDialogLayout.createSequentialGroup()
                .addComponent(chooser, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(okBack)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        foreColorDialog.setTitle("Choose Foreground Color");
        foreColorDialog.setMinimumSize(new java.awt.Dimension(670, 450));
        foreColorDialog.setModal(true);
        foreColorDialog.setResizable(false);

        okFore.setText("OK");
        okFore.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                okForeActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout foreColorDialogLayout = new javax.swing.GroupLayout(foreColorDialog.getContentPane());
        foreColorDialog.getContentPane().setLayout(foreColorDialogLayout);
        foreColorDialogLayout.setHorizontalGroup(
            foreColorDialogLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(foreColorDialogLayout.createSequentialGroup()
                .addComponent(chooser1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(0, 10, Short.MAX_VALUE))
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, foreColorDialogLayout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(okFore)
                .addContainerGap())
        );
        foreColorDialogLayout.setVerticalGroup(
            foreColorDialogLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(foreColorDialogLayout.createSequentialGroup()
                .addComponent(chooser1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(okFore)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        fontColorDialog.setTitle("Choose Font Color");
        fontColorDialog.setMinimumSize(new java.awt.Dimension(670, 450));
        fontColorDialog.setModal(true);
        fontColorDialog.setResizable(false);

        okFont.setText("OK");
        okFont.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                okFontActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout fontColorDialogLayout = new javax.swing.GroupLayout(fontColorDialog.getContentPane());
        fontColorDialog.getContentPane().setLayout(fontColorDialogLayout);
        fontColorDialogLayout.setHorizontalGroup(
            fontColorDialogLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(fontColorDialogLayout.createSequentialGroup()
                .addComponent(chooser2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(0, 10, Short.MAX_VALUE))
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, fontColorDialogLayout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(okFont)
                .addContainerGap())
        );
        fontColorDialogLayout.setVerticalGroup(
            fontColorDialogLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(fontColorDialogLayout.createSequentialGroup()
                .addComponent(chooser2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(okFont)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

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

        lAxisRulers.setFont(new java.awt.Font("Tahoma", 1, 11));
        lAxisRulers.setText("Axis Rulers");

        sAxisRulers.setPreferredSize(new java.awt.Dimension(50, 2));

        lLocation.setBackground(new java.awt.Color(255, 255, 255));
        lLocation.setText(" Location");
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

        cLocationX.setModel(new javax.swing.DefaultComboBoxModel(new String[] {"bottom", "middle", "origin", "top"}));
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

        cLocationY.setModel(new javax.swing.DefaultComboBoxModel(new String[] {"left", "middle", "origin", "right"}));
        cLocationY.setPreferredSize(new java.awt.Dimension(84, 20));
        cLocationY.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cLocationYActionPerformed(evt);
            }
        });

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

        titleLocation();

        lReverse.setBackground(new java.awt.Color(255, 255, 255));
        lReverse.setText(" Reverse");
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

        cReverseX.setModel(new javax.swing.DefaultComboBoxModel(new String[] { 
            "on", "off" 
        }));
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

        cReverseY.setModel(new javax.swing.DefaultComboBoxModel(new String[] { 
            "on", "off" 
        }));
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

        cReverseZ.setModel(new javax.swing.DefaultComboBoxModel(new String[] { 
            "on", "off" 
        }));
        cReverseZ.setPreferredSize(new java.awt.Dimension(58, 20));
        cReverseZ.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cReverseZActionPerformed(evt);
            }
        });

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

        titleReverse();

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

        bLabel.setBorder(null);
        bLabel.setBorderPainted(false);
        bLabel.setContentAreaFilled(false);
        bLabel.setMaximumSize(new java.awt.Dimension(16, 16));
        bLabel.setMinimumSize(new java.awt.Dimension(16, 16));
        bLabel.setPreferredSize(new java.awt.Dimension(16, 16));
        bLabel.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bLabelActionPerformed(evt);
            }
        });

        lLabel.setFont(new java.awt.Font("Tahoma", 1, 11));
        lLabel.setText("Label");

        sLabel.setPreferredSize(new java.awt.Dimension(50, 2));

        lTitlePage.setBackground(new java.awt.Color(255, 255, 255));
        lTitlePage.setText(" Title Page");
        lTitlePage.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lTitlePage.setOpaque(true);
        lTitlePage.setPreferredSize(new java.awt.Dimension(120, 16));

        cTitlePage.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cTitlePage.setPreferredSize(new java.awt.Dimension(55, 20));
        cTitlePage.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cTitlePageActionPerformed(evt);
            }
        });

        bAxisTitle.setSelected(true);
        bAxisTitle.setBorder(null);
        bAxisTitle.setBorderPainted(false);
        bAxisTitle.setContentAreaFilled(false);
        bAxisTitle.setMaximumSize(new java.awt.Dimension(16, 16));
        bAxisTitle.setMinimumSize(new java.awt.Dimension(16, 16));
        bAxisTitle.setPreferredSize(new java.awt.Dimension(16, 16));
        bAxisTitle.setRolloverEnabled(false);
        bAxisTitle.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bAxisTitleActionPerformed(evt);
            }
        });

        lAxisTitle.setBackground(new java.awt.Color(255, 255, 255));
        lAxisTitle.setText(" Axis Title");
        lAxisTitle.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lAxisTitle.setOpaque(true);
        lAxisTitle.setPreferredSize(new java.awt.Dimension(120, 16));

        cAxisTitle.setBackground(new java.awt.Color(255, 255, 255));
        cAxisTitle.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cAxisTitle.setOpaque(true);

        pAxisTitle.setVisible(false);

        lTitleX.setBackground(new java.awt.Color(255, 255, 255));
        lTitleX.setText(" X");
        lTitleX.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lTitleX.setOpaque(true);

        cTitleX.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cTitleX.setPreferredSize(new java.awt.Dimension(55, 20));
        cTitleX.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cTitleXActionPerformed(evt);
            }
        });

        lTitleY.setBackground(new java.awt.Color(255, 255, 255));
        lTitleY.setText(" Y");
        lTitleY.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lTitleY.setOpaque(true);

        cTitleY.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cTitleY.setPreferredSize(new java.awt.Dimension(55, 20));
        cTitleY.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cTitleYActionPerformed(evt);
            }
        });

        lTitleZ.setBackground(new java.awt.Color(255, 255, 255));
        lTitleZ.setText(" Z");
        lTitleZ.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lTitleZ.setOpaque(true);

        cTitleZ.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cTitleZ.setPreferredSize(new java.awt.Dimension(55, 20));
        cTitleZ.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cTitleZActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout pAxisTitleLayout = new javax.swing.GroupLayout(pAxisTitle);
        pAxisTitle.setLayout(pAxisTitleLayout);
        pAxisTitleLayout.setHorizontalGroup(
            pAxisTitleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pAxisTitleLayout.createSequentialGroup()
                .addComponent(lTitleX, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cTitleX, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pAxisTitleLayout.createSequentialGroup()
                .addComponent(lTitleY, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cTitleY, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pAxisTitleLayout.createSequentialGroup()
                .addComponent(lTitleZ, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cTitleZ, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        pAxisTitleLayout.setVerticalGroup(
            pAxisTitleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pAxisTitleLayout.createSequentialGroup()
                .addGroup(pAxisTitleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lTitleX, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cTitleX, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pAxisTitleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lTitleY, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cTitleY, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pAxisTitleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lTitleZ, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cTitleZ, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );

        javax.swing.GroupLayout pLabelLayout = new javax.swing.GroupLayout(pLabel);
        pLabel.setLayout(pLabelLayout);
        pLabelLayout.setHorizontalGroup(
            pLabelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pLabelLayout.createSequentialGroup()
                .addComponent(bAxisTitle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(2, 2, 2)
                .addComponent(lAxisTitle, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(4, 4, 4)
                .addComponent(cAxisTitle, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pLabelLayout.createSequentialGroup()
                .addGap(18, 18, 18)
                .addGroup(pLabelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(pLabelLayout.createSequentialGroup()
                        .addComponent(lTitlePage, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cTitlePage, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(pAxisTitle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );
        pLabelLayout.setVerticalGroup(
            pLabelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pLabelLayout.createSequentialGroup()
                .addGroup(pLabelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lTitlePage, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cTitlePage, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(6, 6, 6)
                .addGroup(pLabelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(pLabelLayout.createSequentialGroup()
                        .addGap(3, 3, 3)
                        .addComponent(bAxisTitle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(lAxisTitle, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cAxisTitle, javax.swing.GroupLayout.PREFERRED_SIZE, 20, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(6, 6, 6)
                .addComponent(pAxisTitle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
        );

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
        lPosition.setText("Position");

        sPosition.setPreferredSize(new java.awt.Dimension(50, 2));

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
        lMargins.setText(" Margins");
        lMargins.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lMargins.setOpaque(true);
        lMargins.setPreferredSize(new java.awt.Dimension(120, 16));

        cMargins.setBackground(new java.awt.Color(255, 255, 255));
        cMargins.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cMargins.setOpaque(true);

        lMarginsLeft.setBackground(new java.awt.Color(255, 255, 255));
        lMarginsLeft.setText(" Left");
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
        lMarginsRight.setText(" Right");
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
        lMarginsTop.setText(" Top");
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
        lMarginsBottom.setText(" Bottom");
        lMarginsBottom.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lMarginsBottom.setOpaque(true);

        cMarginsBottom.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cMarginsBottom.setPreferredSize(new java.awt.Dimension(55, 20));
        cMarginsBottom.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cMarginsBottomActionPerformed(evt);
            }
        });

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
        lAxesBounds.setText(" Axes Bounds");
        lAxesBounds.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lAxesBounds.setOpaque(true);
        lAxesBounds.setPreferredSize(new java.awt.Dimension(120, 16));

        cAxesBounds.setBackground(new java.awt.Color(255, 255, 255));
        cAxesBounds.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cAxesBounds.setOpaque(true);

        lBoundsLeft.setBackground(new java.awt.Color(255, 255, 255));
        lBoundsLeft.setText(" Left");
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
        lBoundsUp.setText(" Up");
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
        lBoundsWidth.setText(" Width");
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
        lBoundsHeight.setText(" Height");
        lBoundsHeight.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lBoundsHeight.setOpaque(true);

        cBoundsHeight.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cBoundsHeight.setPreferredSize(new java.awt.Dimension(55, 20));
        cBoundsHeight.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cBoundsHeightActionPerformed(evt);
            }
        });

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

        lBoundsHeight.getAccessibleContext().setAccessibleName("");
        cBoundsHeight.getAccessibleContext().setAccessibleName("");

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
                .addGap(35, 35, 35)
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
                .addGap(35, 35, 35)
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

        lFontColor.setBackground(new java.awt.Color(255, 255, 255));
        lFontColor.setText(" Font Color");
        lFontColor.setAlignmentX(0.5F);
        lFontColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lFontColor.setOpaque(true);
        lFontColor.setPreferredSize(new java.awt.Dimension(153, 20));

        pFontColor.setBackground(new java.awt.Color(255, 255, 255));
        pFontColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        pFontColor.setPreferredSize(new java.awt.Dimension(157, 19));

        bFontColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        bFontColor.setContentAreaFilled(false);
        bFontColor.setPreferredSize(new java.awt.Dimension(16, 16));
        bFontColor.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bFontColorActionPerformed(evt);
            }
        });

        cFontColor.setBackground(new java.awt.Color(239, 77, 31));
        cFontColor.setText(" ");
        cFontColor.setToolTipText("");
        cFontColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cFontColor.setOpaque(true);

        javax.swing.GroupLayout pFontColorLayout = new javax.swing.GroupLayout(pFontColor);
        pFontColor.setLayout(pFontColorLayout);
        pFontColorLayout.setHorizontalGroup(
            pFontColorLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pFontColorLayout.createSequentialGroup()
                .addGap(35, 35, 35)
                .addComponent(bFontColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(11, 11, 11)
                .addComponent(cFontColor, javax.swing.GroupLayout.PREFERRED_SIZE, 27, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        pFontColorLayout.setVerticalGroup(
            pFontColorLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pFontColorLayout.createSequentialGroup()
                .addGap(1, 1, 1)
                .addComponent(bFontColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(pFontColorLayout.createSequentialGroup()
                .addGap(3, 3, 3)
                .addComponent(cFontColor, javax.swing.GroupLayout.PREFERRED_SIZE, 11, javax.swing.GroupLayout.PREFERRED_SIZE))
        );

        lFontStyle.setBackground(new java.awt.Color(255, 255, 255));
        lFontStyle.setText(" Font Style");
        lFontStyle.setAlignmentX(0.5F);
        lFontStyle.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lFontStyle.setOpaque(true);
        lFontStyle.setPreferredSize(new java.awt.Dimension(153, 20));

        cFontStyle.setModel(new javax.swing.DefaultComboBoxModel(new String[] { 
            "Monospaced", "Symbol", "Serif", "Serif Italic", "Serif Bold", 
            "Serif Bold Italic", "SansSerif", "SansSerif Italic", 
            "SansSerif Bold", "SansSerif Bold Italic" 
        }));
        cFontStyle.setBorder(null);
        cFontStyle.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        cFontStyle.setEditor(null);
        cFontStyle.setPreferredSize(new java.awt.Dimension(150, 20));

        lVisible.setBackground(new java.awt.Color(255, 255, 255));
        lVisible.setText(" Visible");
        lVisible.setAlignmentX(0.5F);
        lVisible.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lVisible.setOpaque(true);
        lVisible.setPreferredSize(new java.awt.Dimension(153, 20));

        cVisible.setModel(new javax.swing.DefaultComboBoxModel(new String[] { 
            "on", "off" 
        }));
        cVisible.setBorder(null);
        cVisible.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        cVisible.setEditor(null);
        cVisible.setPreferredSize(new java.awt.Dimension(150, 20));

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
                        .addComponent(lFontColor, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(pFontColor, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lFontStyle, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cFontStyle, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(pStyleLayout.createSequentialGroup()
                        .addComponent(lVisible, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cVisible, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))))
        );
        pStyleLayout.setVerticalGroup(
            pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pStyleLayout.createSequentialGroup()
                .addGroup(pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lBackColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(pBackColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(6, 6, 6)
                .addGroup(pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lForeColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(pForeColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(6, 6, 6)
                .addGroup(pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lFontColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(pFontColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(4, 4, 4)
                .addGroup(pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lFontStyle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cFontStyle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(6, 6, 6)
                .addGroup(pStyleLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lVisible, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cVisible, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addComponent(pAxisRulers, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addComponent(bAxisRulers, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
            .addGroup(layout.createSequentialGroup()
                .addGap(21, 21, 21)
                .addComponent(lAxisRulers))
            .addGroup(layout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addComponent(sAxisRulers, javax.swing.GroupLayout.PREFERRED_SIZE, 338, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addComponent(bLabel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
            .addGroup(layout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addComponent(pLabel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(layout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addComponent(sLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 338, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(layout.createSequentialGroup()
                .addGap(21, 21, 21)
                .addComponent(lLabel))
            .addGroup(layout.createSequentialGroup()
                .addGap(21, 21, 21)
                .addComponent(lPosition))
            .addComponent(bPosition, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
            .addGroup(layout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addComponent(sPosition, javax.swing.GroupLayout.PREFERRED_SIZE, 338, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(layout.createSequentialGroup()
                .addGap(10, 10, 10)
                .addComponent(pPosition, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(layout.createSequentialGroup()
                .addComponent(bStyle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(5, 5, 5)
                .addComponent(lStyle))
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
                        .addComponent(pAxisRulers, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(bAxisRulers, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(2, 2, 2)
                        .addComponent(lAxisRulers))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(15, 15, 15)
                        .addComponent(sAxisRulers, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addGap(8, 8, 8)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(bLabel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(20, 20, 20)
                        .addComponent(pLabel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(13, 13, 13)
                        .addComponent(sLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(lLabel))
                .addGap(8, 8, 8)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lPosition)
                    .addComponent(bPosition, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(13, 13, 13)
                        .addComponent(sPosition, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(20, 20, 20)
                        .addComponent(pPosition, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addGap(9, 9, 9)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(bStyle, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lStyle)
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
        bAxisRulers.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_minus.gif"));
        bAxisRulers.setSelectedIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_plus.gif"));
        bLocation.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_minus.gif"));
        bLocation.setSelectedIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_plus.gif"));
        bReverse.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_minus.gif"));
        bReverse.setSelectedIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_plus.gif"));
        bLabel.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_minus.gif"));
        bLabel.setSelectedIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_plus.gif"));
        bAxisTitle.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_minus.gif"));
        bAxisTitle.setSelectedIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_plus.gif"));
        bPosition.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_minus.gif"));
        bPosition.setSelectedIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_plus.gif"));
        bMargins.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_minus.gif"));
        bMargins.setSelectedIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_plus.gif"));
        bAxesBounds.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_minus.gif"));
        bAxesBounds.setSelectedIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_plus.gif"));
        bStyle.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_minus.gif"));
        bStyle.setSelectedIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "icon_plus.gif"));
        bBackColor.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "color_fill.png"));
        bForeColor.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "color_fill.png"));
        bFontColor.setIcon(new javax.swing.ImageIcon(Inspector.IMAGEPATH + "color_fill.png"));
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
    * Updates the status of the corresponding label.
    */
    private void cLocationXActionPerformed(java.awt.event.ActionEvent evt) {
        titleLocation();
    }
    private void cLocationYActionPerformed(java.awt.event.ActionEvent evt) {
        titleLocation();
    }

    /**
    * Inserts the current state of functionality: REVERSE in your main label.
    */
    public void titleReverse(){
        String titleReverseX = cReverseX.getSelectedItem().toString();
        String titleReverseY = cReverseY.getSelectedItem().toString();
        String titleReverseZ = cReverseZ.getSelectedItem().toString();
        String titleReverse = titleReverseX + " , " + titleReverseY + " , " + titleReverseZ;
        cReverse.setText(" [" + titleReverse + "]");
    }
    /**
    * Updates the status of the corresponding label.
    */
    private void cReverseXActionPerformed(java.awt.event.ActionEvent evt) {
        titleReverse();
    }
    private void cReverseYActionPerformed(java.awt.event.ActionEvent evt) {
        titleReverse();
    }
    private void cReverseZActionPerformed(java.awt.event.ActionEvent evt) {
        titleReverse();
    }

    /**
    * Inserts the current state of functionality: AXIS TITLE in your main label.
    */
    public void titleAxis(){
        String titleAxisX = cTitleX.getText();
        String titleAxisY = cTitleY.getText();
        String titleAxisZ = cTitleZ.getText();
        String titleAxis = titleAxisX + " , " + titleAxisY + " , " + titleAxisZ;
        cAxisTitle.setText(" [" + titleAxis + "]");
    }
    /**
    * Updates the status of the corresponding label.
    */
    private void cTitleXActionPerformed(java.awt.event.ActionEvent evt) {
        titleAxis();
    }
    private void cTitleYActionPerformed(java.awt.event.ActionEvent evt) {
        titleAxis();
    }
    private void cTitleZActionPerformed(java.awt.event.ActionEvent evt) {
        titleAxis();
    }

    /**
    * Inserts the current state of functionality: AXES BOUNDS in your main label.
    */
    public void titleBounds() {
        String titleBoundsLeft = cBoundsLeft.getText();
        String titleBoundsUp = cBoundsUp.getText();
        String titleBoundsWidth = cBoundsWidth.getText();
        String titleBoundsHeight = cBoundsHeight.getText();
        String titleBounds = titleBoundsLeft + " , " + titleBoundsUp + " , " + titleBoundsWidth+ " , " + titleBoundsHeight;
        cAxesBounds.setText(" [" + titleBounds + "]");
    }
    /**
    * Updates the status of the corresponding label.
    */
    private void cBoundsLeftActionPerformed(java.awt.event.ActionEvent evt) {
        titleBounds();
    }
    private void cBoundsUpActionPerformed(java.awt.event.ActionEvent evt) {
        titleBounds();
    }
    private void cBoundsWidthActionPerformed(java.awt.event.ActionEvent evt) {
        titleBounds();
    }
    private void cBoundsHeightActionPerformed(java.awt.event.ActionEvent evt) {
        titleBounds();
    }

    /**
    * Implement the action button to show/hide.
    */
    private void bAxisRulersActionPerformed(java.awt.event.ActionEvent evt) {
        if (bAxisRulers.isSelected() == true) {
            pAxisRulers.setVisible(false);
        } else {
            pAxisRulers.setVisible(true);
        }
    }
    private void bLabelActionPerformed(java.awt.event.ActionEvent evt) {
        if (bLabel.isSelected() == true) {
            pLabel.setVisible(false);
        } else {
            pLabel.setVisible(true);
        }
    }
    private void bStyleActionPerformed(java.awt.event.ActionEvent evt) {
        if (bStyle.isSelected() == true) {
            pStyle.setVisible(false);
        } else {
            pStyle.setVisible(true);
        }
    }
    private void bPositionActionPerformed(java.awt.event.ActionEvent evt) {
        if (bPosition.isSelected() == true) {
            pPosition.setVisible(false);
        } else {
            pPosition.setVisible(true);
        }
    }
    private void bAxesBoundsActionPerformed(java.awt.event.ActionEvent evt) {
        if (bAxesBounds.isSelected() == true) {
            pAxesBounds.setVisible(false);
        } else {
            pAxesBounds.setVisible(true);
        }
    }
    private void bMarginsActionPerformed(java.awt.event.ActionEvent evt) {
        if (bMargins.isSelected() == true) {
            pMargins.setVisible(false);
        } else {
            pMargins.setVisible(true);
        }
    }
    private void bAxisTitleActionPerformed(java.awt.event.ActionEvent evt) {
        if (bAxisTitle.isSelected() == true) {
            pAxisTitle.setVisible(false);
        } else {
            pAxisTitle.setVisible(true);
        }
    }
    private void bLocationActionPerformed(java.awt.event.ActionEvent evt) {
        if (bLocation.isSelected() == true) {
            pLocation.setVisible(false);
        } else {
            pLocation.setVisible(true);
        }
    }
    private void bReverseActionPerformed(java.awt.event.ActionEvent evt) {
        if (bReverse.isSelected() == true) {
            pReverse.setVisible(false);
        } else {
            pReverse.setVisible(true);
        }
    }

    /**
    * Implement the action button to open a JDialog with a JColorChoose.
    */
    private void bBackColorActionPerformed(java.awt.event.ActionEvent evt) {
        backColorDialog.setVisible(true);
    }
    private void bForeColorActionPerformed(java.awt.event.ActionEvent evt) {
        foreColorDialog.setVisible(true);
    }
    private void bFontColorActionPerformed(java.awt.event.ActionEvent evt) {
        fontColorDialog.setVisible(true);
    }

    /**
    * Implement the action on the OK button to save the color chosen by the user.
    */
    private void okBackActionPerformed(java.awt.event.ActionEvent evt) {
        choice = chooser.getColor();
        cBackColor.setBackground(choice);
        backColorDialog.dispose();
    }
    private void okForeActionPerformed(java.awt.event.ActionEvent evt) {
        choice1 = chooser1.getColor();
        cForeColor.setBackground(choice1);
        foreColorDialog.dispose();
    }
    private void okFontActionPerformed(java.awt.event.ActionEvent evt) {
        choice2 = chooser2.getColor();
        cFontColor.setBackground(choice2);
        fontColorDialog.dispose();
    }

    private void cTitlePageActionPerformed(java.awt.event.ActionEvent evt) {}
    private void cMarginsLeftActionPerformed(java.awt.event.ActionEvent evt) {}
    private void cMarginsRightActionPerformed(java.awt.event.ActionEvent evt) {}
    private void cMarginsTopActionPerformed(java.awt.event.ActionEvent evt) {}
    private void cMarginsBottomActionPerformed(java.awt.event.ActionEvent evt) {}
}
