/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import java.awt.Cursor;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;

import javax.swing.JFileChooser;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.SciFileFilter;
import org.scilab.modules.jvm.LoadClassPath;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.utils.CodeExporter;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * Class Export action for SciNotes
 * @author Calixte DENIZET
 */
public class ExportAction extends DefaultAction {

    private static final long serialVersionUID = 7796680521955058413L;

    private static final String DOT = ".";

    private boolean codeConverterLoaded;
    private File currentFile;

    /**
     * Default constructor
     * @param name the name of the action
     * @param editor the editor
     */
    public ExportAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Function doAction
     */
    public void doAction() {
        String extension = null;
        String title = "Export";
        String path = "";

        String initialDirectoryPath = path;
        if (initialDirectoryPath == null) {
            initialDirectoryPath = getEditor().getTextPane().getName();
        }
        if (initialDirectoryPath == null) {
            initialDirectoryPath =  ConfigManager.getLastOpenedDirectory();
        }

        SciFileFilter pdfFilter = new SciFileFilter("*.pdf", null, 0);
        SciFileFilter psFilter = new SciFileFilter("*.ps", null, 1);
        SciFileFilter epsFilter = new SciFileFilter("*.eps", null, 2);
        SciFileFilter rtfFilter = new SciFileFilter("*.rtf", null, 3);

        final SwingScilabFileChooser fileChooser = ((SwingScilabFileChooser) ScilabFileChooser.createFileChooser().getAsSimpleFileChooser());

        fileChooser.setInitialDirectory(ConfigManager.getLastOpenedDirectory());
        fileChooser.setAcceptAllFileFilterUsed(false);
        fileChooser.setInitialDirectory(initialDirectoryPath);
        fileChooser.setDialogTitle(Messages.gettext(title));
        fileChooser.setApproveButtonText(Messages.gettext(title));

        // order is also important here
        fileChooser.addChoosableFileFilter(pdfFilter);
        fileChooser.addChoosableFileFilter(psFilter);
        fileChooser.addChoosableFileFilter(epsFilter);
        fileChooser.addChoosableFileFilter(rtfFilter);

        //select default file type
        fileChooser.setFileFilter(pdfFilter);
        fileChooser.setTitle(title);

        fileChooser.addPropertyChangeListener(JFileChooser.FILE_FILTER_CHANGED_PROPERTY, new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent e) {
                Object val = e.getNewValue();
                if (val != null && (val instanceof SciFileFilter)) {
                    String filter = ((SciFileFilter) val).getDescription();
                    String file = currentFile.getName();
                    int dotpos = file.lastIndexOf(".");
                    if (dotpos != -1) {
                        file = file.substring(0, dotpos);
                    }
                    dotpos = filter.lastIndexOf(".");
                    if (dotpos != -1) {
                        filter = filter.substring(dotpos, filter.length() - 1);
                    }
                    fileChooser.setSelectedFile(new File(file + filter));
                }
            }
        });

        fileChooser.addPropertyChangeListener(JFileChooser.SELECTED_FILE_CHANGED_PROPERTY , new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent e) {
                Object val = e.getNewValue();
                if (val != null && (val instanceof File)) {
                    currentFile = (File) val;
                }
            }
        });

        String name = getEditor().getTextPane().getName();
        if (name == null) {
            name = ((ScilabDocument) getEditor().getTextPane().getDocument()).getFirstFunctionName();
            if (name == null) {
                name = "";
            }
        }
        int dotpos = name.lastIndexOf(".");
        if (dotpos != -1) {
            name = name.substring(0, dotpos);
        }
        if (name != null) {
            name += ".pdf";
            fileChooser.setSelectedFile(new File(name));
        }

        int retval = fileChooser.showSaveDialog(getEditor());

        String fileName = null;
        String type = null;

        if (retval == JFileChooser.APPROVE_OPTION) {
            File f = SciNotes.fileToCanonicalFile(fileChooser.getSelectedFile());
            initialDirectoryPath = f.getPath();
            if (f.exists()) {
                if (ScilabModalDialog.show(getEditor(), SciNotesMessages.REPLACE_FILE_TITLE,
                                           SciNotesMessages.FILE_ALREADY_EXIST, IconType.QUESTION_ICON,
                                           ButtonType.YES_NO) == AnswerOption.NO_OPTION) {
                    return;
                }
            }

            boolean hasNoExtension = true;
            fileName = f.getName();
            if (fileName.lastIndexOf(DOT) != -1) {
                int len = fileName.substring(fileName.lastIndexOf(DOT), fileName.length()).length();
                if (len >= 2 && len <= 4) {
                    hasNoExtension = false;
                    extension = fileName.substring(fileName.lastIndexOf(DOT) + 1, fileName.length());
                }
            }

            if (extension == null) {
                if (fileChooser.getFileFilter() == pdfFilter) {
                    extension = "pdf";
                    type = CodeExporter.PDF;
                } else if (fileChooser.getFileFilter() == psFilter) {
                    extension = "ps";
                    type = CodeExporter.PS;
                } else if (fileChooser.getFileFilter() == epsFilter) {
                    extension = "eps";
                    type = CodeExporter.EPS;
                } else if (fileChooser.getFileFilter() == rtfFilter) {
                    extension = "rtf";
                    type = CodeExporter.RTF;
                } else {
                    extension = "";
                    type = null;
                }
            } else {
                if (extension.equalsIgnoreCase("pdf")) {
                    type = CodeExporter.PDF;
                } else if (extension.equalsIgnoreCase("ps")) {
                    type = CodeExporter.PS;
                } else if (extension.equalsIgnoreCase("eps")) {
                    type = CodeExporter.EPS;
                } else if (extension.equalsIgnoreCase("rtf")) {
                    type = CodeExporter.RTF;
                }
            }

            if (hasNoExtension) {
                fileName = f.getPath() + DOT + extension;
            } else {
                fileName = f.getPath();
            }
        } else if (retval == JFileChooser.CANCEL_OPTION) {
            fileName = null;
        }

        if (!codeConverterLoaded) {
            LoadClassPath.loadOnUse("copyAsHTMLinScinotes");
            LoadClassPath.loadOnUse("pdf_ps_eps_graphic_export");
            codeConverterLoaded = true;
        }

        if (fileName != null && type != null) {
            getEditor().getTextPane().setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
            CodeExporter.convert(getEditor().getTextPane(), fileName, type, PageSetupAction.getPageFormat());
            getEditor().getTextPane().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
        }
    }

    /**
     * Create the MenuItem for print action
     * @param label label of the menu
     * @param editor Editor
     * @param key KeyStroke
     * @return a MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new ExportAction(label, editor), key);
    }
}
