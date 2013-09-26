/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;

/**
 *
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class GEDView implements GraphicView {
    private String lastID = null;

    public GEDView() {
        GraphicController.getController().register(this);
    }

    @Override
    public void updateObject(String id, int property) {
        if (SwapObject.getLastType() == (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__)) {
            Integer value = null;
            if (property != 27 && property != 82 && property != 92 && property != 93 && property != 94 && property != 101 &&
                property != 213 && property != 223 && property != 169 && property != 181 && property != 174 &&
                property != 205 && property != 244 && property != 173 && property != 179 && property != 310 &&
                property != 306 && property != 311 && property != 305 && property != 307)
            {
                Inspector.getInspector(id);
            }
        }
    }

    @Override
    public void createObject(String id) {
    }

    @Override
    public void deleteObject(String id) {
    }

    public void close() {
        GraphicController.getController().unregister(this);
    }
}