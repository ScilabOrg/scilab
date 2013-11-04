/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2010-2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.gui.bridge.label;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.image.BufferedImage;

import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.JComponent;
import javax.swing.JEditorPane;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.SwingConstants;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;

import org.scilab.modules.console.utils.ScilabSpecialTextUtilities;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.label.SimpleLabel;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.WebBrowser;

/**
 * Swing implementation for Scilab Labels in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
@SuppressWarnings(value = { "serial" })
public class SwingScilabLabel extends JScrollPane implements SwingViewObject, SimpleLabel {

    private static final long serialVersionUID = 7177323379068859441L;

    private Integer uid;

    private JComponent label = new JLabel();

    private boolean isHtmlLabel = false;

    private String horizontalAlignment = "left"; /* Horizontal alignment property */

    private String verticalAlignment = "middle"; /* Vertical alignment property */

    private final JPanel alignmentPanel = new JPanel(); /* Used for alignment */

    private final GridBagLayout alignmentLayout = new GridBagLayout();

    private String labelText = ""; /* Used to save user given text */

    private static HyperlinkListener urlOpener = new HyperlinkListener() {
        public void hyperlinkUpdate(HyperlinkEvent event) {
            if (event.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
                WebBrowser.openUrl(event.getURL(), event.getDescription());
            }
        }
    };

    /**
     * Constructor
     */
    public SwingScilabLabel() {
        super();
        alignmentPanel.setLayout(alignmentLayout);
        alignmentPanel.add(label);
        getViewport().add(alignmentPanel);
        setBorder(BorderFactory.createEmptyBorder());
        setViewportBorder(BorderFactory.createEmptyBorder());
        setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        // Initialize display
        setAlignment();
    }

    /**
     * Apply a new font for the label.
     * @param font new font to use.
     */
    public void setFont(Font font) {
        super.setFont(font);
        if (label != null) {
            label.setFont(font);
        }
    }

    /**
     * To set the Foreground color of the element.
     * @param color the Color
     */
    public void setForeground(Color color) {
        super.setForeground(color);
        if (label != null) {
            label.setForeground(color);
        }
    }

    /**
     * To set the Background color of the element.
     * @param color the Color
     */
    public void setBackground(Color color) {
        super.setBackground(color);
        if (alignmentPanel != null) {
            alignmentPanel.setBackground(color);
        }
        if (label != null) {
            label.setBackground(color);
        }
    }

    /**
     * Draws a swing Scilab PushButton
     * @see org.scilab.modules.gui.uielement.UIElement#draw()
     */
    public void draw() {
        this.setVisible(true);
        this.doLayout();
    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab element
     * @return the dimensions of the element
     * @see org.scilab.modules.gui.uielement.UIElement#getDims()
     */
    public Size getDims() {
        return new Size(getWidth(), getHeight());
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab element
     * @return the position of the element
     * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
     */
    public Position getPosition() {
        return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab element
     * @param newSize the dimensions to set to the element
     * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        setSize(newSize.getWidth(), newSize.getHeight());
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab element
     * @param newPosition the position to set to the element
     * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
        Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
        setLocation(javaPosition.getX(), javaPosition.getY());
    }

    /**
     * Add a callback to the Label
     * @param callback the callback to set.
     */
    public void setCallback(CommonCallBack callback) {
        // Nothing to do...
    }

    /**
     * Setter for MenuBar
     * @param menuBarToAdd the MenuBar associated to the Label.
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        /* Unimplemented for Labels */
        throw new UnsupportedOperationException();
    }

    /**
     * Setter for ToolBar
     * @param toolBarToAdd the ToolBar associated to the Label.
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        /* Unimplemented for Labels */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for MenuBar
     * @return MenuBar: the MenuBar associated to the Label.
     */
    public MenuBar getMenuBar() {
        /* Unimplemented for Labels */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for ToolBar
     * @return ToolBar: the ToolBar associated to the Label.
     */
    public ToolBar getToolBar() {
        /* Unimplemented for Labels */
        throw new UnsupportedOperationException();
    }

    /**
     * Set the horizontal alignment for the Label text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        horizontalAlignment = alignment;
        setAlignment();
    }

    /**
     * Set the vertical alignment for the Label text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        verticalAlignment = alignment;
        setAlignment();

    }

    /**
     * Set the Relief of the Label
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        setBorder(ScilabRelief.getBorderFromRelief(reliefType));
    }

    /**
     * Destroy the Label
     */
    public void destroy() {
        ScilabSwingUtilities.removeFromParent(this);
    }

    /**
     * Setter for InfoBar
     * @param infoBarToAdd the InfoBar associated to the Label.
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        /* Unimplemented for Labels */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the Label.
     */
    public TextBox getInfoBar() {
        /* Unimplemented for Labels */
        throw new UnsupportedOperationException();
    }

    /**
     * get the text displayed in the label
     * @return the label
     * @see org.scilab.modules.gui.text.SimpleText#getText()
     */
    public String getText() {
        return labelText;
    }

    /**
     * Set the text displayed in the label
     * @param newText the text
     * @see org.scilab.modules.gui.text.SimpleText#setText(java.lang.String)
     */
    public void setText(String newText) {
        // Save the data given by the user so that it can be retrieved
        // (Java adds HTML tags in the getlabel().getText() returned value)
        labelText = newText;
        if (labelText.startsWith("<")) {
            if (!isHtmlLabel) {
                changeLabelType(!isHtmlLabel);
            }
            ((JEditorPane) label).setText(newText);
        } else {
            if (isHtmlLabel) {
                changeLabelType(!isHtmlLabel);
            }
            if (!ScilabSpecialTextUtilities.setText(label, newText)) {
                ((JLabel) label).setText(newText);
            }
        }
    }

    /**
     * Change Label type to switch between JLabel abd JEditorPane
     * JLabel is quicker on simple text
     * JEditorPane can enable HyperLinks
     * @param isHtmlLabel
     */
    private void changeLabelType(boolean isHtmlLabel) {
        this.isHtmlLabel = isHtmlLabel;
        Color bgColor = label.getBackground();
        Color fgColor = label.getForeground();
        Font font = label.getFont();

        alignmentPanel.remove(label);

        if (isHtmlLabel) {
            JTextPane newLabel = new JTextPane();
            newLabel.addHyperlinkListener(urlOpener);
            newLabel.setContentType("text/html");
            newLabel.setEditable(false);
            newLabel.setOpaque(false);
            newLlabel.setBorder(null);
            label = newLabel;
        } else {
            label = new JLabel();
        }

        label.setBackground(bgColor);
        label.setForeground(fgColor);
        label.setFont(font);
        alignmentPanel.add(label);
    }

    /**
     * Set alignment of the text component
     */
    private void setAlignment() {
        GridBagConstraints gbc = new GridBagConstraints();

        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.weightx = 1;
        gbc.weighty = 1;
        gbc.gridwidth = 1;
        gbc.gridheight = 1;

        switch (ScilabAlignment.toSwingAlignment(horizontalAlignment)) {
            case SwingConstants.LEFT:
                switch (ScilabAlignment.toSwingAlignment(verticalAlignment)) {
                    case SwingConstants.TOP:
                        gbc.anchor = GridBagConstraints.NORTHWEST;
                        break;
                    case SwingConstants.CENTER:
                        gbc.anchor = GridBagConstraints.WEST;
                        break;
                    default: // SwingConstants.BOTTOM
                        gbc.anchor = GridBagConstraints.SOUTHWEST;
                        break;
                }
                break;
            case SwingConstants.CENTER:
                switch (ScilabAlignment.toSwingAlignment(verticalAlignment)) {
                    case SwingConstants.TOP:
                        gbc.anchor = GridBagConstraints.NORTH;
                        break;
                    case SwingConstants.CENTER:
                        gbc.anchor = GridBagConstraints.CENTER;
                        break;
                    default: // SwingConstants.BOTTOM
                        gbc.anchor = GridBagConstraints.SOUTH;
                        break;
                }
                break;
            default: // SwingConstants.RIGHT
                switch (ScilabAlignment.toSwingAlignment(verticalAlignment)) {
                    case SwingConstants.TOP:
                        gbc.anchor = GridBagConstraints.NORTHEAST;
                        break;
                    case SwingConstants.CENTER:
                        gbc.anchor = GridBagConstraints.EAST;
                        break;
                    default: // SwingConstants.BOTTOM
                        gbc.anchor = GridBagConstraints.SOUTHEAST;
                        break;
                }
                break;
        }

        alignmentLayout.setConstraints(label, gbc);
        alignmentPanel.revalidate();
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
        SwingViewWidget.update(this, property, value);
    }

    /**
     * Inner class to handle the case where the label is in LaTeX or in MathML
     */
    public class ScilabJTextPane extends JTextPane {

        private int descent;
        private int fontSize;
        private String expression;
        private Icon icon;

        /**
         * Get the icon set in the textpane
         * @return the set icon
         */
        public Icon getIcon() {
            return this.icon;
        }

        /**
         * Sets the Icon of this textPane
         * @param icon to set
         */
        public void setIcon(Icon icon) {
            this.icon = icon;
            if (icon != null) {
                insertIcon(icon);
                int width = icon.getIconWidth();
                // We need to add the descent to avoid the vertical scrollbar
                int height = icon.getIconHeight() + descent;
                setSize(width, height);
                SwingScilabLabel.this.setDims(new Size(width, height));
            }
        }

        /**
         * @param font to use in this textPane
         */
        public void setFont(Font font) {
            super.setFont(font);
            descent = getDescent(font);
            if (font.getSize() != fontSize && expression != null) {
                try {
                    //If we have an icon, it is considered as one char
                    getDocument().remove(0, 1);
                } catch (BadLocationException e) { }
                ScilabSpecialTextUtilities.setText(this, expression);
                fontSize = font.getSize();
            }
        }

        /**
         * Sets the text of a Text Widget
         * @param newText the text to set to the Text Widget
         */
        public void setText(String newText) {
            Document doc = getDocument();
            try {
                doc.remove(0, doc.getLength());
            } catch (BadLocationException e) {
                System.err.println(e);
            }
            if (ScilabSpecialTextUtilities.setText(this, newText)) {
                expression = newText;
            } else {
                super.setText(newText);
            }
        }

        /**
         * @param font where to get the descent
         * @return the descent of the font
         */
        private int getDescent(Font font) {
            Graphics g = new BufferedImage(1, 1, BufferedImage.TYPE_INT_RGB).createGraphics();
            g.setFont(font);
            int d = (int) (g.getFontMetrics().getDescent() + 0.99f);
            g.dispose();
            return d;
        }
    }
}
