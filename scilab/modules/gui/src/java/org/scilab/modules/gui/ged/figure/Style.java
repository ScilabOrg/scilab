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
package org.scilab.modules.gui.ged.figure;

import javax.swing.JPanel;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.Inspector;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Style/Appeareance.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Style extends Data {
    protected static javax.swing.JToggleButton bStyle;
    protected static JPanel pStyle;
    protected JPanel pColor;
    protected javax.swing.JButton bColor;
    protected javax.swing.JLabel lStyle;
    protected javax.swing.JSeparator sStyle;
    protected javax.swing.JLabel lColor;
    protected javax.swing.JLabel cColor;
    protected javax.swing.JComboBox cPixmap;
    protected javax.swing.JLabel lPixmap;
    protected javax.swing.JComboBox cPixelDrawing;
    protected javax.swing.JLabel lPixelDrawing;
    protected javax.swing.JComboBox cRotationStyle;
    protected javax.swing.JLabel lRotationStyle;
    protected javax.swing.JDialog colorDialog;

    /**
    * Receives and passes the objectID to the parent class.
    *
    * @param objectID Enters the identification of Figure.
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
        pStyle = new JPanel();
        bStyle = new javax.swing.JToggleButton();
        lStyle = new javax.swing.JLabel();
        sStyle = new javax.swing.JSeparator();
        pColor = new JPanel();
        bColor = new javax.swing.JButton();
        cColor = new javax.swing.JLabel();
        lColor = new javax.swing.JLabel();
        lPixmap = new javax.swing.JLabel();
        cPixmap = new javax.swing.JComboBox();
        lPixelDrawing = new javax.swing.JLabel();
        cPixelDrawing = new javax.swing.JComboBox();
        lRotationStyle = new javax.swing.JLabel();
        cRotationStyle = new javax.swing.JComboBox();

        //Components of the header: Style/Appeareance.
        pStyle.setAlignmentX(0.0F);
        pStyle.setAlignmentY(0.0F);

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

        lStyle.setText(MessagesGED.style_appearance);

        sStyle.setPreferredSize(new java.awt.Dimension(50, 2));

	//Components of the property: Color.
        pColor.setBackground(new java.awt.Color(255, 255, 255));
        pColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        pColor.setPreferredSize(new java.awt.Dimension(157, 20));


        bColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        bColor.setContentAreaFilled(false);
        bColor.setPreferredSize(new java.awt.Dimension(16, 16));
        bColor.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bColorActionPerformed(evt);
            }
        });

        cColor.setText(" ");
        cColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cColor.setOpaque(true);

        lColor.setBackground(new java.awt.Color(255, 255, 255));
        lColor.setText(" " + MessagesGED.background_color);
        lColor.setAlignmentX(0.5F);
        lColor.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lColor.setOpaque(true);
        lColor.setPreferredSize(new java.awt.Dimension(153, 20));

	//Positioning the components of the property: Color.
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

	//Components of the property: Pixmap.
        lPixmap.setBackground(new java.awt.Color(255, 255, 255));
        lPixmap.setText(" " + MessagesGED.pixmap);
        lPixmap.setAlignmentX(0.5F);
        lPixmap.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lPixmap.setOpaque(true);
        lPixmap.setPreferredSize(new java.awt.Dimension(153, 20));

        cPixmap.setModel(new javax.swing.DefaultComboBoxModel(new String[] {MessagesGED.off, MessagesGED.on}));
        cPixmap.setBorder(null);
        cPixmap.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        cPixmap.setEditor(null);
        cPixmap.setPreferredSize(new java.awt.Dimension(150, 20));
        cPixmap.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cPixmapActionPerformed(evt);
            }
        });

	//Components of the property: Pixel Drawing Mode.
        lPixelDrawing.setBackground(new java.awt.Color(255, 255, 255));
        lPixelDrawing.setText(" " + MessagesGED.pixel_drawing_mode);
        lPixelDrawing.setAlignmentX(0.5F);
        lPixelDrawing.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lPixelDrawing.setOpaque(true);
        lPixelDrawing.setPreferredSize(new java.awt.Dimension(153, 20));

        cPixelDrawing.setModel(new javax.swing.DefaultComboBoxModel(new String[] { 
            "clear", "and", "andReverse", "copy", "andInverted",
            "noop", "xor", "or", "nor", "equiv", "invert", "orReverse",
            "copyInverted", "orInverted", "nand", "set" }));
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

	//Components of the property: Rotation Style.
        lRotationStyle.setBackground(new java.awt.Color(255, 255, 255));
        lRotationStyle.setText(" " + MessagesGED.rotation_style);
        lRotationStyle.setAlignmentX(0.5F);
        lRotationStyle.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lRotationStyle.setOpaque(true);
        lRotationStyle.setPreferredSize(new java.awt.Dimension(153, 20));

        cRotationStyle.setModel(new javax.swing.DefaultComboBoxModel(new String[] { 
            MessagesGED.unary, MessagesGED.multiple }));
        cRotationStyle.setBorder(null);
        cRotationStyle.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        cRotationStyle.setEditor(null);
        cRotationStyle.setPreferredSize(new java.awt.Dimension(150, 20));
        cRotationStyle.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cRotationStyleActionPerformed(evt);
            }
        });
   }

    /**
    * Positioning all the components of the Style/Appearance.
    */
    private void position() {
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
    }

    /**
    * Loads the current properties of the section Style/Appearance.
    *
    * @param figureUid Enters the identification of Figure.
    */
    public void initPropertiesStyle(String figureUid) {
        if (figureUid != null) {
            currentfigure = figureUid;

            /** Get the current status of the property: Background Color */
            Integer scilabColor = (Integer) GraphicController.getController()
                  .getProperty(currentfigure, GraphicObjectProperties.__GO_BACKGROUND__);
            Double[] rgbColor = ColorMapHandler.getRGBcolor(currentfigure,scilabColor);
            cColor.setBackground(new java.awt.Color(rgbColor[0].intValue(), rgbColor[1].intValue(), rgbColor[2].intValue()));

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
    * Method that will be overwritten by ColorDialog.
    */
    public void dialogColor() { }

    /**
    * Insert the icons on buttons.
    */
    private void setIconsStyle() {
        bStyle.setIcon(new javax.swing.ImageIcon(Inspector.icon_collapse));
        bStyle.setSelectedIcon(new javax.swing.ImageIcon(Inspector.icon_expand));
        bColor.setIcon(new javax.swing.ImageIcon(Inspector.icon_color_fill));
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
    * Updates the property: Color.
    *
    * @param evt
    */
    private void bColorActionPerformed(java.awt.event.ActionEvent evt) {
        colorDialog.setVisible(true);
        colorDialog.setModal(true);
    }

    /**
    * Updates the property: Pixmap.
    *
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
    *
    * @param evt
    */
    private void cPixelDrawingActionPerformed(java.awt.event.ActionEvent evt) {
        int setPixelDrawing = cPixelDrawing.getSelectedIndex();
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_PIXEL_DRAWING_MODE__, setPixelDrawing);
    }

    /**
    * Updates the property: Rotation Style.
    *
    * @param evt
    */
    private void cRotationStyleActionPerformed(java.awt.event.ActionEvent evt) {
        int setRotationStyle = cRotationStyle.getSelectedIndex();
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_ROTATION_TYPE__, setRotationStyle);
    }
}