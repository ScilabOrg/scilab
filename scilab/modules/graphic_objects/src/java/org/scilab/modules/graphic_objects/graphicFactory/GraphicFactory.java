/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.graphicFactory;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.types.ScilabType;

public final class GraphicFactory {

    private static final GraphicFactory factory = new GraphicFactory();
    private static final Map<GraphicObject.Type, GraphicObjectFactory> map = new HashMap<GraphicObject.Type, GraphicObjectFactory>();
    private static final GOHandler handler = new GOHandler();

    private GraphicFactory() { }

    public static void register(GraphicObject.Type type, GraphicObjectFactory gof) {
        map.put(type, gof);
    }

    public static void unregister(GraphicObject.Type type) {
        map.remove(type);
    }

    public static GraphicObject createObject(GraphicObject.Type type) {
        List<ScilabType> args = getArguments();
        GraphicObjectFactory gof = map.get(type);
        if (gof != null) {
            return gof.createObject(args);
        }

        return null;
    }

    public static void deleteObject(int id, GraphicObject.Type type) {
        GraphicObjectFactory gof = map.get(type);
        if (gof != null) {
            gof.deleteObject(id);
        }
    }
    /**
     * Get the handler id to use to send Scilab data
     * @return the handler id
     */
    public static int getGOHandler() {
        return handler.getHandlerId();
    }

    public static List<ScilabType> getArguments() {
        return handler.getArgumentList();
    }
}
