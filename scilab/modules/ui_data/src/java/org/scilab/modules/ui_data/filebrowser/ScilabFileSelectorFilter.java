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
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.KeyboardFocusManager;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
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

import org.scilab.modules.core.Scilab;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 *
 * @author Calixte DENIZET
 */
public class ScilabFileSelectorFilter extends JPanel {
    
    private static boolean isWindows = Scilab.isWindowsPlateform();
    private static final int GAP = 5;
    private static final JLabel FILTER = new JLabel(UiDataMessages.FILEFILTER + ": ");
    private static final Icon VALIDATE = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/system-search.png");
    
    private JTextField textfield;
    private SwingScilabTreeTable stt;
    private JCheckBox caseSensitive;
    private JCheckBox regexp;
    private JButton validate;

    public ScilabFileSelectorFilter(SwingScilabTreeTable stt) {
	super();
	this.stt = stt;
	initPanel();
    }
    
    private void initPanel() {
	setLayout(new GridBagLayout());
	textfield = new JTextField();
	textfield.setEditable(true);
	textfield.setFocusTraversalKeys(KeyboardFocusManager.FORWARD_TRAVERSAL_KEYS, Collections.EMPTY_SET);// TODO: virer ca pr mettre un vrai focus manager
	textfield.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_TAB, 0), "TAB");
	textfield.getActionMap().put("TAB", new CallBack(null) {
		public void callBack() {
		    validate.requestFocus();
		}
	    });
	textfield.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0), "ENTER");
	textfield.getActionMap().put("ENTER", new CallBack(null) {
		public void callBack() {
		    stt.setFilter(getPattern());
		}
	    });
	textfield.addFocusListener(new FocusAdapter() {
		public void focusGained(FocusEvent e) {
		    textfield.setForeground(Color.BLACK);
		}
	    });

	validate = new JButton(new CallBack(null) {
		public void callBack() {
		    stt.setFilter(getPattern());
		}
	    });
	validate.setIcon(VALIDATE);
	
	caseSensitive = new JCheckBox(UiDataMessages.CASESENSITIVE, !isWindows);
	regexp = new JCheckBox(UiDataMessages.REGEXP, false);

	GridBagConstraints gbc = new GridBagConstraints();
	gbc.gridx = gbc.gridy = 0;
	gbc.gridwidth = gbc.gridheight = 1;
        gbc.weightx = gbc.weighty = 0;
        gbc.anchor = gbc.LINE_START;
	add(FILTER, gbc);

	gbc.gridx = 1;
	gbc.gridwidth = gbc.gridheight = 1;
        gbc.weightx = 1;
	gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = gbc.LINE_START;
	add(textfield, gbc);

	gbc.gridx = 2;
	gbc.gridwidth = gbc.gridheight = 1;
        gbc.weightx = 0;
        gbc.anchor = gbc.LINE_START;
	add(validate, gbc);

	gbc.gridx = 3;
	gbc.gridwidth = gbc.gridheight = 1;
        gbc.weightx = 0;
        gbc.anchor = gbc.LINE_START;
	add(caseSensitive, gbc);

	gbc.gridx = 4;
	gbc.gridwidth = gbc.gridheight = 1;
        gbc.weightx = 0;
        gbc.anchor = gbc.LINE_START;
	add(regexp, gbc);
    }

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
	    } catch (Exception e) {
		textfield.setForeground(Color.RED);
		validate.requestFocus();
	    }
	}
	
	return null;
    }
}
