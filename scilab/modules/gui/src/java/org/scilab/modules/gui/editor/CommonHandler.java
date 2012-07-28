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


import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.modules.graphic_objects.SurfaceData;


/**
* Implements all object common manipulation functions for the editor.
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-07-26
*/
public class CommonHandler {
    /**
     * Checks if the object exists.
     *
     * @param uid object unique identifier.
     * @return True if exists, false otherwise.
     */
    public static Boolean objectExists(String uid) {
        if (uid != null) {
            if (GraphicController.getController().getObjectFromId(uid) != null) {
                return true;
            }
        }
        return false;
    }

    /**
     * Checks if the object uses line mode.
     *
     * @param uid object unique identifier.
     * @return True if line mode is enabled, false otherwise.
     */
    public static Boolean isLineEnabled(String uid) {
        return (Boolean)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_LINE_MODE__);
    }

    /**
     * Checks if the object uses mark mode.
     *
     * @param uid object unique identifier.
     * @return True if mark mode is enabled, false otherwise.
     */
    public static Boolean isMarkEnabled(String uid) {
        return (Boolean)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_MARK_MODE__);
    }

    /**
     * Get the mark style from the given object.
     *
     * @param uid object unique identifier.
     * @return Style number.
     */
    public static Integer getMarkStyle(String uid) {
        return (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_MARK_STYLE__);
    }

    /**
     * Get the mark size from the given object.
     *
     * @param uid object unique identifier.
     * @return Mark size.
     */
    public static Integer getMarkSize(String uid) {
        return (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_MARK_SIZE__);
    }

    /**
     * Get the mark size from the given object.
     *
     * @param uid object unique identifier.
     * @return Mark size.
     */
    public static Integer getMarkSizeUnit(String uid) {
        return (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_MARK_SIZE_UNIT__);
    }

    /**
     * Change object line/mark color.
     *
     * @param uid object unique identifier.
     * @param newColor Color to be used.
     * @return Returns the old color of the object.
     */
    public static Integer setColor(String uid, Integer newColor) {

        if (uid == null) {
            return 0;
        }
        Integer oldColor;
        Boolean markon = isMarkEnabled(uid);
        if (markon == true) {
            oldColor = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_MARK_FOREGROUND__);
            GraphicController.getController().setProperty(uid, GraphicObjectProperties.__GO_MARK_FOREGROUND__, newColor );
        } else {
            oldColor = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_LINE_COLOR__);
            GraphicController.getController().setProperty(uid, GraphicObjectProperties.__GO_LINE_COLOR__, newColor );
        }

        return oldColor;

    }

    /**
     * Duplicate the object and its data.
     *
     * @param uid object unique identifier.
     * @return New duplicated object uid or null if it fails.
     */
    public static String duplicate(String uid) {
        String dup = null;
        String ret = null;
        dup = GraphicController.getController().cloneObject(uid);
        GraphicController.getController().setGraphicObjectRelationship("", dup);
        String typeName = (String)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_TYPE__);

        if (typeName == GraphicObjectProperties.__GO_POLYLINE__) {
            ret = PolylineData.createPolylineData(uid, dup);
        }
        else if (typeName == GraphicObjectProperties.__GO_PLOT3D__) {
            ret = SurfaceData.createObject3dData(uid, dup, GraphicObjectProperties.__GO_PLOT3D__);
        }

        if (ret == null) {
            delete(dup);
        }
        return dup;

    }

    /**
     * Inserts the given object in the given axes.
     *
     * @param axes Axes unique identifier.
     * @param uid object unique identifier.
     */
    public static void insert(String axes, String uid) {
        GraphicController.getController().setGraphicObjectRelationship(axes, uid);
        String typeName = (String)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_TYPE__);

        if (typeName == GraphicObjectProperties.__GO_POLYLINE__) {
            String newCompound = GraphicController.getController().askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_COMPOUND__));
            GraphicController.getController().setGraphicObjectRelationship(axes, newCompound);
            GraphicController.getController().setGraphicObjectRelationship(newCompound, uid);
        }
        else if (typeName == GraphicObjectProperties.__GO_PLOT3D__) {
            GraphicController.getController().setGraphicObjectRelationship(axes, uid);
        }
    }

    /**
     * Remove the relationship from the given object
     * from its parent.
     *
     * @param uid object unique object.
     */
    public static void cut(String uid) {
        GraphicController.getController().setGraphicObjectRelationship("", uid);
    }

    /**
     * Remove the relationship from the given object
     * from its parent and deletes it.
     *
     * @param uid object unique identifier.
     */
    public static void delete(String uid) {
        GraphicController.getController().removeRelationShipAndDelete(uid);
    }

    /**
     * Retrieve if the object is visible.
     *
     * @param uid object unique identifier.
     * @return True if is visible, false otherwise.
     */
    public static Boolean isVisible(String uid) {
        return (Boolean)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_VISIBLE__);
    }

    /**
     * Get The parent UID from an object
     *
     * @param object The object
     * @return the parent UID
     */
    public static String getParent(String object) {

        return (String)GraphicController.getController().getProperty(object, GraphicObjectProperties.__GO_PARENT__);
    }
}
