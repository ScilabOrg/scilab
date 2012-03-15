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

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.BuffersManager;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.interaction.util.HelpersGeometry;
import org.scilab.modules.renderer.JoGLView.interaction.util.PointAComputer;
import org.scilab.modules.renderer.JoGLView.interaction.util.PointBComputer;
import org.scilab.modules.renderer.JoGLView.interaction.util.PointCComputer;
import org.scilab.modules.renderer.JoGLView.interaction.util.PointDComputer;
import org.scilab.modules.renderer.JoGLView.postRendering.PostRendered;

import javax.swing.event.EventListenerList;
import java.awt.Component;
import java.awt.Point;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

/**
 * @author Pierre Lando
 */
public class RubberBox extends FigureInteraction implements PostRendered, MouseListener, MouseMotionListener, KeyListener {

    /** Rubber box status */
    public static enum Status {
        WAIT_POINT_A,
        WAIT_POINT_B,
        WAIT_POINT_C,
        WAIT_POINT_D,
    }

    /** Rubber box color */
    private static final Color RUBBER_BOX_COLOR = new Color(.2f, .3f, .4f);

    /** Rubber box thickness */
    private static final float RUBBER_BOX_THICKNESS = 2;

    /** Rubber box pattern */
    private static final short RUBBER_BOX_PATTERN = (short) 0xFAFA;

    /** The mouse button used to perform a rubber box */
    private static final int MOUSE_MODIFIER = MouseEvent.BUTTON1_MASK;

    /** This key stop the rubber box */
    private static final int STOP_KEY = KeyEvent.VK_ESCAPE;

    /** The event listener list */
    private final EventListenerList listenerList = new EventListenerList();

    /** Rubber box cube geometry */
    private DefaultGeometry cubeGeometry;

    /** Rubber box cube appearance */
    private static Appearance cubeAppearance;

    /** Helpers geometry */
    private HelpersGeometry helpersGeometry;

    /** Helpers appearance */
    private static Appearance helpersAppearance;
    
    /** Current status */
    private Status status;

    private Axes axes;

    private PointAComputer pointAComputer;
    private PointBComputer pointBComputer;
    private PointCComputer pointCComputer;
    private Vector3d firstPoint;
    private Vector3d secondPoint;

    public RubberBox(DrawerVisitor drawerVisitor) {
        super(drawerVisitor);
        status = Status.WAIT_POINT_A;
    }

    @Override
    public void draw(DrawingTools drawingTools) throws SciRendererException {
        if (isEnable() && (axes != null)) {
            drawingTools.getTransformationManager().useSceneCoordinate();
            drawingTools.getTransformationManager().getModelViewStack().push(getDrawerVisitor().getAxesDrawer().getSceneProjection(axes.getIdentifier()));

            if (status != Status.WAIT_POINT_A) {
                drawingTools.draw(getCubeGeometry(drawingTools), getCubeAppearance());
            }

            if (secondPoint != null) {
                drawingTools.draw(getHelpersGeometry(drawingTools), getHelpersAppearance());
            }

            drawingTools.getTransformationManager().getModelViewStack().pop();
        }
    }

    @Override
    public void changeEnable(boolean isEnable) {
        Component component = getDrawerVisitor().getComponent();
        if (isEnable) {
            status = Status.WAIT_POINT_A;
            pointAComputer = null;
            component.addMouseListener(this);
            component.addMouseMotionListener(this);
            component.addKeyListener(this);
            component.setFocusTraversalKeysEnabled(false);
            component.setFocusable(true);
            component.requestFocus();
        } else {
            component.removeMouseListener(this);
            component.removeMouseMotionListener(this);
            component.removeKeyListener(this);
        }
        getDrawerVisitor().getCanvas().redraw();
    }

    /**
     * Notify all listener that the rubber box have ended
     * @param succeed true if the rubber box ended with a success.
     */
    private void fireRubberBox(boolean succeed) {
        if (succeed) {
            for (RubberBoxListener rubberBoxListener : listenerList.getListeners(RubberBoxListener.class)) {
                rubberBoxListener.rubberBoxEnd(firstPoint, secondPoint);
            }
        } else {
            for (RubberBoxListener rubberBoxListener : listenerList.getListeners(RubberBoxListener.class)) {
                rubberBoxListener.rubberBoxStopped();
            }
        }
    }

