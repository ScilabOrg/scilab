/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.graphic_objects.graphicView;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;

import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.text.Document;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;

public class GuiLogView implements GraphicView {
    private static GuiLogView me;
    private JEditorPane htmlLogPane;
    private JFrame frame;

    public static GuiLogView createGuiLogView() {
        if (me == null) {
            me = new GuiLogView();
        }
        return me;
    }

    private GuiLogView() {
        frame = new JFrame("GuiLogView");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        htmlLogPane = new JEditorPane();
        htmlLogPane.setEditable(false);
        JScrollPane logView = new JScrollPane(htmlLogPane);
        frame.setContentPane(logView);
        frame.setVisible(false);
        frame.setSize(800, 600);
    }

    public void show() {
        frame.setVisible(true);
    }

    public void createObject(Integer id) {
        try {
            int objectType = (Integer) GraphicController.getController().getProperty(id, __GO_TYPE__);
            Document doc = htmlLogPane.getDocument();
            doc.insertString(doc.getLength(), "+++" + id + " Object Created type is " + objectType + ".\n", null);
            htmlLogPane.setCaretPosition(doc.getLength());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    public void deleteObject(Integer id) {
        try {
            Document doc = htmlLogPane.getDocument();
            doc.insertString(doc.getLength(), "---" + id + " Object Deleted.\n", null);
            htmlLogPane.setCaretPosition(doc.getLength());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    public void updateObject(Integer id, int property) {
        try {
            Document doc = htmlLogPane.getDocument();
            doc.insertString(doc.getLength(), "===" + id + " Object Property " + property + " Updated.\n", null);
            htmlLogPane.setCaretPosition(doc.getLength());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
