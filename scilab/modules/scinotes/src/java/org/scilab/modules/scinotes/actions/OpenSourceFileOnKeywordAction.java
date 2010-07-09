/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import java.util.Iterator;
import java.util.NavigableSet;
import java.util.TreeSet;
import java.awt.Dimension;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabLexer;
import org.scilab.modules.scinotes.ScilabLexerConstants;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.KeywordEvent;
import org.scilab.modules.scinotes.utils.SciNotesMessages;
import org.scilab.modules.action_binding.InterpreterManagement;

/**
 * OpenSourceFileOnKeywordAction Class
 * @author Calixte DENIZET
 */
public class OpenSourceFileOnKeywordAction extends DefaultAction {

    private static final int GAP = 5;
    private static boolean windowAlreadyExist;
    private static JFrame mainFrame;

    private NavigableSet<String> macrosSet;
    private JButton okButton;
    private JButton cancelButton;
    private JComboBox comboComplete;
    private boolean onMenu;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public OpenSourceFileOnKeywordAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     * @param onMenu if true the dialog will be open when a menu is actionned
     */
    public OpenSourceFileOnKeywordAction(String name, SciNotes editor, boolean onMenu) {
        this(name, editor);
        this.onMenu = onMenu;
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = (ScilabEditorPane) getEditor().getTextPane();
        KeywordEvent kwe = sep.getKeywordEvent(sep.getSelectionEnd());
        ScilabDocument doc = (ScilabDocument) sep.getDocument();
        if (ScilabLexerConstants.isOpenable(kwe.getType())) {
            String name = "";
            try {
                name = doc.getText(kwe.getStart(), kwe.getLength());
            } catch (BadLocationException e) { }
            if (onMenu) {
                openSourceBox(name);
            } else {
                openSource(name);
            }
        } else {
            openSourceBox("");
        }
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new OpenSourceFileOnKeywordAction(label, editor, true), key);
    }

    /**
     * Close the window
     */
    public static void closeOpenSourceWindow() {
        if (windowAlreadyExist) {
            mainFrame.dispose();
            windowAlreadyExist = false;
        }
    }

    /**
     * Open a window to get the name of the macro
     * @param name the name of a macro
     */
    public void openSourceBox(String name) {
        if (windowAlreadyExist) {
            mainFrame.setVisible(true);
            if (name.length() != 0) {
                comboComplete.removeAllItems();
                comboComplete.getEditor().setItem(name);
                comboComplete.hidePopup();
            }
            return;
        }

        int dimX = 450;
        int dimY = 90;

        macrosSet = new TreeSet(ScilabLexer.macros);

        mainFrame = new JFrame();
        mainFrame.setIconImage(new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png").getImage());
        mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        mainFrame.setLayout(new GridBagLayout());

        mainFrame.setPreferredSize(new Dimension(dimX, dimY));
        mainFrame.setMinimumSize(new Dimension(dimX, dimY));
        mainFrame.setMaximumSize(new Dimension(dimX, dimY));
        windowAlreadyExist = true;

        JLabel label = new JLabel(SciNotesMessages.OPEN_SOURCE_FILE_OF);

        comboComplete = new JComboBox();
        comboComplete.setEditable(true);
        if (name.length() != 0) {
            comboComplete.removeAllItems();
            comboComplete.getEditor().setItem(name);
        } else {
            updateComboComplete(" ");
        }

        comboComplete.hidePopup();

        cancelButton = new JButton(SciNotesMessages.CANCEL);
        okButton = new JButton(SciNotesMessages.OK);
        okButton.setPreferredSize(cancelButton.getPreferredSize());

        JPanel panelButton = new JPanel();
        panelButton.setLayout(new GridLayout(1, 4, 2 * GAP, GAP));
        panelButton.add(new JLabel());
        panelButton.add(new JLabel());
        panelButton.add(cancelButton);
        panelButton.add(okButton);

        JPanel panelCombo = new JPanel();
        panelCombo.setLayout(new GridLayout(1, 2, GAP, GAP));
        panelCombo.add(label);
        panelCombo.add(comboComplete);

        JPanel panelFrame = new JPanel();
        panelFrame.setBorder(BorderFactory.createEmptyBorder(GAP, GAP, GAP, GAP));
        panelFrame.setLayout(new BoxLayout(panelFrame, BoxLayout.PAGE_AXIS));
        panelFrame.add(panelCombo);
        panelFrame.add(panelButton);

        mainFrame.setContentPane(panelFrame);

        cancelButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    OpenSourceFileOnKeywordAction.windowAlreadyExist = false;
                    mainFrame.dispose();
                }
            });

        okButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    OpenSourceFileOnKeywordAction.windowAlreadyExist = false;
                    mainFrame.dispose();
                    openSource((String) comboComplete.getEditor().getItem());
                }
            });

        comboComplete.getEditor().getEditorComponent().addKeyListener(new KeyListener() {

                private String old = "";

                public void keyReleased(KeyEvent e) {
                    int code = e.getKeyCode();
                    if (code == KeyEvent.VK_ENTER) {
                        comboComplete.hidePopup();
                        okButton.requestFocus();
                    } else if (code == KeyEvent.VK_ESCAPE) {
                        comboComplete.hidePopup();
                    } else if (code != KeyEvent.VK_UP && code != KeyEvent.VK_DOWN) {
                        updateComboComplete(old);
                        comboComplete.setPopupVisible(true);
                    }
                }

                public void keyPressed(KeyEvent arg0) {
                    old = (String) comboComplete.getEditor().getItem();
                }

                public void keyTyped(KeyEvent arg0) { }
            });

        mainFrame.addWindowListener(new WindowListener() {
                public void windowClosed(WindowEvent arg0) { }
                public void windowDeiconified(WindowEvent arg0) { }
                public void windowActivated(WindowEvent arg0) { }

                public void windowClosing(WindowEvent arg0) {
                    OpenSourceFileOnKeywordAction.windowAlreadyExist = false;
                    mainFrame.dispose();
                }

                public void windowDeactivated(WindowEvent arg0) { }
                public void windowIconified(WindowEvent arg0) { };
                public void windowOpened(WindowEvent arg0) { }
            });

        mainFrame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        mainFrame.setTitle(SciNotesMessages.OPEN_SOURCE);
        mainFrame.pack();
        mainFrame.setLocationRelativeTo(null);
        mainFrame.setVisible(true);
    }

    /**
     * Open the source of a macro
     * @param name of the macro
     */
    private void openSource(String name) {
        ScilabEditorPane sep = (ScilabEditorPane) getEditor().getTextPane();
        ScilabDocument doc = (ScilabDocument) sep.getDocument();
        int pos = doc.searchFunctionByName(name);
        if (pos != -1) {
            sep.scrollTextToPos(pos);
        } else {
            String path = "get_function_path('" + name + "')";
            InterpreterManagement.requestScilabExec("if " + path + " ~=[] then editor(" + path + ");end");
        }
    }

    /**
     * Update the comboBox with the possible completion of the name
     * entered in the comboBox.
     * @param old the previous name
     */
    private void updateComboComplete(String old) {
        String name = (String) comboComplete.getEditor().getItem();
        if (name != null && !name.equals(old)) {
            String bound = "";
            Iterator<String> iter;
            int len = name.length();
            if (len != 0) {
                if (len == 1) {
                    bound = new String(new char[]{(char) (name.charAt(0) + 1)});
                } else {
                    bound = name.substring(0, len - 1);
                    bound += (char) (name.charAt(len - 1) + 1);
                }
                /* If "abc" is entered, we built "abd" (last letter + 1) and we
                   retrieve all the names between "abc" and "abd" (excluded) */
                iter = macrosSet.subSet(name, true, bound, false).iterator();
            } else {
                iter = macrosSet.iterator();
            }

            comboComplete.removeAllItems();
            while (iter.hasNext()) {
                comboComplete.addItem(iter.next());
            }

            comboComplete.getEditor().setItem(name);
            comboComplete.setPopupVisible(false);
        }
    }
}