    public void addListener(RubberBoxListener rubberBoxListener) {
        listenerList.add(RubberBoxListener.class, rubberBoxListener);
    }

    public void removeListener(RubberBoxListener rubberBoxListener) {
        listenerList.remove(RubberBoxListener.class, rubberBoxListener);
    }

    @Override
    public void keyTyped(KeyEvent e) {
        if (e.getKeyChar() == STOP_KEY) {
            setEnable(false);
            fireRubberBox(false);
        }
    }

    @Override
    public void mouseClicked(MouseEvent e) {
        if (e.getModifiers() == MOUSE_MODIFIER) {
            switch (status) {
                case WAIT_POINT_A:
                    if (setPointA(e.getPoint())) {
                        status = Status.WAIT_POINT_B;
                    } else {
                        setEnable(false);
                        fireRubberBox(false);
                    }
                    break;
                case WAIT_POINT_B:
                    setPointB(e.getPoint());
                    if (pointBComputer.is2D()) {
                        set();
                        setEnable(false);
                        fireRubberBox(true);
                    } else {
                        status = Status.WAIT_POINT_C;
                    }
                    break;
                case WAIT_POINT_C:
                    setPointC(e.getPoint());
                    status = Status.WAIT_POINT_D;
                    break;
                case WAIT_POINT_D:
                    setPointD(e.getPoint());
                    set();
                    setEnable(false);
                    fireRubberBox(true);
                    break;
            }
        }
    }

    @Override
    public void mouseMoved(MouseEvent e) {
        switch (status) {
            case WAIT_POINT_A:
                setPointA(e.getPoint());
                getDrawerVisitor().getCanvas().redraw();
                break;
            case WAIT_POINT_B:
                setPointB(e.getPoint());
                getDrawerVisitor().getCanvas().redraw();
                break;
            case WAIT_POINT_C:
                setPointC(e.getPoint());
                getDrawerVisitor().getCanvas().redraw();
                break;
            case WAIT_POINT_D:
                setPointD(e.getPoint());
                getDrawerVisitor().getCanvas().redraw();
                break;
        }
    }

