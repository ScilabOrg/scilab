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

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.CommentManager;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * UnCommentAction Class
 * @author Calixte DENIZET
 *
 */
public final class UnCommentAction extends DefaultAction {

	/**
	 * Constructor
	 * @param editor SciNotes
	 */
	public UnCommentAction(SciNotes editor) {
		super(SciNotesMessages.UNCOMMENT_SELECTION, editor);
	}
	
	/**
	 * doAction
	 */
	public void doAction() {
	    ScilabEditorPane sep = (ScilabEditorPane) getEditor().getTextPane();
	    int start = sep.getSelectionStart();
	    int end   = sep.getSelectionEnd();
	    CommentManager com = sep.getCommentManager();
	    ScilabDocument doc = (ScilabDocument) sep.getDocument();
		
	    doc.mergeEditsBegin();
	    if (start == end) {
		com.uncommentText(start);
	    } else {
		int[] ret = com.uncommentLines(start, end - 1);
		if (ret != null) {
		    sep.setSelectionStart(ret[0]);
		    sep.setSelectionEnd(ret[1]);
		}
	    }
	    doc.mergeEditsEnd();
	}
	
	/**
	 * createMenu
	 * @param editor SciNotes
	 * @param key KeyStroke
	 * @return MenuItem
	 */
        public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
		return createMenu(SciNotesMessages.UNCOMMENT_SELECTION, null, new UnCommentAction(editor), key);
	}
}
