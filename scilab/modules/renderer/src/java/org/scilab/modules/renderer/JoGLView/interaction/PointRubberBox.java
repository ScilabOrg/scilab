/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.interaction;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

/**
 * @author Bruno JOFRET
 */
public class PointRubberBox extends RubberBox {

    /**
     * Default constructor.
     *
     * @param drawerVisitor parent drawer visitor.
     */
    public PointRubberBox(DrawerVisitor drawerVisitor) {
        super(drawerVisitor);
        status = Status.WAIT_POINT_A;
    }

    /**
     * Actually set the zoom box depending on current value of firstPoint and secondPoint.
     */
    @Override
    protected void process() {
        Double[] bounds = {
            Math.min(firstPoint.getX(), secondPoint.getX()), Math.max(firstPoint.getX(), secondPoint.getX()),
            Math.min(firstPoint.getY(), secondPoint.getY()), Math.max(firstPoint.getY(), secondPoint.getY()),
            Math.min(firstPoint.getZ(), secondPoint.getZ()), Math.max(firstPoint.getZ(), secondPoint.getZ()),
        };

        if (bounds[0].compareTo(bounds[1]) != 0 && bounds[2].compareTo(bounds[3]) != 0 && bounds[4].compareTo(bounds[5]) != 0) {
            Boolean zoomed = tightZoomBounds(axes, bounds);
            GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_BOX__, bounds);
            GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_ENABLED__, zoomed);
            getDrawerVisitor().getCanvas().redraw();
        }
    }
}
