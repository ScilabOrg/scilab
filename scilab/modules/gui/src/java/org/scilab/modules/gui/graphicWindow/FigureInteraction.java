/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */
package org.scilab.modules.gui.graphicWindow;

import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.events.GlobalEventWatcher;

import java.awt.Component;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionAdapter;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXES__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;

/**
 * This class manage figure interaction.
 *
 * @author Pierre Lando
 */
public class FigureInteraction {

    private static final int XY_TRANSLATION_MODIFIER = MouseEvent.BUTTON1_MASK;
    private static final int Z_TRANSLATION_MODIFIER = MouseEvent.BUTTON1_MASK | MouseEvent.CTRL_MASK;
    private static final int ROTATION_MODIFIER = MouseEvent.BUTTON3_MASK;

    /**
     * The box size is multiply by this value.
     */
    private static final double ZOOM_FACTOR = 1.02;

    private final MouseListener mouseListener;
    private final MouseWheelListener mouseWheelListener;
    private final MouseMotionListener mouseMotionListener;

    private final Component component;
    private final String figureId;

    /**
     * Current enable status.
     */
    private boolean isEnable;

    /**
     * Last important mouse event.
     */
    private MouseEvent previousEvent;

    /**
     * Default constructor.
     * @param component Component to listen.
     * @param figureId the Scilab figure Id.
     */
    public FigureInteraction(Component component, String figureId) {
        this.component = component;
        this.figureId = figureId;

        mouseMotionListener = new FigureMouseMotionListener();
        mouseWheelListener = new FigureMouseWheelListener();
        mouseListener = new FigureMouseListener();

        isEnable = false;
    }

    /**
     * Enable status setter.
     * @param isEnable the new enable status.
     */
    public void setEnable(boolean isEnable) {
        if (isEnable() != isEnable) {
            if (isEnable) {
                component.addMouseListener(mouseListener);
                component.addMouseMotionListener(mouseMotionListener);
                component.addMouseWheelListener(mouseWheelListener);
            } else {
                component.removeMouseListener(mouseListener);
                component.removeMouseMotionListener(mouseMotionListener);
                component.removeMouseWheelListener(mouseWheelListener);
            }
            this.isEnable = isEnable;
        }
    }

    /**
     * Enable status getter.
     * @return the enable status.
     */
    public boolean isEnable() {
        return isEnable;
    }

    /**
     * This {@see MouseListner} activate the {@see MouseMotionListener} when at least
     * one button is pressed.
     * The event is saved in {@see previousEvent}
     */
    private class FigureMouseListener extends MouseAdapter implements MouseListener {

        private int pressedButtons = 0;

        @Override
        public void mousePressed(MouseEvent e) {
            if (pressedButtons == 0) {
                component.addMouseMotionListener(mouseMotionListener);
                previousEvent = e;
            }
            pressedButtons++;
        }

        @Override
        public void mouseReleased(MouseEvent e) {
            pressedButtons--;
            if (pressedButtons == 0) {
                component.removeMouseMotionListener(mouseMotionListener);
            }
        }

        @Override
        public void mouseEntered(MouseEvent e) {
            GlobalEventWatcher.setAxesUID(figureId);
        }
    }

    /**
     * This {@see MouseWheelListener} manage zoom/un-zoom on the figure.
     */
    private class FigureMouseWheelListener implements MouseWheelListener {

