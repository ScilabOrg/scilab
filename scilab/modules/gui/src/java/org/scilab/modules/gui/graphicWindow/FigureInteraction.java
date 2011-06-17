package org.scilab.modules.gui.graphicWindow;

import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import javax.media.opengl.GLAutoDrawable;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionAdapter;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

/**
 * This class manage figure interaction.
 *
 * @author Pierre Lando
 */
public class FigureInteraction {

    private static final double ZOOM_FACTOR = 1.1;

    private final MouseListener mouseListener;
    private final MouseWheelListener mouseWheelListener;
    private final MouseMotionListener mouseMotionListener;

    private final GLAutoDrawable panel;
    private final String figureId;

    private boolean isEnable = false;

    private MouseEvent previousEvent;

    public FigureInteraction(GLAutoDrawable panel, String figureId) {
        this.panel = panel;
        this.figureId = figureId;

        mouseListener = createMouseListener();
        mouseMotionListener = createMouseMotionListener();
        mouseWheelListener = createMouseWheelListener();
    }

    public void setEnable(boolean isEnable) {
        this.isEnable = isEnable;
        if (isEnable) {
            enable();
        } else {
            disable();
        }
    }

    public boolean isEnable() {
        return isEnable;
    }

    private void disable() {
        panel.removeMouseListener(mouseListener);
        panel.removeMouseMotionListener(mouseMotionListener);
        panel.removeMouseWheelListener(mouseWheelListener);
    }

    private void enable() {
        panel.addMouseListener(mouseListener);
        panel.addMouseWheelListener(mouseWheelListener);
    }

    private MouseListener createMouseListener() {
        return new MouseAdapter() {
            int pressedButtons = 0;

            @Override
            public void mousePressed(MouseEvent e) {
                if (pressedButtons == 0) {
                    panel.addMouseMotionListener(mouseMotionListener);
                    previousEvent = e;
                }
                pressedButtons++;
            }

            @Override
            public void mouseReleased(MouseEvent e) {
                pressedButtons--;
                if (pressedButtons == 0) {
                    panel.removeMouseMotionListener(mouseMotionListener);
                }
            }
        };
    }

    /**
     * Return the current visible bounds of the given axes.
     * // TODO : tight limit.
     * @param axes the given axes.
     * @return the current visible bounds of the given axes.
     */
    private Double[] getCurrentBounds(Axes axes) {
        if (axes.getZoomEnabled()) {
            return axes.getZoomBox();
        } else {
            return axes.getDataBounds();
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
        Double[] dataBounds = axes.getDataBounds();
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

    private MouseWheelListener createMouseWheelListener() {
        return new MouseWheelListener() {
            @Override
            public void mouseWheelMoved(MouseWheelEvent e) {
                // TODO : picking to find current children
                String[] children = (String[]) GraphicController.getController().getProperty(figureId, GraphicObjectProperties.__GO_CHILDREN__);
                for (String child : children) {
                    GraphicObject object = GraphicController.getController().getObjectFromId(child);
                    if (object instanceof Axes) {

                        double scale = Math.pow(ZOOM_FACTOR, e.getUnitsToScroll());
                        System.out.println(scale);

                        Axes axes = (Axes) object;
                        Double[] bounds = getCurrentBounds(axes);

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
        };
    }


    /**
     * Tight given bounds to axes data bounds.
     * Bounds length along axes are conserved.
     * @param axes the given axes.
     * @param zoomBounds the zoomBounds.
     * @return true if actually there is a zoom.
     */
    private boolean tightTranslationBounds(Axes axes, Double[] zoomBounds) {
        boolean zoomed = false;
        Double[] dataBounds = axes.getDataBounds();
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

    private MouseMotionListener createMouseMotionListener() {
        return new MouseMotionAdapter() {

            @Override
            public void mouseDragged(MouseEvent e) {
                if (isRotationEvent(e)) {
                    doRotation(e);
                } else if (isTranslationEvent(e)) {
                    doTranslation(e);
                }

                previousEvent = e;
            }

            private boolean isTranslationEvent(MouseEvent e) {
                return e.getModifiers() == MouseEvent.BUTTON1_MASK;
            }

            private boolean isRotationEvent(MouseEvent e) {
                return e.getModifiers() == MouseEvent.BUTTON3_MASK;
            }

            private void doRotation(MouseEvent e) {
                int dx = e.getX() - previousEvent.getX();
                int dy = e.getY() - previousEvent.getY();

                // TODO : picking to find current children
                String[] children = (String[]) GraphicController.getController().getProperty(figureId, GraphicObjectProperties.__GO_CHILDREN__);
                for (String child : children) {
                    Double[] angles = (Double[]) GraphicController.getController().getProperty(child, GraphicObjectProperties.__GO_ROTATION_ANGLES__);
                    angles[0] -= dy / 4.0;
                    angles[1] -= dx / 4.0;
                    GraphicController.getController().setProperty(child, GraphicObjectProperties.__GO_ROTATION_ANGLES__, angles);
                }
            }

            private void doTranslation(MouseEvent e) {
                int dx = e.getX() - previousEvent.getX();
                int dy = e.getY() - previousEvent.getY();

                // TODO : picking to find current children
                String[] children = (String[]) GraphicController.getController().getProperty(figureId, GraphicObjectProperties.__GO_CHILDREN__);
                for (String child : children) {
                    GraphicObject object = GraphicController.getController().getObjectFromId(child);
                    if (object instanceof Axes) {
                        Axes axes = (Axes) object;
                        Double[] bounds = getCurrentBounds(axes);

                        double xDelta = (bounds[1] - bounds[0])/100;
                        bounds[0] += xDelta * dx;
                        bounds[1] += xDelta * dx;

                        double yDelta = (bounds[3] - bounds[2])/100;
                        bounds[2] += yDelta * dy;
                        bounds[3] += yDelta * dy;

                        Boolean zoomed = tightTranslationBounds(axes, bounds);
                        GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_BOX__, bounds);
                        GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_ENABLED__, zoomed);
                    }
                }
            }
        };
    }
}
