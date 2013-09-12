/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.types;

import java.util.HashSet;
import java.util.Set;

import javax.swing.event.EventListenerList;

/**
 * Class to handle the refresh
 */
public final class ScilabVariablesRefresh implements ScilabVariablesHandler {

    private static final EventListenerList eventListeners = new EventListenerList();
    private static ScilabVariablesRefresh instance;
    private static int id;

    /**
     * Constructor
     */
    private ScilabVariablesRefresh() {
    }

    /**
     * {@inheritDoc}
     */
    public void handle(ScilabType var) {
        Object[] listeners = eventListeners.getListenerList();
        ScilabVariablesEvent event = null;
        for (int i = listeners.length - 2; i >= 0; i -= 2) {
            if (listeners[i] == ScilabVariablesListener.class && ((ScilabVariablesListener) listeners[i + 1]).getListenedVariables().contains(var.getVarName())) {
                if (event == null) {
                    event = new ScilabVariablesEvent(var);
                }
                ((ScilabVariablesListener) listeners[i + 1]).scilabVariableUpdated(event);
            }
        }
    }

    /**
     * Get the id to use to send data from C/C++ to this Java handler
     *
     * @return the handler id
     */
    public static final int getScilabVariablesRefreshId() {
        if (instance == null) {
            instance = new ScilabVariablesRefresh();
            id = ScilabVariables.addScilabVariablesHandler(instance);
        }

        return id;
    }

    /**
     * Get all the listened variables from the different listeners This function
     * is used from C++ to retrieve exactly what is needed and should not be
     * used from Java.
     *
     * @return all the listened variables
     */
    public static final String[] getAllListenedVariables() {
        Object[] listeners = eventListeners.getListenerList();
        Set<String> set = new HashSet<String>();
        for (int i = listeners.length - 2; i >= 0; i -= 2) {
            if (listeners[i] == ScilabVariablesListener.class) {
                set.addAll(((ScilabVariablesListener) listeners[i + 1]).getListenedVariables());
            }
        }
        String[] arr = new String[set.size() + 1];
        arr = set.toArray(arr);
        arr[set.size()] = null;

        return arr;
    }

    /**
     * Add a new ScilabVariablesListener
     *
     * @param listener
     *            the listener to add
     */
    public static final void addScilabVariablesListener(ScilabVariablesListener listener) {
        eventListeners.add(ScilabVariablesListener.class, listener);
    }

    /**
     * Remove a ScilabVariablesListener
     *
     * @param listener
     *            the listener to remove
     */
    public static final void removeScilabVariablesListener(ScilabVariablesListener listener) {
        eventListeners.remove(ScilabVariablesListener.class, listener);
    }
}
