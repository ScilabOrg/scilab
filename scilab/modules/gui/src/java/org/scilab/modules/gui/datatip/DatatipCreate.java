package org.scilab.modules.gui.datatip;

import javax.swing.JWindow;
import javax.swing.JButton;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

public class DatatipCreate implements ActionListener {
    JWindow datatipWindow;
    JButton datatipExitButton;
    String PolylineUID;
    int x, y;

    public DatatipCreate (int x, int y, String PolylineUID) {
        CreateDatatip (x, y);
    }

    public void CreateDatatip (int x, int y) {
        datatipWindow = new JWindow ();
        DatatipLayout ();
        DatatipBounds (x, y);
        DatatipVisible ();
        DatatipOnTop ();
        CreateExitButton ();
    }

    public void DatatipLayout () {
        datatipWindow.setLayout(null);
    }

    public void DatatipBounds (int x, int y) {
        datatipWindow.setBounds(x, y, 50, 40);
    }

    public void DatatipVisible () {
        datatipWindow.setVisible(true);
    }

    public void DatatipOnTop () {
        datatipWindow.setAlwaysOnTop(true);
    }

    public void CreateExitButton () {
        datatipExitButton = new JButton ();
        ExitButtonListener ();
        ExitButtonBounds ();
        datatipWindow.add(datatipExitButton);
    }

    public void ExitButtonListener () {
        datatipExitButton.addActionListener(this);
    }

    public void ExitButtonBounds () {
        datatipExitButton.setBounds (x + 33, y + 2, 15, 15);
    }

    public void actionPerformed(ActionEvent close_datatip_event) {
        if (close_datatip_event.getSource() == datatipExitButton) {
            datatipWindow.setVisible(false);
        }
    }
}
