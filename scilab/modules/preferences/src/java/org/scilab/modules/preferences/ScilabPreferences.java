/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.preferences;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.swing.SwingUtilities;

import org.scilab.modules.localization.Messages;

/**
 * @author Calixte DENIZET
 */
public class ScilabPreferences {

    private static Map<String, ToolboxInfos> tbxs = new HashMap<String, ToolboxInfos>();

    /**
     * Open the preferences window
     */
    public static void openPreferences() {
        openPreferences(null);
    }

    /**
     * Open the preferences window
     */
    public static void openPreferences(final String initialPath) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                XConfigManager.openPreferences(initialPath);
            }
        });
    }

    /**
     * @param name the toolbox name
     * @param path the toolbox path
     * @param prefFile the toolbox preference file path
     */
    public static void addToolboxInfos(String name, String path, String prefFile) throws Exception {
        File f = new File(path);
        if (!f.exists()) {
            throw new Exception(Messages.gettext("Invalid path:") + " " + path + ".");
        }
        f = new File(prefFile);
        if (!f.exists()) {
            throw new Exception(Messages.gettext("Invalid preferences file:") + " " + prefFile + ".");
        }
        tbxs.put(path, new ToolboxInfos(name, path, prefFile));
    }

    /**
     * @return a list of the toolboxes infos
     */
    public static List<ToolboxInfos> getToolboxesInfos() {
        return new ArrayList<ToolboxInfos>(tbxs.values());
    }

    /**
     * @param path the toolbox to remove
     */
    public static void removeToolboxInfos(String path) {
        tbxs.remove(path);
    }

    /**
     * Toolbox infos
     */
    public static class ToolboxInfos {

        private String name;
        private String path;
        private String prefFile;

        /**
         * Default constructor
         * @param name the toolbox name
         * @param path the toolbox path
         * @param prefFile the toolbox preference file path
         */
        public ToolboxInfos(String name, String path, String prefFile) {
            this.name = name;
            this.path = path;
            this.prefFile = prefFile;
        }

        /**
         * @return the toolbox name
         */
        public String getName() {
            return name;
        }

        /**
         * @return the toolbox path
         */
        public String getPath() {
            return path;
        }

        /**
         * @return the toolbox preference file path
         */
        public String getPrefFile() {
            return prefFile;
        }

        /**
         * {@inheritDoc}
         */
        public String toString() {
            return "Toolbox: " + name + "\nPath: " + path + "\nPreference file: " + prefFile;
        }
    }
}
