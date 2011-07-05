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

import java.awt.AWTEvent;
import java.awt.Toolkit;
import java.awt.event.AWTEventListener;
import java.io.File;
import java.io.FileFilter;
import java.util.TreeSet;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JRootPane;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.BadLocationException;
import javax.swing.text.JTextComponent;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.gui.events.callback.CallBack;

/**
 *
 * @author Calixte DENIZET
 */
public class ScilabFileSelectorComboBox extends JComboBox implements DocumentListener {

    private static final FileFilter DIRFILTER = new FileFilter() {
            public boolean accept(File f) {
                return f.isDirectory();
            }
        };

    private int lastSize = -1;
    private String oldName = "";
    private TreeSet<String> currentSet;
    private JTextComponent textComponent;
    private boolean disableUpdateCombo;
    private boolean disableShowPopup;
    private boolean isValidate;
    private CallBack validation;
    private ScilabFileSelectorButtons buttons;

    /**
     * Default constructor
     * @param baseDir the base directory to open
     * @param validation the callback to execute when the user valids the directory
     */
    public ScilabFileSelectorComboBox(String baseDir) {
        super();
        textComponent = (JTextComponent) getEditor().getEditorComponent();
        textComponent.getDocument().addDocumentListener(this);

        getEditor().getEditorComponent().addKeyListener(new KeyListener() {
                public void keyReleased(KeyEvent e) {
                    int code = e.getKeyCode();
                    if (code != KeyEvent.VK_ESCAPE && code != KeyEvent.VK_ENTER) {
                        showPopup();
                    } else if (code == KeyEvent.VK_ENTER) {
                        if (!isValidate) {
                            updateComboBoxLater();
                            if (!currentSet.isEmpty()) {
                                showPopup();
                            }
                        } else {
                            isValidate = false;
                        }
                    }
                }

                public void keyPressed(KeyEvent e) {
                    int code = e.getKeyCode();
                    if (code == KeyEvent.VK_DOWN || code == KeyEvent.VK_KP_DOWN || code == KeyEvent.VK_PAGE_DOWN
                        || code == KeyEvent.VK_UP || code == KeyEvent.VK_KP_UP || code == KeyEvent.VK_PAGE_UP
                        || code == KeyEvent.VK_END || code == KeyEvent.VK_BEGIN) {
                        disableUpdateCombo = true;
                    } else {
                        disableUpdateCombo = false;
                    }
                }

                public void keyTyped(KeyEvent e) { }
            });

        /* Workaround to override the default behaviour of TAB key */
        Toolkit.getDefaultToolkit().addAWTEventListener(new AWTEventListener() {
                public void eventDispatched(AWTEvent e) {
                    KeyEvent ke = (KeyEvent) e;
                    if (ke.getKeyCode() == KeyEvent.VK_TAB && ke.getID() == KeyEvent.KEY_RELEASED && ke.getSource() == textComponent) {
                        updateComboBoxLater();
                        String cp = getCommonPart();
                        getEditor().setItem(cp);
                        showPopup();
                        ke.consume();
                    }
                }
            }, AWTEvent.KEY_EVENT_MASK);
        
	setEditable(true);
        getEditor().setItem(baseDir);
    }

