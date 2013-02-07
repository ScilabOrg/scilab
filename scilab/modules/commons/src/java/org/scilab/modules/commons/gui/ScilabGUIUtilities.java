/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.commons.gui;

import java.awt.Component;
import java.awt.Frame;
import java.lang.reflect.Method;

import javax.swing.SwingUtilities;

import sun.awt.X11ComponentPeer;
import sun.awt.X11.XAtom;
import sun.awt.X11.XClientMessageEvent;
import sun.awt.X11.XComponentPeer;
import sun.awt.X11.XlibWrapper;
import sun.awt.X11.XToolkit;

/**
 * GUI utilities
 * @author Calixte DENIZET
 */
public class ScilabGUIUtilities {

    /**
     * Bring a window to the front
     * @param c the child component of the window to bring to front
     */
    public static void toFront(final Component c) {
        if (c != null) {
            if (SwingUtilities.isEventDispatchThread()) {
                bringToFront(c);
            } else {
                SwingUtilities.invokeLater(new Runnable() {
                        public void run() {
                            bringToFront(c);
                        }
                    });
            }
        }
    }

    /**
     * Bring a window to front
     * @param c the child component of the window to bring to front
     */
    private static void bringToFront(Component c) {
        Frame win = (Frame) SwingUtilities.getAncestorOfClass(Frame.class, c);
        if (win == null && c instanceof Frame) {
            win = (Frame) c;
        }
        if (win != null) {
            if (win.getPeer() instanceof X11ComponentPeer) {
                XToFront(win);
            } else {
                WindowsToFront(win);
            }
        }
    }

    /**
     * Bring a window to front (Windows version)
     * @param win the window to bring to front
     */
    private static void WindowsToFront(Frame win) {
        int state = win.getExtendedState() & ~Frame.ICONIFIED;
        win.setExtendedState(state);
        win.toFront();
        win.requestFocus();
    }

    /**
     * With X11, toFront() calls XRaiseWindow but the current WM catches this call and
     * makes what it wants.
     * This code is an awful workaround to force the window to be on the top of the windows stack.
     * @param win the window to bring to front
     */
    private static void XToFront(Frame win) {
        XToolkit.awtLock();
        try {
            Method getAtom = XAtom.class.getDeclaredMethod("getAtom");
            getAtom.setAccessible(true);
            Method getCurrentTimeServer = XToolkit.class.getDeclaredMethod("getCurrentServerTime");
            getCurrentTimeServer.setAccessible(true);
            Method XSendEvent = XlibWrapper.class.getDeclaredMethod("XSendEvent", long.class, long.class, boolean.class, long.class, long.class);
            XSendEvent.setAccessible(true);

            XAtom atom = XAtom.get("_NET_ACTIVE_WINDOW");
            XClientMessageEvent msg = new XClientMessageEvent();
            msg.set_type(XlibWrapper.ClientMessage);
            msg.set_message_type((long) (Long) getAtom.invoke(atom));
            msg.set_display(XToolkit.getDisplay());
            msg.set_window(((XComponentPeer) win.getPeer()).getWindow());
            msg.set_format(32);
            msg.set_data(0, 2);
            msg.set_data(1, (long) (Long) getCurrentTimeServer.invoke(null));
            msg.set_data(2, 0);
            msg.set_data(3, 0);
            msg.set_data(4, 0);
            XSendEvent.invoke(null, XToolkit.getDisplay(), XToolkit.getDefaultRootWindow(), false, XlibWrapper.SubstructureNotifyMask | XlibWrapper.SubstructureRedirectMask, msg.getPData());
            msg.dispose();
        } catch (Exception e) {
            win.toFront();
        } finally {
            XToolkit.awtUnlock();
        }
    }
}
