/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.interaction;

import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;

import java.awt.Component;
import java.awt.Point;

/**
 * @author Pierre Lando
 */
public class ZoomRubberBoxListener extends FigureInteraction implements RubberBoxListener {

    /** The resolution limit */
    private static final double RESOLUTION_LIMIT = 1e-12f;

    /** The parent rubber box */
    private final RubberBox rubberBox;

    /**
     * Default constructor.
     * @param figure listened figure.
     * @param component listened component.
     * @param rubberBox parent rubber box.
     */
    public ZoomRubberBoxListener(Figure figure, Component component, RubberBox rubberBox) {
        super(figure, component);
        this.rubberBox = rubberBox;
    }

    @Override
    public void rubberBoxEnd(Point firstPoint, Point secondPoint) {
        performZoom(firstPoint, secondPoint);
        rubberBox.setEnable(false);
    }

    @Override
    public void rubberBoxStopped() {
        rubberBox.setEnable(false);
    }

    @Override
    protected void changeEnable(boolean isEnable) {
        rubberBox.setEnable(isEnable);
    }

    /**
     * Actually perform the zoom
     * @param firstPoint first rubber box corner.
     * @param secondPoint second rubber box corner.
     */
    private void performZoom(Point firstPoint, Point secondPoint) {
        Axes axes = getUnderlyingAxes(firstPoint);
        if (!firstPoint.equals(secondPoint)) {
            Double[] bounds = axes.getDisplayedBounds().clone();
            Vector3d p1 = AxesDrawer.unproject(axes, new Vector3d(firstPoint.x, firstPoint.y, 0));
            Vector3d p2 = AxesDrawer.unproject(axes, new Vector3d(secondPoint.x, secondPoint.y, 0));

            if (Math.abs(p1.getX() - p2.getX()) > RESOLUTION_LIMIT) {
                bounds[0] = (double) Math.nextAfter((float) Math.min(p1.getX(), p2.getX()), -Double.MAX_VALUE);
                bounds[1] = (double) Math.nextAfter((float) Math.max(p1.getX(), p2.getX()), Double.MAX_VALUE);
            }

            if (Math.abs(p1.getY() - p2.getY()) > RESOLUTION_LIMIT) {
                bounds[2] = (double) Math.nextAfter((float) Math.min(p1.getY(), p2.getY()), -Double.MAX_VALUE);
                bounds[3] = (double) Math.nextAfter((float) Math.max(p1.getY(), p2.getY()), Double.MAX_VALUE);
            }

            if (Math.abs(p1.getZ() - p2.getZ()) > RESOLUTION_LIMIT) {
                bounds[4] = (double) Math.nextAfter((float) Math.min(p1.getZ(), p2.getZ()), -Double.MAX_VALUE);
                bounds[5] = (double) Math.nextAfter((float) Math.max(p1.getZ(), p2.getZ()), Double.MAX_VALUE);
            }

            Boolean zoomed = tightZoomBounds(axes, bounds);
            GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_BOX__, bounds);
            GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_ENABLED__, zoomed);
        }
    }
}
