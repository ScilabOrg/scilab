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
* Construction and startup of all components of the section: Control.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Control extends BaseProperties {
    protected static javax.swing.JToggleButton bControl;
    protected static JPanel pControl;
    protected javax.swing.JLabel lControl;
    protected javax.swing.JSeparator sControl;
    protected javax.swing.JComboBox cAutoResize;
    protected javax.swing.JLabel lAutoResize;
    protected javax.swing.JLabel lxposition;
    protected javax.swing.JTextField cxposition;
    protected javax.swing.JLabel lyposition;
    protected javax.swing.JTextField cyposition;
    protected javax.swing.JTextField cxsize;
    protected javax.swing.JLabel lxsize;
    protected javax.swing.JTextField cysize;
    protected javax.swing.JLabel lysize;


    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of Figure.
    */
    public Control(String objectID) {
        super(objectID);
        position();
        setIconsControl();
        initPropertiesControl(objectID);
    }

    /**
    * It has all the components of the section Control.
    */
    @Override
    public void controlComponents() {
        bControl = new javax.swing.JToggleButton();
        lControl = new javax.swing.JLabel();
        sControl = new javax.swing.JSeparator();
        pControl = new JPanel();
        lAutoResize = new javax.swing.JLabel();
        cAutoResize = new javax.swing.JComboBox();
        lxposition = new javax.swing.JLabel();
        cxposition = new javax.swing.JTextField();
        lyposition = new javax.swing.JLabel();
        cyposition = new javax.swing.JTextField();
        lxsize = new javax.swing.JLabel();
        cxsize = new javax.swing.JTextField();
        lysize = new javax.swing.JLabel();
        cysize = new javax.swing.JTextField();

        //Components of the header: Control.
        pControl.setAlignmentX(0.0F);
        pControl.setAlignmentY(0.0F);

        bControl.setBorder(null);
        bControl.setBorderPainted(false);
        bControl.setContentAreaFilled(false);
        bControl.setMaximumSize(new java.awt.Dimension(16, 16));
        bControl.setMinimumSize(new java.awt.Dimension(16, 16));
        bControl.setPreferredSize(new java.awt.Dimension(16, 16));
        bControl.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                bControlActionPerformed(evt);
            }
        });

        lControl.setText(MessagesGED.control);

        sControl.setPreferredSize(new java.awt.Dimension(50, 2));

        //Components of the property: Auto Resize.
        lAutoResize.setBackground(new java.awt.Color(255, 255, 255));
        lAutoResize.setText(" " + MessagesGED.auto_resize);
        lAutoResize.setAlignmentX(0.5F);
        lAutoResize.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lAutoResize.setOpaque(true);
        lAutoResize.setPreferredSize(new java.awt.Dimension(153, 20));

        cAutoResize.setModel(new javax.swing.DefaultComboBoxModel(new String[] {MessagesGED.off, MessagesGED.on}));
        cAutoResize.setBorder(null);
        cAutoResize.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        cAutoResize.setEditor(null);
        cAutoResize.setPreferredSize(new java.awt.Dimension(150, 20));
        cAutoResize.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cAutoResizeActionPerformed(evt);
            }
        });

	//Components of the property: Position X.
        lxposition.setBackground(new java.awt.Color(255, 255, 255));
        lxposition.setText(" " + MessagesGED.x_position);
        lxposition.setAlignmentX(0.5F);
        lxposition.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lxposition.setOpaque(true);
        lxposition.setPreferredSize(new java.awt.Dimension(153, 20));

        cxposition.setToolTipText(MessagesGED.x_position_tooltip);
        cxposition.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cxposition.setPreferredSize(new java.awt.Dimension(150, 20));
        cxposition.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cxpositionActionPerformed(evt);
            }
        });

        //Components of the property: Position Y.
        lyposition.setBackground(new java.awt.Color(255, 255, 255));
        lyposition.setText(" " + MessagesGED.y_position);
        lyposition.setAlignmentX(0.5F);
        lyposition.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lyposition.setOpaque(true);
        lyposition.setPreferredSize(new java.awt.Dimension(153, 20));

        cyposition.setToolTipText(MessagesGED.y_position_tooltip);
        cyposition.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cyposition.setPreferredSize(new java.awt.Dimension(150, 20));
        cyposition.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cypositionActionPerformed(evt);
            }
        });

	//Components of the property: Size X.
        lxsize.setBackground(new java.awt.Color(255, 255, 255));
        lxsize.setText(" " + MessagesGED.x_size);
        lxsize.setAlignmentX(0.5F);
        lxsize.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lxsize.setOpaque(true);
        lxsize.setPreferredSize(new java.awt.Dimension(153, 20));

        cxsize.setToolTipText(MessagesGED.x_size_tooltip);
        cxsize.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cxsize.setPreferredSize(new java.awt.Dimension(150, 20));
        cxsize.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cxsizeActionPerformed(evt);
            }
        });

	//Components of the property: Size Y.
        lysize.setBackground(new java.awt.Color(255, 255, 255));
        lysize.setText(" " + MessagesGED.y_size);
        lysize.setAlignmentX(0.5F);
        lysize.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        lysize.setOpaque(true);
        lysize.setPreferredSize(new java.awt.Dimension(153, 20));

        cysize.setToolTipText(MessagesGED.y_size_tooltip);
        cysize.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        cysize.setPreferredSize(new java.awt.Dimension(150, 20));
        cysize.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cysizeActionPerformed(evt);
            }
        });
   }

    /**
    * Positioning all the components of the Control.
    */
    private void position() {
        javax.swing.GroupLayout pControlLayout = new javax.swing.GroupLayout(pControl);
        pControl.setLayout(pControlLayout);
        pControlLayout.setHorizontalGroup(
            pControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pControlLayout.createSequentialGroup()
                .addGap(18, 18, 18)
                .addGroup(pControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(pControlLayout.createSequentialGroup()
                        .addComponent(lAutoResize, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cAutoResize, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(pControlLayout.createSequentialGroup()
                        .addComponent(lxposition, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cxposition, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(pControlLayout.createSequentialGroup()
                        .addComponent(lyposition, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cyposition, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(pControlLayout.createSequentialGroup()
                        .addComponent(lxsize, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cxsize, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(pControlLayout.createSequentialGroup()
                        .addComponent(lysize, javax.swing.GroupLayout.PREFERRED_SIZE, 159, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(4, 4, 4)
                        .addComponent(cysize, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE))))
        );
        pControlLayout.setVerticalGroup(
            pControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pControlLayout.createSequentialGroup()
                .addGroup(pControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lAutoResize, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cAutoResize, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lxposition, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cxposition, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lyposition, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cyposition, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lxsize, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cxsize, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(5, 5, 5)
                .addGroup(pControlLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lysize, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cysize, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );
    }

    /**
    * Loads the current properties of the section Control.
    *
    * @param objectID Enters the identification of Figure.
    */
    public void initPropertiesControl(String objectID) {
        if (objectID != null) {
            currentfigure = objectID;
            /** Get the current status of the property: Auto Resize */
            boolean isAutoResize = (Boolean) GraphicController.getController()
                    .getProperty(currentfigure, GraphicObjectProperties.__GO_AUTORESIZE__);
            if (isAutoResize) {
                cAutoResize.setSelectedIndex(1);
            } else {
		cAutoResize.setSelectedIndex(0);
            }
            /** Get the current status of the property: Position */
            Integer[] currentPosition = (Integer[]) GraphicController.getController()
                        .getProperty(currentfigure, GraphicObjectProperties.__GO_POSITION__);
            String currentXposition;
            currentXposition = currentPosition[0].toString();
            cxposition.setText(currentXposition);
            String currentYposition;
            currentYposition = currentPosition[1].toString();
            cyposition.setText(currentXposition);
            /** Get the current status of the property: Size */
            Integer[] currentSize = (Integer[]) GraphicController.getController()
                    .getProperty(currentfigure, GraphicObjectProperties.__GO_SIZE__);
            String currentXsize;
            currentXsize = currentSize[0].toString();
            cxsize.setText(currentXsize);
            String currentYsize;
            currentYsize = currentSize[1].toString();
            cysize.setText(currentXsize);
        }
    }

    /**
    * Insert the icons on buttons.
    */
    private void setIconsControl() {
        bControl.setIcon(new javax.swing.ImageIcon(Inspector.icon_collapse));
        bControl.setSelectedIcon(new javax.swing.ImageIcon(Inspector.icon_expand));
    }

    /**
    * Implement the action button to show/hide.
    *
    * @param evt
    */
    private void bControlActionPerformed(java.awt.event.ActionEvent evt) {
        if (bControl.isSelected()) {
            pControl.setVisible(false);
        } else {
            pControl.setVisible(true);
        }
    }

    /**
    * Updates the property: Auto Resize.
    *
    * @param evt
    */
    private void cAutoResizeActionPerformed(java.awt.event.ActionEvent evt) {
        boolean setAutoResize = true;
        if (cAutoResize.getSelectedIndex() == 0) {
            setAutoResize = false;
        }
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_AUTORESIZE__, setAutoResize);
    }

    /**
    * Updates the property: xPosition.
    *
    * @param evt
    */
    private void cxpositionActionPerformed(java.awt.event.ActionEvent evt) {
        int setXposition = Integer.parseInt(cxposition.getText());
        int setYposition = Integer.parseInt(cyposition.getText());
        Integer[] setPosition = new Integer[2];
        setPosition[0] = setXposition;
        setPosition[1] = setYposition;
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_POSITION__, setPosition);
    }

    /**
    * Updates the property: yPosition.
    *
    * @param evt
    */
    private void cypositionActionPerformed(java.awt.event.ActionEvent evt) {
        cxpositionActionPerformed(evt);
    }

    /**
    * Updates the property: xSize.
    *
    * @param evt
    */
    private void cxsizeActionPerformed(java.awt.event.ActionEvent evt) {
        int setXsize = Integer.parseInt(cxsize.getText());
        int setYsize = Integer.parseInt(cysize.getText());
        Integer[] setSize = new Integer[2];
        setSize[0] = setXsize;
        setSize[1] = setYsize;
        GraphicController.getController()
                .setProperty(currentfigure, GraphicObjectProperties.__GO_SIZE__, setSize);
    }

    /**
    * Updates the property: ySize.
    *
    * @param evt
    */
    private void cysizeActionPerformed(java.awt.event.ActionEvent evt) {
        cxsizeActionPerformed(evt);
    }
}