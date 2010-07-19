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

package org.scilab.modules.scinotes;

import java.awt.event.KeyListener;
import java.awt.event.KeyEvent;

import javax.swing.text.BadLocationException;

/**
 * This class handles the help on typing
 * @author Calixte DENIZET
 */
public class HelpOnTypingManager implements KeyListener {

    /**
     * Constructor
     * @param pane the associated TextPane
     */
    public HelpOnTypingManager(ScilabEditorPane pane) { }

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
        ScilabEditorPane textPane = ScilabEditorPane.getFocusedPane();
        ScilabDocument doc = (ScilabDocument) textPane.getDocument();
        if (c != KeyEvent.CHAR_UNDEFINED && textPane.getSelectionStart() == textPane.getSelectionEnd()) {
            int pos = textPane.getCaretPosition();
            if (c == ' ' && e.getModifiers() == 0) {
                int end = doc.getDefaultRootElement().getElement(doc.getDefaultRootElement().getElementIndex(pos)).getEndOffset() - 1;
                /* the following test is used to know if an insertion in the line is done */
                if (pos == end) {
                    try {
                        doc.insertString(pos, " ", null);
                        e.consume();
                        KeywordEvent kwe = textPane.getKeywordEvent(pos);
                        switch (kwe.getType()) {
                        case ScilabLexerConstants.OSKEYWORD :
                            doc.insertString(pos + 1, "end", null);
                            textPane.setCaretPosition(pos + 1);
                            break;
                        case ScilabLexerConstants.FKEYWORD :
                            /* We have 'function' or 'endfunction' */
                            if ("f".equals(doc.getText(kwe.getStart(), 1))) {
                                doc.insertString(pos + 1, "()endfunction", null);
                                textPane.setCaretPosition(pos + 1);
                            }
                            break;
                        default :
                        }
                    } catch (BadLocationException exc) { }
                }
            } else {
                String str = null;
                switch (c) {
                case '(' :
                    str = "()";
                    break;
                case '[' :
                    str = "[]";
                    break;
                case '{' :
                    str = "{}";
                    break;
                case '\"' :
                    str = "\"\"";
                default :
                }

                if (str != null) {
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
