/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.preferences;

import java.util.Set;

import org.scilab.modules.commons.xml.XConfigurationEvent;
import org.scilab.modules.commons.xml.XConfigurationListener;
import org.scilab.modules.console.ConsoleOptions;

public class XcosConfiguration implements XConfigurationListener {

    public XcosConfiguration() {
    }

    /**
     * Prevent the listener that configuration has changed
     *
     * @param e
     *            the event
     */
    @Override
    public void configurationChanged(XConfigurationEvent e) {
        Conf conf = new Conf(e.getModifiedPaths());
        if (conf.changed()) {
            // ConsoleOptions.invalidate(conf);
            // sciConsole.configurationChanged(conf);
        }
    }

    public static class Conf {

        public boolean edition;
        public boolean shortcuts;
        public boolean simulation;

        public Conf(Set<String> path) {
            if (path.contains("ALL")) {
                edition = true;
                shortcuts = true;
                simulation = true;
            } else {
                edition = path.contains(XcosOptions.CONSOLEFONTPATH) || path.contains(ConsoleOptions.FONTPATH);
                color = path.contains(ConsoleOptions.COLORSPATH);
                display = path.contains(ConsoleOptions.DISPLAYPATH);
                keymap = path.contains(ConsoleOptions.KEYMAPPATH);
            }
        }

        public boolean changed() {
            return font || color || display || keymap;
        }
    }
}
