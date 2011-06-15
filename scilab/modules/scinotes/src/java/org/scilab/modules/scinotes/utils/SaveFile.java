/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 *
 */
package org.scilab.modules.scinotes.utils;

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

import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;

/**
 * Save File utility class
 * @author Allan CORNET
 * @author Calixte DENIZET
 */
public final class SaveFile {

    private static final String LINE_SEPARATOR = "line.separator";

    /**
     * private Constructor
     */
    private SaveFile() { }


    /**
     * save a document
     * @param doc the document to save
     * @param fOut File
     * @param editorKit EditorKit
     * @return true if saved
     */
    public static boolean doSave(ScilabDocument doc, File fOut, EditorKit editorKit) throws IOException {
        // get default eol
        String defaultEol = System.getProperty(LINE_SEPARATOR);
        boolean bReturn = false;
        BufferedWriter bw = null;
        OutputStreamWriter osw = null;
        FileOutputStream fos = null;

        // set eol used to save file
        if (doc.getEOL().compareTo(defaultEol) != 0) {
            System.setProperty(LINE_SEPARATOR, doc.getEOL());
        }

        try {
            fos = new FileOutputStream(fOut);
            osw = new OutputStreamWriter(fos, ConfigSciNotesManager.getDefaultEncoding());
            bw = new BufferedWriter(osw);
            editorKit.write(bw, doc, 0, doc.getLength());
            bw.flush();
            bReturn = true;
        } catch (BadLocationException e) {
            System.err.println(e);
        } finally {
            try {
                if (fos != null) {
                    fos.close();
                }
                if (osw != null) {
                    osw.close();
                }
                if (bw != null) {
                    bw.close();
                }
            } catch (IOException e) {
                System.err.println(e);
            }
        }

        // restore default eol
        System.setProperty(LINE_SEPARATOR, defaultEol);

        return bReturn;
    }

    /**
     * save text in JEditorPane
     * @param textPane JEditorPane
     * @param index the index of the textPane
     * @param fOut File
     * @param editorKit EditorKit
     * @return true if saved
     */
    public static boolean doSave(ScilabEditorPane textPane, int index, File fOut, EditorKit editorKit) {
        ScilabDocument doc = (ScilabDocument) textPane.getDocument();
        boolean enc = false;
	boolean bReturn = false;
        if (!doc.getEncoding().equalsIgnoreCase(ConfigSciNotesManager.getDefaultEncoding())) {
            String msg = String.format(SciNotesMessages.DIFFERENT_ENCODINGS, doc.getEncoding(), ConfigSciNotesManager.getDefaultEncoding());
            if (ScilabModalDialog.show(textPane.getEditor(), msg, SciNotesMessages.DIFFERENT_ENCODINGS_TITLE, IconType.QUESTION_ICON, ButtonType.YES_NO) == AnswerOption.NO_OPTION) {
                return false;
            }
            enc = true;
        }

        try {
            fOut.createNewFile();
        } catch (IOException e) {
            System.err.println(e);
        }

        if (!fOut.canWrite()) {
            ScilabModalDialog.show(textPane.getEditor(), SciNotesMessages.NOTWRITABLE, SciNotesMessages.SCINOTES_ERROR, IconType.ERROR_ICON);
            return false;
        }

        if (enc) {
            textPane.getEditor().reload(index);
        }

        try {
            bReturn = doSave(doc, fOut, editorKit);
        } catch (IOException e) {
            System.err.println(e);
        }

        return bReturn;
    }
}
