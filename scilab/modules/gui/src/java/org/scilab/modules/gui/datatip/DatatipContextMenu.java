package org.scilab.modules.gui.datatip;

import java.awt.event.*;
import java.awt.Window;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JButton;

import org.scilab.modules.gui.datatip.*;
import org.scilab.modules.gui.editor.*;

public class DatatipContextMenu {
    JFrame DatatipContextMenuFrame;
    JButton DeleteLastDatatip, DeleteAllDatatipsCurve, DeleteAllDatatips, RedrawAllDatatips, DatatipPropertiesWindow;
    String[] ButtonNames = {"Delete Last Created Datatip", "Delete All Datatips For The Selected Curve", "Delete All Datatips", "Redraw All Datatips", "Open Datatip Properties Window"};

    public DatatipContextMenu () {
        InitDatatipContextMenuFrame ();
    }

    public void InitDatatipContextMenuFrame () {
        DatatipContextMenuFrame = new JFrame ("Datatip Context Menu");
        DatatipContextMenuLayout ();
        DatatipContextMenuSize ();
        DatatipContextMenuLocation ();
        DatatipContextMenuVisible ();
        DatatipContextMenuResizeable ();
        InitButtons ();
    }

    public void DatatipContextMenuLayout () {
        DatatipContextMenuFrame.setLayout(null);
    }

    public void DatatipContextMenuSize () {
        DatatipContextMenuFrame.setSize(320, 150);
    }

    public void DatatipContextMenuLocation () {
        DatatipContextMenuFrame.setLocation(300, 200);
    }

    public void DatatipContextMenuVisible () {
        DatatipContextMenuFrame.setVisible(true);
    }

    public void DatatipContextMenuResizeable () {
        DatatipContextMenuFrame.setResizable(false);
    }

    public void InitButtons () {
        DeleteLastDatatip = new JButton (ButtonNames[0]);
        DeleteLastDatatip.setBounds (0, 0, 320, 30);
        DeleteLastDatatip.setVisible(true);

        DeleteAllDatatipsCurve = new JButton (ButtonNames[1]);
        DeleteAllDatatipsCurve.setBounds (0, 30, 320, 30);
        DeleteAllDatatipsCurve.setVisible(true);

        DeleteAllDatatips = new JButton (ButtonNames[2]);
        DeleteAllDatatips.setBounds (0, 60, 320, 30);
        DeleteAllDatatips.setVisible(true);

        RedrawAllDatatips = new JButton (ButtonNames[3]);
        RedrawAllDatatips.setBounds (0, 90, 320, 30);
        RedrawAllDatatips.setVisible(true);

        DatatipPropertiesWindow = new JButton (ButtonNames[4]);
        DatatipPropertiesWindow.setBounds (0, 120, 320, 30);
        DatatipPropertiesWindow.setVisible(true);


        DeleteLastDatatip.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                DeleteLastDatatipOnClick ();
            }
        });

        DeleteAllDatatipsCurve.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                DeleteAllDatatipsCurveOnClick ();
            }
        });

        DeleteAllDatatips.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                DeleteAllDatatipsOnClick();
            }
        });


        RedrawAllDatatips.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                RedrawAllDatatipsOnClick ();
            }
        });

        DatatipPropertiesWindow.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {

            }
        });


        DatatipContextMenuFrame.add(DeleteLastDatatip);
        DatatipContextMenuFrame.add(DeleteAllDatatipsCurve);
        DatatipContextMenuFrame.add(DeleteAllDatatips);
        DatatipContextMenuFrame.add(RedrawAllDatatips);
        DatatipContextMenuFrame.add(DatatipPropertiesWindow);
    }

    public void DeleteAllDatatipsOnClick () {
        if (EditorEventListener.NumberCreatedDatatips != 0) {
            Window win[] = Window.getWindows();
            for (int i = 2 ; i < win.length ; i++) {
                win[i].dispose();
            }
        }
    }

    public void RedrawAllDatatipsOnClick () {
        DeleteAllDatatipsOnClick ();
        for (int i = 0 ; i < EditorEventListener.NumberCreatedDatatips ; i++) {
            new DatatipCreate (EditorEventListener.CoordXRedraw.get(i), EditorEventListener.CoordYRedraw.get(i), EditorEventListener.PolylineUIDRedraw.get(i));
        }
    }

    public void DeleteLastDatatipOnClick () {
        Window win[] = Window.getWindows();
        int i = win.length;
        win[i - 1].dispose();
        win[i - 2].dispose();
    }

    public void DeleteAllDatatipsCurveOnClick () {
        DeleteAllDatatipsOnClick ();
        for (int i = 0 ; i < EditorEventListener.NumberCreatedDatatips ; i++) {
            if (EditorEventListener.PolylineUIDRedraw.get(i) != EditorEventListener.picked) {
                new DatatipCreate (EditorEventListener.CoordXRedraw.get(i), EditorEventListener.CoordYRedraw.get(i), EditorEventListener.PolylineUIDRedraw.get(i));
            }
        }
    }
}

