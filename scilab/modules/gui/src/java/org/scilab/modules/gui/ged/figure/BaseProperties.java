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
* Construction and startup of all components of the section: Base Properties.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class BaseProperties extends Roof {
    protected static javax.swing.JToggleButton bBaseProperties;
    protected static JPanel pBaseProperties;
    protected javax.swing.JLabel lBaseProperties;
    protected javax.swing.JSeparator sBaseProperties;
    protected javax.swing.JComboBox cVisible;
    private javax.swing.JLabel lVisible;

    protected String currentfigure = null;

    /**
    * Initializes the properties and the icons of the buttons.
    *
    * @param objectID Enters the identification of Figure.
    */
    public BaseProperties(String objectID) {
        position();
        setIconsBaseProperties();
        initPropertiesBase(objectID);
    }

    /**
    * It has all the components of the section Base Properties.
    */
    @Override
    public void basePropertiesComponents() {
        pBaseProperties = new JPanel();
        bBaseProperties = new javax.swing.JToggleButton();
        lBaseProperties = new javax.swing.JLabel();
        sBaseProperties = new javax.swing.JSeparator();
        lVisible = new javax.swing.JLabel();
        cVisible = new javax.swing.JComboBox();

        //Components of the header: Base Properties.
        pBaseProperties.setAlignmentX(0.0F);
        pBaseProperties.setAlignmentY(0.0F);

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

        lBaseProperties.setText(MessagesGED.base_properties);

        sBaseProperties.setPreferredSize(new java.awt.Dimension(50, 2));

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
    * Positioning all the components of the Basic Properties.
    */
    private void position() {
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
    }

    /**
    * Loads the current properties of the section Base Properties.
    *
    * @param objectID Enters the identification of Figure.
    */
    private void initPropertiesBase(String objectID) {
        if (objectID != null) {
            currentfigure = objectID;
            // Get the current status of the property: Visible
            boolean isVisible = (Boolean) GraphicController.getController()
                    .getProperty(currentfigure, GraphicObjectProperties.__GO_VISIBLE__);
            if (isVisible) {
                cVisible.setSelectedIndex(1);
            } else {
                cVisible.setSelectedIndex(0);
            }
        }
    }

    /**
    * Insert the icons on buttons.
    */
    private final void setIconsBaseProperties() {
        bBaseProperties.setIcon(new javax.swing.ImageIcon(Inspector.icon_collapse));
        bBaseProperties.setSelectedIcon(new javax.swing.ImageIcon(Inspector.icon_expand));
    }

    /**
    * Implement the action button to show/hide.
    */
    private void bBasePropertiesActionPerformed(java.awt.event.ActionEvent evt) {
        if (bBaseProperties.isSelected()) {
            pBaseProperties.setVisible(false);
        } else {
            pBaseProperties.setVisible(true);
        }
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
                .setProperty(currentfigure, GraphicObjectProperties.__GO_VISIBLE__, setVisible);
    }
}