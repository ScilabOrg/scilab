/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.filebrowser;

import java.awt.Color;
import java.awt.ContainerOrderFocusTraversalPolicy;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.KeyboardFocusManager;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.Collections;
import java.util.regex.Pattern;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.border.EmptyBorder;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

import org.scilab.modules.core.Scilab;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * A panel with file filter and the combo to set the cwd.
 * @author Calixte DENIZET
 */
public class ScilabFileSelectorFilter extends JPanel {

    private static boolean isWindows = Scilab.isWindowsPlateform();
    private static final int GAP = 3;
    private static final Icon VALIDATE = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/filter.png");

    private MyJTextField textfield;
    private SwingScilabTreeTable stt;
    private JCheckBox caseSensitive;
    private JCheckBox regexp;
    private JButton validate;
    private ScilabFileSelectorComboBox comboPath;

    /**
     * Default constructor
     * @param stt the SwingScilabTree containing the cwd
     * @param comboPath the combobox where to set the path
     */
    public ScilabFileSelectorFilter(SwingScilabTreeTable stt, ScilabFileSelectorComboBox comboPath) {
        super();
        this.stt = stt;
        this.comboPath = comboPath;
        initPanel();
    }

    /**
     * Init the panel
     */
    private void initPanel() {
        setBorder(new EmptyBorder(0, 0, GAP, 0));
        setLayout(new GridBagLayout());
        textfield = new MyJTextField();
        setFocusCycleRoot(true);
        setFocusTraversalPolicy(new ContainerOrderFocusTraversalPolicy());

        validate = new JButton(new CallBack(null) {
                public void callBack() {
                    stt.setFilter(getPattern());
                }
            });
        validate.setIcon(VALIDATE);

        caseSensitive = new JCheckBox(UiDataMessages.CASESENSITIVE, !isWindows);
        regexp = new JCheckBox(UiDataMessages.REGEXP, false);

        GridBagConstraints gbc = new GridBagConstraints();

        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.gridwidth = gbc.gridheight = 1;
        gbc.weightx = 1;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = gbc.LINE_START;
        add(textfield, gbc);

        gbc.gridx = 1;
        gbc.gridwidth = gbc.gridheight = 1;
        gbc.weightx = 0;
        gbc.anchor = gbc.LINE_START;
        add(validate, gbc);

        gbc.gridx = 2;
        gbc.gridwidth = gbc.gridheight = 1;
        gbc.weightx = 0;
        gbc.anchor = gbc.LINE_START;
        add(caseSensitive, gbc);

        gbc.gridx = 3;
        gbc.gridwidth = gbc.gridheight = 1;
        gbc.weightx = 0;
        gbc.anchor = gbc.LINE_START;
        add(regexp, gbc);

        gbc.gridx = 0;
        gbc.gridy = 1;
        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbc.gridheight = 1;
        gbc.weightx = 1;
        gbc.anchor = gbc.LINE_START;
        add(comboPath, gbc);
    }

    /**
     * Test the pattern and change the color of the text in textfield if the pattern is invalid
     */
    private void testPattern() {
        if (regexp.isSelected()) {
            if (getPattern() == null) {
                textfield.setForeground(Color.RED);
            } else {
                textfield.setForeground(textfield.defaultFg);
            }
        }
    }

    /**
     * Get the pattern according to the textfield content
     * @return the pattern
     */
    private Pattern getPattern() {
        String text = textfield.getText();
        if (text != null && !text.isEmpty()) {
            if (!regexp.isSelected()) {
                text = text.replaceAll("\\*", "\\\\E.*\\\\Q").replaceAll("\\?", "\\\\E.?\\\\Q");
                text = "\\Q" + text + "\\E";
            }
            if (!caseSensitive.isSelected()) {
                text = "(?i)" + text;
            }

            if (regexp.isSelected()) {
                text = "(?m)" + text;
            }

            try {
                return Pattern.compile(text);
            } catch (Exception e) { }
        }

        return null;
    }

    /**
     * Inner class to have the possibility to add a default text in the textfield but in this case the textfield is empty !
     * It is a workaround for a GTK bug which avoids me to paint directly in the field.
     */
    private class MyJTextField extends JTextField implements DocumentListener, FocusListener {

        boolean isEmpty = true;
        Color defaultFg;

	/**
	 * {@inheritDoc}
	 */
	MyJTextField() {
            super();
            defaultFg = getForeground();
            setEditable(true);
            addFocusListener(this);
            getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0), "ENTER");
            getActionMap().put("ENTER", new CallBack(null) {
                    public void callBack() {
                        stt.setFilter(getPattern());
                    }
                });
            toggleContents();
        }

	/**
	 * Toggle the contents
	 */
        private void toggleContents() {
            if (isEmpty) {
                setForeground(Color.LIGHT_GRAY);
                setText(UiDataMessages.FILEFILTER);
            }
        }
	
	/**
	 * {@inheritDoc}
	 */
        public String getText() {
            if (isEmpty) {
                return "";
            }

            return super.getText();
        }

        /**
	 * {@inheritDoc}
	 */
        public void focusGained(FocusEvent e) {
            getDocument().addDocumentListener(this);
            if (isEmpty) {
                setForeground(defaultFg);
                setText("");
            }
        }

        /**
	 * {@inheritDoc}
	 */
        public void focusLost(FocusEvent e) {
            getDocument().removeDocumentListener(this);
            isEmpty = super.getText().isEmpty();
            toggleContents();
        }

        /**
	 * {@inheritDoc}
	 */
        public void insertUpdate(DocumentEvent e) {
            isEmpty = super.getText().isEmpty();
            testPattern();
        }

        /**
	 * {@inheritDoc}
	 */
        public void removeUpdate(DocumentEvent e) {
            isEmpty = super.getText().isEmpty();
            testPattern();
        }

        /**
	 * {@inheritDoc}
	 */
        public void changedUpdate(DocumentEvent e) { }
    }
}
