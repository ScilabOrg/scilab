/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 *
 */
package org.scilab.modules.xpad.utils;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;

import javax.swing.JEditorPane;
import javax.swing.text.BadLocationException;
import javax.swing.text.EditorKit;

import org.scilab.modules.xpad.ScilabDocument;

/**
 * Save File utility class
 * @author Allan CORNET
 *
 */
public final class SaveFile {

	private static final String LINE_SEPARATOR = "line.separator";
	
	/**
	 * private Constructor
	 */
	private SaveFile() {
	}
	/**
	 * save text in JEditorPane
	 * @param textPane JEditorPane
	 * @param fOut File
	 * @param editorKit EditorKit
	 * @return true if saved
	 */
	public static boolean doSave(JEditorPane textPane, File fOut, EditorKit editorKit) {
		
		ScilabDocument styledDocument = (ScilabDocument) textPane.getDocument();

		// get default eol
		String defaultEol = System.getProperty(LINE_SEPARATOR);

		// set eol used to save file 
		if (styledDocument.getEOL().compareTo(defaultEol) != 0) {
			System.setProperty(LINE_SEPARATOR, styledDocument.getEOL());
		}
		boolean bReturn = false;
		
		BufferedWriter out = null;
		try {
			out = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(fOut), styledDocument.getEncoding()));
			try {
				editorKit.write(out, styledDocument, 0, styledDocument.getLength());
				out.flush();
				out.close();
				
				// restore default eol
				System.setProperty(LINE_SEPARATOR, defaultEol);
				
				bReturn = true;
				
			} catch (IOException e) {
				// restore default eol
				System.setProperty(LINE_SEPARATOR, defaultEol);
				bReturn = false;
			} catch (BadLocationException e) {
				// restore default eol
				System.setProperty(LINE_SEPARATOR, defaultEol);
				bReturn = false;
			}
		} catch (UnsupportedEncodingException e) {
			// restore default eol
			System.setProperty(LINE_SEPARATOR, defaultEol);
			bReturn = false;
		} catch (FileNotFoundException e) {
			// restore default eol
			System.setProperty(LINE_SEPARATOR, defaultEol);
			bReturn = false;
		}
		return bReturn;
	}
}
