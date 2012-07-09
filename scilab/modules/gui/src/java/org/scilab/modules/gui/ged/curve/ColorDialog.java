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

import org.scilab.modules.gui.ged.MessagesGED;

/**
* Implementation of the windows to color selection.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class ColorDialog extends Style {
    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of curve.
    */
    public ColorDialog(String objectID) {
        super(objectID);
    }

    /**
    * JDialog - Selection of background colors.
    */
    @Override
    public void dialogBackgroundColor() {
        backcolorDialog.setTitle(MessagesGED.choose_background_color);
        backcolorDialog.setMinimumSize(new java.awt.Dimension(670, 450));
        backcolorDialog.setModal(true);
        backcolorDialog.setResizable(false);

        okBack.setText("OK");
        okBack.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                okBackActionPerformed(evt);
            }
        });

        //Position settings.
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
    }

    /**
    * JDialog - Selection of foreground colors.
    */
    @Override
    public void dialogForegroungColor() {
        forecolorDialog.setTitle(MessagesGED.choose_foreground_color);
        forecolorDialog.setMinimumSize(new java.awt.Dimension(670, 450));
        forecolorDialog.setModal(true);
        forecolorDialog.setResizable(false);

        okFore.setText("OK");
        okFore.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                okForeActionPerformed(evt);
            }
        });

        //Position settings.
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
}