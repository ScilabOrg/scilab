/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.editor;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JTextField;

import org.scilab.modules.gui.editor.ScilabClipboard;
import org.scilab.modules.gui.editor.SystemClipboard;
import org.scilab.modules.gui.editor.PolylineHandler;
import org.scilab.modules.gui.editor.LabelHandler;
import org.scilab.modules.gui.editor.LegendHandler;

import org.scilab.modules.localization.Messages;


import javax.swing.JInternalFrame;
import javax.swing.JFrame;
import javax.media.opengl.awt.GLJPanel;


/**
* Point and click figure editor.
*
* Provides polyline selection by mouse click,
* copy, cut, paste, delete, hide, unhide polylines
* by popup menus and keyboard shortcuts.
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/

public class Editor {

    JPopupMenu menu;
    JMenuItem copy, cut, paste, delete, clear, hide, unhide, clipboardCopy, labelX, labelY, labelZ, insert, remove;
    JMenu labels, legends;
    JTextField textBox;
    JInternalFrame textFrame;

    String selected = null;
    String figureUid = null;
    Integer oriColor = 0;
    Integer[] lastClick = { 0, 0 };
    int axisTo = 0;
    boolean isLegend = false;
	GLJPanel myPanel;

    public Editor() {
        init();
        MyTextBox();
        setSelected(null);
        setFigure(null);
    }

    /**
    * On right mouse click, set popup menu visible.
    * @param event MouseEvent to retrieve click positon in figure.
    */
    public void onMouseClick(MouseEvent event) {
        boolean b = ScilabClipboard.getInstance().canPaste();
        paste.setEnabled(b);

        boolean notBlank = AxesHandler.isAxesNotBlank(figureUid);
        clipboardCopy.setEnabled(notBlank);

        menu.show(event.getComponent(), event.getX(), event.getY());
        lastClick[0] = event.getX();
        lastClick[1] = event.getY();
    }

    public void onClick(MouseEvent event) {

        myPanel = (GLJPanel)event.getComponent();
        textFrame.setVisible(false);
    }

    /**
    * Initializes the popup menu.
    *
    * Create the popup menu and all popup menu items
    * and set the properly action for each menu item.
    */
    public void init() {
        menu = new JPopupMenu();
        labels = new JMenu(Messages.gettext("Label"));
        legends = new JMenu(Messages.gettext("Legend"));

        copy = new JMenuItem(Messages.gettext("Copy"));
        copy.setToolTipText(Messages.gettext("Copy selected curve."));
        cut = new JMenuItem(Messages.gettext("Cut"));
        cut.setToolTipText(Messages.gettext("Cut selected curve."));
        paste = new JMenuItem(Messages.gettext("Paste"));
        paste.setToolTipText(Messages.gettext("Paste copied curve to this figure."));
        delete = new JMenuItem(Messages.gettext("Delete"));
        delete.setToolTipText(Messages.gettext("Delete selected curve."));
        clear = new JMenuItem(Messages.gettext("Clear"));
        clear.setToolTipText(Messages.gettext("Clear figure."));
        hide = new JMenuItem(Messages.gettext("Hide"));
        hide.setToolTipText(Messages.gettext("Hide selected curve."));
        unhide = new JMenuItem(Messages.gettext("Unhide all"));
        unhide.setToolTipText(Messages.gettext("Unhide all curves."));
        clipboardCopy = new JMenuItem(Messages.gettext("Copy to Clipboard"));
        clipboardCopy.setToolTipText(Messages.gettext("Copy figure to system clipboard."));
        labelX = new JMenuItem(Messages.gettext("Label X"));
        labelX.setToolTipText(Messages.gettext("Insert a label in X axis"));
        labelY = new JMenuItem(Messages.gettext("Label Y"));
        labelY.setToolTipText(Messages.gettext("Insert a label in Y axis"));
        labelZ = new JMenuItem(Messages.gettext("Label Z"));
        labelZ.setToolTipText(Messages.gettext("Insert a label in Z axis"));
        insert = new JMenuItem(Messages.gettext("Insert"));
        insert.setToolTipText(Messages.gettext("Insert a legend to current selected item"));
        remove = new JMenuItem(Messages.gettext("Remove"));
        remove.setToolTipText(Messages.gettext("Remove a legend of current selected item"));

        copy.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickCopy();
            }
        });

        cut.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickCut();
            }
        });

        paste.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickPaste();
            }
        });

        delete.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickDelete();
            }
        });

        clear.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickClear();
            }
        });

        hide.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickHide();
            }
        });

        unhide.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickUnhide();
            }
        });

        clipboardCopy.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickCCopy();
            }
        });

        labelX.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickLabel(AxesHandler.__X__);
            }
        });

        labelY.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickLabel(AxesHandler.__Y__);
            }
        });

        labelZ.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickLabel(AxesHandler.__Z__);
            }
        });

        insert.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickInsert();
            }
        });

        remove.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickRemove();
            }
        });


        labels.add(labelX);
        labels.add(labelY);
        labels.add(labelZ);
        legends.add(insert);
        legends.add(remove);
        menu.add(copy);
        menu.add(cut);
        menu.add(paste);
        menu.addSeparator();
        menu.add(delete);
        menu.add(clear);
        menu.addSeparator();
        menu.add(hide);
        menu.add(unhide);
        menu.addSeparator();
        menu.add(clipboardCopy);
        menu.addSeparator();
        menu.add(labels);
        menu.add(legends);

    }

    /**
    * Initialize the textbox that will be used to legend and label.
    */

    void MyTextBox()
    {
        textBox = new JTextField(16);
        textBox.setToolTipText(Messages.gettext("Press Enter to go or ESC to exit"));
        textBox.setVisible(true);
        textBox.addKeyListener(new KeyListener() {
            public void keyPressed(KeyEvent e) { 

                if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                    if (textBox.getText().length() > 0) {
                        setText();
                    }
                    textFrame.setVisible(false);
                }
                if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
                    textFrame.setVisible(false);
                }
            }

            public void keyReleased(KeyEvent e) { 				
		   
                if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                    textFrame.setVisible(false);
                }
                if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
                    textFrame.setVisible(false);
                } 
            }

            public void keyTyped(KeyEvent e) { 				

                if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                    textFrame.setVisible(false);
                }
                if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
                    textFrame.setVisible(false);
                } 
            }
        });
        textFrame = new JInternalFrame();
        textFrame.setSize(20,100);
        textFrame.add(textBox);
        textFrame.setFrameIcon(null);
        //((javax.swing.plaf.basic.BasicInternalFrameUI)textFrame.getUI()).setNorthPane(null);
        //textFrame.setBorder(null);
        textFrame.setVisible(false);
	}

    /**
    * Set the current selected polyline, change its color,
    * if there is a previous selected polyline restores its color,
    * enable/disable popup menu items if there is/isn't
    * a polyline selected.
    *
    * @param uid polyline unique identifier. Null uid unselect previous selection.
    */
    public void setSelected(String uid) {

        if (PolylineHandler.getInstance().polylineExists(selected)) {
            PolylineHandler.getInstance().setColor(selected, oriColor);
        }

        selected = uid;

        if (selected != null) {
            copy.setEnabled(true);
            cut.setEnabled(true);
            delete.setEnabled(true);
            hide.setEnabled(true);
			legends.setEnabled(true);
            oriColor = PolylineHandler.getInstance().setColor(selected, 12);
        } else {
            copy.setEnabled(false);
            cut.setEnabled(false);
            delete.setEnabled(false);
            hide.setEnabled(false);
			legends.setEnabled(false);
        }
    }

    /**
    * Returns selected polyline unique identifier.
    * @return selected polyline uid or null if there isn't any selected.
    */
    public String getSelected() {
        if (PolylineHandler.getInstance().polylineExists(selected)) {
            return selected;
        } else {
            setSelected(null);
            return null;
        }
    }

    /**
    * Set the figure uid wich the editor belongs.
    * @param uid Figure unique identifier.
    */
    public void setFigure(String uid) {
        figureUid = uid;
    }

    /**
    * Implements copy menu item action(Callback).
    */
    public void onClickCopy() {
        ScilabClipboard.getInstance().copy(getSelected());
        ScilabClipboard.getInstance().setCopiedColor(oriColor);
    }

    /**
    * Implements paste menu item action(Callback).
    */
    public void onClickPaste() {
        ScilabClipboard.getInstance().paste(figureUid, lastClick);
    }

    /**
    * Implements cut menu item action
    */
    public void onClickCut() {
        String s = getSelected();
        if (s != null) {
            setSelected(null);
            ScilabClipboard.getInstance().cut(s);
            ScilabClipboard.getInstance().setCopiedColor(oriColor);
        }
    }

    /**
    * Implements delete menu item action(Callback).
    */
    public void onClickDelete() {
        String toDelete = getSelected();
        if (toDelete != null) {
            setSelected(null);
            PolylineHandler.getInstance().delete(toDelete);
        }
    }

    /**
    * Implements clear menu item action(Callback).
    */
    public void onClickClear() {
        setSelected(null);
        String axesTo = AxesHandler.clickedAxes(figureUid, lastClick);
        if (axesTo != null) {
            PolylineHandler.getInstance().deleteAll(axesTo);
        }
    }

    /**
    * Implements hide menu item action(Callback).
    */
    public void onClickHide() {
        if (getSelected() != null) {
            PolylineHandler.getInstance().visible(selected, false);
            setSelected(null);
        }
    }

    /**
    * Implements unhide menu item action(Callback).
    */
    public void onClickUnhide() {
        PolylineHandler.getInstance().visible(figureUid, true);
    }

    /**
    * Implements clipboard copy menu item action(Callback).
    */
    public void onClickCCopy() {
        SystemClipboard.copyToSysClipboard(figureUid);
    }

    /**
    * Implements label insert action(Callback).
    */
    public void onClickLabel(int axis) {

        String axes = AxesHandler.clickedAxes(figureUid, lastClick);
        String text = LabelHandler.getLabelText(axes, axis);
        if (text != null) {
            textBox.setText(text);
        } else {
        textBox.setText("");
        }
        textFrame.setVisible(true);
        textBox.requestFocus();
        textFrame.setLocation(lastClick[0],lastClick[1]);
        textFrame.setTitle(Messages.gettext("Insert a text (Label)"));
        axisTo = axis;
        isLegend = false;
        myPanel.add(textFrame);
    }

    /**
    * Implements legend insert action(Callback).
    */
    public void onClickInsert() {

        String axes = AxesHandler.clickedAxes(figureUid, lastClick);
        String text = LegendHandler.getLegendText(axes, selected);
        if (text != null) {
            textBox.setText(text);
        } else {
            textBox.setText("");
        }
        textFrame.setVisible(true);
        textBox.requestFocus();
        textFrame.setLocation(lastClick[0],lastClick[1]);
        textFrame.setTitle(Messages.gettext("Insert a text (Legend)"));
        isLegend = true;
        myPanel.add(textFrame);
    }

    /**
    * Implements legend remove action(Callback).
    */
    public void onClickRemove() {

        String axesTo = AxesHandler.clickedAxes(figureUid, lastClick);
        LegendHandler.removeLegend(axesTo, selected);
    }

    /**
    * Implements textbox action that is used for label and legend(Callback).
    */
    public void setText() {

        String axesTo = AxesHandler.clickedAxes(figureUid, lastClick);
        if (isLegend) {
            LegendHandler.setLegend(axesTo, selected, textBox.getText());
        } else {
            String[] text = { textBox.getText() };
            LabelHandler.setLabel(axesTo, text, axisTo);
        }
    }
}

