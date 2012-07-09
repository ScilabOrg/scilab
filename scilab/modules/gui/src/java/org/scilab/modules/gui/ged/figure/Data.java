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
import org.scilab.modules.gui.ged.Inspector;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
* Construction and startup of all components of the section: Data.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Data extends Control {
    protected static javax.swing.JToggleButton bData;
    protected static JPanel pData;
    protected javax.swing.JLabel lData;
    protected javax.swing.JSeparator sData;
    protected javax.swing.JTextField cFigureID;
    protected javax.swing.JLabel lFigureID;
    protected javax.swing.JTextField cFigureName;
    protected javax.swing.JLabel lFigureName;
    protected javax.swing.JComboBox cUnits;
    protected javax.swing.JLabel lUnits;

    /**
    * Receives and passes the objectID to the parent class.
    *
    * @param objectID Enters the identification of Figure.
    */
    public Data(String objectID) {
        super(objectID);
        position();
        setIconsData();
        initPropertiesData(objectID);
    }

    /**
    * It has all the components of the section Control.
    */
    @Override
    public void dataComponents() {
        pData = new JPanel();
        bData = new javax.swing.JToggleButton();
        lData = new javax.swing.JLabel();
        sData = new javax.swing.JSeparator();
        lFigureID = new javax.swing.JLabel();
        cFigureID = new javax.swing.JTextField();
        lFigureName = new javax.swing.JLabel();
        cFigureName = new javax.swing.JTextField();
        lUnits = new javax.swing.JLabel();
        cUnits = new javax.swing.JComboBox();

        //Components of the header: Data.
        pData.setAlignmentX(0.0F);
        pData.setAlignmentY(0.0F);

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

        lData.setText(MessagesGED.data);

        sData.setPreferredSize(new java.awt.Dimension(50, 2));

        //Components of the property: Figure ID.
        lFigureID.setBackground(new java.awt.Color(255, 255, 255));
        lFigureID.setText(" " + MessagesGED.figure_id);
        lFigureID.setAlignmentX(0.5F);
        lFigureID.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lFigureID.setOpaque(true);
        lFigureID.setPreferredSize(new java.awt.Dimension(153, 20));

        cFigureID.setBackground(new java.awt.Color(238, 238, 238));
        cFigureID.setEditable(true);
        cFigureID.setToolTipText(MessagesGED.figure_id_tooltip);
        cFigureID.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cFigureID.setPreferredSize(new java.awt.Dimension(150, 20));
        cFigureID.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cFigureIDActionPerformed(evt);
            }
        });

        //Components of the property: Figure Name.
        lFigureName.setBackground(new java.awt.Color(255, 255, 255));
        lFigureName.setText(" " + MessagesGED.figure_name);
        lFigureName.setAlignmentX(0.5F);
        lFigureName.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lFigureName.setOpaque(true);
        lFigureName.setPreferredSize(new java.awt.Dimension(153, 20));

        cFigureName.setToolTipText(MessagesGED.figure_name_tooltip);
        cFigureName.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cFigureName.setPreferredSize(new java.awt.Dimension(150, 20));
        cFigureName.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cFigureNameActionPerformed(evt);
            }
        });

	//Components of the property: Units.
        lUnits.setBackground(new java.awt.Color(255, 255, 255));
        lUnits.setText(" " + MessagesGED.units);
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
   }

    /**
    * Positioning all the components of the Data.
    */
    private void position() {
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
    }

    /**
    * Loads the current properties of the section Data.
    *
    * @param objectID Enters the identification of Figure.
    */
    public void initPropertiesData(String objectID) {
        if (objectID != null) {
            currentfigure = objectID;
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
        }
    }

    /**
    * Insert the icons on buttons.
    */
    private void setIconsData() {
        bData.setSelectedIcon(new javax.swing.ImageIcon(Inspector.icon_expand));
        bData.setIcon(new javax.swing.ImageIcon(Inspector.icon_collapse));
    }

    /**
    * Implement the action button to show/hide.
    *
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
    * Updates the property: Figure ID.
    *
    * @param evt
    */
    private void cFigureIDActionPerformed(java.awt.event.ActionEvent evt) {
        int setfigureID = Integer.parseInt(cFigureID.getText());
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_ID__, setfigureID);
    }

    /**
    * Updates the property: Figure Name.
    *
    * @param evt
    */
    private void cFigureNameActionPerformed(java.awt.event.ActionEvent evt) {
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_NAME__, cFigureName.getText());
    }
}