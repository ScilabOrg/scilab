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

package org.scilab.modules.xpad;

import java.awt.event.KeyListener;
import java.awt.event.KeyEvent;

import javax.swing.text.BadLocationException;

/**
 * This class handles the help on typing
 * @author Calixte DENIZET
 */
public class HelpOnTypingManager implements KeyListener {

    private ScilabEditorPane textPane;
    private ScilabDocument doc;

    /**
     * Constructor
     * @param pane the associated TextPane
     */
    public HelpOnTypingManager(ScilabEditorPane pane) {
        this.textPane = pane;
        this.doc = (ScilabDocument) pane.getDocument();
    }

    /**
     * Nothing !
     * @param e the event
     */
    public void keyPressed(KeyEvent e) { }

    /**
     * Nothing !
     * @param e the event
     */
    public void keyReleased(KeyEvent e) { }

    /**
     * Called when a key is typed
     * @param e the event
     */
    public void keyTyped(KeyEvent e) {
        char c = e.getKeyChar();
        if (c != KeyEvent.CHAR_UNDEFINED) {
            int pos = textPane.getCaretPosition();

            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                try {
                    doc.insertString(pos, Character.toString(c), null);
                    e.consume();
                    KeywordEvent kwe = textPane.getKeywordEvent(pos + 1);
                    switch (kwe.getType()) {
                    case ScilabLexerConstants.OSKEYWORD :
                        doc.insertString(pos + 1, "\nend", null);
                        textPane.getIndentManager().indentDoc(pos + 1, pos + 4);
                        textPane.setCaretPosition(pos + 1);
                        break;
                    case ScilabLexerConstants.FKEYWORD :
                        if ("f".equals(doc.getText(kwe.getStart(), 1))) {
                            doc.insertString(pos + 1, " ()\nendfunction", null);
                            textPane.getIndentManager().indentDoc(pos + 4, pos + 15);
                            textPane.setCaretPosition(pos + 2);
                        }
                        break;
                    }
                } catch (BadLocationException exc) { }
            } else {
                if (c == '(' || c == '[' || c == '{') {
                    String str;
                    switch (c) {
                    case '(' :
                        str = "()";
                        break;
                    case '[' :
                        str = "[]";
                        break;
                    default :
                        str = "{}";
                    }
                    try {
                        doc.insertString(pos, str, null);
                        e.consume();
                        textPane.setCaretPosition(pos + 1);
                    } catch (BadLocationException exc) { }
                }
            }
        }
    }
}
