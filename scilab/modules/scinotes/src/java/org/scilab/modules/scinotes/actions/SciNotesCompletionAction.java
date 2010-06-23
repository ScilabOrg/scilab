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

import java.awt.Point;
import java.awt.Rectangle;

import javax.swing.JComponent;
import javax.swing.KeyStroke;
import javax.swing.ActionMap;
import javax.swing.InputMap;
import javax.swing.text.Element;
import javax.swing.text.StyledDocument;
import javax.swing.text.BadLocationException;

import com.artenum.rosetta.interfaces.ui.CompletionWindow;
import com.artenum.rosetta.interfaces.ui.OutputView;
import com.artenum.rosetta.interfaces.ui.PromptView;
import com.artenum.rosetta.interfaces.ui.InputCommandView;
import com.artenum.rosetta.interfaces.core.InputParsingManager;
import com.artenum.rosetta.interfaces.core.HistoryManager;
import com.artenum.rosetta.interfaces.core.GenericInterpreter;
import com.artenum.rosetta.interfaces.core.ConsoleConfiguration;
import com.artenum.rosetta.interfaces.core.CompletionManager;

import org.scilab.modules.completion.Completion;
import org.scilab.modules.console.CompletionAction;
import org.scilab.modules.console.SciCompletionManager;
import org.scilab.modules.console.SciInputParsingManager;

import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.utils.SciNotesCompletionWindow;

/**
 * @author Calixte DENIZET
 */
public final class SciNotesCompletionAction extends CompletionAction {

    private SciNotes editor;

    /**
     * Constructor
     * @param editor SciNotes
     */
    private SciNotesCompletionAction(JComponent textPane, SciNotes editor) {
        this.editor = editor;
        configuration = new SciNotesCompletionConfiguration(textPane);
    }

    /**
     * Put input map
     * @param textPane JTextpane
     * @param key KeyStroke
     * @param editor Editor
     */
    public static void putInInputMap(JComponent textPane, SciNotes editor, KeyStroke key) {
        textPane.getInputMap().put(key, new SciNotesCompletionAction(textPane, editor));
    }

    /**
     * Inner class to manage the completion
     */
    class SciNotesInputParsingManager extends SciInputParsingManager {

        /**
         * Reset
         */
        public void reset() {
            try {
                ((ScilabDocument) editor.getTextPane().getDocument()).setFocused(true);
                Element root = editor.getTextPane().getDocument().getDefaultRootElement();
                int pos = editor.getTextPane().getCaretPosition();
                int line = root.getElementIndex(pos);
                int start = root.getElement(line).getStartOffset();
                editor.getTextPane().getDocument().remove(start, pos - start);
                editor.getTextPane().setCaretPosition(start);
            } catch (BadLocationException e) { }
        }

        /**
         * @return the caret position
         */
        public int getCaretPosition() {
            Element root = editor.getTextPane().getDocument().getDefaultRootElement();
            int pos = editor.getTextPane().getCaretPosition();
            int line = root.getElementIndex(pos);
            int start = root.getElement(line).getStartOffset();
            return pos - start;
        }

        /**
         * @return the commandLine
         */
        public String getCommandLine() {
            try {
                Element root = editor.getTextPane().getDocument().getDefaultRootElement();
                int pos = editor.getTextPane().getCaretPosition();
                int line = root.getElementIndex(pos);
                int start = root.getElement(line).getStartOffset();
                int end = root.getElement(line).getEndOffset();
                return editor.getTextPane().getDocument().getText(start, pos - start);
            } catch (BadLocationException e) { }
            return "";
        }

        /**
         * @param content to append
         */
        public void append(String content) {
            try {
                ScilabDocument doc = (ScilabDocument) editor.getTextPane().getDocument();
                doc.setFocused(true);
                Element root = editor.getTextPane().getDocument().getDefaultRootElement();
                int pos = editor.getTextPane().getCaretPosition();
                doc.mergeEditsBegin();
                editor.getTextPane().getDocument().insertString(pos, content, null);
                doc.mergeEditsEnd();
            } catch (BadLocationException e) { }
        }

        /**
         * @return a Point where to put the completion window
         */
        public Point getWindowCompletionLocation() {
            try {
                int pos = editor.getTextPane().getCaretPosition();
                Rectangle r = editor.getTextPane().modelToView(pos);
                return new Point(r.x, r.y + r.height);
            } catch (BadLocationException e) { }
            return new Point(0, 0);
        }