    public void setAction(CallBack validation) {
	this.validation = validation;
	/* Workaround to have an action executed when the popup is not visible and ENTER was typed 
	 * The idea comes from the source of BasicComboBoxUI.java */
        Thread t = new Thread(new Runnable() {
                public void run() {
                    while (SwingUtilities.getRootPane(ScilabFileSelectorComboBox.this) == null) {
                        try {
                            Thread.sleep(10);
                        } catch (InterruptedException e) { }
                    }
                    JRootPane root = SwingUtilities.getRootPane(ScilabFileSelectorComboBox.this);
                    root.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0), "ENTER");
                    root.getActionMap().put("ENTER", new CallBack(null) {
                            public void callBack() {
                                isValidate = true;
				//buttons.addPathInHistory(getText());
                                ScilabFileSelectorComboBox.this.validation.callBack();
                            }
                        });
                }
            });
	
        try {
            t.start();
        } catch (Exception e) { }
    }

    /**
     * Set the navigation buttons
     * @param buttons the buttons
     */
    public void setButtons(ScilabFileSelectorButtons buttons) {
	this.buttons = buttons;
    }

    /**
     * Set the base directory and validate
     * @param path the path to set
     */
    public void setBaseDirAndValidate(String path) {
	disableUpdateCombo = true;
        getEditor().setItem(path);
	disableUpdateCombo = false;
	validation.callBack();
    }

    /**
     * @return the base directory
     */
    public void setBaseDir(String baseDir) {
	if (!baseDir.endsWith(File.separator)) {
	    baseDir += File.separator;
	}
	disableShowPopup = true;
	textComponent.setText(baseDir);
        //getEditor().setItem(baseDir);
	disableShowPopup = false;
    }

    /**
     * @return the base directory
     */
    public String getBaseDir() {
	return getTruePath(getText());
    }

    /**
     * {@inheritDoc}
     */
    public void changedUpdate(DocumentEvent e) { }

    /**
     * {@inheritDoc}
     */
    public void insertUpdate(DocumentEvent e) {
        updateComboBox();
    }

    /**
     * {@inheritDoc}
     */
    public void removeUpdate(DocumentEvent e) {
        updateComboBox();
    }

    /**
     * @return the text in the textfield
     */
    private String getText() {
        int len = textComponent.getDocument().getLength();
        try {
            return textComponent.getDocument().getText(0, len);
        } catch (BadLocationException e) {
            return "";
        }
    }

    /**
     * Get the path where the jokers such as ~, SCI or TMPDIR are replaced by their values
     * @param path the path
     * @return the true path
     */
    private static final String getTruePath(String path) {
        if (path != null) {
	    if (path.startsWith("SCI\\") || path.startsWith("SCI/") || path.startsWith("SCI")) {
		return path.replaceFirst("SCI", ScilabConstants.SCI.getAbsolutePath());
            }
            if (path.startsWith("~\\") || path.startsWith("~/") || path.startsWith("~")) {
                return path.replaceFirst("~", ScilabConstants.USERHOME);
            }
            if (path.startsWith("TMPDIR\\") || path.startsWith("TMPDIR/") || path.startsWith("TMPDIR")) {
		return path.replaceFirst("TMPDIR", ScilabConstants.TMPDIR.getAbsolutePath());
	    }
        }

        return path;
    }

    /**
     * Get the path where the jokers such as ~, SCI or TMPDIR are replaced by their values
     * @param path the path
     * @param joker the joker
     * @return the true path
     */
    private static final String getTruePath(String path, String joker) {
        if (path != null && joker != null) {
            if (joker.equals("SCI")) {
                return path.replaceFirst("SCI", ScilabConstants.SCI.getAbsolutePath());
            }
            if (joker.equals("~")) {
                return path.replaceFirst("~", ScilabConstants.USERHOME);
            }
            if (joker.equals("TMPDIR")) {
                return path.replaceFirst("TMPDIR", ScilabConstants.TMPDIR.getAbsolutePath());
            }
        }

        return path;
    }

    /**
     * Get the joker, if one exists, in the path. Jokers can be ~, SCI or TMPDIR
     * @param path the path to analyze
     * @return the joker
     */
    private static final String getJoker(String path) {
        if (path != null) {
            if (path.startsWith("SCI\\") || path.startsWith("SCI/")) {
                return "SCI";
            }
            if (path.startsWith("~\\") || path.startsWith("~/")) {
                return "~";
            }
            if (path.startsWith("TMPDIR\\") || path.startsWith("TMPDIR/")) {
                return "TMPDIR";
            }
        }

        return null;
    }

    /**
     * Get a path where the value of SCI is replaced by the string "SCI" (or ~, TMPDIR)
     * @param path the path
     * @param joker the joker
     * @return the fake path
     */
    private static final String getFakePath(String path, String joker) {
        if (path != null && joker != null) {
            if (joker.equals("SCI")) {
                return path.replaceFirst(ScilabConstants.SCI.getAbsolutePath(), "SCI");
            }
            if (joker.equals("~")) {
                return path.replaceFirst(ScilabConstants.USERHOME, "~");
            }
            if (joker.equals("TMPDIR")) {
                return path.replaceFirst(ScilabConstants.TMPDIR.getAbsolutePath(), "TMPDIR");
            }
        }

        return path;
    }

    /**
     * Get the directory where to list the file
     * @param name the directory name
     * @return the directory
     */
    private static final File getFileDirectory(String name) {
        File file = new File(name);

        if (name.endsWith(File.separator) && file.isDirectory()) {
            return file;
        }

        File parent = file.getParentFile();
        if (parent != null && parent.isDirectory()) {
            return parent;
        }

        return null;
    }

    /**
     * Get the common part of the strings present in currentSet (in fine, the combobox content list)
     * @return the common part
     */
    private String getCommonPart() {
        if (currentSet != null && !currentSet.isEmpty()) {
            if (currentSet.size() == 1) {
                return currentSet.first();
            }

            String parent = new File(currentSet.first()).getParent();
            int lenParent = 0;
            if (parent != null) {
                lenParent = parent.length();
            }

            int min = Integer.MAX_VALUE;
            char[][] strs = new char[currentSet.size()][];
            int i = 0;

            for (String s : currentSet) {
                strs[i] = s.substring(lenParent).toCharArray();
                if (strs[i].length < min) {
                    min = strs[i].length;
                }
                i++;
            }

            /* Now we get the common part */
            int j = strs.length;
            for (i = 0; i < min && j == strs.length; i++) {
                char c = strs[0][i];
                j = 1;
                for (;j < strs.length && strs[j][i] == c; j++);
            }

            if (lenParent != 0) {
                return parent + new String(strs[0], 0, i - 1);
            }
            return new String(strs[0], 0, i - 1);
        }

        return getText();
    }

    /**
     * Update the combobox. Since this function is called from the DocumentListener and methods in JComboBox change the JTextComponent content
     * (so the listener would try to write under a writeLock()... see DocumentListener javadoc), the update is called outside the main thread.
     */
    private void updateComboBox() {
	final boolean b = disableShowPopup;
        SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    textComponent.getDocument().removeDocumentListener(ScilabFileSelectorComboBox.this);
                    if (!disableUpdateCombo) {
                        updateComboBoxLater(b);
                    }
                    textComponent.getDocument().addDocumentListener(ScilabFileSelectorComboBox.this);
                }
            });
    }

    /**
     * Update the comboBox with the possible completion of the name
     * entered in the comboBox.
     */
    private void updateComboBoxLater() {
	updateComboBoxLater(false);
    }

    /**
     * Update the comboBox with the possible completion of the name
     * entered in the comboBox.
     */
    private void updateComboBoxLater(final boolean hidepopup) {
        String trueName = getText();
        if (trueName != null && !trueName.equals(oldName)) {
            String joker = getJoker(trueName);
            String name = getTruePath(trueName, joker);
            oldName = trueName;

            TreeSet<String> set = null;
            File[] files;

            if (name.isEmpty()) {
                files = File.listRoots();
            } else {
                File parent = getFileDirectory(name);

                if (parent != null) {
                    files = parent.listFiles(DIRFILTER);
                } else {
                    files = File.listRoots();
                }
            }

            set = new TreeSet<String>();
            for (File f : files) {
                String extra = File.separator;
                if (f.getParent() == null) {
                    extra = "";
                }
                set.add(getFakePath(f.getAbsolutePath(), joker) + extra);
            }

            if (set != null) {
                String bound = "";
                int len = trueName.length();
                if (len != 0) {
                    if (len == 1) {
                        bound = new String(new char[]{(char) (trueName.charAt(0) + 1)});
                    } else {
                        bound = trueName.substring(0, len - 1);
                        bound += (char) (trueName.charAt(len - 1) + 1);
                    }
                    currentSet = (TreeSet) set.subSet(trueName, true, bound, false);
                } else {
                    currentSet = set;
                }

                if (currentSet != null && currentSet.size() != lastSize) {
                    hidePopup();
                    removeAllItems();
                    for (String f : currentSet) {
                        addItem(f);
		    }
                    lastSize = currentSet.size();
                }
            }

            getEditor().setItem(trueName);
            textComponent.setSelectionStart(textComponent.getSelectionEnd());

	    if (isShowing()) {System.out.println(hidepopup);
		setPopupVisible(!hidepopup);
	    }
	}
    }
}
