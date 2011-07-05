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

import java.awt.Dimension;
import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.regex.Pattern;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

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
	connect();
    }
    
    private void initPanel() {
	setLayout(new BorderLayout());
	textfield = new JTextField();
	textfield.setEditable(true);
	validate = new JButton(new CallBack(null) {
		public void callBack() {
		    stt.setFilter(getPattern());
		}
	    });
	validate.setIcon(VALIDATE);
	validate.setPreferredSize(new Dimension(22, 22));
	
	JPanel panelChecks = new JPanel(new FlowLayout(FlowLayout.LEFT, GAP, GAP));
	caseSensitive = new JCheckBox(UiDataMessages.CASESENSITIVE, isWindows);
	regexp = new JCheckBox(UiDataMessages.REGEXP, false);
	panelChecks.add(validate);
	panelChecks.add(caseSensitive);
	panelChecks.add(regexp);
	add(FILTER, BorderLayout.WEST);
	add(textfield, BorderLayout.CENTER);
	add(panelChecks, BorderLayout.EAST);
    }

    private void connect() {
	textfield.addKeyListener(new KeyAdapter() {
		public void keyTyped(KeyEvent e) {
		    if (e.getKeyCode() == KeyEvent.VK_ENTER) {
			stt.setFilter(getPattern());
		    }
		}
	    });

    }

    private Pattern getPattern() {
	String text = textfield.getText();
	if (text != null && !text.isEmpty()) {
            if (!regexp.isSelected()) {
                text = "\\Q" + text.replace(".", "\\.").replace("*", ".*").replace("?", ".?") + "\\E";
            }

            if (!caseSensitive.isSelected()) {
                text = "(?i)" + text;
            }

            if (regexp.isSelected()) {
                text = "(?m)" + text;
            }

            return Pattern.compile(text);
        }
	
	return null;
    }
}