        /**
         * @return an int
         */
        public int getCompletionLevel() {
            return 0;
        }

        /**
         * @param level an int
         * @return a String to be completed
         */
        public String getPartLevel(int level) {
            return Completion.getPartLevel(getCommandLine());
        }

        /**
         * Return an empty string to be sure that word won't be completed
         * into filename.
         * @param level an unused int
         */
        public String getFilePartLevel(int level) {
            return "";
        }

        /**
         * @param completionResult a String
         */
        public void writeCompletionPart(String completionResult) { }

        /**
         * @return an int
         */
        public int getNumberOfLines() {
            return 0;
        }

        /**
         * @return a boolean
         */
        public boolean isBlockEditing() {
            return true;
        }

        /**
         * @param inputCommandView an InputCommandView
         */
        public void setInputCommandView(InputCommandView inputCommandView) { }

        /**
         * Put a backspace
         */
        public void backspace() {
            try {
                Element root = editor.getTextPane().getDocument().getDefaultRootElement();
                int pos = editor.getTextPane().getCaretPosition();
                editor.getTextPane().getDocument().remove(pos - 1, 1);
                editor.getTextPane().setCaretPosition(pos - 1);
            } catch (BadLocationException e) { }
        }
    }

    /**
     * Inner class which implements interface ConsoleConfiguration.
     * Only use to be compatible with the way to complete in the console
     */
    class SciNotesCompletionConfiguration implements ConsoleConfiguration {

        private SciNotesInputParsingManager xipm = new SciNotesInputParsingManager();
        private SciCompletionManager scm = new SciCompletionManager();
        private SciNotesCompletionWindow cwi;

        /**
         * Constructor
         */
        public SciNotesCompletionConfiguration(JComponent textPane) {
            scm.setInputParsingManager(xipm);
            cwi = new SciNotesCompletionWindow(editor, scm);
            cwi.setInputParsingManager(xipm);
            cwi.setGraphicalContext(textPane);
        }

        /**
         * @return an InputParsingManager
         */
        public InputParsingManager getInputParsingManager() {
            return xipm;
        }

        /**
         * Returns the current completion manager
         * @return the completion manager
         */
        public CompletionManager getCompletionManager() {
            return scm;
        }

        /**
         * @return the CompletionWindow
         */
        public CompletionWindow getCompletionWindow() {
            return cwi;
        }

        /**
         * @return null
         */
        public PromptView getPromptView() {
            return null;
        }

        /**
         * @return null
         */
        public OutputView getOutputView() {
            return null;
        }

        /**
         * @return null
         */
        public InputCommandView getInputCommandView() {
            return null;
        }

        /**
         * @return null
         */
        public StyledDocument getOutputViewStyledDocument() {
            return null;
        }

        /**
         * @return null
         */
        public StyledDocument getInputCommandViewStyledDocument() {
            return null;
        }

        /**
         * @return null
         */
        public HistoryManager getHistoryManager() {
            return null;
        }

        /**
         * @return null
         */
        public String getBackgroundColor() {
            return null;
        }

        /**
         * @return null
         */
        public String getForegroundColor() {
            return null;
        }

        /**
         * @return 0
         */
        public int getScrollableUnitIncrement() {
            return 0;
        }

        /**
         * @return 0
         */
        public int getScrollableBlockIncrement() {
            return 0;
        }

        /**
         * @return true
         */
        public boolean getHorizontalWrapAllowed() {
            return true;
        }

        /**
         * @return true
         */
        public boolean getVerticalWrapAllowed() {
            return true;
        }

        /**
         * @return null
         */
        public String getFontName() {
            return null;
        }

        /**
         * @return 0
         */
        public int getFontStyle() {
            return 0;
        }

        /**
         * @return 0
         */
        public int getFontSize() {
            return 0;
        }

        /**
         * @return null
         */
        public String getWelcomeLine() {
            return null;
        }

        /**
         * @return null
         */
        public GenericInterpreter getGenericInterpreter() {
            return null;
        }

        /**
         * @return null
         */
        public ActionMap getActionMapping() {
            return null;
        }

        /**
         * @param profileName a String
         */
        public void setActiveProfile(String profileName) { }

        /**
         * @return null
         */
        public String getActiveProfile() {
            return null;
        }

        /**
         * @return null
         */
        public InputMap getKeyMapping() {
            return null;
        }
    }
}
