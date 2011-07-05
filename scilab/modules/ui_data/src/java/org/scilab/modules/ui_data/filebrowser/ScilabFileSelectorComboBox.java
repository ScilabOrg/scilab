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

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import javax.swing.Icon;
import javax.swing.ImageIcon;

/**
 *
 * @author Calixte DENIZET
 */
public class ScilabFileSelectorComboBox extends JComboBox {

    private int lastSize = -1;
    private String old = "";

    public ScilabFileSelectorComboBox(String baseDir, final Action validation) {
        super();
        updateCombo(baseDir);
        getEditor().getEditorComponent().addKeyListener(new KeyListener() {

                public void keyReleased(KeyEvent e) {
                    int code = e.getKeyCode();
                    String name = (String) getEditor().getItem();
                    switch (code) {
                    case KeyEvent.VK_ENTER :
                        if (getSelectedIndex() == -1 && getItemCount() > 0) {
                            setSelectedIndex(0);
                        }
                        hidePopup();
                        validation.action();
                        break;
                    case KeyEvent.VK_ESCAPE :
                        hidePopup();
                        break;
                    case KeyEvent.VK_TAB :
                        if (getSelectedIndex() == -1 && getItemCount() == 1) {
                            setSelectedIndex(0);
                        }
                        updateCombo(name);
                        break;
                    default :
                        if (code != KeyEvent.VK_UP && code != KeyEvent.VK_DOWN) {
                            updateCombo(name);
                            setPopupVisible(true);
                        }
                    }
                }

                public void keyPressed(KeyEvent arg0) {
                    old = (String) comboComplete.getEditor().getItem();
                }

                public void keyTyped(KeyEvent arg0) { }
            });
    }

    private static final String getTruePath(String path) {
        if (path != null) {
            if (path.startsWith("SCI\\") || path.startsWith("SCI/") || path.equals("SCI")) {
                return path.replaceFirst("SCI", ScilabConstants.SCI.getAbsolutePath());
            }
            if (path.startsWith("~\\") || path.startsWith("~/") || path.equals("~")) {
                return path.replaceFirst("~", ScilabConstants.USERHOME);
            }
            if (path.startsWith("TMPDIR\\") || path.startsWith("TMPDIR/") || path.equals("TMPDIR")) {
                return path.replaceFirst("TMPDIR", ScilabConstants.TMPDIR.getAbsolutePath());
            }
        }

        return path;
    }

    private static final File getFileDirectory(File file) {
	if (file.isDirectory()) {
	    return file;
	}
	
	File parent = file.getParent();
	if (parent.isDirectory()) {
	    return parent;
	}

	return null;
    }

    /**
     * Update the comboBox with the possible completion of the name
     * entered in the comboBox.
     * @param old the previous name
     */
    private void updateCombo(String name) {
	name = getTruePath(name);
        if (name != null && !name.equals(old)) {
            File file = new File(name);
            File parent = getFileDirectory(file);

            Set<String> set = null;

            if (parent != null) {
		set = new TreeSet<String>(Arrays.asList(parent.listFiles()));
            } else {
                set = new TreeSet<String>(Arrays.asList(File.listRoots()));
            }

            if (set != null) {
                String bound = "";
                int len = name.length();
                if (len == 1) {
                    bound = new String(new char[]{(char) (name.charAt(0) + 1)});
                } else {
                    bound = name.substring(0, len - 1);
                    bound += (char) (name.charAt(len - 1) + 1);
                }
                set = set.subSet(name, true, bound, false);

                if (set != null && set.size() != lastSize) {
                    removeAllItems();
                    for (String f : set) {
                        addItem(f);
                    }
                    lastSize = set.size();
                    setPopupVisible(false);
                }
            }

            getEditor().setItem(name);
            JTextComponent c = (JTextComponent) getEditor().getEditorComponent();
            c.setSelectionStart(c.getSelectionEnd());
        }
    }
}