        @Override
        public void mouseWheelMoved(MouseWheelEvent e) {
            // TODO : picking to find current children
            String[] children = (String[]) GraphicController.getController().getProperty(figureId, GraphicObjectProperties.__GO_CHILDREN__);
            for (String child : children) {
                GraphicObject object = GraphicController.getController().getObjectFromId(child);
                if (object instanceof Axes) {
                    double scale = Math.pow(ZOOM_FACTOR, e.getUnitsToScroll());

                    Axes axes = (Axes) object;
                    Double[] bounds = axes.getDisplayedBounds();

                    double xDelta = (bounds[1] - bounds[0]) / 2;
                    double xMiddle = (bounds[1] + bounds[0]) / 2;
                    bounds[0] = xMiddle - xDelta * scale;
                    bounds[1] = xMiddle + xDelta * scale;

                    double yDelta = (bounds[3] - bounds[2]) / 2;
                    double yMiddle = (bounds[3] + bounds[2]) / 2;
                    bounds[2] = yMiddle - yDelta * scale;
                    bounds[3] = yMiddle + yDelta * scale;

                    double zDelta = (bounds[5] - bounds[4]) / 2;
                    double zMiddle = (bounds[5] + bounds[4]) / 2;
                    bounds[4] = zMiddle - zDelta * scale;
                    bounds[5] = zMiddle + zDelta * scale;

                    Boolean zoomed = tightZoomBounds(axes, bounds);
                    GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_BOX__, bounds);
                    GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_ENABLED__, zoomed);
                }
            }
        }


        /**
         * Tight given bounds to axes data bounds.
         * @param axes the given axes.
         * @param zoomBounds the zoomBounds.
         * @return true if actually there is a zoom.
         */
        private boolean tightZoomBounds(Axes axes, Double[] zoomBounds) {
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

    /**
     * This {@see MouseMotionListener} manage rotation and translation on the figure.
     */
    private class FigureMouseMotionListener extends MouseMotionAdapter implements MouseMotionListener {

        @Override
        public void mouseDragged(MouseEvent e) {
            switch (e.getModifiers()) {
                case XY_TRANSLATION_MODIFIER:
                    doXYTranslation(e);
                    break;
                case Z_TRANSLATION_MODIFIER:
                    doZTranslation(e);
                    break;
                case ROTATION_MODIFIER:
                    doRotation(e);
                    break;
            }

            previousEvent = e;
        }

        private void doRotation(MouseEvent e) {
            int dx = e.getX() - previousEvent.getX();
            int dy = e.getY() - previousEvent.getY();

            // TODO : picking to find current children
            String[] children = (String[]) GraphicController.getController().getProperty(figureId, GraphicObjectProperties.__GO_CHILDREN__);
            for (String child : children) {
                String childType = (String) GraphicController.getController().getProperty(child, __GO_TYPE__);
                if (__GO_AXES__.equals(childType)) {
                    Double[] angles = (Double[]) GraphicController.getController().getProperty(child, GraphicObjectProperties.__GO_ROTATION_ANGLES__);
                    angles[0] -= dy / 4.0;
                    angles[1] -= dx / 4.0;
                    GraphicController.getController().setProperty(child, GraphicObjectProperties.__GO_ROTATION_ANGLES__, angles);
                }
            }
        }

        private void doXYTranslation(MouseEvent e) {
            int dx = e.getX() - previousEvent.getX();
            int dy = e.getY() - previousEvent.getY();

            // TODO : picking to find current children
            String[] children = (String[]) GraphicController.getController().getProperty(figureId, GraphicObjectProperties.__GO_CHILDREN__);
            for (String child : children) {
                GraphicObject object = GraphicController.getController().getObjectFromId(child);
                if (object instanceof Axes) {
                    Axes axes = (Axes) object;
                    if (axes.getZoomEnabled()) {
                        Double[] bounds = axes.getDisplayedBounds();
                        double orientation = Math.signum(Math.cos(Math.toRadians(axes.getRotationAngles()[0])));
                        double angle = - orientation * Math.toRadians(axes.getRotationAngles()[1]);

                        double xDelta = (bounds[0] - bounds[1])/100;
                        double yDelta = (bounds[2] - bounds[3])/100;

                        double rotatedDX = dx * Math.sin(angle) + dy * Math.cos(angle);
                        double rotatedDY = dx * Math.cos(angle) - dy * Math.sin(angle);

                        bounds[0] += xDelta * rotatedDX * orientation;
                        bounds[1] += xDelta * rotatedDX * orientation;

                        bounds[2] += yDelta * rotatedDY;
                        bounds[3] += yDelta * rotatedDY;

                        Boolean zoomed = tightZoomBoxToDataBounds(axes, bounds);
                        GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_BOX__, bounds);
                        GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_ENABLED__, zoomed);
                    }
                }
            }
        }

        private void doZTranslation(MouseEvent e) {
            int dy = e.getY() - previousEvent.getY();

            // TODO : picking to find current children
            String[] children = (String[]) GraphicController.getController().getProperty(figureId, GraphicObjectProperties.__GO_CHILDREN__);
            for (String child : children) {
                GraphicObject object = GraphicController.getController().getObjectFromId(child);
                if (object instanceof Axes) {
                    Axes axes = (Axes) object;
                    Double[] bounds = axes.getDisplayedBounds();

                    double zDelta = (bounds[5] - bounds[4])/100;

                    bounds[4] += zDelta * dy;
                    bounds[5] += zDelta * dy;

                    Boolean zoomed = tightZoomBoxToDataBounds(axes, bounds);
                    GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_BOX__, bounds);
                    GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_ENABLED__, zoomed);
                }
            }
        }

        /**
         * Tight given bounds to axes data bounds.
         * Bounds length along axes are conserved.
         * @param axes the given axes.
         * @param zoomBounds the zoomBounds.
         * @return true if actually there is a zoom.
         */
        private boolean tightZoomBoxToDataBounds(Axes axes, Double[] zoomBounds) {
            boolean zoomed = false;
            Double[] dataBounds = axes.getMaximalDisplayedBounds();
            for (int i : new int[] {0, 2, 4}) {
                if (zoomBounds[i] < dataBounds[i]) {
                    double delta = dataBounds[i] - zoomBounds[i];
                    zoomBounds[i] = dataBounds[i]; // zoomBounds[i] += delta;
                    zoomBounds[i + 1] += delta;
                } else {
                    zoomed = true;
                }
            }

            for (int i : new int[] {1, 3, 5}) {
                if (zoomBounds[i] > dataBounds[i]) {
                    double delta = dataBounds[i] - zoomBounds[i];
                    zoomBounds[i] = dataBounds[i]; // zoomBounds[i] += delta;
                    zoomBounds[i - 1] += delta;
                } else {
                    zoomed = true;
                }
            }

            return zoomed;
        }
    }
}
