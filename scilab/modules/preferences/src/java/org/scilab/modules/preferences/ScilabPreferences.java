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

package org.scilab.modules.preferences;

/**
 * Class to get the toolboxes and the installed atoms informations
 * @author Calixte DENIZET
 */
public class ScilabPreferences {

    private static String[] tbxNames = new String[0];
    private static String[] tbxPaths = new String[0];
    private static String[] tbxConfNames = new String[0];

    private static String[] atomsNames = new String[0];
    private static String[] atomsPaths = new String[0];
    private static String[] atomsConfNames = new String[0];

    /**
     * For internal use only... This function should not be used
     */
    public static void setToolboxesInfos(String[] names, String[] paths, String[] confNames) {
        tbxNames = names;
        tbxPaths = paths;
        tbxConfNames = confNames;
    }

    /**
     * For internal use only... This function should not be used
     */
    public static void setAtomsInfos(String[] names, String[] paths, String[] confNames) {
        atomsNames = names;
        atomsPaths = paths;
        atomsConfNames = confNames;
    }

    /**
     * @return the toolboxes name
     */
    public static String[] getToolboxesName() {
        return tbxNames;
    }

    /**
     * @return the toolboxes path
     */
    public static String[] getToolboxesPath() {
        return tbxPaths;
    }

    /**
     * @return the preferences configuration file path for the toolboxes
     */
    public static String[] getToolboxesConfName() {
        return tbxConfNames;
    }

    /**
     * @return the atoms name
     */
    public static String[] getAtomsName() {
        return atomsNames;
    }

    /**
     * @return the atoms path
     */
    public static String[] getAtomsPath() {
        return atomsPaths;
    }

    /**
     * @return the preferences configuration file path for the atoms
     */
    public static String[] getAtomsConfName() {
        return atomsConfNames;
    }
}
