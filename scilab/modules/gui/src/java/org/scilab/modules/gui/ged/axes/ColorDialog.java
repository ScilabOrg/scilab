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

import java.awt.Color;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Implementation of the windows to color selection.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class ColorDialog extends Style {
    protected javax.swing.JColorChooser chooser;
    protected javax.swing.JButton okBack;
    protected Color choice;
    
    protected javax.swing.JColorChooser chooser1;
    protected javax.swing.JButton okFore;
    protected Color choice1;
    
    protected javax.swing.JColorChooser chooser2;
    protected javax.swing.JButton okFont;
    protected Color choice2;

    /**
    * Receives and passes the objectID to the parent class.
    *
    * @param objectID Enters the identification of Axes.
    */
    public ColorDialog(String objectID) {
        super(objectID);
    }

    /**
    * JDialog - Selection of background colors.
    */
    @Override
    public void dialogBackgroundColor() {
        backColorDialog = new javax.swing.JDialog();
        chooser = new javax.swing.JColorChooser();
        okBack = new javax.swing.JButton();

        backColorDialog.setTitle(MessagesGED.choose_background_color);
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
    }

    /**
    * JDialog - Selection of foreground colors.
    */
    @Override
    public void dialogForegroundColor() {        
        foreColorDialog = new javax.swing.JDialog();
        chooser1 = new javax.swing.JColorChooser();
        okFore = new javax.swing.JButton();

        foreColorDialog.setTitle(MessagesGED.choose_foreground_color);
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
    }

    /**
    * JDialog - Selection of font colors.
    */
    @Override
    public void dialogFontColor() {
        fontColorDialog = new javax.swing.JDialog();
        chooser2 = new javax.swing.JColorChooser();
        okFont = new javax.swing.JButton();

        fontColorDialog.setTitle(MessagesGED.choose_font_color);
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
    }

    /**
    * Implement the action on the OK button to save the color chosen by the user.
    *
    * @param evt
    */
    private void okBackActionPerformed(java.awt.event.ActionEvent evt) {
        choice = chooser.getColor();
        double red = choice.getRed();
        double green = choice.getGreen();
        double blue = choice.getBlue();
        Integer scilabColor = ColorMapHandler.getScilabColor(red, green, blue, parentFigure);
        setBackgroungColor(scilabColor);
        cBackColor.setBackground(choice);
        backColorDialog.dispose();
    }

    /**
    * Implement the action on the OK button to save the color chosen by the user.
    *
    * @param evt
    */
    private void okForeActionPerformed(java.awt.event.ActionEvent evt) {
        choice1 = chooser1.getColor();
        double red = choice1.getRed();
        double green = choice1.getGreen();
        double blue = choice1.getBlue();
        Integer scilabColor = ColorMapHandler.getScilabColor(red, green, blue, parentFigure);
        setForegroundColor(scilabColor);
        cForeColor.setBackground(choice1);
        foreColorDialog.dispose();
    }

    /**
    * Implement the action on the OK button to save the color chosen by the user.
    *
    * @param evt
    */
    private void okFontActionPerformed(java.awt.event.ActionEvent evt) {
        choice2 = chooser2.getColor();
        double red = choice2.getRed();
        double green = choice2.getGreen();
        double blue = choice2.getBlue();
        Integer scilabColor = ColorMapHandler.getScilabColor(red, green, blue, parentFigure);
        setFontColor(scilabColor);
        cFontColor.setBackground(choice2);
        fontColorDialog.dispose();
    }

    /**
    * Change the color of the object.
    *
    * @param scilabColor index of the color map.
    */
    private void setBackgroungColor(Integer scilabColor) {
        GraphicController.getController()
              .setProperty(currentaxes, GraphicObjectProperties.__GO_BACKGROUND__, scilabColor);
    }
    
    /**
    * Change the color of the object.
    *
    * @param scilabColor index of the color map.
    */
    private void setForegroundColor(Integer scilabColor) {
        GraphicController.getController()
              .setProperty(currentaxes, GraphicObjectProperties.__GO_LINE_COLOR__, scilabColor);
    }
    
    /**
    * Change the color of the object.
    *
    * @param scilabColor index of the color map.
    */
    private void setFontColor(Integer scilabColor) {
        GraphicController.getController()
              .setProperty(currentaxes, GraphicObjectProperties.__GO_FONT_COLOR__, scilabColor);
    }
}