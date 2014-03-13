/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graph.actions;

import static org.scilab.modules.commons.OS.MAC;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

import javax.swing.ActionMap;
import javax.swing.InputMap;
import javax.swing.JButton;
import javax.swing.KeyStroke;

import org.scilab.modules.commons.OS;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.gui.menuitem.MenuItem;

/**
 * Zoom management
 */
@SuppressWarnings(value = { "serial" })
public class ZoomInAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = ScilabGraphMessages.ZOOM_IN;
    /** Icon name of the action */
    public static final String SMALL_ICON = "zoom-in";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_ADD;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    /** key used on {@link InputMap} for this action */
    private static final String ZOOM_IN = "zoomIn";

    /**
     * Implement custom mouse handling for the zoom
     */
    private static final class CustomMouseWheelListener implements MouseWheelListener {
        private final ScilabGraph scilabGraph;

        /**
         * Default constructor
         *
         * @param scilabGraph
         *            the current graph
         */
        public CustomMouseWheelListener(ScilabGraph scilabGraph) {
            this.scilabGraph = scilabGraph;
        }

        /**
         * When the wheel is used
         *
         * @param e
         *            the parameters
         * @see java.awt.event.MouseWheelListener#mouseWheelMoved(java.awt.event.MouseWheelEvent)
         */
        @Override
        public void mouseWheelMoved(MouseWheelEvent e) {
            if ((e.getModifiers() & ACCELERATOR_KEY) != 0) {
                if (e.getWheelRotation() < 0) {
                    scilabGraph.getAsComponent().zoomIn();
                }
            }
        }
    }

    /**
     * Constructor
     *
     * @param scilabGraph
     *            corresponding Scilab Graph
     */
    public ZoomInAction(ScilabGraph scilabGraph) {
        super(scilabGraph);

        MouseWheelListener mouseListener = new CustomMouseWheelListener(scilabGraph);
        scilabGraph.getAsComponent().addMouseWheelListener(mouseListener);

        // Multi-shortcut action
        final ActionMap am = scilabGraph.getAsComponent().getActionMap();
        final InputMap map = scilabGraph.getAsComponent().getInputMap();

        // register the action to a unique action keyword
        am.put(ZOOM_IN, this);

        // add custom key stroke for this action
        final KeyStroke[] keystrokes;
        if (OS.get() == MAC) {
            /*
             * AZERTY for Mac has a non-supported classic layout
             */
            keystrokes = new KeyStroke[] { KeyStroke.getKeyStroke('/', ACCELERATOR_KEY),
                                           KeyStroke.getKeyStroke('/', ACCELERATOR_KEY | KeyEvent.SHIFT_DOWN_MASK),
                                         };
        } else {
            keystrokes = new KeyStroke[] { KeyStroke.getKeyStroke('=', ACCELERATOR_KEY),
                                           KeyStroke.getKeyStroke('=', ACCELERATOR_KEY | KeyEvent.SHIFT_DOWN_MASK), KeyStroke.getKeyStroke('+', ACCELERATOR_KEY),
                                           KeyStroke.getKeyStroke('+', ACCELERATOR_KEY | KeyEvent.SHIFT_DOWN_MASK),
                                         };
        }

        for (KeyStroke k : keystrokes) {
            map.put(k, ZOOM_IN);
        }
    }

    /**
     * Create a button for a graph toolbar
     *
     * @param scilabGraph
     *            corresponding Scilab Graph
     * @return the button
     */
    public static JButton zoominButton(ScilabGraph scilabGraph) {
        return createButton(scilabGraph, ZoomInAction.class);
    }

    /**
     * Create a menu for a graph menubar
     *
     * @param scilabGraph
     *            corresponding Scilab Graph
     * @return the menu
     */
    public static MenuItem zoominMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, ZoomInAction.class);
    }

    /**
     * Action associated
     *
     * @param e
     *            the event
     * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        getGraph(e).getAsComponent().zoomIn();
    }
}
