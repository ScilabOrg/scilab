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

import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.ClipboardOwner;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;

import javax.swing.KeyStroke;

import org.scilab.modules.jvm.LoadClassPath;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.utils.HTMLCodeConverter;

/**
 * CopyAsHTMLAction Class
 * @author Calixte Denizet
 */
public class CopyAsHTMLAction extends DefaultAction {

    protected boolean printLineNumber;
    private boolean codeConverterLoaded;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public CopyAsHTMLAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        if (!codeConverterLoaded) {
            LoadClassPath.loadOnUse("copyAsHTMLinScinotes");
            codeConverterLoaded = true;
        }
        String selection = getEditor().getTextPane().getSelectedText();
        if (selection != null) {
            selection = new HTMLCodeConverter((ScilabEditorPane) getEditor().getTextPane()).convert(selection, printLineNumber);
            HTMLSelection sel = new HTMLSelection(selection);
            Toolkit.getDefaultToolkit().getSystemClipboard().setContents(sel, sel);
        }
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return createMenu
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new CopyAsHTMLAction(label, editor), key);
    }

    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static PushButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new CopyAsHTMLAction(tooltip, editor));
    }

    /**
     * Inner class to handle HTML selectoin
     */
    class HTMLSelection implements Transferable, ClipboardOwner {

        private DataFlavor htmlFlavor;
        private String data;

        /**
         * Default constructor
         * @param data the datas to copy
         */
        HTMLSelection(String data) {
            this.data = data;
            try {
                htmlFlavor = new DataFlavor("text/html;class=java.lang.String");
            } catch (ClassNotFoundException e) {
                e.printStackTrace();
            }
        }

        /**
         * {@inheritDoc}
         */
        public Object getTransferData(DataFlavor flavor) {
            return data;
        }

        /**
         * {@inheritDoc}
         */
        public DataFlavor[] getTransferDataFlavors() {
            return new DataFlavor[]{htmlFlavor};
        }

        /**
         * {@inheritDoc}
         */
        public boolean isDataFlavorSupported(DataFlavor flavor) {
            return flavor.equals(htmlFlavor);
        }

        /**
         * {@inheritDoc}
         */
        public void lostOwnership(Clipboard clipboard, Transferable contents) { }
    }
}