    private void set() {
        Double[] bounds = {
                Math.min(firstPoint.getX(), secondPoint.getX()), Math.max(firstPoint.getX(), secondPoint.getX()),
                Math.min(firstPoint.getY(), secondPoint.getY()), Math.max(firstPoint.getY(), secondPoint.getY()),
                Math.min(firstPoint.getZ(), secondPoint.getZ()), Math.max(firstPoint.getZ(), secondPoint.getZ()),
        };
        Boolean zoomed = tightZoomBounds(axes, bounds);
        GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_BOX__, bounds);
        GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_ENABLED__, zoomed);
        getDrawerVisitor().getCanvas().redraw();
    }

    /**
     * Set the first point.
     * @param point first point AWT coordinate.
     * @return true if the first point is valid.
     */
    private boolean setPointA(Point point) {
        axes = getUnderlyingAxes(point);
        if (axes != null) {
            PointAComputer pointAComputer = new PointAComputer(axes, point);
            if (pointAComputer.isValid()) {
                this.pointAComputer = pointAComputer;
                firstPoint = pointAComputer.getPosition();
                secondPoint = firstPoint;
                return true;
            }
        }
        return false;
    }

    /**
     * Set second point in 3D zoom.
     * @param point second point.
     * @return true if the point is valid.
     */
    private boolean setPointB(Point point) {
        PointBComputer pointBComputer = new PointBComputer(axes, pointAComputer, point);
        if (pointBComputer.isValid()) {
            this.pointBComputer = pointBComputer;
            firstPoint = pointBComputer.getFirstPosition();
            secondPoint = pointBComputer.getSecondPosition();
            getDrawerVisitor().getCanvas().redraw();
            return true;
        } else {
            return false;
        }
    }

    /**
     * Set zoom box position in 3D zoom.
     * @param point mouse position.
     * @return true if the point is valid.
     */
    private boolean setPointC(Point point) {
        PointCComputer pointCComputer = new PointCComputer(axes, pointBComputer, point);
        if (pointCComputer.isValid()) {
            this.pointCComputer = pointCComputer;
            firstPoint = pointCComputer.getFirstPosition();
            secondPoint = pointCComputer.getSecondPosition();
            getDrawerVisitor().getCanvas().redraw();
            return true;
        } else {
            return false;
        }
    }

    /**
     * Set zoom box position in 3D zoom.
     * @param point mouse position.
     * @return true if the point is valid.
     */
    private boolean setPointD(Point point) {
        PointDComputer pointDComputer = new PointDComputer(axes, pointCComputer, point);
        if (pointDComputer.isValid()) {
            firstPoint = pointDComputer.getFirstPosition();
            secondPoint = pointDComputer.getSecondPosition();
            getDrawerVisitor().getCanvas().redraw();
            return true;
        } else {
            return false;
        }
    }

    private Geometry getHelpersGeometry(DrawingTools drawingTools) {
        if (helpersGeometry == null) {
            helpersGeometry = new HelpersGeometry(drawingTools);
        }
        helpersGeometry.updateVertex(axes, pointAComputer, secondPoint, status);
        return helpersGeometry;
    }

    public Appearance getHelpersAppearance() {
        if (helpersAppearance == null) {
            helpersAppearance = new Appearance();
            helpersAppearance.setLineColor(new Color(1, 0, 0));
            helpersAppearance.setLineWidth(2);
        }
        return helpersAppearance;
    }

    /**
     * Rubber box cube geometry getter.
     * @param drawingTools the drawing tools.
     * @return the rubber box cubeGeometry.
     */
    private Geometry getCubeGeometry(DrawingTools drawingTools) {
        if (cubeGeometry == null) {
            cubeGeometry = new DefaultGeometry();

            BuffersManager bufferManager = drawingTools.getCanvas().getBuffersManager();
            ElementsBuffer vertexBuffer = bufferManager.createElementsBuffer();
            vertexBuffer.setData(new float[4 * 8], 4);
            IndicesBuffer indicesBuffer = bufferManager.createIndicesBuffer();
            indicesBuffer.setData(new int[] {
                    0, 1, 0, 3, 0, 4,
                    6, 5, 6, 7, 6, 2,
                    2, 1, 2, 3,
                    4, 5, 4, 7,
                    1, 5, 3 ,7
            });

            cubeGeometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
            cubeGeometry.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
            cubeGeometry.setVertices(vertexBuffer);
            cubeGeometry.setWireIndices(indicesBuffer);
        }

        cubeGeometry.getVertices().setData(new float[] {
                (float) firstPoint.getX(), (float) firstPoint.getY(), (float) firstPoint.getZ(), 1,
                (float) firstPoint.getX(), (float) firstPoint.getY(), (float) secondPoint.getZ(), 1,
                (float) firstPoint.getX(), (float) secondPoint.getY(), (float) secondPoint.getZ(), 1,
                (float) firstPoint.getX(), (float) secondPoint.getY(), (float) firstPoint.getZ(), 1,
                (float) secondPoint.getX(), (float) firstPoint.getY(), (float) firstPoint.getZ(), 1,
                (float) secondPoint.getX(), (float) firstPoint.getY(), (float) secondPoint.getZ(), 1,
                (float) secondPoint.getX(), (float) secondPoint.getY(), (float) secondPoint.getZ(), 1,
                (float) secondPoint.getX(), (float) secondPoint.getY(), (float) firstPoint.getZ(), 1
        }, 4);

        return cubeGeometry;
    }

    /**
     * Rubber-box cube appearance getter.
     * @return the rubber-box cube appearance.
     */
    private Appearance getCubeAppearance() {
        if (cubeAppearance == null) {
            cubeAppearance = new Appearance();
            cubeAppearance.setLineColor(RUBBER_BOX_COLOR);
            cubeAppearance.setLineWidth(RUBBER_BOX_THICKNESS);
            cubeAppearance.setLinePattern(RUBBER_BOX_PATTERN);
        }
        return cubeAppearance;
    }

    @Override
    public void mousePressed(MouseEvent e) {
    }

    @Override
    public void mouseReleased(MouseEvent e) {
    }

    @Override
    public void mouseEntered(MouseEvent e) {
    }

    @Override
    public void mouseExited(MouseEvent e) {
    }

    @Override
    public void mouseDragged(MouseEvent e) {
    }

    @Override
    public void keyPressed(KeyEvent e) {
    }

    @Override
    public void keyReleased(KeyEvent e) {
    }
}
