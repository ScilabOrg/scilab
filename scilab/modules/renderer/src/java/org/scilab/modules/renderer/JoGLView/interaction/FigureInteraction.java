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

import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;

import java.awt.Component;
import java.awt.Point;

/**
 * @author Pierre Lando
 */
public abstract class FigureInteraction {

    /** The figure with witch there are interaction */
    private final Figure figure;

    /** The listened AWT component */
    private final Component component;

    /** Enable status */
    private boolean isEnable;

    /**
     * Default constructor.
     * @param figure the figure with witch there are interaction.
     * @param component listened AWT component.
     */
    public FigureInteraction(Figure figure, Component component) {
        this.component = component;
        this.figure = figure;
        isEnable = false;
    }


    /**
     * Compute the underlying {@link org.scilab.modules.graphic_objects.axes.Axes} for the given point in figure coordinates.
     * @param point given point in figure coordinates.
     * @return the underlying {@link org.scilab.modules.graphic_objects.axes.Axes} for the given point in figure coordinates.
     */
    protected Axes getUnderlyingAxes(Point point) {
        Axes underlyingAxes = null;
        Integer[] size = figure.getAxesSize();
        double x = point.getX() / size[0];
        double y = point.getY() / size[1];
        for (String childId : figure.getChildren()) {
            GraphicObject child = GraphicController.getController().getObjectFromId(childId);
            if (child instanceof Axes) {
                Double[] axesBounds = ((Axes) child).getAxesBounds();  // x y w h
                if ((x >= axesBounds[0]) && (x <= axesBounds[0] + axesBounds[2]) && (y >= axesBounds[1]) && (y <= axesBounds[1] + axesBounds[3])) {
                    underlyingAxes = (Axes) child;
                }
            }
        }
        return underlyingAxes;
    }

    /**
     * Listened AWT component getter.
     * @return the listened component.
     */
    public Component getComponent() {
        return component;
    }

    /**
     * Figure getter.
     * @return the figure.
     */
    public Figure getFigure() {
        return figure;
    }

    /**
     * Enable status getter.
     * @return the enable status.
     */
    public boolean isEnable() {
        return isEnable;
    }

    /**
     * Enable status setter.
     * @param isEnable the new enable status setter.
     */
    public void setEnable(boolean isEnable) {
        if (this.isEnable != isEnable) {
            this.isEnable = isEnable;
            changeEnable(isEnable);
        }
    }

    /**
     * Called when the enable status have changed.
     * @param isEnable the new enable status.
     */
    protected abstract void changeEnable(boolean isEnable);

    /**
     * Tight given bounds to axes data bounds.
     * @param axes the given axes.
     * @param zoomBounds the zoomBounds.
     * @return true if actually there is a zoom.
     */
    protected boolean tightZoomBounds(Axes axes, Double[] zoomBounds) {
        boolean zoomed = false;
        Double[] dataBounds = axes.getMaximalDisplayedBounds();
        for (int i : new int[] {0, 2, 4}) {
            if (zoomBounds[i] < dataBounds[i]) {
                zoomBounds[i] = dataBounds[i];
            } else {
                zoomed = true;
            }
        }

        for (int i : new int[] {1, 3, 5}) {
            if (zoomBounds[i] > dataBounds[i]) {
                zoomBounds[i] = dataBounds[i];
            } else {
                zoomed = true;
            }
        }

        return zoomed;
    }
}
