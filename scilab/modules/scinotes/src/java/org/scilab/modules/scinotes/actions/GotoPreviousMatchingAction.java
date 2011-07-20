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

package org.scilab.modules.scinotes.actions;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.MatchingBlockScanner;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.ScilabLexer;
import org.scilab.modules.scinotes.ScilabLexerConstants;

/**
 * GotoPreviousMatchingAction Class
 * @author Calixte DENIZET
 */
public final class GotoPreviousMatchingAction extends DefaultAction {

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public GotoPreviousMatchingAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = (ScilabEditorPane) getEditor().getTextPane();
        int pos = sep.getCaretPosition();
        ScilabLexer lexer = sep.getLexer();
        int tok = lexer.getKeyword(pos, true);
        MatchingBlockScanner.MatchingPositions mpos = null;
        if (ScilabLexerConstants.isMatchable(tok)) {
            ScilabDocument doc = (ScilabDocument) sep.getDocument();
            MatchingBlockScanner matcher = new MatchingBlockScanner(doc);
            mpos = matcher.getMatchingBlock(lexer.start + lexer.yychar() + lexer.yylength(), false);
            if (mpos != null) {
                sep.setCaretPosition(mpos.secondB);
            }
        } else {
            tok = lexer.getKeyword(pos, false);
            if (ScilabLexerConstants.isMatchable(tok)) {
                ScilabDocument doc = (ScilabDocument) sep.getDocument();
                MatchingBlockScanner matcher = new MatchingBlockScanner(doc);
                mpos = matcher.getMatchingBlock(lexer.start + lexer.yychar(), true);
                if (mpos != null) {
                    sep.setCaretPosition(mpos.secondE);
                }
            }
        }

    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new GotoPreviousMatchingAction(label, editor), key);
    }
}
