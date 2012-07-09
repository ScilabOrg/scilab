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

import java.awt.event.ActionEvent;
import org.scilab.modules.gui.ged.Inspector;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
* Construction and startup of all components of the section: Style/Appeareance.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Style extends Position {
    //Header - Style/Appearance
    protected static javax.swing.JToggleButton bStyle;
    protected javax.swing.JLabel lStyle;
    protected javax.swing.JSeparator sStyle;
    protected static javax.swing.JPanel pStyle;
    //Components of the property: Background Color
    protected javax.swing.JLabel lBackColor;
    protected static javax.swing.JPanel pBackColor;
    protected javax.swing.JLabel cBackColor;
    protected static javax.swing.JButton bBackColor;
    protected javax.swing.JDialog backColorDialog;
    //Components of the property: Foreground Color
    protected javax.swing.JLabel lForeColor;
    protected static javax.swing.JPanel pForeColor;
    protected javax.swing.JLabel cForeColor;
    protected static javax.swing.JButton bForeColor;
    protected javax.swing.JDialog foreColorDialog;
    //Components of the property: Fontground Color
    protected javax.swing.JLabel lFontColor;
    protected static javax.swing.JPanel pFontColor;
    protected javax.swing.JLabel cFontColor;
    protected static javax.swing.JButton bFontColor;
    protected javax.swing.JDialog fontColorDialog;
    //Components of the property: Font Style
    protected javax.swing.JLabel lFontStyle;
    protected javax.swing.JComboBox cFontStyle;
    //Components of the property: Visible
    protected javax.swing.JLabel lVisible;
    protected javax.swing.JComboBox cVisible;

    /**
    * Receives and passes the objectID to the parent class.
    *
    * @param objectID Enters the identification of Axes.
    */
    public Style(String objectID) {
        super(objectID);
        position();
        setIconsStyle();
        initPropertiesStyle(objectID);
    }

    /**
    * It has all the components of the section Style/Appeareance.
    */
    @Override
    public void styleComponents() {
        //Header
        bStyle = new javax.swing.JToggleButton();
        lStyle = new javax.swing.JLabel();
        sStyle = new javax.swing.JSeparator();
        pStyle = new javax.swing.JPanel();
        //Background Color
        lBackColor = new javax.swing.JLabel();
        pBackColor = new javax.swing.JPanel();
        bBackColor = new javax.swing.JButton();
        cBackColor = new javax.swing.JLabel();
        //Foreground Color
        lForeColor = new javax.swing.JLabel();
        pForeColor = new javax.swing.JPanel();
        bForeColor = new javax.swing.JButton();
        cForeColor = new javax.swing.JLabel();
        //Font Color
        lFontColor = new javax.swing.JLabel();
        pFontColor = new javax.swing.JPanel();
        bFontColor = new javax.swing.JButton();
        cFontColor = new javax.swing.JLabel();
        //Font Style
        lFontStyle = new javax.swing.JLabel();
        cFontStyle = new javax.swing.JComboBox();
        //Visible
        lVisible = new javax.swing.JLabel();
        cVisible = new javax.swing.JComboBox();

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

        //Components of the property: Font Color.
        lFontColor.setBackground(new java.awt.Color(255, 255, 255));
        lFontColor.setText(" " + MessagesGED.font_color);
        lFontColor.setAlignmentX(0.5F);
        lFontColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lFontColor.setOpaque(true);
        lFontColor.setPreferredSize(new java.awt.Dimension(153, 20));

        pFontColor.setBackground(new java.awt.Color(255, 255, 255));
        pFontColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        pFontColor.setPreferredSize(new java.awt.Dimension(157, 20));

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
        cFontColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cFontColor.setOpaque(true);

        //Positioning the components of the property: Font Color.
        javax.swing.GroupLayout pFontColorLayout = new javax.swing.GroupLayout(pFontColor);
        pFontColor.setLayout(pFontColorLayout);
        pFontColorLayout.setHorizontalGroup(
            pFontColorLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pFontColorLayout.createSequentialGroup()
                .addGap(35, 35, 35)
                .addComponent(bFontColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(11, 11, 11)
                .addComponent(cFontColor, javax.swing.GroupLayout.PREFERRED_SIZE, 30, javax.swing.GroupLayout.PREFERRED_SIZE))
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

        //Components of the property: Font Style.
        lFontStyle.setBackground(new java.awt.Color(255, 255, 255));
        lFontStyle.setText(" " + MessagesGED.font_style);
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
        cFontStyle.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cFontStyleActionPerformed(evt);
            }
        });

        //Components of the property: Visible.
        lVisible.setBackground(new java.awt.Color(255, 255, 255));
        lVisible.setText(" " + MessagesGED.visible);
        lVisible.setAlignmentX(0.5F);
        lVisible.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lVisible.setOpaque(true);
        lVisible.setPreferredSize(new java.awt.Dimension(153, 20));

        cVisible.setModel(new javax.swing.DefaultComboBoxModel(new String[] {MessagesGED.off, MessagesGED.on}));
        cVisible.setBorder(null);
        cVisible.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        cVisible.setEditor(null);
        cVisible.setPreferredSize(new java.awt.Dimension(150, 20));
        cVisible.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cVisibleActionPerformed(evt);
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
    }

    /**
    * Loads the current properties of the section Style/Appearance.
    *
    * @param axesID Enters the identification of Axes.
    */
    private void initPropertiesStyle(String axesID) {
        if (axesID != null) {
            currentaxes = axesID;
            /** Get the current status of the property: Font Style */
            int currentFontStyle = (Integer) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_FONT_STYLE__);
            cFontStyle.setSelectedIndex(currentFontStyle);

            /** Get the current status of the property: Visible */
            boolean isVisible = (Boolean) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_VISIBLE__);
            if (isVisible) {
                cVisible.setSelectedIndex(1);
            } else {
                cVisible.setSelectedIndex(0);
            }
        }
    }

    /**
    * Method that will be overwritten by ColorDialog.
    */
    @Override
    public void dialogBackgroundColor() { }

    /**
    * Method that will be overwritten by ColorDialog.
    */
    @Override
    public void dialogForegroundColor() { }

    /**
    * Method that will be overwritten by ColorDialog.
    */
    @Override
    public void dialogFontColor() { }

    /**
    * Insert the icons on buttons.
    */
    private void setIconsStyle() {
        bStyle.setIcon(new javax.swing.ImageIcon(Inspector.icon_collapse));
        bStyle.setSelectedIcon(new javax.swing.ImageIcon(Inspector.icon_expand));
        bBackColor.setIcon(new javax.swing.ImageIcon(Inspector.icon_color_fill));
        bForeColor.setIcon(new javax.swing.ImageIcon(Inspector.icon_color_fill));
        bFontColor.setIcon(new javax.swing.ImageIcon(Inspector.icon_color_fill));
    }

    /**
    * Implement the action button to show/hide.
    *
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
    * Implement the action button to open a JDialog with a JColorChoose.
    *
    * @param evt
    */
    private void bBackColorActionPerformed(java.awt.event.ActionEvent evt) {
        backColorDialog.setVisible(true);
    }

    /**
    * Implement the action button to open a JDialog with a JColorChoose.
    *
    * @param evt
    */
    private void bForeColorActionPerformed(java.awt.event.ActionEvent evt) {
        foreColorDialog.setVisible(true);
    }

    /**
    * Implement the action button to open a JDialog with a JColorChoose.
    *
    * @param evt
    */
    private void bFontColorActionPerformed(java.awt.event.ActionEvent evt) {
        fontColorDialog.setVisible(true);
    }

    /**
    * Updates the property: Font Style.
    *
    * @param evt
    */
    private void cFontStyleActionPerformed(ActionEvent evt) {
        int setFontStyle = cFontStyle.getSelectedIndex();
        GraphicController.getController()
                .setProperty(currentaxes, GraphicObjectProperties.__GO_FONT_STYLE__, setFontStyle);
    }

    /**
    * Updates the property: Visible.
    *
    * @param evt
    */
    private void cVisibleActionPerformed(java.awt.event.ActionEvent evt) {
        boolean setVisible = true;
        if (cVisible.getSelectedIndex() == 0) {
            setVisible = false;
        }
        GraphicController.getController()
                .setProperty(currentaxes, GraphicObjectProperties.__GO_VISIBLE__, setVisible);
    }
}