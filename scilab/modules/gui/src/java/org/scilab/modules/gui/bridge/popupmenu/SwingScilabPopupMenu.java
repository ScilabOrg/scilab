/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.bridge.popupmenu;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING_COLNB__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;

import java.awt.Color;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.ListCellRenderer;
import javax.swing.UIManager;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.popupmenu.SimplePopupMenu;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.ColorBox;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.SwingScilabColorItem;
import org.scilab.modules.gui.utils.SwingScilabTextItem;

/**
 * Swing implementation for Scilab PopupMenu in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabPopupMenu extends JComboBox implements SwingViewObject, SimplePopupMenu {

    private static final long serialVersionUID = -4366581303317502544L;

    private Integer uid;

    private CommonCallBack callback;

    private ActionListener defaultActionListener;

    private boolean colorBox = false;

    private ListCellRenderer defaultRenderer = null;
    private ListCellRenderer textRenderer = null;
    private ListCellRenderer colorRenderer = null;

    /**
     * Constructor
     */
    public SwingScilabPopupMenu() {
        super();
        defaultRenderer = getRenderer();
        textRenderer = new ListCellRenderer() {
            public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
                JLabel label = (JLabel) defaultRenderer.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);
                if (value instanceof SwingScilabTextItem) {
                    label.setText(value.toString());
                    label.setIcon(null);
                }
                return label;
            }
        };

        setRenderer(textRenderer);
        /* Bug 3635 fixed: allow arrow keys to browse items */
        putClientProperty("JComboBox.isTableCellEditor", Boolean.TRUE);
        defaultActionListener = new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                Double[] scilabIndices = new Double[1];
                scilabIndices[0] = (double) getUserSelectedIndex();
                GraphicController.getController().setProperty(uid, __GO_UI_VALUE__, scilabIndices);
                if (callback != null) {
                    callback.actionPerformed(null);
                }
            }
        };
        addActionListener(defaultActionListener);
    }

    /**
     * Draws a swing Scilab tab
     * @see org.scilab.modules.gui.uielement.UIElement#draw()
     */
    public void draw() {
        this.setVisible(true);
    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab tab
     * @return the dimensions of the tab
     * @see org.scilab.modules.gui.uielement.UIElement#getDims()
     */
    public Size getDims() {
        return new Size(getWidth(), getHeight());
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
     * @return the position of the tab
     * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
     */
    public Position getPosition() {
        return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab tab
     * @param newSize the dimensions we want to set to the tab
     * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        setSize(newSize.getWidth(), newSize.getHeight());
        doLayout(); /* Needed !! because PopupMenu is badly drawn else */
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab tab
     * @param newPosition the position we want to set to the tab
     * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
        Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
        setLocation(javaPosition.getX(), javaPosition.getY());
    }

    /**
     * Add a callback to the PopupMenu
     * @param callback the callback to set.
     */
    public void setCallback(CommonCallBack callback) {
        this.callback = callback;
    }

    /**
     * Setter for MenuBar
     * @param menuBarToAdd the MenuBar associated to the Tab.
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        /* Unimplemented for PopupMenus */
        throw new UnsupportedOperationException();
    }

    /**
     * Setter for ToolBar
     * @param toolBarToAdd the ToolBar associated to the Tab.
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        /* Unimplemented for PopupMenus */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for MenuBar
     * @return MenuBar: the MenuBar associated to the Tab.
     */
    public MenuBar getMenuBar() {
        /* Unimplemented for PopupMenus */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for ToolBar
     * @return ToolBar: the ToolBar associated to the Tab.
     */
    public ToolBar getToolBar() {
        /* Unimplemented for PopupMenus */
        throw new UnsupportedOperationException();
    }

    /**
     * Get the text if the PopupMenu items
     * @return the items
     * @see org.scilab.modules.gui.widget.Widget#getText()
     */
    public String getText() {
        /* Unimplemented for PopupMenus */
        throw new UnsupportedOperationException();
    }

    /**
     * Set the text of the PopupMenu items
     * @param text the text of the items
     * @see org.scilab.modules.gui.widget.Widget#setText(java.lang.String)
     */
    public void setText(String text) {
        /* Unimplemented for PopupMenus */
        throw new UnsupportedOperationException();
    }

    /**
     * Set the horizontal alignment for the PopupMenu text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        // Nothing to do here
    }

    /**
     * Set the vertical alignment for the PopupMenu text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        // Nothing to do here
    }

    /**
     * Set the selected index of the PopupMenu
     * @param index the index of the item to be selected
     */
    public void setUserSelectedIndex(int index) {
        /* Remove the listener to avoid the callback to be executed */
        removeActionListener(defaultActionListener);

        for (int i = 0; i < getItemCount(); i++) {
            // Scilab indices in Value begin at 1 and Java indices begin at 0
            if (i == (index - 1)) {
                getModel().setSelectedItem(getItemAt(i));
            }
        }

        /* Put back the listener */
        addActionListener(defaultActionListener);
    }

    /**
     * Get the selected index of the PopupMenu
     * @return the index of the item selected
     */
    public int getUserSelectedIndex() {
        Object itemSelected = getModel().getSelectedItem();
        for (int i = 0; i < getItemCount(); i++) {
            if (itemSelected.equals(getItemAt(i))) {
                // Scilab indices in Value begin at 1 and Java indices begin at 0
                return i + 1;
            }
        }
        return -1;
    }

    /**
     * Get the text of all the PopupMenu items
     * @return the text items
     */
    public String[] getAllItemsText() {
        String[] retValue = new String[getItemCount()];
        for (int i = 0; i < getItemCount(); i++) {
            retValue[i] = getItemAt(i).toString();
        }
        return retValue;

    }

    /**
     * Get the number of items in the PopupMenu
     * @return the number of items
     */
    public int getNumberOfItems() {
        return getItemCount();
    }

    /**
     * Set the text of the PopupMenu items
     * @param text the text of the items
     */
    public void setText(String[] text) {

        // check numbers of columns
        GraphicController controller = GraphicController.getController();
        Integer nbCol = (Integer)controller.getProperty(getId(), __GO_UI_STRING_COLNB__);

        colorBox = false;
        if (nbCol == 2) {
            //combocolor ?
            colorBox = true;

            if (colorRenderer == null) {
                colorRenderer = new ListCellRenderer() {
                    public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
                        JLabel label = (JLabel) defaultRenderer.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);

                        if (value instanceof SwingScilabColorItem) {
                            SwingScilabColorItem item = (SwingScilabColorItem)value;
                            String text = item.toString();
                            label.setText(text);
                            label.setIcon(ColorBox.createColorBox(16, 16, item.getColor()));
                        }
                        return label;
                    }
                };

            }

            setRenderer(colorRenderer);

            /* Remove the listener to avoid the callback to be executed */
            if (callback != null) {
                removeActionListener(callback);
            }

            /* Clear previous items */
            removeAllItems();

            int colorOffset = text.length / 2;
            for (int i = 0 ; i < colorOffset ; i++) {
                try {
                    Color color = Color.decode(text[colorOffset + i]);
                    //add item in combobox
                    addItem(new SwingScilabColorItem(text[i], color));
                } catch (NumberFormatException e) {
                    removeAllItems();
                    colorBox = false;
                    break;
                }
            }

            /* Remove the listener to avoid the callback to be executed */
            if (callback != null) {
                addActionListener(callback);
            }
        }

        //default case or colorBox failed
        if (colorBox == false) {
            setRenderer(textRenderer);
            /* Do we need to update the strings */
            /* Test performed to avoid loops when the model is updated from here */
            boolean updateNeeded = false;
            String[] previousText = getAllItemsText();
            if (previousText.length != text.length) {
                updateNeeded = true;
            } else {
                for (int k = 0; k < text.length; k++) {
                    if (!text[k].equals(previousText[k])) {
                        updateNeeded = true;
                        break;
                    }
                }
            }
            if (!updateNeeded) {
                return;
            }

            /* Clear previous items */
            removeAllItems();

            if (text.length == 1 & text[0].length() == 0) {
                /* Clear the popup items */
                return;
            } else {
                for (int i = 0; i < text.length; i++) {
                    addItem(new SwingScilabTextItem(text[i]));
                }
            }

            /* Remove the listener to avoid the callback to be executed */
            if (callback != null) {
                addActionListener(callback);
            }
        } else {
        }
    }

    /**
     * Set the Relief of the PopupMenu
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        if (reliefType.equals("") == false) {
            setBorder(ScilabRelief.getBorderFromRelief(reliefType));
        }
    }

    /**
     * Destroy the PopupMenu
     */
    public void destroy() {
        ScilabSwingUtilities.removeFromParent(this);
    }

    /**
     * Setter for InfoBar
     * @param infoBarToAdd the InfoBar associated to the PopupMenu.
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        /* Unimplemented for PopupMenus */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the PopupMenu.
     */
    public TextBox getInfoBar() {
        /* Unimplemented for PopupMenus */
        throw new UnsupportedOperationException();
    }


    /**
     * Set the UID
     * @param id the UID
     */
    public void setId(Integer id) {
        uid = id;
    }

    /**
     * Get the UID
     * @return the UID
     */
    public Integer getId() {
        return uid;
    }

    /**
     * Generic update method
     * @param property property name
     * @param value property value
     */
    public void update(int property, Object value) {
        switch (property) {
            case __GO_UI_STRING__: {
                setText((String[]) value);
                break;
            }
            case __GO_UI_VALUE__: {
                Double[] doubleValue = ((Double[]) value);
                if (doubleValue.length == 0) {
                    return;
                }

                int[] intValue = new int[doubleValue.length];
                for (int k = 0; k < doubleValue.length; k++) {
                    intValue[k] = doubleValue[k].intValue();
                }

                // Update selected items in the popupmenu
                setUserSelectedIndex(intValue[0]);
                break;
            }
            default: {
                SwingViewWidget.update(this, property, value);
                break;
            }
        }
    }

    public void resetBackground() {
        Color color = (Color) UIManager.getLookAndFeelDefaults().get("ComboBox.background");
        if (color != null) {
            setBackground(color);
        }
    }
}
