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
package org.scilab.modules.gui.ged.curve;

import java.awt.Color;
import org.scilab.modules.gui.ged.Inspector;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
* Construction and startup of all components of the section: Style/Appeareance.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Style extends BaseProperties {
    protected javax.swing.JButton bBackColor;
    protected javax.swing.JButton bForeColor;
    public static javax.swing.JToggleButton bStyle;
    public static javax.swing.JLabel cBackColor;
    public static javax.swing.JLabel cForeColor;
    protected javax.swing.JComboBox cLine;
    protected javax.swing.JComboBox cPolyline;
    protected javax.swing.JLabel lBackColor;
    protected javax.swing.JLabel lForeColor;
    protected javax.swing.JLabel lLine;
    protected javax.swing.JLabel lPolyline;
    protected javax.swing.JLabel lStyle;
    protected javax.swing.JPanel pBackColor;
    protected javax.swing.JPanel pForeColor;
    public static javax.swing.JPanel pStyle;
    protected javax.swing.JSeparator sStyle;
    public static javax.swing.JDialog backcolorDialog;
    public static javax.swing.JColorChooser chooserBack;
    public static javax.swing.JColorChooser chooserFore;
    public static javax.swing.JDialog forecolorDialog;
    public static javax.swing.JButton okBack;
    public static javax.swing.JButton okFore;
    public static Color choiceBack;
    public static Color choiceFore;
    private String currentcurve = null;

    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of curve.
    */
    public Style(String objectID) {
        super(objectID);
        initPropertiesStyle(objectID);
        position();
        setIconsStyle();
    }

    /**
    * It has all the components of the section Style/Appeareance.
    */
    @Override
    public void styleComponents() {
        backcolorDialog = new javax.swing.JDialog();
        chooserBack = new javax.swing.JColorChooser();
        okBack = new javax.swing.JButton();
        forecolorDialog = new javax.swing.JDialog();
        chooserFore = new javax.swing.JColorChooser();
        okFore = new javax.swing.JButton();
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

        //Components of the header: Style/Appeareance.
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
        lStyle.setText(MessagesGED.style_appearance);

        sStyle.setPreferredSize(new java.awt.Dimension(50, 2));

        pStyle.setAlignmentX(0.0F);
        pStyle.setAlignmentY(0.0F);

	//Components of the property: Background Color.
        lBackColor.setBackground(new java.awt.Color(255, 255, 255));
        lBackColor.setText(" " + MessagesGED.background_color);
        lBackColor.setAlignmentX(0.5F);
        lBackColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lBackColor.setOpaque(true);
        lBackColor.setPreferredSize(new java.awt.Dimension(153, 20));

        pBackColor.setBackground(new java.awt.Color(255, 255, 255));
        pBackColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        pBackColor.setPreferredSize(new java.awt.Dimension(157, 20));

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
        cBackColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cBackColor.setOpaque(true);

	//Positioning the components of the property: Background Color.
        javax.swing.GroupLayout pBackColorLayout = new javax.swing.GroupLayout(pBackColor);
        pBackColor.setLayout(pBackColorLayout);
        pBackColorLayout.setHorizontalGroup(
            pBackColorLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pBackColorLayout.createSequentialGroup()
                .addGap(35, 35, 35)
                .addComponent(bBackColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(11, 11, 11)
                .addComponent(cBackColor, javax.swing.GroupLayout.PREFERRED_SIZE, 30, javax.swing.GroupLayout.PREFERRED_SIZE))
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

	//Components of the property: Foreground Color.
        lForeColor.setBackground(new java.awt.Color(255, 255, 255));
        lForeColor.setText(" " + MessagesGED.foreground_color);
        lForeColor.setAlignmentX(0.5F);
        lForeColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lForeColor.setOpaque(true);
        lForeColor.setPreferredSize(new java.awt.Dimension(153, 20));

        pForeColor.setBackground(new java.awt.Color(255, 255, 255));
        pForeColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        pForeColor.setPreferredSize(new java.awt.Dimension(157, 20));

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
        cForeColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cForeColor.setOpaque(true);

	//Positioning the components of the property: Foreground Color.
        javax.swing.GroupLayout pForeColorLayout = new javax.swing.GroupLayout(pForeColor);
        pForeColor.setLayout(pForeColorLayout);
        pForeColorLayout.setHorizontalGroup(
            pForeColorLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pForeColorLayout.createSequentialGroup()
                .addGap(35, 35, 35)
                .addComponent(bForeColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(11, 11, 11)
                .addComponent(cForeColor, javax.swing.GroupLayout.PREFERRED_SIZE, 30, javax.swing.GroupLayout.PREFERRED_SIZE))
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

	//Components of the property: Line Style.
        lLine.setBackground(new java.awt.Color(255, 255, 255));
        lLine.setText(" " + MessagesGED.line_style);
        lLine.setAlignmentX(0.5F);
        lLine.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lLine.setOpaque(true);
        lLine.setPreferredSize(new java.awt.Dimension(153, 20));

        cLine.setModel(new javax.swing.DefaultComboBoxModel(new String[] { 
            MessagesGED.solid, MessagesGED.dash, MessagesGED.dash_dot, 
            MessagesGED.longdash_dot, MessagesGED.bigdash_dot, 
            MessagesGED.bigdash_longdash, MessagesGED.dot, MessagesGED.double_dot }));
        cLine.setBorder(null);
        cLine.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        cLine.setEditor(null);
        cLine.setPreferredSize(new java.awt.Dimension(150, 20));
        cLine.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cLineActionPerformed(evt);
            }
        });

	//Components of the property: Polyline Style.
        lPolyline.setBackground(new java.awt.Color(255, 255, 255));
        lPolyline.setText(" " + MessagesGED.polyline_style);
        lPolyline.setAlignmentX(0.5F);
        lPolyline.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lPolyline.setOpaque(true);
        lPolyline.setPreferredSize(new java.awt.Dimension(153, 20));

        cPolyline.setModel(new javax.swing.DefaultComboBoxModel(new String[] { 
            MessagesGED.interpolated, MessagesGED.staircase, MessagesGED.barplot, 
            MessagesGED.bar, MessagesGED.arrowed, MessagesGED.filled }));
        cPolyline.setBorder(null);
        cPolyline.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        cPolyline.setEditor(null);
        cPolyline.setPreferredSize(new java.awt.Dimension(150, 20));
        cPolyline.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cPolylineActionPerformed(evt);
            }
        });
   }

    /**
    * Positioning all the components of the Style/Appearance.
    */
    private void position() {
	//Positioning all components.
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
    }

    /**
    * Loads the current properties of the section Style/Appearance.
    *
    * @param objectID Enters the identification of curve.
    */
    public void initPropertiesStyle(String objectID) {
        if (objectID != null) {
            currentcurve = objectID;
            // Get the current status of the property: Line Style 
            int currentLineStyle = (Integer) GraphicController.getController()
                    .getProperty(currentcurve, GraphicObjectProperties.__GO_LINE_STYLE__);
            cLine.setSelectedIndex(currentLineStyle);
            // Get the current status of the property: Polyline Style
            int currentPolylineStyle = (Integer) GraphicController.getController()
                    .getProperty(currentcurve, GraphicObjectProperties.__GO_POLYLINE_STYLE__);
            cPolyline.setSelectedIndex(currentPolylineStyle);
        }
    }

    /**
    * Method that will be overwritten by ColorDialog.
    */
    public void dialogBackgroundColor() { }

    /**
    * Method that will be overwritten by ColorDialog.
    */
    public void dialogForegroungColor() { }

    /**
    * Insert the icons on buttons.
    */
    private void setIconsStyle() {
        bStyle.setIcon(new javax.swing.ImageIcon(Inspector.icon_collapse));
        bStyle.setSelectedIcon(new javax.swing.ImageIcon(Inspector.icon_expand));
        bBackColor.setIcon(new javax.swing.ImageIcon(Inspector.icon_color_fill));
        bForeColor.setIcon(new javax.swing.ImageIcon(Inspector.icon_color_fill));
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
    * Updates the property: Line Style.
    * @param evt
    */
    private void cLineActionPerformed(java.awt.event.ActionEvent evt) {
        int setLine = cLine.getSelectedIndex();
        GraphicController.getController().setProperty(currentcurve, GraphicObjectProperties.__GO_LINE_STYLE__, setLine);
    }

    /**
    * Updates the property: Polyline Style.
    * @param evt
    */
    private void cPolylineActionPerformed(java.awt.event.ActionEvent evt) {
        int setPolyline = cPolyline.getSelectedIndex();
        GraphicController.getController()
                .setProperty(currentcurve, GraphicObjectProperties.__GO_POLYLINE_STYLE__, setPolyline);
    }

    /**
    * Updates the property: Background Color.
    * @param evt
    */
    private void bBackColorActionPerformed(java.awt.event.ActionEvent evt) {
        backcolorDialog.setVisible(true);
    }

    /**
    * Updates the property: Foreground Color.
    * @param evt
    */
    private void bForeColorActionPerformed(java.awt.event.ActionEvent evt) {
        forecolorDialog.setVisible(true);
    }
}