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

package org.scilab.modules.gui.uiwidget.go;

import java.util.List;

import org.scilab.modules.graphic_objects.graphicFactory.GraphicObjectFactory;
import org.scilab.modules.graphic_objects.graphicFactory.GraphicFactory;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.types.ScilabType;

public class UIWidgetGraphicObjectFactory implements GraphicObjectFactory  {

    private static GraphicObjectFactory factory;

    private UIWidgetGraphicObjectFactory() { }

    public static GraphicObjectFactory newInstance() {
        if (factory == null) {
            factory = new UIWidgetGraphicObjectFactory();
        }

        return factory;
    }

    /**
     * {@inheritDoc}
     */
    public GraphicObject createObject(List<ScilabType> args) {
	try {
	    return UIWidgetGraphicObject.getGO(args);
	} catch (Exception e) {
	    System.err.println(e);
	    return null;
	}
    }

    public void deleteObject(int id) {
	UIWidgetGraphicObject.delete(id);
    }

    public static void register() {
        GraphicFactory.register(GraphicObject.Type.UICONTROL, newInstance());
    }

    public static void unregister() {
        GraphicFactory.unregister(GraphicObject.Type.UICONTROL);
    }
}
