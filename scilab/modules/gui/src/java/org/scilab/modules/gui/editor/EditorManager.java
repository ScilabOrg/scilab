/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.editor;


import java.util.HashMap;
import java.util.Map;

/**
 * Manages all the editors.
 *
 * @author Caio Souza <caioc2bolado@gmail.com>
 * @author Pedro Souza <bygrandao@gmail.com>
 *
 * @since 2012-067-12
 */

public class EditorManager {

    private static Map<String, Editor> allEditors = new HashMap<String, Editor>();

    /**
     * Create an editor and put it on the map.
     * @param figureUid Figure which the editor belongs.
     * @return The created editor instance.
     */
    public static Editor newEditor(String figureUid) {
        Editor editor = new Editor();
        editor.setFigure(figureUid);
        allEditors.put(figureUid, editor);
        return editor;
    }

    /**
     * Enables the editor which belongs to the given figure uid.
     * @param figureUid Figure wich the editor belongs.
     */
    public static void enable(String uid) {
        Editor editor = allEditors.get(uid);
        if (editor != null) {
            editor.setEnabled(true);
        }
    }

    /**
     * Disables the editor which belongs to the given figure uid.
     * @param figureUid Figure wich the editor belongs.
     */
    public static void disable(String uid) {
        Editor editor = allEditors.get(uid);
        if (editor != null) {
            editor.setEnabled(false);
        }
    }

    /**
     * Checks if the editor which belongs to the given figure uid is enabled.
     * @return True if the editor is enableb, false otherwise.
     */
    public static boolean isEnabled(String uid) {
        Editor editor = allEditors.get(uid);
        if (editor != null) {
            return editor.isEnabled();
        }
        return false;
    }

    /**
     * Remove the editor from the map.
     */
    public static void deleteEditor(String uid) {
        allEditors.remove(uid);
    }

    /**
     * Get the editor by figure uid.
     * @param uid The figure uid.
     * @return The editor instance.
     */
    public Editor getFromUid(String uid) {
        return allEditors.get(uid);
    }
}
