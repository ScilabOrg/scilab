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

package org.scilab.modules.ui_data.filebrowser;

import java.awt.Rectangle;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import java.util.ArrayList;
import java.util.List;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JComboBox;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.SwingUtilities;

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;

/**
 *
 * @author Calixte DENIZET
 */
public class ScilabFileBrowserHistory {

    private static final String PREVIOUSICON = System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/go-previous.png";
    private static final String NEXTICON = System.getenv("SCI") + "/modules/gui/images/icons/16x16/filebrowser/go-next.png";

    private SwingScilabTreeTable stt;
    private List<String> history = new ArrayList<String>();
    private int position = -1;
    private PushButton previous;
    private PushButton next;
    private JPopupMenu popup;

    public ScilabFileBrowserHistory(SwingScilabTreeTable stt) {
        this.stt = stt;
        this.popup = new JPopupMenu();

        previous = ScilabPushButton.createPushButton();
        previous.setIcon(PREVIOUSICON);
        previous.setCallback(new CallBack(null) {
                public void callBack() {
                    ScilabFileBrowserHistory.this.stt.setBaseDir(history.get(position - 1), false);
                    setPositionInHistory(position - 1);
                }
            });

        ((SwingScilabPushButton) previous.getAsSimplePushButton()).addMouseListener(new MouseAdapter() {
                public void mouseClicked(MouseEvent e) {
                    if (SwingUtilities.isRightMouseButton(e) && previous.isEnabled() && (!popup.isVisible() || popup.getInvoker() != previous)) {
                        addPreviousItems();
                        Rectangle r = ((SwingScilabPushButton) previous.getAsSimplePushButton()).getBounds(null);
                        popup.show(((SwingScilabPushButton) previous.getAsSimplePushButton()), 0, r.height);
                    }
                }
            });

        next = ScilabPushButton.createPushButton();
        next.setIcon(NEXTICON);
        next.setCallback(new CallBack(null) {
                public void callBack() {
                    ScilabFileBrowserHistory.this.stt.setBaseDir(history.get(position + 1), false);
                    setPositionInHistory(position + 1);
                }
            });

        ((SwingScilabPushButton) next.getAsSimplePushButton()).addMouseListener(new MouseAdapter() {
                public void mouseClicked(MouseEvent e) {
                    if (SwingUtilities.isRightMouseButton(e) && next.isEnabled() && (!popup.isVisible() || popup.getInvoker() != next)) {
                        addNextItems();
                        Rectangle r = ((SwingScilabPushButton) next.getAsSimplePushButton()).getBounds(null);
                        popup.show(((SwingScilabPushButton) next.getAsSimplePushButton()), 0, r.height);
                    }
                }
            });

        updateButton(0);
    }

    private void addPreviousItems() {
	    cleanPopup();
	    for (int i = position - 1; i >= 0; i--) {
		JMenuItem item = new JMenuItem(history.get(i));
                final int j = i;
                item.addActionListener(new CallBack(null) {
                        public void callBack() {
                            ScilabFileBrowserHistory.this.stt.setBaseDir(history.get(j), false);
                            setPositionInHistory(j);
                        }
                    });
		popup.add(item);
            }
	    popup.pack();
    }

    private void addNextItems() {
	    cleanPopup();
            for (int i = position + 1; i < history.size(); i++) {
		JMenuItem item = new JMenuItem(history.get(i));
                final int j = i;
                item.addActionListener(new CallBack(null) {
                        public void callBack() {
                            ScilabFileBrowserHistory.this.stt.setBaseDir(history.get(j), false);
                            setPositionInHistory(j);
                        }
                    });
		popup.add(item);
            }
	    popup.pack();
    }

    private void cleanPopup() {
	popup.removeAll();
    }

    public void addPathInHistory(String path) {
        history.add(path);
        setPositionInHistory(position + 1);
    }

    public PushButton getPreviousButton() {
        return previous;
    }

    public PushButton getNextButton() {
        return next;
    }

    private void setPositionInHistory(int pos) {
        position = pos;
        updateButton(pos);
    }

    private void updateButton(int pos) {
        previous.setEnabled(history.size() >= 2 && pos != 0);
        next.setEnabled(history.size() >= 2 && pos != history.size() - 1);
    }
}
