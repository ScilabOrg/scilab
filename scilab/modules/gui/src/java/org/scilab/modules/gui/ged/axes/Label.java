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

import javax.swing.JPanel;
import org.scilab.modules.gui.ged.Inspector;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
* Construction and startup of all components of the section: Label.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Label extends AxisRulers {
    //Header - Label
    protected static javax.swing.JToggleButton bLabel;
    protected javax.swing.JLabel lLabel;
    protected static JPanel pLabel;
    protected javax.swing.JSeparator sLabel;
    //Components of the property: Title Page
    protected javax.swing.JLabel lTitlePage;
    protected javax.swing.JTextField cTitlePage;
    //Components of the property: Axis Title
    protected static javax.swing.JToggleButton bAxisTitle;
    protected javax.swing.JLabel lAxisTitle;
    protected javax.swing.JLabel cAxisTitle;
    protected static JPanel pAxisTitle;
    protected javax.swing.JLabel lTitleX;
    protected javax.swing.JTextField cTitleX;
    protected javax.swing.JLabel lTitleY;
    protected javax.swing.JTextField cTitleY;
    protected javax.swing.JLabel lTitleZ;
    protected javax.swing.JTextField cTitleZ;

    /**
    * Initializes the properties and the icons of the buttons.
    *
    * @param objectID Enters the identification of Axes.
    */
    public Label(String objectID) {
        super(objectID);
        position();
        setIconsLabel();
        initPropertiesLabel(objectID);
    }

    /**
    * It has all the components of the section Label.
    */
    @Override
    public void labelComponents() {
        //Header
        bLabel = new javax.swing.JToggleButton();
        lLabel = new javax.swing.JLabel();
        sLabel = new javax.swing.JSeparator();
        pLabel = new JPanel();
        //Title Page
        lTitlePage = new javax.swing.JLabel();
        cTitlePage = new javax.swing.JTextField();
        //Axis Title
        bAxisTitle = new javax.swing.JToggleButton();
        lAxisTitle = new javax.swing.JLabel();
        cAxisTitle = new javax.swing.JLabel();
        pAxisTitle = new JPanel();
        lTitleX = new javax.swing.JLabel();
        cTitleX = new javax.swing.JTextField();
        lTitleY = new javax.swing.JLabel();
        cTitleY = new javax.swing.JTextField();
        lTitleZ = new javax.swing.JLabel();
        cTitleZ = new javax.swing.JTextField();

        //Components of the header: Axis Rulers
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

        lLabel.setText(MessagesGED.label);

        sLabel.setPreferredSize(new java.awt.Dimension(50, 2));

        //Components of the property: Title Page.
        lTitlePage.setBackground(new java.awt.Color(255, 255, 255));
        lTitlePage.setText(" " + MessagesGED.title_page);
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

        //Components of the property: Axis Title.
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
        lAxisTitle.setText(" " + MessagesGED.axis_title);
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

        //Positioning the components of the property: Axis Title.
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
    }

    /**
    * Positioning all the components of the Label.
    */
    private void position() {
        //Positioning all components.
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
    }

    /**
    * Loads the current properties of the section Label.
    *
    * @param axesID Enters the identification of Axes.
    */
    private void initPropertiesLabel(String axesID) {
        if (axesID != null) {
            currentaxes = axesID;
            /** Get the current status of the property: Title Page */
            String titlePage = (String) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_TITLE__);

            String[] textTitlePage = (String[])GraphicController.getController()
                    .getProperty(titlePage, GraphicObjectProperties.__GO_TEXT_STRINGS__);
            cTitlePage.setText(textTitlePage[0]);

            /** Get the current status of the property: Axis Title */
            String axisTitleX = (String) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_X_AXIS_LABEL__);

            String[] textTitleX = (String[])GraphicController.getController()
                    .getProperty(axisTitleX, GraphicObjectProperties.__GO_TEXT_STRINGS__);
            cTitleX.setText(textTitleX[0]);

            String axisTitleY = (String) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);

            String[] textTitleY = (String[])GraphicController.getController()
                    .getProperty(axisTitleY, GraphicObjectProperties.__GO_TEXT_STRINGS__);
            cTitleY.setText(textTitleY[0]);

            String axisTitleZ = (String) GraphicController.getController()
                    .getProperty(currentaxes, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);

            String[] textTitleZ = (String[])GraphicController.getController()
                    .getProperty(axisTitleZ, GraphicObjectProperties.__GO_TEXT_STRINGS__);
            cTitleZ.setText(textTitleZ[0]);

            titleAxis();
        }
    }

    /**
    * Insert the icons on buttons.
    */
    private final void setIconsLabel() {
        bLabel.setIcon(new javax.swing.ImageIcon(Inspector.icon_collapse));
        bLabel.setSelectedIcon(new javax.swing.ImageIcon(Inspector.icon_expand));
        bAxisTitle.setIcon(new javax.swing.ImageIcon(Inspector.icon_collapse));
        bAxisTitle.setSelectedIcon(new javax.swing.ImageIcon(Inspector.icon_expand));
    }

    /**
    * Implement the action button to show/hide.
    *
    * @param evt
    */
    private void bLabelActionPerformed(java.awt.event.ActionEvent evt) {
        if (bLabel.isSelected()) {
            pLabel.setVisible(false);
        } else {
            pLabel.setVisible(true);
        }
    }

    /**
    * Implement the action button to show/hide.
    *
    * @param evt
    */
    private void bAxisTitleActionPerformed(java.awt.event.ActionEvent evt) {
        if (bAxisTitle.isSelected()) {
            pAxisTitle.setVisible(false);
        } else {
            pAxisTitle.setVisible(true);
        }
    }

    /**
    * Inserts the current state of functionality: AXIS TITLE in main label.
    */
    public void titleAxis() {
        String titleAxisX = cTitleX.getText();
        String titleAxisY = cTitleY.getText();
        String titleAxisZ = cTitleZ.getText();
        String titleAxis = titleAxisX + " , " + titleAxisY + " , " + titleAxisZ;
        cAxisTitle.setText(" [" + titleAxis + "]");
    }

    /**
    * Updates the property: x Title Axis.
    *
    * @param evt
    */
    private void cTitleXActionPerformed(java.awt.event.ActionEvent evt) {
        String axisTitleX = (String) GraphicController.getController()
                .getProperty(currentaxes, GraphicObjectProperties.__GO_X_AXIS_LABEL__);
        String[] text = new String[1];
        text[0] = cTitleX.getText();
        GraphicController.getController()
                .setProperty(axisTitleX, GraphicObjectProperties.__GO_TEXT_STRINGS__, text);

        titleAxis();
    }

    /**
    * Updates the property: y Title Axis.
    *
    * @param evt
    */
    private void cTitleYActionPerformed(java.awt.event.ActionEvent evt) {
        String axisTitleY = (String) GraphicController.getController()
                .getProperty(currentaxes, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);
        String[] text = new String[1];
        text[0] = cTitleY.getText();
        GraphicController.getController()
                .setProperty(axisTitleY, GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
        titleAxis();
    }

    /**
    * Updates the property: z Title Axis.
    *
    * @param evt
    */
    private void cTitleZActionPerformed(java.awt.event.ActionEvent evt) {
        String axisTitleZ = (String) GraphicController.getController()
                .getProperty(currentaxes, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);
        String[] text = new String[1];
        text[0] = cTitleZ.getText();
        GraphicController.getController()
                .setProperty(axisTitleZ, GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
        titleAxis();
    }

    /**
    * Updates the property: Title Page.
    *
    * @param evt
    */
    private void cTitlePageActionPerformed(java.awt.event.ActionEvent evt) {
        String titlePage = (String) GraphicController.getController()
                .getProperty(currentaxes, GraphicObjectProperties.__GO_TITLE__);
        String[] text = new String[1];
        text[0] = cTitlePage.getText();
        GraphicController.getController()
                .setProperty(titlePage, GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
    }
}